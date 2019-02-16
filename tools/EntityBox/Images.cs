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
        private Image ToGrayscale(Image source)
        {
            if (grayscale == true)
            {
                Bitmap grayscaleBitmap = ColorToGrayscale(new Bitmap(source));
                source.Dispose();
                GC.Collect();
                return (Image)grayscaleBitmap;
            }
            else
                return source;
        }

        public static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.HighQuality;
                graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                graphics.SmoothingMode = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }

                graphics.Dispose();
            }

            return destImage;
        }

        //
        // Scene size and origin in screen coordinate space
        //

        private Point DetermineSceneSize(out Point origin)
        {
            Point point = new Point(0, 0);
            Point originOut = new Point(0, 0);

            float savedScrollX = 0, savedScrollY = 0;
            int savedZoom = 0;

            savedScrollX = _ScrollX;
            savedScrollY = _ScrollY;
            savedZoom = _zoom;

            _ScrollX = 0;
            _ScrollY = 0;
            _zoom = 100;

            //
            // Space occupied by Image layers
            //

            for (int n = 2; n >= 0; n--)
            {
                if (_imageOrig[n] != null && HideImage == false)
                {
                    Point offset = LambdaToScreen(_imageScroll[n].X, _imageScroll[n].Y);

                    if (offset.X < originOut.X)
                        originOut.X = offset.X;

                    if (offset.Y < originOut.Y)
                        originOut.Y = offset.Y;

                    float imgZf = (float)_imageZoom[n] / 100F;

                    int rightSide = (int)((float)_imageOrig[n].Width * imgZf) + offset.X;
                    int bottomSide = (int)((float)_imageOrig[n].Height * imgZf) + offset.Y;

                    if (rightSide > point.X)
                        point.X = rightSide;

                    if (bottomSide > point.Y)
                        point.Y = bottomSide;
                }
            }

            //
            // Space occupied by Entities
            //

            if (Lambda > 0)
            {
                foreach (Entity entity in GetEntities())
                {
                    Point screenCoords = new Point();

                    //
                    // Bottom Right Bounds
                    //

                    if (entity.IsWire())
                    {
                        screenCoords = LambdaToScreen(Math.Max(entity.LambdaX, entity.LambdaEndX),
                                                        Math.Max(entity.LambdaY, entity.LambdaEndY));
                        screenCoords.X += WireBaseSize;
                        screenCoords.Y += WireBaseSize;
                    }
                    else if (entity.IsCell())
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX + entity.LambdaWidth,
                                                        entity.LambdaY + entity.LambdaHeight);
                    }
                    else if (entity.IsRegion())
                    {
                        screenCoords.X = int.MinValue;
                        screenCoords.Y = int.MinValue;

                        foreach (PointF p in entity.PathPoints)
                        {
                            Point sp = LambdaToScreen(p.X, p.Y);

                            if (sp.X > screenCoords.X)
                            {
                                screenCoords.X = sp.X;
                            }

                            if (sp.Y > screenCoords.Y)
                            {
                                screenCoords.Y = sp.Y;
                            }
                        }

                        screenCoords.X += 2 * ViasBaseSize;
                        screenCoords.Y += 2 * ViasBaseSize;
                    }
                    else
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                        screenCoords.X += ViasBaseSize;
                        screenCoords.Y += ViasBaseSize;
                    }

                    if (screenCoords.X > point.X)
                        point.X = screenCoords.X;

                    if (screenCoords.Y > point.Y)
                        point.Y = screenCoords.Y;

                    //
                    // Top Left Bounds
                    //

                    if (entity.IsWire())
                    {
                        screenCoords = LambdaToScreen(Math.Min(entity.LambdaX, entity.LambdaEndX),
                                                        Math.Min(entity.LambdaY, entity.LambdaEndY));
                        screenCoords.X -= 2 * WireBaseSize;
                        screenCoords.Y -= 2 * WireBaseSize;
                    }
                    else if (entity.IsCell())
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX,
                                                        entity.LambdaY);
                    }
                    else if (entity.IsRegion())
                    {
                        screenCoords.X = int.MaxValue;
                        screenCoords.Y = int.MaxValue;

                        foreach (PointF p in entity.PathPoints)
                        {
                            Point sp = LambdaToScreen(p.X, p.Y);

                            if (sp.X < screenCoords.X)
                            {
                                screenCoords.X = sp.X;
                            }

                            if (sp.Y < screenCoords.Y)
                            {
                                screenCoords.Y = sp.Y;
                            }
                        }

                        screenCoords.X -= 2 * ViasBaseSize;
                        screenCoords.Y -= 2 * ViasBaseSize;
                    }
                    else
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                        screenCoords.X -= 2 * ViasBaseSize;
                        screenCoords.Y -= 2 * ViasBaseSize;
                    }

                    if (screenCoords.X < originOut.X)
                        originOut.X = screenCoords.X;

                    if (screenCoords.Y < originOut.Y)
                        originOut.Y = screenCoords.Y;
                }
            }

            _ScrollX = savedScrollX;
            _ScrollY = savedScrollY;
            _zoom = savedZoom;

            origin = originOut;

            return point;
        }

        public void SaveSceneAsImage(string FileName)
        {
            ImageFormat imageFormat;
            string ext;
            Point origin;
            Point sceneSize = DetermineSceneSize(out origin);

            int bitmapWidth = sceneSize.X - origin.X;
            int bitmapHeight = sceneSize.Y - origin.Y;

            Bitmap bitmap = new Bitmap(bitmapWidth, bitmapHeight, PixelFormat.Format16bppRgb565);

            Graphics gr = Graphics.FromImage(bitmap);

            DrawScene(gr, sceneSize.X, sceneSize.Y, true, origin);

            ext = Path.GetExtension(FileName);

            if (ext.ToLower() == ".jpg" || ext.ToLower() == ".jpeg")
                imageFormat = ImageFormat.Jpeg;
            else if (ext.ToLower() == ".png")
                imageFormat = ImageFormat.Png;
            else if (ext.ToLower() == ".bmp")
                imageFormat = ImageFormat.Bmp;
            else
                imageFormat = ImageFormat.Jpeg;

            bitmap.Save(FileName, imageFormat);

            bitmap.Dispose();
            gr.Dispose();
        }


        public static Bitmap ColorToGrayscale(Bitmap bmp)
        {
            int w = bmp.Width,
            h = bmp.Height,
            r, ic, oc, bmpStride, outputStride, bytesPerPixel;
            PixelFormat pfIn = bmp.PixelFormat;
            ColorPalette palette;
            Bitmap output;
            BitmapData bmpData, outputData;

            //Create the new bitmap
            output = new Bitmap(w, h, PixelFormat.Format8bppIndexed);

            //Build a grayscale color Palette
            palette = output.Palette;
            for (int i = 0; i < 256; i++)
            {
                Color tmp = Color.FromArgb(255, i, i, i);
                palette.Entries[i] = Color.FromArgb(255, i, i, i);
            }
            output.Palette = palette;

            //No need to convert formats if already in 8 bit
            if (pfIn == PixelFormat.Format8bppIndexed)
            {
                output = (Bitmap)bmp.Clone();

                //Make sure the palette is a grayscale palette and not some other
                //8-bit indexed palette
                output.Palette = palette;

                return output;
            }

            //Get the number of bytes per pixel
            switch (pfIn)
            {
                case PixelFormat.Format24bppRgb: bytesPerPixel = 3; break;
                case PixelFormat.Format32bppArgb: bytesPerPixel = 4; break;
                case PixelFormat.Format32bppRgb: bytesPerPixel = 4; break;
                default: throw new InvalidOperationException("Image format not supported");
            }

            //Lock the images
            bmpData = bmp.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadOnly,
            pfIn);
            outputData = output.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.WriteOnly,
            PixelFormat.Format8bppIndexed);
            bmpStride = bmpData.Stride;
            outputStride = outputData.Stride;

            //Traverse each pixel of the image
            unsafe
            {
                byte* bmpPtr = (byte*)bmpData.Scan0.ToPointer(),
                outputPtr = (byte*)outputData.Scan0.ToPointer();

                if (bytesPerPixel == 3)
                {
                    for (r = 0; r < h; r++)
                    {
                        for (ic = oc = 0; oc < w; ic += 3, ++oc)
                        {
                            int yc = (int)
                            (0.2126f * (float)bmpPtr[r * bmpStride + ic + 1] +
                            0.752f * (float)bmpPtr[r * bmpStride + ic + 2] +
                            0.0722f * (float)bmpPtr[r * bmpStride + ic + 3]);

                            yc = Math.Max(0, Math.Min(yc, 255));

                            outputPtr[r * outputStride + oc] = (byte)yc;
                        }
                    }
                }
                else //bytesPerPixel == 4
                {
                    for (r = 0; r < h; r++)
                    {
                        for (ic = oc = 0; oc < w; ic += 4, ++oc)
                        {
                            int yc = (int)
                            (0.2126f * (float)bmpPtr[r * bmpStride + ic + 1] +
                            0.752f * (float)bmpPtr[r * bmpStride + ic + 2] +
                            0.0722f * (float)bmpPtr[r * bmpStride + ic + 3]);

                            yc = Math.Max(0, Math.Min(yc, 255));

                            outputPtr[r * outputStride + oc] = (byte)yc;
                        }
                    }
                }
            }

            //Unlock the images
            bmp.UnlockBits(bmpData);
            output.UnlockBits(outputData);

            return output;
        }

        public void LoadImage(Image image)
        {
            switch (drawMode)
            {
                case EntityMode.ImageLayer0:
                default:
                    Image0 = image;
                    break;
                case EntityMode.ImageLayer1:
                    Image1 = image;
                    break;
                case EntityMode.ImageLayer2:
                    Image2 = image;
                    break;
            }
        }

        public void UnloadImage()
        {
            switch (drawMode)
            {
                case EntityMode.ImageLayer0:
                default:
                    _imageOrig[0].Dispose();
                    _imageOrig[0] = null;
                    GC.Collect();
                    Invalidate();
                    break;
                case EntityMode.ImageLayer1:
                    _imageOrig[1].Dispose();
                    _imageOrig[1] = null;
                    GC.Collect();
                    Invalidate();
                    break;
                case EntityMode.ImageLayer2:
                    _imageOrig[2].Dispose();
                    _imageOrig[2] = null;
                    GC.Collect();
                    Invalidate();
                    break;
            }
        }


    }
}
