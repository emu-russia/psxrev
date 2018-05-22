// Немного геометрии

using System;
using System.Drawing;

public class Point2D
{
    public float X;
    public float Y;
    public string name;

    public Point2D (float xCoord, float yCoord)
    {
        X = xCoord;
        Y = yCoord;
    }
}

public class Triangle
{
    public Point2D a;
    public Point2D b;
    public Point2D c;
}

public class Vec
{
    public Point2D begin;
    public Point2D end;

    public Vec(Point2D beginPoint, Point2D endPoint)
    {
        begin = beginPoint;
        end = endPoint;
    }

    public float Length()
    {
        return (float)Math.Sqrt(
            Math.Pow((end.X - begin.X), 2) +
            Math.Pow((end.Y - begin.Y), 2));
    }

    public Vec Normalize()
    {
        return new Vec(begin, new Point2D(end.X / Length(), end.Y / Length()));
    }
}

public class Geom
{

    public static Point2D LineCross(Vec a, Vec b)
    {
        Point2D p1 = a.begin;
        Point2D p2 = a.end;
        Point2D q1 = b.begin;
        Point2D q2 = b.end;

        float x21 = p2.X - p1.X;
        float y21 = p2.Y - p1.Y;
        float x31 = q1.X - p1.X;
        float y31 = q1.Y - p1.Y;
        float x43 = q2.X - q1.X;
        float y43 = q2.Y - q1.Y;

        float paramDenominator = x43 * y21 - x21 * y43;

        if (paramDenominator == 0)
        {
            Console.WriteLine("Denominator = 0, lines are parallel");
        }

        float s = (x43 * y31 - x31 * y43) / paramDenominator;
        float t = (x21 * y31 - x31 * y21) / paramDenominator;

        return new Point2D(p1.X + (p2.X - p1.X) * s,
             p1.Y + (p2.Y - p1.Y) * s);
    }

    public static double DistanceFromPointToLine(Point2D point, Vec line)
    {
        // given a line based on two points, and a point away from the line,
        // find the perpendicular distance from the point to the line.
        // see http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
        // for explanation and defination.
        Point2D l1 = line.begin;
        Point2D l2 = line.end;

        return Math.Abs((l2.X - l1.X) * (l1.Y - point.Y) - (l1.X - point.X) * (l2.Y - l1.Y)) /
                Math.Sqrt(Math.Pow(l2.X - l1.X, 2) + Math.Pow(l2.Y - l1.Y, 2));
    }

    /// <summary>
    /// Удлинить (n >= 1.0) или укоротить (n < 1.0) вектор
    /// </summary>
    /// <param name="a"></param>
    /// <param name="n"></param>
    /// <returns></returns>
    public static Vec VecScale(Vec a, float n)
    {
        Vec v1 = new Vec(new Point2D(0, 0), new Point2D(a.end.X - a.begin.X, a.end.Y - a.begin.Y));
        Vec ident = v1.Normalize();
        ident.end.X *= a.Length() * n;
        ident.end.Y *= a.Length() * n;

        return new Vec(a.begin, new Point2D(a.begin.X + ident.end.X, a.begin.Y + ident.end.Y));
    }

    public static Point2D BaryCenter(Triangle tri)
    {
        float d = 2 * (tri.a.X * (tri.b.Y - tri.c.Y) + tri.b.X * (tri.c.Y - tri.a.Y) + tri.c.X * (tri.a.Y - tri.b.Y));

        if (d == 0)
        {
            return new Point2D(tri.a.X, tri.a.Y);
        }

        float ux = (1 / d) * ((tri.a.X * tri.a.X + tri.a.Y * tri.a.Y) * (tri.b.Y - tri.c.Y) +
            (tri.b.X * tri.b.X + tri.b.Y * tri.b.Y) * (tri.c.Y - tri.a.Y) +
            (tri.c.X * tri.c.X + tri.c.Y * tri.c.Y) * (tri.a.Y - tri.b.Y));
        float uy = (1 / d) * ((tri.a.X * tri.a.X + tri.a.Y * tri.a.Y) * (tri.c.X - tri.b.X) +
            (tri.b.X * tri.b.X + tri.b.Y * tri.b.Y) * (tri.a.X - tri.c.X) +
            (tri.c.X * tri.c.X + tri.c.Y * tri.c.Y) * (tri.b.X - tri.a.X));

        return new Point2D(ux, uy);
    }

}
