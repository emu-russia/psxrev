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
        // Coordinate space conversion
        //
        // lx = (sx - scroll) / (zoom * lambda)
        //
        // sx = lx * zoom * lambda + scroll
        //

        public PointF ScreenToLambda(int ScreenX, int ScreenY)
        {
            PointF point = new PointF(0.0F, 0.0F);
            float zf = (float)Zoom / 100F;

            point.X = (float)ScreenX / (zf * Lambda) - ScrollX;
            point.Y = (float)ScreenY / (zf * Lambda) - ScrollY;

            return point;
        }

        public PointF ImageToLambda(int ImageX, int ImageY)
        {
            PointF point = new PointF(0.0F, 0.0F);

            point.X = (float)ImageX / Lambda;
            point.Y = (float)ImageY / Lambda;

            return point;
        }

        public Point LambdaToScreen(float LambdaX, float LambdaY)
        {
            Point point = new Point(0, 0);
            float zf = (float)Zoom / 100F;

            float x = (LambdaX + ScrollX) * (zf * Lambda);
            float y = (LambdaY + ScrollY) * (zf * Lambda);

            point.X = (int)x;
            point.Y = (int)y;

            return point;
        }

        public Point LambdaToImage(float LambdaX, float LambdaY)
        {
            Point point = new Point(0, 0);

            float x = LambdaX * Lambda;
            float y = LambdaY * Lambda;

            point.X = (int)x;
            point.Y = (int)y;

            return point;
        }

        public Point LambdaToImage(PointF imageScroll, PointF origin)
        {
            Point dest = new Point();
            dest.X = (int)((origin.X - imageScroll.X) * Lambda);
            dest.Y = (int)((origin.Y - imageScroll.Y) * Lambda);
            return dest;
        }

        public PointF ImageToLamda(PointF imageScroll, Point origin)
        {
            PointF dest = new PointF();
            dest.X = (float)origin.X / Lambda + imageScroll.X;
            dest.Y = (float)origin.Y / Lambda + imageScroll.Y;
            return dest;
        }

        // Detect intersection of various objects

        public static bool LineIntersectsRect(PointF p1, PointF p2, RectangleF r)
        {
            return LineIntersectsLine(p1, p2, new PointF(r.X, r.Y), new PointF(r.X + r.Width, r.Y)) ||
                   LineIntersectsLine(p1, p2, new PointF(r.X + r.Width, r.Y), new PointF(r.X + r.Width, r.Y + r.Height)) ||
                   LineIntersectsLine(p1, p2, new PointF(r.X + r.Width, r.Y + r.Height), new PointF(r.X, r.Y + r.Height)) ||
                   LineIntersectsLine(p1, p2, new PointF(r.X, r.Y + r.Height), new PointF(r.X, r.Y)) ||
                   (r.Contains(p1) && r.Contains(p2));
        }

        private static bool LineIntersectsLine(PointF l1p1, PointF l1p2, PointF l2p1, PointF l2p2)
        {
            float q = (l1p1.Y - l2p1.Y) * (l2p2.X - l2p1.X) - (l1p1.X - l2p1.X) * (l2p2.Y - l2p1.Y);
            float d = (l1p2.X - l1p1.X) * (l2p2.Y - l2p1.Y) - (l1p2.Y - l1p1.Y) * (l2p2.X - l2p1.X);

            if (d == 0)
            {
                return false;
            }

            float r = q / d;

            q = (l1p1.Y - l2p1.Y) * (l1p2.X - l1p1.X) - (l1p1.X - l2p1.X) * (l1p2.Y - l1p1.Y);
            float s = q / d;

            if (r < 0 || r > 1 || s < 0 || s > 1)
            {
                return false;
            }

            return true;
        }

        private bool PointInPoly(PointF[] poly, PointF point)
        {
            int max_point = poly.Length - 1;
            float total_angle = GetAngle(
                poly[max_point].X, poly[max_point].Y,
                point.X, point.Y,
                poly[0].X, poly[0].Y);

            for (int i = 0; i < max_point; i++)
            {
                total_angle += GetAngle(
                    poly[i].X, poly[i].Y,
                    point.X, point.Y,
                    poly[i + 1].X, poly[i + 1].Y);
            }

            return (Math.Abs(total_angle) > 0.000001);
        }

        private float GetAngle(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float dot_product = DotProduct(Ax, Ay, Bx, By, Cx, Cy);

            float cross_product = CrossProductLength(Ax, Ay, Bx, By, Cx, Cy);

            return (float)Math.Atan2(cross_product, dot_product);
        }

        private float DotProduct(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float BAx = Ax - Bx;
            float BAy = Ay - By;
            float BCx = Cx - Bx;
            float BCy = Cy - By;

            return (BAx * BCx + BAy * BCy);
        }

        private float CrossProductLength(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float BAx = Ax - Bx;
            float BAy = Ay - By;
            float BCx = Cx - Bx;
            float BCy = Cy - By;

            return (BAx * BCy - BAy * BCx);
        }

        // Transformations

        private PointF RotatePoint(PointF point, double angle)
        {
            PointF rotated_point = new Point();
            double rad = angle * Math.PI / 180.0F;
            rotated_point.X = point.X * (float)Math.Cos(rad) - point.Y * (float)Math.Sin(rad);
            rotated_point.Y = point.X * (float)Math.Sin(rad) + point.Y * (float)Math.Cos(rad);
            return rotated_point;
        }

    }
}
