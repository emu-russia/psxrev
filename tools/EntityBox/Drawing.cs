// Drawing

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
        // Drawing
        //

        private void DrawLambdaScale(Graphics gr)
        {
            if (hideLambdaMetrics)
                return;

            float scaleWidth = (int)Lambda * 5;

            scaleWidth *= (float)Zoom / 100.0F;

            Pen linePen = new Pen(Color.LightGray, 3);

            gr.DrawLine(linePen,
                         Width - scaleWidth - 5,
                         Height - 5,
                         Width - 5,
                         Height - 5);

            string label = "5λ";
            int labelWidth = (int)gr.MeasureString(label, this.Font).Width;

            gr.DrawString(label, this.Font, Brushes.Black,
                           this.Width - labelWidth - scaleWidth / 2,
                           this.Height - this.Font.Height - linePen.Width - 5);
        }

        private void DrawLambdaGrid(Graphics gr)
        {
            float x, y;

            if (Zoom <= 50)
                return;

            float scaleWidth = (int)Lambda * 5;

            PointF topLeft = ScreenToLambda(0, 0);
            PointF bottomRight = ScreenToLambda(Width, Height);

            for (y = 0; y < bottomRight.Y; y += Lambda)
            {
                for (x = 0; x < bottomRight.X; x += Lambda)
                {
                    Point screen = LambdaToScreen(x, y);
                    gr.FillRectangle(Brushes.LightGray, screen.X, screen.Y, 1, 1);
                }
            }

            for (y = 0; y >= topLeft.Y; y -= Lambda)
            {
                for (x = 0; x < bottomRight.X; x += Lambda)
                {
                    Point screen = LambdaToScreen(x, y);
                    gr.FillRectangle(Brushes.LightGray, screen.X, screen.Y, 1, 1);
                }
            }

            for (y = 0; y >= topLeft.Y; y -= Lambda)
            {
                for (x = 0; x >= topLeft.X; x -= Lambda)
                {
                    Point screen = LambdaToScreen(x, y);
                    gr.FillRectangle(Brushes.LightGray, screen.X, screen.Y, 1, 1);
                }
            }

            for (y = 0; y < bottomRight.Y; y += Lambda)
            {
                for (x = 0; x >= topLeft.X; x -= Lambda)
                {
                    Point screen = LambdaToScreen(x, y);
                    gr.FillRectangle(Brushes.LightGray, screen.X, screen.Y, 1, 1);
                }
            }
        }

        private void DrawOrigin(Graphics gr)
        {
            float zf = (float)Zoom / 100.0F;
            Point point = LambdaToScreen(0, 0);

            int centerX = point.X;
            int centerY = point.Y;
            int radius = (int)((float)ViasBaseSize * zf);

            gr.FillEllipse(new SolidBrush(Color.Red),
                            centerX - radius, centerY - radius,
                            radius + radius, radius + radius);
        }

        private void DrawRightMouseCross(Graphics gr)
        {
            int crossWidth = 16;

            if (LastRMB.X < 0 || LastRMB.Y < 0)
                return;

            Pen pen = new Pen(new SolidBrush(Color.Black));

            gr.DrawLine(pen, new Point(LastRMB.X - crossWidth / 2, LastRMB.Y - crossWidth / 2),
                            new Point(LastRMB.X + crossWidth / 2, LastRMB.Y + crossWidth / 2));
            gr.DrawLine(pen, new Point(LastRMB.X - crossWidth / 2, LastRMB.Y + crossWidth / 2),
                            new Point(LastRMB.X + crossWidth / 2, LastRMB.Y - crossWidth / 2));
        }

        private void DrawEntityRecursive(Entity parent, Graphics gr)
        {
            if (!parent.Visible)
                return;

            DrawEntity(parent, gr);

            foreach(var entity in parent.Children)
            {
                DrawEntityRecursive(entity, gr);
            }
        }

        private void DrawEntity(Entity entity, Graphics gr)
        {
            Color viasColor;
            int centerX;
            int centerY;
            int radius;
            Color wireColor;
            int startX;
            int startY;
            int endX;
            int endY;
            float zf = (float)Zoom / 100.0F;
            Color cellColor;

            switch (entity.Type)
            {
                //
                // Vias
                //

                case EntityType.ViasConnect:
                case EntityType.ViasFloating:
                case EntityType.ViasGround:
                case EntityType.ViasInout:
                case EntityType.ViasInput:
                case EntityType.ViasOutput:
                case EntityType.ViasPower:

                    if (hideVias == true)
                        break;

                    if (entity.Type == EntityType.ViasConnect)
                        viasColor = ViasConnectColor;
                    else if (entity.Type == EntityType.ViasFloating)
                        viasColor = ViasFloatingColor;
                    else if (entity.Type == EntityType.ViasGround)
                        viasColor = ViasGroundColor;
                    else if (entity.Type == EntityType.ViasInout)
                        viasColor = ViasInoutColor;
                    else if (entity.Type == EntityType.ViasInput)
                        viasColor = ViasInputColor;
                    else if (entity.Type == EntityType.ViasOutput)
                        viasColor = ViasOutputColor;
                    else if (entity.Type == EntityType.ViasPower)
                        viasColor = ViasPowerColor;
                    else
                        viasColor = Color.Black;

                    if (entity.ColorOverride != Color.Black)
                        viasColor = entity.ColorOverride;

                    viasColor = Color.FromArgb(ViasOpacity, viasColor);

                    Point point = LambdaToScreen(entity.LambdaX, entity.LambdaY);

                    centerX = point.X;
                    centerY = point.Y;
                    int viasSize = ViasBaseSize;
                    if (entity.WidthOverride > 0)
                        viasSize = entity.WidthOverride;
                    radius = (int)((float)viasSize * zf);

                    if (ViasShape == ViasShape.Round)
                    {
                        if (entity.Selected == true)
                        {
                            radius += (int)Lambda;

                            gr.FillEllipse(new SolidBrush(SelectionColor),
                                            centerX - radius, centerY - radius,
                                            radius + radius, radius + radius);

                            radius -= (int)Lambda;
                        }

                        gr.FillEllipse(new SolidBrush(viasColor),
                                        centerX - radius, centerY - radius,
                                        radius + radius, radius + radius);
                    }
                    else
                    {
                        if (entity.Selected == true)
                        {
                            radius += (int)Lambda;

                            gr.FillRectangle(new SolidBrush(SelectionColor),
                                               centerX - radius, centerY - radius,
                                               2 * radius, 2 * radius);

                            radius -= (int)Lambda;
                        }

                        gr.FillRectangle(new SolidBrush(viasColor),
                                           centerX - radius, centerY - radius,
                                           2 * radius, 2 * radius);
                    }

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0 && Zoom > 50)
                    {
                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = ViasTextAlignment;

                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

                        Point origin = new Point(centerX, centerY);

                        switch (align)
                        {
                            case TextAlignment.Top:
                            default:
                                origin.Y = centerY - radius - (int)(textSize.Height * zf);
                                break;

                            case TextAlignment.Bottom:
                                origin.Y = centerY + radius;
                                break;

                            case TextAlignment.TopLeft:
                            case TextAlignment.BottomLeft:
                                origin.Y = centerY + 2 * radius - (int)(textSize.Height * zf);
                                break;

                            case TextAlignment.TopRight:
                            case TextAlignment.BottomRight:
                                origin.Y = centerY + 2 * radius - (int)(textSize.Height * zf);
                                break;
                        }

                        switch (align)
                        {
                            case TextAlignment.Top:
                            case TextAlignment.Bottom:
                            default:
                                origin.X = centerX - (int)(textSize.Width * zf / 2);
                                break;

                            case TextAlignment.TopLeft:
                            case TextAlignment.BottomLeft:
                                origin.X = centerX - radius - (int)(textSize.Width * zf);
                                break;

                            case TextAlignment.TopRight:
                            case TextAlignment.BottomRight:
                                origin.X = centerX + radius;
                                break;
                        }

                        gr.TranslateTransform(origin.X, origin.Y);
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor), 0, 0);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Wires
                //

                case EntityType.WireGround:
                case EntityType.WirePower:
                case EntityType.WireInterconnect:

                    if (hideWires == true)
                        break;

                    if (entity.Type == EntityType.WireGround)
                        wireColor = WireGroundColor;
                    else if (entity.Type == EntityType.WirePower)
                        wireColor = WirePowerColor;
                    else if (entity.Type == EntityType.WireInterconnect)
                        wireColor = WireInterconnectColor;
                    else
                        wireColor = Color.Blue;

                    if (entity.ColorOverride != Color.Black)
                        wireColor = entity.ColorOverride;

                    wireColor = Color.FromArgb(WireOpacity, wireColor);

                    Point point1 = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point point2 = LambdaToScreen(entity.LambdaEndX, entity.LambdaEndY);

                    startX = point1.X;
                    startY = point1.Y;
                    endX = point2.X;
                    endY = point2.Y;

                    int wireSize = WireBaseSize;
                    if (entity.WidthOverride > 0)
                        wireSize = entity.WidthOverride;

                    if (entity.Selected == true)
                    {
                        gr.DrawLine(new Pen(SelectionColor, (float)wireSize * zf + (int)Lambda),
                                     startX, startY,
                                     endX, endY);
                    }

                    //gr.CompositingQuality = CompositingQuality;
                    //gr.CompositingMode = CompositingMode.SourceCopy;


                    gr.DrawLine(new Pen(wireColor, (float)wireSize * zf),
                                 startX, startY,
                                 endX, endY);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0 && Zoom > 50)
                    {
                        Point start = new Point(startX, startY);
                        Point end = new Point(endX, endY);
                        Point temp;

                        if (startX == endX && startY == endY)
                            break;

                        if (end.X < start.X)
                        {
                            temp = start;
                            start = end;
                            end = temp;
                        }

                        int a = end.Y - start.Y;
                        int b = end.X - start.X;
                        float Tga = (float)a / (float)b;
                        float alpha = (float)Math.Atan(Tga);

                        int wireLength = (int)(Math.Sqrt(Math.Pow(end.X - start.X, 2) +
                                                         Math.Pow(end.Y - start.Y, 2)) / (double)zf);

                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

                        int origin;

                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = WireTextAlignment;

                        switch (align)
                        {
                            case TextAlignment.BottomLeft:
                            case TextAlignment.TopLeft:
                            default:
                                origin = (int)(textSize.Width / entity.Label.Length);
                                break;
                            case TextAlignment.Top:
                            case TextAlignment.Bottom:
                                origin = wireLength / 2 - (int)textSize.Width / 2;
                                break;
                            case TextAlignment.BottomRight:
                            case TextAlignment.TopRight:
                                origin = wireLength - (int)textSize.Width - (int)(textSize.Width / entity.Label.Length);
                                break;
                        }

                        gr.TranslateTransform(start.X, start.Y);
                        gr.RotateTransform((float)(180.0F * alpha / Math.PI));
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor),
                                       origin, -textSize.Height / 2);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Cells and Units
                //

                case EntityType.CellNot:
                case EntityType.CellBuffer:
                case EntityType.CellMux:
                case EntityType.CellLogic:
                case EntityType.CellAdder:
                case EntityType.CellBusSupp:
                case EntityType.CellFlipFlop:
                case EntityType.CellLatch:
                case EntityType.CellOther:
                case EntityType.UnitRegfile:
                case EntityType.UnitMemory:
                case EntityType.UnitCustom:

                    if (hideCells == true)
                        break;

                    if (entity.Type == EntityType.CellNot)
                        cellColor = CellNotColor;
                    else if (entity.Type == EntityType.CellBuffer)
                        cellColor = CellBufferColor;
                    else if (entity.Type == EntityType.CellMux)
                        cellColor = CellMuxColor;
                    else if (entity.Type == EntityType.CellLogic)
                        cellColor = CellLogicColor;
                    else if (entity.Type == EntityType.CellAdder)
                        cellColor = CellAdderColor;
                    else if (entity.Type == EntityType.CellBusSupp)
                        cellColor = CellBusSuppColor;
                    else if (entity.Type == EntityType.CellFlipFlop)
                        cellColor = CellFlipFlopColor;
                    else if (entity.Type == EntityType.CellLatch)
                        cellColor = CellLatchColor;
                    else if (entity.Type == EntityType.CellOther)
                        cellColor = CellOtherColor;
                    else if (entity.Type == EntityType.UnitRegfile)
                        cellColor = UnitRegfileColor;
                    else if (entity.Type == EntityType.UnitMemory)
                        cellColor = UnitMemoryColor;
                    else if (entity.Type == EntityType.UnitCustom)
                        cellColor = UnitCustomColor;
                    else
                        cellColor = Color.Gray;

                    if (entity.ColorOverride != Color.Black)
                        cellColor = entity.ColorOverride;

                    cellColor = Color.FromArgb(CellOpacity, cellColor);

                    Point topLeft = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point bottomRight = LambdaToScreen(entity.LambdaX + entity.LambdaWidth,
                                                        entity.LambdaY + entity.LambdaHeight);
                    Point rectSize = new Point(bottomRight.X - topLeft.X,
                                               bottomRight.Y - topLeft.Y);

                    if (entity.Selected)
                    {
                        gr.FillRectangle(new SolidBrush(SelectionColor),
                                           topLeft.X - (int)Lambda, topLeft.Y - (int)Lambda,
                                           rectSize.X + 2 * (int)Lambda, rectSize.Y + 2 * (int)Lambda);
                    }

                    gr.FillRectangle(new SolidBrush(cellColor), topLeft.X, topLeft.Y,
                                       rectSize.X, rectSize.Y);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0 && Zoom > 50)
                    {
                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

                        Point origin = new Point(0, 0);

                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = CellTextAlignment;

                        rectSize.X = (int)((float)rectSize.X / zf);
                        rectSize.Y = (int)((float)rectSize.Y / zf);

                        switch (align)
                        {
                            case TextAlignment.TopLeft:
                            default:
                                origin.X = 0;
                                origin.Y = 0;
                                break;

                            case TextAlignment.BottomLeft:
                                origin.X = 0;
                                origin.Y = rectSize.Y - (int)(textSize.Height);
                                break;

                            case TextAlignment.Top:
                                origin.X = rectSize.X / 2 - (int)(textSize.Width / 2);
                                origin.Y = 0;
                                break;

                            case TextAlignment.Bottom:
                                origin.X = rectSize.X / 2 - (int)(textSize.Width / 2);
                                origin.Y = rectSize.Y - (int)textSize.Height;
                                break;

                            case TextAlignment.TopRight:
                                origin.X = rectSize.X - (int)textSize.Width;
                                origin.Y = 0;
                                break;

                            case TextAlignment.BottomRight:
                                origin.X = rectSize.X - (int)textSize.Width;
                                origin.Y = rectSize.Y - (int)textSize.Height;
                                break;
                        }

                        gr.TranslateTransform(topLeft.X, topLeft.Y);
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor), origin.X, origin.Y);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Beacon
                //

                case EntityType.Beacon:

                    Point beaconOrigin = LambdaToScreen(entity.LambdaX, entity.LambdaY);

                    Point imageOrigin = new Point(beaconOrigin.X - (int)((float)(beaconImage.Width * zf) / 2),
                                                    beaconOrigin.Y - (int)((float)beaconImage.Height * zf));

                    if (entity.Selected)
                    {
                        ColorMatrix colorMatrix = new ColorMatrix(
                            new float[][]
                            {
                                new float[] {.0f, 1f, .0f, 0, 0},
                                new float[] {.0f, 1f, .0f, 0, 0},
                                new float[] {.0f, 1f, .0f, 0, 0},
                                new float[] {0, 0, 0, 1, 0},
                                new float[] {0, 0, 0, 0, 1}
                            });

                        ImageAttributes attributes = new ImageAttributes();

                        attributes.SetColorMatrix(colorMatrix);

                        float borderSize = 3F * zf;

                        gr.DrawImage(beaconImage,
                            new Rectangle(
                                imageOrigin.X - (int)borderSize,
                                imageOrigin.Y - (int)borderSize,
                                (int)((float)beaconImage.Width * zf) + (int)(borderSize * 2F),
                                (int)((float)beaconImage.Height * zf) + (int)(borderSize * 2F)),
                            0, 0, beaconImage.Width, beaconImage.Height, GraphicsUnit.Pixel, attributes);
                    }

                    gr.DrawImage(beaconImage,
                                  imageOrigin.X, imageOrigin.Y,
                                  beaconImage.Width * zf,
                                  beaconImage.Height * zf);

                    // Debug
                    //Rectangle rect = new Rectangle(beaconOrigin.X, beaconOrigin.Y, 1, 1);
                    //gr.DrawRectangle(Pens.White, rect);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0)
                    {
                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

                        Point textOrigin = new Point(beaconOrigin.X - (int)(textSize.Width * zf / 2F),
                                                       imageOrigin.Y - (int)(textSize.Height * zf));

                        gr.TranslateTransform(textOrigin.X, textOrigin.Y);
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor), 0, 0);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Region
                //

                case EntityType.Region:

                    if (hideRegions)
                        break;

                    Brush brush = new SolidBrush(Color.FromArgb(RegionOpacity, entity.ColorOverride));

                    GraphicsPath gp = new GraphicsPath();

                    PointF prev = (PointF)LambdaToScreen(entity.PathPoints[0].X, entity.PathPoints[0].Y);
                    PointF first = prev;

                    foreach (PointF pathPoint in entity.PathPoints)
                    {
                        PointF translated = (PointF)LambdaToScreen(pathPoint.X, pathPoint.Y);

                        if (translated == prev)
                            continue;

                        gp.AddLine(prev, translated);

                        prev = translated;
                    }

                    gp.AddLine(prev, first);

                    if (entity.Selected == true)
                    {
                        gr.DrawPath(new Pen(SelectionColor, (float)WireBaseSize * zf + (int)Lambda),
                                      gp);
                    }

                    gr.FillPath(brush, gp);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0)
                    {
                        Point p0 = LambdaToScreen(entity.PathPoints[0].X, entity.PathPoints[0].Y);
                        Point p1 = LambdaToScreen(entity.PathPoints[1].X, entity.PathPoints[1].Y);

                        Point start = new Point(p0.X, p0.Y);
                        Point end = new Point(p1.X, p1.Y);
                        Point temp;

                        if (p0.X == p1.X && p0.Y == p1.Y)
                            break;

                        if (end.X < start.X)
                        {
                            temp = start;
                            start = end;
                            end = temp;
                        }

                        int a = end.Y - start.Y;
                        int b = end.X - start.X;
                        float Tga = (float)a / (float)b;
                        float alpha = (float)Math.Atan(Tga);

                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        gr.TranslateTransform(start.X, start.Y);
                        gr.RotateTransform((float)(180.0F * alpha / Math.PI));
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor),
                                       0, 0);
                        gr.ResetTransform();
                    }

                    break;

            }
        }

        private void DrawImage(Graphics gr, int n)
        {
            float zf = (float)Zoom / 100F;

            if (_imageOrig[n] != null && hideImage == false)
            {
                Point imageOffset = LambdaToScreen(_imageScroll[n].X, _imageScroll[n].Y);
                float imageWidth = (float)_imageOrig[n].Width;
                float imageHeight = (float)_imageOrig[n].Height;
                float sx = imageOffset.X;
                float sy = imageOffset.Y;

                float imgZf = (float)_imageZoom[n] / 100F;

                ColorMatrix colorMatrix = new ColorMatrix();
                colorMatrix.Matrix33 = _imageOpacity[n] / 100F;

                ImageAttributes imageAtt = new ImageAttributes();
                imageAtt.SetColorMatrix(
                   colorMatrix,
                   ColorMatrixFlag.Default,
                   ColorAdjustType.Bitmap);

                if (EnableOpacity == false)
                {
                    gr.DrawImage(_imageOrig[n],
                                  sx, sy,
                                  imageWidth * zf / imgZf,
                                  imageHeight * zf / imgZf);
                }
                else
                {
                    gr.InterpolationMode = InterpolationMode.HighQualityBilinear;

                    gr.DrawImage(_imageOrig[n],
                        new Rectangle(
                            (int)sx, (int)sy,
                            (int)(imageWidth * zf), (int)(imageHeight * zf)),
                        0, 0,
                        imageWidth / imgZf,
                        imageHeight / imgZf,
                        GraphicsUnit.Pixel,
                        imageAtt);
                }
            }
        }

        //
        // width / height - Scene viewport size
        // WholeScene - draw Scene for saving to bitmap
        // origin - Scene offset (px).
        //

        private void DrawScene(Graphics gr, int width, int height, bool WholeScene, Point origin)
        {
            DrawInProgress = true;

            if (DrawStats)
                Console.WriteLine("---------- DRAW SCENE STATS START ----------");

            long Stamp1;
            long Stamp2;

            float savedScrollX = 0, savedScrollY = 0;
            int savedZoom = 0;

            if (WholeScene == true)
            {
                savedScrollX = _ScrollX;
                savedScrollY = _ScrollY;
                savedZoom = _zoom;

                PointF originLambda = ScreenToLambda(origin.X, origin.Y);

                _ScrollX = -originLambda.X - ScrollX;
                _ScrollY = -originLambda.Y - ScrollY;
                _zoom = 100;
            }

            //
            // Background
            //

            Stamp1 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            Region region = new Region(new Rectangle(0, 0, width - origin.X, height - origin.Y));

            gr.FillRegion(new SolidBrush(BackColor), region);

            Stamp2 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            if (DrawStats)
                Console.WriteLine("Background: " + (Stamp2 - Stamp1).ToString() + " ms");

            //
            // Image Layers
            //

            Stamp1 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            for (int n = 2; n >= 0; n--)
            {
                if (n == 0 && ImageOpacity0 == 0 && EnableOpacity)
                    continue;
                if (n == 1 && ImageOpacity1 == 0 && EnableOpacity)
                    continue;
                if (n == 2 && ImageOpacity2 == 0 && EnableOpacity)
                    continue;

                DrawImage(gr, n);
            }

            Stamp2 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            if (DrawStats)
                Console.WriteLine("Image Layers: " + (Stamp2 - Stamp1).ToString() + " ms");

            //
            // Grid
            //

            Stamp1 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            if (WholeScene == false && HideGrid == false)
                DrawLambdaGrid(gr);

            Stamp2 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            if (DrawStats)
                Console.WriteLine("Grid: " + (Stamp2 - Stamp1).ToString() + " ms");

            //
            // Entities
            //

            if (Lambda > 0.0F)
            {
                Stamp1 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

                if (root.Visible)
                {
                    foreach (Entity entity in root.Children)
                    {
                        DrawEntityRecursive(entity, gr);
                    }
                }

                Stamp2 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                if (DrawStats)
                    Console.WriteLine("Entities: " + (Stamp2 - Stamp1).ToString() + " ms");

                //
                // Wire drawing animation
                //

                if (WholeScene == false && DrawingBegin && (Mode == EntityMode.WireGround ||
                       Mode == EntityMode.WireInterconnect || Mode == EntityMode.WirePower))
                {
                    Entity virtualEntity = new Entity();

                    PointF point1 = ScreenToLambda(SavedMouseX, SavedMouseY);
                    PointF point2 = ScreenToLambda(LastMouseX, LastMouseY);

                    virtualEntity.LambdaX = point1.X;
                    virtualEntity.LambdaY = point1.Y;
                    virtualEntity.LambdaEndX = point2.X;
                    virtualEntity.LambdaEndY = point2.Y;
                    virtualEntity.Type = (EntityType)Mode;
                    virtualEntity.Priority = WirePriority;
                    virtualEntity.ColorOverride = Color.Black;

                    DrawEntity(virtualEntity, gr);
                }

                //
                // Cell drawing animation
                //

                if (WholeScene == false && DrawingBegin && (
                        Mode == EntityMode.CellNot ||
                        Mode == EntityMode.CellBuffer ||
                        Mode == EntityMode.CellMux ||
                        Mode == EntityMode.CellLogic ||
                        Mode == EntityMode.CellAdder ||
                        Mode == EntityMode.CellBusSupp ||
                        Mode == EntityMode.CellFlipFlop ||
                        Mode == EntityMode.CellLatch ||
                        Mode == EntityMode.CellOther ||
                        Mode == EntityMode.UnitRegfile ||
                        Mode == EntityMode.UnitMemory ||
                        Mode == EntityMode.UnitCustom
                       ))
                {
                    Entity virtualEntity = new Entity();

                    PointF point1 = ScreenToLambda(SavedMouseX, SavedMouseY);
                    PointF point2 = ScreenToLambda(LastMouseX, LastMouseY);
                    PointF originPos = new PointF();
                    PointF size = new PointF();

                    size.X = Math.Abs(point2.X - point1.X);
                    size.Y = Math.Abs(point2.Y - point1.Y);

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

                    virtualEntity.LambdaX = originPos.X;
                    virtualEntity.LambdaY = originPos.Y;
                    virtualEntity.LambdaWidth = size.X;
                    virtualEntity.LambdaHeight = size.Y;
                    virtualEntity.Type = (EntityType)Mode;
                    virtualEntity.Priority = CellPriority;
                    virtualEntity.ColorOverride = Color.Black;
                    virtualEntity.Label = Mode.ToString();

                    DrawEntity(virtualEntity, gr);
                }

                //
                // Lambda Scale
                //

                if (WholeScene == false)
                    DrawLambdaScale(gr);

                //
                // Origin
                //

                DrawOrigin(gr);

                //
                // RMB
                //

                if (WholeScene == false)
                    DrawRightMouseCross(gr);
            }

            //
            // Select box animation
            //

            if (SelectionBegin && WholeScene == false)
            {
                Point[] points = new Point[5];

                points[0] = new Point(SelectStartMouseX, SelectStartMouseY);
                points[1] = new Point(SelectStartMouseX, LastMouseY);
                points[2] = new Point(LastMouseX, LastMouseY);
                points[3] = new Point(LastMouseX, SelectStartMouseY);
                points[4] = new Point(SelectStartMouseX, SelectStartMouseY);

                Pen pen = new Pen(SelectionBoxColor, 2);

                gr.DrawLines(pen, points);
            }

            if (WholeScene == true)
            {
                _ScrollX = savedScrollX;
                _ScrollY = savedScrollY;
                _zoom = savedZoom;
            }

            if (DrawStats)
                Console.WriteLine("---------- DRAW SCENE STATS END ----------");

            DrawInProgress = false;
        }

        private void ReallocateGraphics()
        {
            context = BufferedGraphicsManager.Current;
            context.MaximumBuffer = new Size(Width + 1, Height + 1);

            gfx = context.Allocate(CreateGraphics(),
                 new Rectangle(0, 0, Width, Height));
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (gfx == null)
                ReallocateGraphics();

            long beginTime = DateTime.Now.Ticks;

            Point origin = new Point(0, 0);
            DrawScene(gfx.Graphics, Width, Height, false, origin);

            gfx.Render(e.Graphics);

            long endTime = DateTime.Now.Ticks;

            if (OnFrameDone != null)
                OnFrameDone(this, (endTime - beginTime) / 10000, EventArgs.Empty);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            if (gfx != null)
            {
                gfx.Dispose();
                ReallocateGraphics();
            }

            Invalidate();
            base.OnSizeChanged(e);
        }


    }
}
