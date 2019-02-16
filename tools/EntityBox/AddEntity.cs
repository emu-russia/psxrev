// Add various entities by user

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

        private string GenBeaconName()
        {
            int NumBeacons = GetBeaconCount() + 1;
            return "Beacon" + NumBeacons.ToString();
        }

        private Entity AddBeacon(int ScreenX, int ScreenY)
        {
            Entity item = new Entity();

            PointF point = ScreenToLambda(ScreenX, ScreenY);

            item.Label = GenBeaconName();
            item.LambdaX = point.X;
            item.LambdaY = point.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = EntityType.Beacon;
            item.ColorOverride = Color.Black;
            item.Priority = BeaconPriority;
            item.FontOverride = null;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);

            return item;
        }

        public Entity AddVias(EntityType Type, int ScreenX, int ScreenY, Color debugColor)
        {
            Entity item = new Entity();

            PointF point = ScreenToLambda(ScreenX, ScreenY);

            //
            // Get rid of clutching viases
            //

            List<Entity> _entities = GetEntities();

            if (debugColor == Color.Black)
            {
                foreach (Entity entity in _entities)
                {
                    if (entity.IsVias())
                    {
                        float dist = (float)Math.Sqrt(Math.Pow(entity.LambdaX - point.X, 2) +
                                                       Math.Pow(entity.LambdaY - point.Y, 2));

                        if (dist <= 1.5F)
                            return null;
                    }
                }
            }

            item.Label = "";
            item.LambdaX = point.X;
            item.LambdaY = point.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = debugColor == Color.Black ? ViasOverrideColor : debugColor;
            item.Priority = ViasPriority;
            item.FontOverride = null;
            item.WidthOverride = 0;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);

            return item;
        }

        public Entity AddWire(EntityType Type, int StartX, int StartY, int EndX, int EndY)
        {
            Entity item = new Entity();

            PointF point1 = ScreenToLambda(StartX, StartY);
            PointF point2 = ScreenToLambda(EndX, EndY);

            float len = (float)Math.Sqrt(Math.Pow(point2.X - point1.X, 2) +
                                          Math.Pow(point2.Y - point1.Y, 2));

            if (len < 1.0F)
            {
                Invalidate();
                return null;
            }

            item.Label = "";
            item.LambdaX = point1.X;
            item.LambdaY = point1.Y;
            item.LambdaEndX = point2.X;
            item.LambdaEndY = point2.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = WireOverrideColor;
            item.Priority = WirePriority;
            item.FontOverride = null;
            item.WidthOverride = 0;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);

            return item;
        }

        public Entity AddWireOnImage(EntityType Type, int StartX, int StartY, int EndX, int EndY)
        {
            Entity item = new Entity();

            PointF point1 = ImageToLambda(StartX, StartY);
            PointF point2 = ImageToLambda(EndX, EndY);

            float len = (float)Math.Sqrt(Math.Pow(point2.X - point1.X, 2) +
                                          Math.Pow(point2.Y - point1.Y, 2));

            if (len < 1.0F)
            {
                Invalidate();
                return null;
            }

            item.Label = "";
            item.LambdaX = point1.X;
            item.LambdaY = point1.Y;
            item.LambdaEndX = point2.X;
            item.LambdaEndY = point2.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = WireOverrideColor;
            item.Priority = WirePriority;
            item.FontOverride = null;
            item.WidthOverride = 0;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);

            return item;
        }

        public Entity AddCell(EntityType Type, int StartX, int StartY, int EndX, int EndY)
        {
            Entity item = new Entity();

            PointF point1 = ScreenToLambda(StartX, StartY);
            PointF point2 = ScreenToLambda(EndX, EndY);

            PointF originPos = new PointF();
            PointF size = new PointF();

            size.X = Math.Abs(point2.X - point1.X);
            size.Y = Math.Abs(point2.Y - point1.Y);

            float square = size.X * size.Y;

            if (square < 4.0F)
            {
                Invalidate();
                return null;
            }

            if (point2.X > point1.X)
            {
                if (point2.Y > point1.Y)
                {
                    originPos.X = point1.X;
                    originPos.Y = point1.Y;
                }
                else
                {
                    originPos.X = point1.X;
                    originPos.Y = point2.Y;
                }
            }
            else
            {
                if (point2.Y > point1.Y)
                {
                    originPos.X = point2.X;
                    originPos.Y = point1.Y;
                }
                else
                {
                    originPos.X = point2.X;
                    originPos.Y = point2.Y;
                }
            }

            item.Label = "";
            item.LambdaX = originPos.X;
            item.LambdaY = originPos.Y;
            item.LambdaEndX = 1F;
            item.LambdaEndY = 1F;
            item.LambdaWidth = size.X;
            item.LambdaHeight = size.Y;
            item.Type = Type;
            item.ColorOverride = CellOverrideColor;
            item.Priority = CellPriority;
            item.FontOverride = null;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);

            return item;
        }

        //
        // Add region
        //

        public Entity AddRegion(List<Point> points, Color color)
        {
            //
            // Fill path (minimum 3 points)
            //

            if (points.Count < 3)
            {
                Invalidate();
                return null;
            }

            List<PointF> path = new List<PointF>();

            foreach (Point point in points)
            {
                PointF p = ScreenToLambda(point.X, point.Y);

                path.Add(p);
            }

            //
            // Add new region entity
            //

            Entity item = new Entity();

            item.Type = EntityType.Region;
            item.Label = "";
            item.LabelAlignment = TextAlignment.GlobalSettings;
            item.Priority = RegionPriority;
            item.Selected = false;
            item.PathPoints = path;
            item.LambdaX = path[0].X;
            item.LambdaY = path[0].Y;
            item.ColorOverride = color;
            item.FontOverride = null;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);

            return item;
        }

        //
        // Draw region between selected viases
        //

        public void DrawRegionBetweenSelectedViases()
        {
            //
            // Grab selected viases (minimum 3)
            //

            List<Entity> selected = GetSelectedVias();

            if (selected.Count < 3)
                return;

            //
            // Fill path
            //

            List<PointF> path = new List<PointF>();

            foreach (Entity entity in selected)
            {
                PointF point = new PointF();

                point.X = entity.LambdaX;
                point.Y = entity.LambdaY;

                path.Add(point);
            }

            //
            // Add new region entity
            //

            Entity item = new Entity();

            item.Type = EntityType.Region;
            item.Label = "";
            item.LabelAlignment = TextAlignment.GlobalSettings;
            item.Priority = RegionPriority;
            item.Selected = false;
            item.PathPoints = path;
            item.LambdaX = path[0].X;
            item.LambdaY = path[0].Y;
            item.ColorOverride = RegionOverrideColor;
            item.FontOverride = null;
            item.SetParentControl(this);
            item.parent = insertionNode;

            while (DrawInProgress) ;

            insertionNode.Children.Add(item);
            SortEntities();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            OnEntityAdd?.Invoke(this, item, EventArgs.Empty);
        }

        public void DrawWireBetweenSelectedViases()
        {
            //
            // Grab selected viases
            //

            List<Entity> selectedVias = GetSelectedVias();

            //
            // Sort by select timestamp
            //

            selectedVias = selectedVias.OrderBy(o => o.SelectTimeStamp).ToList();

            //
            // Connect viases by wires
            //

            if (selectedVias.Count >= 2)
            {
                Entity prevVias = null;

                foreach (Entity vias in selectedVias)
                {
                    if (prevVias == null)
                    {
                        prevVias = vias;
                        continue;
                    }

                    //
                    // Connect prevVias with current
                    //

                    Point Start = LambdaToScreen(prevVias.LambdaX, prevVias.LambdaY);
                    Point End = LambdaToScreen(vias.LambdaX, vias.LambdaY);

                    AddWire(EntityType.WireInterconnect,
                             Start.X, Start.Y, End.X, End.Y);

                    //
                    // Replace prev vias
                    //

                    prevVias = vias;
                }

                Invalidate();
            }
        }


    }
}
