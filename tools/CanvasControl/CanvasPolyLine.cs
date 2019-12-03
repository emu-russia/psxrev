/// Ломаная линия

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace CanvasControl
{
    public class CanvasPolyLine : CanvasItem
    {
        public CanvasPolyLine()
        {
            Category = ItemCategory.Edge;
            Width = 1;
        }

        public CanvasPolyLine(List<PointF> points)
        {
            Category = ItemCategory.Edge;
            Points = points;
            Width = 1;
        }

        public CanvasPolyLine(List<PointF> points, float width)
        {
            Category = ItemCategory.Edge;
            Points = points;
            Width = width;
        }

        public CanvasPolyLine(List<PointF> points, float width, Color color)
        {
            Category = ItemCategory.Edge;
            Points = points;
            Width = width;
            FrontColor = color;
        }

        public override void Draw(Graphics gr)
        {
            if (Points.Count < 2)
                return;

            float zf = (float)parentControl.Zoom / 100.0F;

            List<Point> translated = new List<Point>();

            foreach (var p in Points)
            {
                translated.Add(parentControl.WorldToScreen(p));
            }

            gr.DrawLines(new Pen(FrontColor, Width * zf), translated.ToArray());

            if (Selected)
            {
                gr.DrawLines(new Pen(Color.Lime, Width * zf * 2), translated.ToArray());
            }
        }

        public override bool HitTest(PointF point)
        {
            if (Points.Count < 2)
                return false;

            for (int i=0; i<Points.Count; i++)
            {
                if (i == 0)
                    continue;

                if (CanvasMath.PointOnLine(Points[i], Points[i - 1], point, Width))
                    return true;
            }

            return false;
        }

        public override bool BoxTest(RectangleF rect)
        {
            if (Points.Count < 2)
                return false;

            for (int i = 0; i < Points.Count; i++)
            {
                if (i == 0)
                    continue;

                if (CanvasMath.LineIntersectsRect(Points[i], Points[i-1], rect))
                    return true;
            }

            return false;
        }

    }
}
