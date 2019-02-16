// See docs\wire_recognition.pdf

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
        // Wire Recognition
        //

        private int AverageSubImageColor(Image image, Rectangle srcRect)
        {
            Bitmap bmp = new Bitmap(1, 1);
            Color pixel = new Color();

            if (srcRect.X + srcRect.Width > image.Width)
                return 0;

            if (srcRect.Y + srcRect.Height > image.Height)
                return 0;

            if (srcRect.X < 0 || srcRect.Y < 0)
                return 0;

            using (Graphics gr = Graphics.FromImage(bmp))
            {
                gr.InterpolationMode = InterpolationMode.NearestNeighbor;
                gr.DrawImage(image, new Rectangle(0, 0, 1, 1), srcRect, GraphicsUnit.Pixel);
                pixel = bmp.GetPixel(0, 0);
                gr.Dispose();
            }

            int yc = (int)(0.2126f * (float)pixel.R +
                            0.752f * (float)pixel.G +
                            0.0722f * (float)pixel.B);
            yc = Math.Max(0, Math.Min(yc, 255));
            return yc;
        }

        private Point LambdaToImage(PointF imageScroll, PointF origin)
        {
            Point dest = new Point();
            dest.X = (int)((origin.X - imageScroll.X) * Lambda);
            dest.Y = (int)((origin.Y - imageScroll.Y) * Lambda);
            return dest;
        }

        private PointF ImageToLamda(PointF imageScroll, Point origin)
        {
            PointF dest = new PointF();
            dest.X = (float)origin.X / Lambda + imageScroll.X;
            dest.Y = (float)origin.Y / Lambda + imageScroll.Y;
            return dest;
        }

        public void WireRecognize()
        {
            Image sourceImage = null;
            PointF imageScroll = new PointF();
            int imageZoom;
            int gate = 10;

            //
            // Lock current image
            //

            switch (Mode)
            {
                default:
                    sourceImage = _imageOrig[0];
                    imageScroll = _imageScroll[0];
                    imageZoom = _imageZoom[0];
                    break;
                case EntityMode.ImageLayer1:
                    sourceImage = _imageOrig[1];
                    imageScroll = _imageScroll[1];
                    imageZoom = _imageZoom[1];
                    break;
                case EntityMode.ImageLayer2:
                    sourceImage = _imageOrig[2];
                    imageScroll = _imageScroll[2];
                    imageZoom = _imageZoom[2];
                    break;
            }

            if (sourceImage == null)
            {
                MessageBox.Show("Image not loaded");
                return;
            }

            //
            // Grab first selected wire
            //

            Entity wire = FirstSelectedWire();

            if (wire == null)
            {
                MessageBox.Show("No wire selected");
                return;
            }

            //
            // Determine initial conditions
            //

            float dx = Math.Abs(wire.LambdaEndX - wire.LambdaX);
            float dy = Math.Abs(wire.LambdaEndY - wire.LambdaY);
            bool Horz = dx > dy;

            PointF startLambda = new PointF(wire.LambdaX, wire.LambdaY);
            Point start = LambdaToImage(imageScroll, startLambda);

            PointF endLambda = new PointF(wire.LambdaEndX, wire.LambdaEndY);
            Point end = LambdaToImage(imageScroll, endLambda);

            Rectangle sourceRect = new Rectangle(start.X,
                                         start.Y - 1,
                                         end.X - start.X,
                                         2);

            int avgColor = AverageSubImageColor(sourceImage, sourceRect);

            //MessageBox.Show("Rect: " + sourceRect.ToString() + "\n" + "Avg Color: " + avgColor.ToString());

            //
            // Propagate end
            //

            int saved_x = sourceRect.X;

            while (true)
            {
                sourceRect.X += WireBaseSize;

                int col = AverageSubImageColor(sourceImage, sourceRect);

                int diff = Math.Abs(avgColor - col);
                if (diff > gate)
                    break;

                //
                // Other wire hit test
                //
            }

            Point endPoint = new Point(sourceRect.X, sourceRect.Y);
            PointF lambda = ImageToLamda(imageScroll, endPoint);
            wire.LambdaEndX = lambda.X;

            //
            // Propagate start
            //

            sourceRect.X = saved_x;

            while (true)
            {
                sourceRect.X -= WireBaseSize;

                int col = AverageSubImageColor(sourceImage, sourceRect);

                int diff = Math.Abs(avgColor - col);
                if (diff > gate)
                    break;

                //
                // Other wire hit test
                //
            }

            endPoint.X = sourceRect.X;
            endPoint.Y = sourceRect.Y;
            lambda = ImageToLamda(imageScroll, endPoint);
            wire.LambdaX = lambda.X;

            Invalidate();
        }

    }
}
