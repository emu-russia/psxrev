// Traverse selection.

// The entity contains a special Blacklist to block switching to unwanted entity types.

using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Serialization;
using System.Linq;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {
        const float traverseLamdaDelta = 0.7F;      // Lambdas

        private bool IsViasInWire(Entity vias, Entity wire)
        {
            float delta = traverseLamdaDelta;

            PointF start = new PointF(wire.LambdaX, wire.LambdaY);
            PointF end = new PointF(wire.LambdaEndX, wire.LambdaEndY);

            RectangleF rect = new RectangleF(
                vias.LambdaX - delta, vias.LambdaY - delta,
                2 * delta, 2 * delta);

            return LineIntersectsRect(start, end, rect);
        }

        private void SelectTraverse(Entity source,
                                      int Tier,
                                      int TierMax,
                                      int Depth,
                                      bool Debug)
        {
            PointF[] rect1 = new PointF[4];
            PointF[] rect2 = new PointF[4];
            PointF restrictedStart = new PointF(0, 0);
            PointF restrictedEnd = new PointF(0, 0);

            if (Tier >= TierMax)
                return;

            if (Debug)
                source.Label = Depth.ToString();

            //
            // Wire joint Vias/Wire
            //

            if (source.IsWire())
            {
                float maxDist = traverseLamdaDelta;
                float dist;
                List<Entity> viases = new List<Entity>();

                //
                // Get not selected entities in delta range for Start point
                //
                // Get not selected entities in delta range for End point
                //

                float dx = Math.Abs(source.LambdaEndX - source.LambdaX);
                float dy = Math.Abs(source.LambdaEndY - source.LambdaY);
                bool Vert = dx < dy;

                foreach (Entity entity in GetEntities())
                {
                    if (source.TraverseBlackList != null && entity.TraverseBlackList != null)
                    {
                        if (source.TraverseBlackList.Contains(entity.Type) || entity.TraverseBlackList.Contains(source.Type))
                            continue;
                    }

                    if (entity.Selected == false)
                    {
                        //
                        // Wire -> Vias
                        // 

                        if (entity.IsVias())
                        {
                            if (IsViasInWire(entity, source))
                                viases.Add(entity);
                        }

                        //
                        // Wire -> Wire
                        //

                        else if (entity.IsWire())
                        {
                            PointF pointStart = new PointF(entity.LambdaX, entity.LambdaY);
                            PointF pointEnd = new PointF(entity.LambdaEndX, entity.LambdaEndY);

                            dist = (float)Math.Sqrt(Math.Pow(entity.LambdaX - source.LambdaX, 2) +
                                                     Math.Pow(entity.LambdaY - source.LambdaY, 2));

                            if (dist < maxDist)
                            {
                                SelectEntity(entity);
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                            dist = (float)Math.Sqrt(Math.Pow(entity.LambdaX - source.LambdaEndX, 2) +
                                                     Math.Pow(entity.LambdaY - source.LambdaEndY, 2));

                            if (dist < maxDist && entity.Selected == false)
                            {
                                SelectEntity(entity);
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                            dist = (float)Math.Sqrt(Math.Pow(entity.LambdaEndX - source.LambdaEndX, 2) +
                                                     Math.Pow(entity.LambdaEndY - source.LambdaEndY, 2));

                            if (dist < maxDist && entity.Selected == false)
                            {
                                SelectEntity(entity);
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                            dist = (float)Math.Sqrt(Math.Pow(entity.LambdaEndX - source.LambdaX, 2) +
                                                     Math.Pow(entity.LambdaEndY - source.LambdaY, 2));

                            if (dist < maxDist && entity.Selected == false)
                            {
                                SelectEntity(entity);
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                        }
                    }
                }           // foreach


                //
                // Process viases
                //

                foreach (Entity entity in viases)
                {
                    SelectEntity(entity);
                    SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                }

            }

            //
            // Vias joint Cell/Wire
            //

            else if (source.IsVias())
            {
                PointF point = new PointF(source.LambdaX, source.LambdaY);
                PointF[] rect = new PointF[4];
                List<Entity> wires = new List<Entity>();
                List<Entity> cells = new List<Entity>();

                //
                // Collect all wires/cells by vias intersections
                //

                foreach (Entity entity in GetEntities())
                {
                    if (source.TraverseBlackList != null && entity.TraverseBlackList != null)
                    {
                        if (source.TraverseBlackList.Contains(entity.Type) || entity.TraverseBlackList.Contains(source.Type))
                            continue;
                    }

                    if (entity.Selected == false)
                    {
                        //
                        // Vias -> Wire
                        //

                        if (entity.IsWire())
                        {
                            if (IsViasInWire(source, entity))
                                wires.Add(entity);
                        }

                        //
                        // Vias -> Cell
                        //

                        else if (entity.IsCell())
                        {
                            rect[0].X = entity.LambdaX;
                            rect[0].Y = entity.LambdaY;

                            rect[1].X = entity.LambdaX;
                            rect[1].Y = entity.LambdaY + entity.LambdaHeight;

                            rect[2].X = entity.LambdaX + entity.LambdaWidth;
                            rect[2].Y = entity.LambdaY + entity.LambdaHeight;

                            rect[3].X = entity.LambdaX + entity.LambdaWidth;
                            rect[3].Y = entity.LambdaY;

                            if (PointInPoly(rect, point))
                                cells.Add(entity);
                        }
                    }
                }

                //
                // Process
                //

                foreach (Entity entity in wires)
                {
                    SelectEntity(entity);
                    SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);

                    //
                    // Only single child
                    //

                    break;
                }

                foreach (Entity entity in cells)
                {
                    SelectEntity(entity);
                    SelectTraverse(entity, Tier + 1, TierMax, Depth + 1, Debug);

                    //
                    // Only single child
                    //

                    break;
                }

            }

            //
            // Cell joint Vias
            //

            else if (source.IsCell())
            {
                PointF[] rect = new PointF[4];
                List<Entity> viases = new List<Entity>();

                rect[0].X = source.LambdaX;
                rect[0].Y = source.LambdaY;

                rect[1].X = source.LambdaX;
                rect[1].Y = source.LambdaY + source.LambdaHeight;

                rect[2].X = source.LambdaX + source.LambdaWidth;
                rect[2].Y = source.LambdaY + source.LambdaHeight;

                rect[3].X = source.LambdaX + source.LambdaWidth;
                rect[3].Y = source.LambdaY;

                //
                // Add Output vias intersecting given cell
                //

                foreach (Entity entity in GetEntities())
                {
                    if (source.TraverseBlackList != null && entity.TraverseBlackList != null)
                    {
                        if (source.TraverseBlackList.Contains(entity.Type) || entity.TraverseBlackList.Contains(source.Type))
                            continue;
                    }

                    if (entity.Selected == false &&
                        (entity.Type == EntityType.ViasOutput || entity.Type == EntityType.ViasInout))
                    {
                        PointF point = new PointF(entity.LambdaX, entity.LambdaY);

                        if (PointInPoly(rect, point))
                            viases.Add(entity);
                    }
                }

                //
                // Process added viases
                //

                foreach (Entity entity in viases)
                {
                    SelectEntity(entity);
                    SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                }
            }
        }

        public void TraversalSelection(int TierMax)
        {
            List<Entity> selectedEnts = new List<Entity>();

            foreach (Entity entity in GetEntities())
            {
                if ((entity.IsCell() || entity.IsVias() || entity.IsWire())
                     && entity.Selected)
                    selectedEnts.Add(entity);
            }

            foreach (Entity entity in selectedEnts)
            {
                SelectTraverse(entity, 0, TierMax, 0, false);
            }

            Invalidate();
        }

    }
}
