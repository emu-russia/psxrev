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

        //
        // Wire merger. This operation is potentially dangerous!!!
        //

        public void MergeSelectedWires(bool Vertical)
        {
            //
            // Grab selected 
            //

            List<Entity> selectedWires = GetSelectedWires();

            if (selectedWires.Count <= 1)
                return;

            //
            // Find left-most and right-most points + Determine average wire type
            //

            PointF left = new PointF(float.MaxValue, float.MaxValue);
            PointF right = new PointF(float.MinValue, float.MinValue);
            int[] types = { 0, 0, 0 };

            foreach (Entity entity in selectedWires)
            {
                if (Vertical == true)
                {
                    if (entity.LambdaY < left.Y)
                    {
                        left.X = entity.LambdaX;
                        left.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndY < left.Y)
                    {
                        left.X = entity.LambdaEndX;
                        left.Y = entity.LambdaEndY;
                    }
                    if (entity.LambdaY > right.Y)
                    {
                        right.X = entity.LambdaX;
                        right.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndY > right.Y)
                    {
                        right.X = entity.LambdaEndX;
                        right.Y = entity.LambdaEndY;
                    }
                }
                else
                {
                    if (entity.LambdaX < left.X)
                    {
                        left.X = entity.LambdaX;
                        left.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndX < left.X)
                    {
                        left.X = entity.LambdaEndX;
                        left.Y = entity.LambdaEndY;
                    }
                    if (entity.LambdaX > right.X)
                    {
                        right.X = entity.LambdaX;
                        right.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndX > right.X)
                    {
                        right.X = entity.LambdaEndX;
                        right.Y = entity.LambdaEndY;
                    }
                }

                switch (entity.Type)
                {
                    case EntityType.WireGround:
                        types[0]++;
                        break;
                    case EntityType.WireInterconnect:
                        types[1]++;
                        break;
                    case EntityType.WirePower:
                        types[2]++;
                        break;
                }
            }

            //
            // Remove selected / join label names
            //

            string Label = "";

            foreach (Entity entity in selectedWires)
            {
                Label += entity.Label;
                entity.parent.Children.Remove(entity);
            }

            //
            // Add merger
            //

            Point start = LambdaToScreen(left.X, left.Y);
            Point end = LambdaToScreen(right.X, right.Y);

            int maxValue = types.Max();
            int index = types.ToList().IndexOf(maxValue);

            EntityType type;

            switch (index)
            {
                case 0:
                    type = EntityType.WireGround;
                    break;
                case 1:
                default:
                    type = EntityType.WireInterconnect;
                    break;
                case 2:
                    type = EntityType.WirePower;
                    break;
            }

            Entity mergedWire = AddWire(type, start.X, start.Y, end.X, end.Y);

            if (mergedWire != null)
            {
                mergedWire.Label = Label;
            }

            Invalidate();
        }


    }
}
