// Copy & Paste

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

        public void Copy()
        {
            copied.Clear();
            copied.AddRange(GetSelected());
            RecalcCopyOrigin();
        }

        private void RecalcCopyOrigin()
        {
            TopLeftCopied.X = float.PositiveInfinity;
            TopLeftCopied.Y = float.PositiveInfinity;

            foreach (Entity entity in copied)
            {
                if (entity.IsRegion())
                {
                    if (entity.PathPoints[0].X < TopLeftCopied.X)
                        TopLeftCopied.X = entity.PathPoints[0].X;

                    if (entity.PathPoints[0].Y < TopLeftCopied.Y)
                        TopLeftCopied.Y = entity.PathPoints[0].Y;
                }
                else
                {
                    if (entity.LambdaX < TopLeftCopied.X)
                        TopLeftCopied.X = entity.LambdaX;

                    if (entity.LambdaY < TopLeftCopied.Y)
                        TopLeftCopied.Y = entity.LambdaY;

                    if (entity.IsWire())
                    {
                        if (entity.LambdaEndX < TopLeftCopied.X)
                            TopLeftCopied.X = entity.LambdaEndX;

                        if (entity.LambdaEndY < TopLeftCopied.Y)
                            TopLeftCopied.Y = entity.LambdaEndY;
                    }
                }
            }

            Console.WriteLine("TopLeft Selected: X=" + TopLeftCopied.X.ToString() +
                               ", Y=" + TopLeftCopied.Y.ToString());
        }

        public void Paste()
        {
            PointF delta = ScreenToLambda(LastRMB.X, LastRMB.Y);

            foreach (Entity entity in copied)
            {
                Entity item = new Entity();

                item.Type = entity.Type;
                item.Label = entity.Label;
                item.ColorOverride = entity.ColorOverride;
                item.FontOverride = entity.FontOverride;
                item.WidthOverride = entity.WidthOverride;
                item.Priority = entity.Priority;

                item.LambdaX = entity.LambdaX - TopLeftCopied.X + delta.X;
                item.LambdaY = entity.LambdaY - TopLeftCopied.Y + delta.Y;
                item.LambdaEndX = entity.LambdaEndX - TopLeftCopied.X + delta.X;
                item.LambdaEndY = entity.LambdaEndY - TopLeftCopied.Y + delta.Y;

                if (entity.IsRegion())
                {
                    item.PathPoints = new List<PointF>();

                    for (int i = 0; i < entity.PathPoints.Count; i++)
                    {
                        PointF point = new PointF();

                        point.X = entity.PathPoints[i].X - TopLeftCopied.X + delta.X;
                        point.Y = entity.PathPoints[i].Y - TopLeftCopied.Y + delta.Y;

                        item.PathPoints.Add(point);
                    }
                }

                item.LambdaWidth = entity.LambdaWidth;
                item.LambdaHeight = entity.LambdaHeight;
                item.LabelAlignment = entity.LabelAlignment;
                item.Label = entity.Label;
                item.SetParentControl(this);
                item.parent = insertionNode;

                //
                // Select pasted items, so we can move it around
                //

                SelectEntity(item);

                insertionNode.Children.Add(item);
            }

            SortEntities();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);
        }


    }
}
