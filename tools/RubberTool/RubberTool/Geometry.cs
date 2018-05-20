// Геометрические определения

using System;
using System.Drawing;

public class Point2D
{
    public float X;
    public float Y;

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
