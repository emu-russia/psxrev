using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DerouteSharp
{
    /// <summary>
    /// The class aligns the coordinates of a collection of entities 
    /// </summary>
    class EntityAligner
    {
        public static void CenterFeatureEntities(PointF point, List<Entity> entities)
        {
            if (entities.Count == 0)
                return;

            // Find the left-top and right-bottom corner coordinates for all entities

            Entity pseudoRoot = new Entity();
            pseudoRoot.Children = entities;

            PointF topLeft = new PointF(float.MaxValue, float.MaxValue);
            FindTopLeftPointRecursive(pseudoRoot, ref topLeft);

            PointF bottomRight = new PointF(0, 0);
            FindBottomRightPointRecursive(pseudoRoot, ref bottomRight);

            float w = Math.Abs(bottomRight.X - topLeft.X);
            float h = Math.Abs(bottomRight.Y - topLeft.Y);

            // Move all entities to the new upper-left corner

            PointF newPos = new PointF(point.X - w/2, point.Y - h/2);
            PointF delta = new PointF(newPos.X - topLeft.X, newPos.Y - topLeft.Y);

            AdjustCoordsRecursive (pseudoRoot, delta);
        }

        private static void FindTopLeftPointRecursive(Entity parent, ref PointF point)
        {
            foreach (var entity in parent.Children)
            {
                if (entity.IsRegion())
                {
                    foreach (var pathPoint in entity.PathPoints)
                    {
                        if (pathPoint.X < point.X)
                            point.X = pathPoint.X;
                        if (pathPoint.Y < point.Y)
                            point.Y = pathPoint.Y;
                    }
                }
                else
                {
                    if (entity.LambdaX < point.X)
                        point.X = entity.LambdaX;
                    if (entity.LambdaY < point.Y)
                        point.Y = entity.LambdaY;

                    if (entity.IsWire())
                    {
                        if (entity.LambdaEndX < point.X)
                            point.X = entity.LambdaEndX;
                        if (entity.LambdaEndY < point.Y)
                            point.Y = entity.LambdaEndY;
                    }
                }

                FindTopLeftPointRecursive(entity, ref point);
            }
        }

        private static void FindBottomRightPointRecursive(Entity parent, ref PointF point)
        {
            foreach (var entity in parent.Children)
            {
                if (entity.IsRegion())
                {
                    foreach (var pathPoint in entity.PathPoints)
                    {
                        if (pathPoint.X > point.X)
                            point.X = pathPoint.X;
                        if (pathPoint.Y > point.Y)
                            point.Y = pathPoint.Y;
                    }
                }
                else
                {
                    if (entity.LambdaX > point.X)
                        point.X = entity.LambdaX;
                    if (entity.LambdaY > point.Y)
                        point.Y = entity.LambdaY;

                    if (entity.IsWire())
                    {
                        if (entity.LambdaEndX > point.X)
                            point.X = entity.LambdaEndX;
                        if (entity.LambdaEndY > point.Y)
                            point.Y = entity.LambdaEndY;
                    }
                }

                FindBottomRightPointRecursive(entity, ref point);
            }
        }

        private static void AdjustCoordsRecursive(Entity parent, PointF delta)
        {
            foreach (var entity in parent.Children)
            {
                if (entity.IsRegion())
                {
                    // TODO..
                }
                else
                {
                    entity.LambdaX += delta.X;
                    entity.LambdaY += delta.Y;

                    if (entity.IsWire())
                    {
                        entity.LambdaEndX += delta.X;
                        entity.LambdaEndY += delta.Y;
                    }
                }

                AdjustCoordsRecursive(entity, delta);
            }
        }

    }
}
