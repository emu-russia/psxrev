// Key input handling

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

        protected override void OnKeyUp(KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
                DeleteSelected();

            else if (e.KeyCode == Keys.Escape)
                RemoveSelection();

            else if (e.KeyCode == Keys.Home)
            {
                _ScrollX = 0;
                _ScrollY = 0;
                Invalidate();

                if (OnScrollChanged != null)
                    OnScrollChanged(this, EventArgs.Empty);
            }

            else if ((e.KeyCode == Keys.Right ||
                        e.KeyCode == Keys.Left ||
                        e.KeyCode == Keys.Up ||
                        e.KeyCode == Keys.Down) && Mode == EntityMode.Selection)
            {
                bool NeedUpdate = false;
                float deltaX = 0;
                float deltaY = 0;

                switch (e.KeyCode)
                {
                    case Keys.Right:
                        deltaX = +0.1F;
                        deltaY = 0;
                        break;
                    case Keys.Left:
                        deltaX = -0.1F;
                        deltaY = 0;
                        break;
                    case Keys.Up:
                        deltaX = 0;
                        deltaY = -0.1F;
                        break;
                    case Keys.Down:
                        deltaX = 0;
                        deltaY = +0.1F;
                        break;
                }

                foreach (Entity entity in GetEntities())
                {
                    if (entity.Selected)
                    {
                        if (entity.IsRegion())
                        {
                            entity.LambdaX += deltaX;
                            entity.LambdaY += deltaY;

                            List<PointF> points = new List<PointF>();

                            foreach (PointF point in entity.PathPoints)
                            {
                                PointF newPoint = new PointF();

                                newPoint.X = point.X + deltaX;
                                newPoint.Y = point.Y + deltaY;

                                points.Add(newPoint);
                            }

                            entity.PathPoints = points;

                            NeedUpdate = true;
                        }
                        else
                        {
                            entity.LambdaX += deltaX;
                            entity.LambdaY += deltaY;
                            entity.LambdaEndX += deltaX;
                            entity.LambdaEndY += deltaY;
                            NeedUpdate = true;
                        }
                    }
                }

                if (NeedUpdate)
                    Invalidate();
            }

            base.OnKeyUp(e);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            if (e.KeyCode == Keys.C && e.Control)
            {
                Copy();
            }
            else if (e.KeyCode == Keys.V && e.Control)
            {
                Paste();
            }

            base.OnKeyDown(e);
        }

    }
}
