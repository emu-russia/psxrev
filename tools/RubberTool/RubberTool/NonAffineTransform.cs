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


        TextureTriangle(ref sourceImage, ref destImage, destTri );

    }

    public static void FillTriangle ( ref Image image, Triangle tri, Color color)
    {
        Pen pen = new Pen(color);
        Graphics gr = Graphics.FromImage(image);

        int x0 = tri.a.X, y0 = tri.a.Y;
        int x1 = tri.b.X, y1 = tri.b.Y;
        int x2 = tri.c.X, y2 = tri.c.Y;

        int width = image.Width;
        int height = image.Height;

        // Сортировка вершин
        if ( y1 > y2 )
        {
            Swap<int>(ref x1, ref x2);
            Swap<int>(ref y1, ref y2);
        }
        if (y0 > y1)
        {
            Swap<int>(ref x0, ref x1);
            Swap<int>(ref y0, ref y1);
        }
        if (y1 > y2)
        {
            Swap<int>(ref x1, ref x2);
            Swap<int>(ref y1, ref y2);
        }

        double dx_far = Convert.ToDouble(x2 - x0) / (y2 - y0 + 1);
        double dx_upper = Convert.ToDouble(x1 - x0) / (y1 - y0 + 1);
        double dx_low = Convert.ToDouble(x2 - x1) / (y2 - y1 + 1);
        double xf = x0;
        double xt = x0 + dx_upper;

        for (int y = y0; y <= (y2 > height - 1 ? height - 1 : y2); y++)
        {
            if (y >= 0)
            {
                for (int x = (xf > 0 ? Convert.ToInt32(xf) : 0); x <= (xt < width ? xt : width - 1); x++)
                    gr.DrawRectangle(pen, new Rectangle(x, y, 1, 1));
                for (int x = (xf < width ? Convert.ToInt32(xf) : width - 1); x >= (xt > 0 ? xt : 0); x--)
                    gr.DrawRectangle(pen, new Rectangle(x, y, 1, 1));
            }
            xf += dx_far;
            if (y < y1)
                xt += dx_upper;
            else
                xt += dx_low;
        }
    }

    public static void TextureTriangle(ref Image texImage, ref Image destImage, Triangle tri)
    {
        Bitmap sourceBitmap = new Bitmap(texImage);
        Graphics gr = Graphics.FromImage(destImage);

        int x0 = tri.a.X, y0 = tri.a.Y;
        int x1 = tri.b.X, y1 = tri.b.Y;
        int x2 = tri.c.X, y2 = tri.c.Y;

        int width = destImage.Width;
        int height = destImage.Height;

        // Сортировка вершин
        if (y1 > y2)
        {
            Swap<int>(ref x1, ref x2);
            Swap<int>(ref y1, ref y2);
        }
        if (y0 > y1)
        {
            Swap<int>(ref x0, ref x1);
            Swap<int>(ref y0, ref y1);
        }
        if (y1 > y2)
        {
            Swap<int>(ref x1, ref x2);
            Swap<int>(ref y1, ref y2);
        }

        double dx_far = Convert.ToDouble(x2 - x0) / (y2 - y0 + 1);
        double dx_upper = Convert.ToDouble(x1 - x0) / (y1 - y0 + 1);
        double dx_low = Convert.ToDouble(x2 - x1) / (y2 - y1 + 1);
        double xf = x0;
        double xt = x0 + dx_upper;

        for (int y = y0; y <= (y2 > height - 1 ? height - 1 : y2); y++)
        {
            if (y >= 0)
            {
                for (int x = (xf > 0 ? Convert.ToInt32(xf) : 0); x <= (xt < width ? xt : width - 1); x++)
                {
                    Color pixel = sourceBitmap.GetPixel(x, y);
                    Pen pen = new Pen(pixel);
                    gr.DrawRectangle(pen, new Rectangle(x, y, 1, 1));
                }
                for (int x = (xf < width ? Convert.ToInt32(xf) : width - 1); x >= (xt > 0 ? xt : 0); x--)
                {
                    Color pixel = sourceBitmap.GetPixel(x, y);
                    Pen pen = new Pen(pixel);
                    gr.DrawRectangle(pen, new Rectangle(x, y, 1, 1));
                }
            }
            xf += dx_far;
            if (y < y1)
                xt += dx_upper;
            else
                xt += dx_low;
        }
    }

    private static void Swap<T> ( ref T a, ref T b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

}
