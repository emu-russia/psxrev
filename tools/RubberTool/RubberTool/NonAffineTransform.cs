// Неафинное преобразование растровых изображений

// Афинное преобразование использовать не получится, потому что пропорции сторон  
// могут не соблюдаться

// Credits: Herre Kuijpers

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;

class NonAffineTransform
{

    /// <summary>
    /// Преобразовать треугольник из исходного изображения в треугольник целевого изображения
    /// </summary>
    /// <param name="sourceImage">Исходное изображение</param>
    /// <param name="destImage">Целевое изображение</param>
    /// <param name="sourceTri">Исходный треугольник</param>
    /// <param name="destTri">Целевой треугольник</param>
    public static void WarpTriangle ( Image sourceImage, Image destImage, 
        Triangle sourceTri, Triangle destTri )
    {

        //
        // Использовать встроенные средства Graphics не представляется возможным
        // так как трансформация там поддерживается только афинная
        //
        // Поэтому чтобы не особо заморачиваться мы просто переносим пиксели исходного 
        // треугольника
        //


        TextureTriangle(ref sourceImage, ref destImage, sourceTri, destTri);

        //FillTriangle(ref destImage, sourceTri, destTri, SystemColors.InfoText );

    }

    public static void FillTriangle ( ref Image image, Triangle sourceTri, Triangle destTri, Color color)
    {
        Pen pen = new Pen(color);
        Graphics gr = Graphics.FromImage(image);

        float x0 = destTri.a.X, y0 = destTri.a.Y;
        float x1 = destTri.b.X, y1 = destTri.b.Y;
        float x2 = destTri.c.X, y2 = destTri.c.Y;

        int width = image.Width;
        int height = image.Height;

        // Сортировка вершин
        if ( y1 > y2 )
        {
            Swap<float>(ref x1, ref x2);
            Swap<float>(ref y1, ref y2);
        }
        if (y0 > y1)
        {
            Swap<float>(ref x0, ref x1);
            Swap<float>(ref y0, ref y1);
        }
        if (y1 > y2)
        {
            Swap<float>(ref x1, ref x2);
            Swap<float>(ref y1, ref y2);
        }

        double dx_far = Convert.ToDouble(x2 - x0) / (y2 - y0 + 1);
        double dx_upper = Convert.ToDouble(x1 - x0) / (y1 - y0 + 1);
        double dx_low = Convert.ToDouble(x2 - x1) / (y2 - y1 + 1);
        double xf = x0;
        double xt = x0 + dx_upper;

        for (float y = y0; y <= (y2 > height - 1 ? height - 1 : y2); y++)
        {
            if (y >= 0)
            {
                for (float x = (xf > 0 ? Convert.ToInt32(xf) : 0); x <= (xt < width ? xt : width - 1); x++)
                    gr.DrawRectangle(pen, new Rectangle((int)x, (int)y, 1, 1));
                for (float x = (xf < width ? Convert.ToInt32(xf) : width - 1); x >= (xt > 0 ? xt : 0); x--)
                    gr.DrawRectangle(pen, new Rectangle((int)x, (int)y, 1, 1));
            }
            xf += dx_far;
            if (y < y1)
                xt += dx_upper;
            else
                xt += dx_low;
        }
    }

    public static void TextureTriangle(ref Image texImage, ref Image destImage, Triangle sourceTri, Triangle destTri)
    {
        Bitmap sourceBitmap = new Bitmap(texImage);
        Graphics gr = Graphics.FromImage(destImage);

        float x0 = destTri.a.X, y0 = destTri.a.Y;
        float x1 = destTri.b.X, y1 = destTri.b.Y;
        float x2 = destTri.c.X, y2 = destTri.c.Y;

        int width = destImage.Width;
        int height = destImage.Height;

        // Сортировка вершин
        if (y1 > y2)
        {
            Swap<float>(ref x1, ref x2);
            Swap<float>(ref y1, ref y2);
        }
        if (y0 > y1)
        {
            Swap<float>(ref x0, ref x1);
            Swap<float>(ref y0, ref y1);
        }
        if (y1 > y2)
        {
            Swap<float>(ref x1, ref x2);
            Swap<float>(ref y1, ref y2);
        }

        double dx_far = Convert.ToDouble(x2 - x0) / (y2 - y0 + 1);
        double dx_upper = Convert.ToDouble(x1 - x0) / (y1 - y0 + 1);
        double dx_low = Convert.ToDouble(x2 - x1) / (y2 - y1 + 1);
        double xf = x0;
        double xt = x0 + dx_upper;

        for (float y = y0; y <= (y2 > height - 1 ? height - 1 : y2); y++)
        {
            if (y >= 0)
            {
                for (int x = (xf > 0 ? Convert.ToInt32(xf) : 0); x <= (xt < width ? xt : width - 1); x++)
                {
                    Point2D n2 = TrilateralXform(destTri, sourceTri, new Point2D(x, y));

                    //Console.WriteLine("X: " + x.ToString() + ", Y: " + y.ToString());
                    //Console.WriteLine("X': " + n2.X.ToString() + ", Y': " + n2.Y.ToString());

                    if (float.IsNaN(n2.X) || float.IsNaN(n2.Y) || 
                        n2.X >= sourceBitmap.Width || n2.Y >= sourceBitmap.Height || 
                        n2.X < 0 || n2.Y < 0 )
                    {
                        n2 = new Point2D(0, 0);
                    }


                    Color pixel = sourceBitmap.GetPixel((int)n2.X, (int)n2.Y);
                    Pen pen = new Pen(pixel);
                    gr.DrawRectangle(pen, new Rectangle((int)x, (int)y, 1, 1));
                }
                for (int x = (xf < width ? Convert.ToInt32(xf) : width - 1); x >= (xt > 0 ? xt : 0); x--)
                {
                    Point2D n2 = TrilateralXform(destTri, sourceTri, new Point2D(x, y));

                    //Console.WriteLine("X: " + x.ToString() + ", Y: " + y.ToString());
                    //Console.WriteLine("X': " + n2.X.ToString() + ", Y': " + n2.Y.ToString());

                    if (float.IsNaN(n2.X) || float.IsNaN(n2.Y) ||
                        n2.X >= sourceBitmap.Width || n2.Y >= sourceBitmap.Height ||
                        n2.X < 0 || n2.Y < 0)
                    {
                        n2 = new Point2D(0, 0);
                    }

                    Color pixel = sourceBitmap.GetPixel((int)n2.X, (int)n2.Y);
                    Pen pen = new Pen(pixel);
                    gr.DrawRectangle(pen, new Rectangle((int)x, (int)y, 1, 1));
                }
            }
            xf += dx_far;
            if (y < y1)
                xt += dx_upper;
            else
                xt += dx_low;
        }
    }

