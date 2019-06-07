// Wire extend

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

        private Entity FirstSelectedWire()
        {
            Entity wire = null;

            foreach (Entity entity in GetEntities())
            {
                if (entity.IsWire() && entity.Selected)
                {
                    wire = entity;
                    break;
                }
            }

            return wire;
        }

        public void WireExtendHead()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = (float)Math.Atan(wire.Tangent());

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaX += (float)Math.Cos(alpha);
                wire.LambdaY += (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaX -= (float)Math.Cos(alpha);
                wire.LambdaY -= (float)Math.Sin(alpha);
            }

            Invalidate();
        }

        public void WireExtendTail()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = (float)Math.Atan(wire.Tangent());

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaEndX -= (float)Math.Cos(alpha);
                wire.LambdaEndY -= (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaEndX += (float)Math.Cos(alpha);
                wire.LambdaEndY += (float)Math.Sin(alpha);
            }

            Invalidate();
        }

        public void WireShortenHead()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = (float)Math.Atan(wire.Tangent());

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaX -= (float)Math.Cos(alpha);
                wire.LambdaY -= (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaX += (float)Math.Cos(alpha);
                wire.LambdaY += (float)Math.Sin(alpha);
            }

            Invalidate();
        }

        public void WireShortenTail()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = (float)Math.Atan(wire.Tangent());

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaEndX += (float)Math.Cos(alpha);
                wire.LambdaEndY += (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaEndX -= (float)Math.Cos(alpha);
                wire.LambdaEndY -= (float)Math.Sin(alpha);
            }

            Invalidate();
        }

    }
}