    /// <summary>
    /// Трилатеральный перенос точки исходного треугольника в целевой треугольник
    /// </summary>
    /// <param name="sourceTri">Исходный треугольник</param>
    /// <param name="destTri">Целевой треугольник</param>
    /// <param name="n">Исходная точка</param>
    /// <returns>Точка внутри целевого треугольника</returns>
    public static Point2D TrilateralXform (Triangle sourceTri, Triangle destTri, Point2D n)
    {
        //
        // Получение "срединных" точек
        //

        Vec ab = new Vec(new Point2D(sourceTri.a.X, sourceTri.a.Y), new Point2D(sourceTri.b.X, sourceTri.b.Y));
        Vec cn = new Vec(new Point2D(sourceTri.c.X, sourceTri.c.Y), new Point2D(n.X, n.Y));
        Point2D mpAB = Geom.LineCross(ab, cn);

        Vec ac = new Vec(new Point2D(sourceTri.a.X, sourceTri.a.Y), new Point2D(sourceTri.c.X, sourceTri.c.Y));
        Vec bn = new Vec(new Point2D(sourceTri.b.X, sourceTri.b.Y), new Point2D(n.X, n.Y));
        Point2D mpAC = Geom.LineCross(ac, bn);

        Vec bc = new Vec(new Point2D(sourceTri.b.X, sourceTri.b.Y), new Point2D(sourceTri.c.X, sourceTri.c.Y));
        Vec an = new Vec(new Point2D(sourceTri.a.X, sourceTri.a.Y), new Point2D(n.X, n.Y));
        Point2D mpBC = Geom.LineCross(bc, an);

        //
        // Получение соотношения отрезков разделяемых срединными точками
        //

        Vec a_mpAB = new Vec(new Point2D(sourceTri.a.X, sourceTri.a.Y), new Point2D(mpAB.X, mpAB.Y));
        Vec mpAB_b = new Vec(new Point2D(mpAB.X, mpAB.Y), new Point2D(sourceTri.b.X, sourceTri.b.Y));
        float sAB = a_mpAB.Length() / ab.Length();

        Vec a_mpAC = new Vec(new Point2D(sourceTri.a.X, sourceTri.a.Y), new Point2D(mpAC.X, mpAC.Y));
        Vec mpAC_c = new Vec(new Point2D(mpAC.X, mpAC.Y), new Point2D(sourceTri.c.X, sourceTri.c.Y));
        float sAC = a_mpAC.Length() / ac.Length();

        Vec b_mpBC = new Vec(new Point2D(sourceTri.b.X, sourceTri.b.Y), new Point2D(mpBC.X, mpBC.Y));
        Vec mpBC_c = new Vec(new Point2D(mpBC.X, mpBC.Y), new Point2D(sourceTri.c.X, sourceTri.c.Y));
        float sBC = b_mpBC.Length() / bc.Length();

        //
        // Перенос срединных точек
        //

        Vec ab2 = new Vec(new Point2D(destTri.a.X, destTri.a.Y), new Point2D(destTri.b.X, destTri.b.Y));
        Vec ab2_s = Geom.VecScale(ab2, sAB);

        Vec ac2 = new Vec(new Point2D(destTri.a.X, destTri.a.Y), new Point2D(destTri.c.X, destTri.c.Y));
        Vec ac2_s = Geom.VecScale(ac2, sAC);

        Vec bc2 = new Vec(new Point2D(destTri.b.X, destTri.b.Y), new Point2D(destTri.c.X, destTri.c.Y));
        Vec bc2_s = Geom.VecScale(bc2, sBC);

        //
        // Получение вершин внутреннего треугольника
        //

        Vec mpAB2_c2 = new Vec(new Point2D(ab2_s.end.X, ab2_s.end.Y), new Point2D(destTri.c.X, destTri.c.Y));
        Vec mpAC2_b2 = new Vec(new Point2D(ac2_s.end.X, ac2_s.end.Y), new Point2D(destTri.b.X, destTri.b.Y));
        Vec mpBC2_a2 = new Vec(new Point2D(bc2_s.end.X, bc2_s.end.Y), new Point2D(destTri.a.X, destTri.a.Y));

        Triangle tri = new Triangle();

        tri.a = Geom.LineCross(mpAB2_c2, mpAC2_b2);
        tri.b = Geom.LineCross(mpBC2_a2, mpAC2_b2);
        tri.c = Geom.LineCross(mpAB2_c2, mpBC2_a2);

        //
        // Вычисление барицентра
        //

        Point2D p = Geom.BaryCenter(tri);

        return p;
    }

    private static void Swap<T> ( ref T a, ref T b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

}
