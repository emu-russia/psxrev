// Триангуляция Делоне

using System;
using System.Collections.Generic;
using System.Drawing;

/*
  copyright s-hull.org 2011
  released under the contributors beerware license

  contributors: Phil Atkin, Dr Sinclair.
*/



class Delaunay
{
    private List<Vertex> points;

    /// <summary>
    /// WRAPPER
    /// Сгенерировать сетку треугольников на базе точек
    /// </summary>
    /// <param name="points"></param>
    public List<Triangle> GenMesh (List<Point2D> points)
    {
        List<Triangle> mesh = new List<Triangle>();

        List<Vertex> vertexList = new List<Vertex>();

        foreach (Point2D p in points)
        {
            Vertex v = new Vertex(p.X, p.Y);
            v.name = p.name;

            vertexList.Add(v);
        }

        List<Triad> triads = Triangulation(vertexList);

        foreach ( Triad triad in triads)
        {
            Triangle tri = new Triangle();

            Vertex va = vertexList[triad.a];
            tri.a = new Point2D(va.x, va.y);
            tri.a.name = va.name;

            Vertex vb = vertexList[triad.b];
            tri.b = new Point2D(vb.x, vb.y);
            tri.b.name = vb.name;

            Vertex vc = vertexList[triad.c];
            tri.c = new Point2D(vc.x, vc.y);
            tri.c.name = vc.name;

            mesh.Add(tri);
        }

        return mesh;
    }



    class Set<T> : IEnumerable<T>
    {
        SortedList<T, int> list;

        public Set()
        {
            list = new SortedList<T, int>();
        }

        public void Add(T k)
        {
            if (!list.ContainsKey(k))
                list.Add(k, 0);
        }

        public int Count
        {
            get { return list.Count; }
        }

        public void DeepCopy(Set<T> other)
        {
            list.Clear();
            foreach (T k in other.list.Keys)
                Add(k);
        }

        IEnumerator<T> IEnumerable<T>.GetEnumerator()
        {
            return list.Keys.GetEnumerator();
        }

        public void Clear()
        {
            list.Clear();
        }


        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        #endregion
    }


    public class Triad
    {
        public int a, b, c;
        public int ab, bc, ac;  // adjacent edges index to neighbouring triangle.

        // Position and radius squared of circumcircle
        public float circumcircleR2, circumcircleX, circumcircleY;

        public Triad(int x, int y, int z)
        {
            a = x; b = y; c = z; ab = -1; bc = -1; ac = -1;
            circumcircleR2 = -1; //x = 0; y = 0;
        }

        public void Initialize(int a, int b, int c, int ab, int bc, int ac, List<Vertex> points)
        {
            this.a = a;
            this.b = b;
            this.c = c;
            this.ab = ab;
            this.bc = bc;
            this.ac = ac;

            FindCircumcirclePrecisely(points);
        }

        /// <summary>
        /// If current orientation is not clockwise, swap b<->c
        /// </summary>
        public void MakeClockwise(List<Vertex> points)
        {
            float centroidX = (points[a].x + points[b].x + points[c].x) / 3.0f;
            float centroidY = (points[a].y + points[b].y + points[c].y) / 3.0f;

            float dr0 = points[a].x - centroidX, dc0 = points[a].y - centroidY;
            float dx01 = points[b].x - points[a].x, dy01 = points[b].y - points[a].y;

            float df = -dx01 * dc0 + dy01 * dr0;
            if (df > 0)
            {
                // Need to swap vertices b<->c and edges ab<->bc
                int t = b;
                b = c;
                c = t;

                t = ab;
                ab = ac;
                ac = t;
            }
        }

        /// <summary>
        /// Find location and radius ^2 of the circumcircle (through all 3 points)
        /// This is the most critical routine in the entire set of code.  It must
        /// be numerically stable when the points are nearly collinear.
        /// </summary>
        public bool FindCircumcirclePrecisely(List<Vertex> points)
        {
            // Use coordinates relative to point `a' of the triangle
            Vertex pa = points[a], pb = points[b], pc = points[c];

            double xba = pb.x - pa.x;
            double yba = pb.y - pa.y;
            double xca = pc.x - pa.x;
            double yca = pc.y - pa.y;

            // Squares of lengths of the edges incident to `a'
            double balength = xba * xba + yba * yba;
            double calength = xca * xca + yca * yca;

            // Calculate the denominator of the formulae. 
            double D = xba * yca - yba * xca;
            if (D == 0)
            {
                circumcircleX = 0;
                circumcircleY = 0;
                circumcircleR2 = -1;
                return false;
            }

            double denominator = 0.5 / D;

            // Calculate offset (from pa) of circumcenter
            double xC = (yca * balength - yba * calength) * denominator;
            double yC = (xba * calength - xca * balength) * denominator;

            double radius2 = xC * xC + yC * yC;
            if ((radius2 > 1e10 * balength || radius2 > 1e10 * calength))
            {
                circumcircleX = 0;
                circumcircleY = 0;
                circumcircleR2 = -1;
                return false;
            }

            circumcircleR2 = (float)radius2;
            circumcircleX = (float)(pa.x + xC);
            circumcircleY = (float)(pa.y + yC);

            return true;
        }

        /// <summary>
        /// Return true iff Vertex p is inside the circumcircle of this triangle
        /// </summary>
        public bool InsideCircumcircle(Vertex p)
        {
            float dx = circumcircleX - p.x;
            float dy = circumcircleY - p.y;
            float r2 = dx * dx + dy * dy;
            return r2 < circumcircleR2;
        }

        /// <summary>
        /// Change any adjacent triangle index that matches fromIndex, to toIndex
        /// </summary>
        public void ChangeAdjacentIndex(int fromIndex, int toIndex)
        {
            if (ab == fromIndex)
                ab = toIndex;
            else if (bc == fromIndex)
                bc = toIndex;
            else if (ac == fromIndex)
                ac = toIndex;
        }

        /// <summary>
        /// Determine which edge matches the triangleIndex, then which vertex the vertexIndex
        /// Set the indices of the opposite vertex, left and right edges accordingly
        /// </summary>
        public void FindAdjacency(int vertexIndex, int triangleIndex, out int indexOpposite, out int indexLeft, out int indexRight)
        {
            if (ab == triangleIndex)
            {
                indexOpposite = c;

                if (vertexIndex == a)
                {
                    indexLeft = ac;
                    indexRight = bc;
                }
                else
                {
                    indexLeft = bc;
                    indexRight = ac;
                }
            }
            else if (ac == triangleIndex)
            {
                indexOpposite = b;

                if (vertexIndex == a)
                {
                    indexLeft = ab;
                    indexRight = bc;
                }
                else
                {
                    indexLeft = bc;
                    indexRight = ab;
                }
            }
            else if (bc == triangleIndex)
            {
                indexOpposite = a;

                if (vertexIndex == b)
                {
                    indexLeft = ab;
                    indexRight = ac;
                }
                else
                {
                    indexLeft = ac;
                    indexRight = ab;
                }
            }
            else
            {
                indexOpposite = indexLeft = indexRight = 0;
            }
        }

        public override string ToString()
        {
            return string.Format("Triad vertices {0} {1} {2} ; edges {3} {4} {5}", a, b, c, ab, ac, bc);
        }
    }



    public class Vertex
    {
        public float x, y;
        public string name;

        protected Vertex() { }

        public Vertex(float x, float y)
        {
            this.x = x; this.y = y;
        }

        public float distance2To(Vertex other)
        {
            float dx = x - other.x;
            float dy = y - other.y;
            return dx * dx + dy * dy;
        }

        public float distanceTo(Vertex other)
        {
            return (float)Math.Sqrt(distance2To(other));
        }

        public override string ToString()
        {
            return string.Format("({0},{1})", x, y);
        }
    }



    private void Analyse(List<Vertex> suppliedPoints, Hull hull, List<Triad> triads, bool rejectDuplicatePoints, bool hullOnly)
    {
        if (suppliedPoints.Count < 3)
            throw new ArgumentException("Number of points supplied must be >= 3");

        this.points = suppliedPoints;
        int nump = points.Count;

        float[] distance2ToCentre = new float[nump];
        int[] sortedIndices = new int[nump];

        // Choose first point as the seed
        for (int k = 0; k < nump; k++)
        {
            distance2ToCentre[k] = points[0].distance2To(points[k]);
            sortedIndices[k] = k;
        }

        // Sort by distance to seed point
        Array.Sort(distance2ToCentre, sortedIndices);

        // Duplicates are more efficiently rejected now we have sorted the vertices
        if (rejectDuplicatePoints)
        {
            // Search backwards so each removal is independent of any other
            for (int k = nump - 2; k >= 0; k--)
            {
                // If the points are identical then their distances will be the same,
                // so they will be adjacent in the sorted list
                if ((points[sortedIndices[k]].x == points[sortedIndices[k + 1]].x) &&
                    (points[sortedIndices[k]].y == points[sortedIndices[k + 1]].y))
                {
                    // Duplicates are expected to be rare, so this is not particularly efficient
                    Array.Copy(sortedIndices, k + 2, sortedIndices, k + 1, nump - k - 2);
                    Array.Copy(distance2ToCentre, k + 2, distance2ToCentre, k + 1, nump - k - 2);
                    nump--;
                }
            }
        }

        //Console.WriteLine((points.Count - nump).ToString() + " duplicate points rejected");

        if (nump < 3)
            throw new ArgumentException("Number of unique points supplied must be >= 3");

        int mid = -1;
        float romin2 = float.MaxValue, circumCentreX = 0, circumCentreY = 0;

        // Find the point which, with the first two points, creates the triangle with the smallest circumcircle
        Triad tri = new Triad(sortedIndices[0], sortedIndices[1], 2);
        for (int kc = 2; kc < nump; kc++)
        {
            tri.c = sortedIndices[kc];
            if (tri.FindCircumcirclePrecisely(points) && tri.circumcircleR2 < romin2)
            {
                mid = kc;
                // Centre of the circumcentre of the seed triangle
                romin2 = tri.circumcircleR2;
                circumCentreX = tri.circumcircleX;
                circumCentreY = tri.circumcircleY;
            }
            else if (romin2 * 4 < distance2ToCentre[kc])
                break;
        }

        // Change the indices, if necessary, to make the 2th point produce the smallest circumcircle with the 0th and 1th
        if (mid != 2)
        {
            int indexMid = sortedIndices[mid];
            float distance2Mid = distance2ToCentre[mid];

            Array.Copy(sortedIndices, 2, sortedIndices, 3, mid - 2);
            Array.Copy(distance2ToCentre, 2, distance2ToCentre, 3, mid - 2);
            sortedIndices[2] = indexMid;
            distance2ToCentre[2] = distance2Mid;
        }

        // These three points are our seed triangle
        tri.c = sortedIndices[2];
        tri.MakeClockwise(points);
        tri.FindCircumcirclePrecisely(points);

        // Add tri as the first triad, and the three points to the convex hull
        triads.Add(tri);
        hull.Add(new HullVertex(points, tri.a));
        hull.Add(new HullVertex(points, tri.b));
        hull.Add(new HullVertex(points, tri.c));

        // Sort the remainder according to their distance from its centroid
        // Re-measure the points' distances from the centre of the circumcircle
        Vertex centre = new Vertex(circumCentreX, circumCentreY);
        for (int k = 3; k < nump; k++)
            distance2ToCentre[k] = points[sortedIndices[k]].distance2To(centre);

        // Sort the _other_ points in order of distance to circumcentre
        Array.Sort(distance2ToCentre, sortedIndices, 3, nump - 3);

        // Add new points into hull (removing obscured ones from the chain)
        // and creating triangles....
        int numt = 0;
        for (int k = 3; k < nump; k++)
        {
            int pointsIndex = sortedIndices[k];
            HullVertex ptx = new HullVertex(points, pointsIndex);

            float dx = ptx.x - hull[0].x, dy = ptx.y - hull[0].y;  // outwards pointing from hull[0] to pt.

            int numh = hull.Count, numh_old = numh;
            List<int> pidx = new List<int>(), tridx = new List<int>();
            int hidx;  // new hull point location within hull.....

            if (hull.EdgeVisibleFrom(0, dx, dy))
            {
                // starting with a visible hull facet !!!
                int e2 = numh;
                hidx = 0;

                // check to see if segment numh is also visible
                if (hull.EdgeVisibleFrom(numh - 1, dx, dy))
                {
                    // visible.
                    pidx.Add(hull[numh - 1].pointsIndex);
                    tridx.Add(hull[numh - 1].triadIndex);

                    for (int h = 0; h < numh - 1; h++)
                    {
                        // if segment h is visible delete h
                        pidx.Add(hull[h].pointsIndex);
                        tridx.Add(hull[h].triadIndex);
                        if (hull.EdgeVisibleFrom(h, ptx))
                        {
                            hull.RemoveAt(h);
                            h--;
                            numh--;
                        }
                        else
                        {
                            // quit on invisibility
                            hull.Insert(0, ptx);
                            numh++;
                            break;
                        }
                    }
                    // look backwards through the hull structure
                    for (int h = numh - 2; h > 0; h--)
                    {
                        // if segment h is visible delete h + 1
                        if (hull.EdgeVisibleFrom(h, ptx))
                        {
                            pidx.Insert(0, hull[h].pointsIndex);
                            tridx.Insert(0, hull[h].triadIndex);
                            hull.RemoveAt(h + 1);  // erase end of chain
                        }
                        else
                            break; // quit on invisibility
                    }
                }
                else
                {
                    hidx = 1;  // keep pt hull[0]
                    tridx.Add(hull[0].triadIndex);
                    pidx.Add(hull[0].pointsIndex);

                    for (int h = 1; h < numh; h++)
                    {
                        // if segment h is visible delete h  
                        pidx.Add(hull[h].pointsIndex);
                        tridx.Add(hull[h].triadIndex);
                        if (hull.EdgeVisibleFrom(h, ptx))
                        {                     // visible
                            hull.RemoveAt(h);
                            h--;
                            numh--;
                        }
                        else
                        {
                            // quit on invisibility
                            hull.Insert(h, ptx);
                            break;
                        }
                    }
                }
            }
            else
            {
                int e1 = -1, e2 = numh;
                for (int h = 1; h < numh; h++)
                {
                    if (hull.EdgeVisibleFrom(h, ptx))
                    {
                        if (e1 < 0)
                            e1 = h;  // first visible
                    }
                    else
                    {
                        if (e1 > 0)
                        {
                            // first invisible segment.
                            e2 = h;
                            break;
                        }
                    }
                }

                // triangle pidx starts at e1 and ends at e2 (inclusive).	
                if (e2 < numh)
                {
                    for (int e = e1; e <= e2; e++)
                    {
                        pidx.Add(hull[e].pointsIndex);
                        tridx.Add(hull[e].triadIndex);
                    }
                }
                else
                {
                    for (int e = e1; e < e2; e++)
                    {
                        pidx.Add(hull[e].pointsIndex);
                        tridx.Add(hull[e].triadIndex);   // there are only n-1 triangles from n hull pts.
                    }
                    pidx.Add(hull[0].pointsIndex);
                }

                // erase elements e1+1 : e2-1 inclusive.
                if (e1 < e2 - 1)
                    hull.RemoveRange(e1 + 1, e2 - e1 - 1);

                // insert ptx at location e1+1.
                hull.Insert(e1 + 1, ptx);
                hidx = e1 + 1;
            }

            // If we're only computing the hull, we're done with this point
            if (hullOnly)
                continue;

            int a = pointsIndex, T0;

            int npx = pidx.Count - 1;
            numt = triads.Count;
            T0 = numt;

            for (int p = 0; p < npx; p++)
            {
                Triad trx = new Triad(a, pidx[p], pidx[p + 1]);
                trx.FindCircumcirclePrecisely(points);

                trx.bc = tridx[p];
                if (p > 0)
                    trx.ab = numt - 1;
                trx.ac = numt + 1;

                // index back into the triads.
                Triad txx = triads[tridx[p]];
                if ((trx.b == txx.a && trx.c == txx.b) | (trx.b == txx.b && trx.c == txx.a))
                    txx.ab = numt;
                else if ((trx.b == txx.a && trx.c == txx.c) | (trx.b == txx.c && trx.c == txx.a))
                    txx.ac = numt;
                else if ((trx.b == txx.b && trx.c == txx.c) | (trx.b == txx.c && trx.c == txx.b))
                    txx.bc = numt;

                triads.Add(trx);
                numt++;
            }
            // Last edge is on the outside
            triads[numt - 1].ac = -1;

            hull[hidx].triadIndex = numt - 1;
            if (hidx > 0)
                hull[hidx - 1].triadIndex = T0;
            else
            {
                numh = hull.Count;
                hull[numh - 1].triadIndex = T0;
            }
        }
    }

    /// <summary>
    /// Return the convex hull of the supplied points,
    /// Don't check for duplicate points
    /// </summary>
    /// <param name="points">List of 2D vertices</param>
    /// <returns></returns>
    private List<Vertex> ConvexHull(List<Vertex> points)
    {
        return ConvexHull(points, false);
    }

    /// <summary>
    /// Return the convex hull of the supplied points,
    /// Optionally check for duplicate points
    /// </summary>
    /// <param name="points">List of 2D vertices</param>
    /// <param name="rejectDuplicatePoints">Whether to omit duplicated points</param>
    /// <returns></returns>
    private List<Vertex> ConvexHull(List<Vertex> points, bool rejectDuplicatePoints)
    {
        Hull hull = new Hull();
        List<Triad> triads = new List<Triad>();

        Analyse(points, hull, triads, rejectDuplicatePoints, true);

        List<Vertex> hullVertices = new List<Vertex>();
        foreach (HullVertex hv in hull)
            hullVertices.Add(new Vertex(hv.x, hv.y));

        return hullVertices;
    }

    /// <summary>
    /// Return the Delaunay triangulation of the supplied points
    /// Don't check for duplicate points
    /// </summary>
    /// <param name="points">List of 2D vertices</param>
    /// <returns>Triads specifying the triangulation</returns>
    private List<Triad> Triangulation(List<Vertex> points)
    {
        return Triangulation(points, false);
    }



    /// <summary>
    /// Return the Delaunay triangulation of the supplied points
    /// Optionally check for duplicate points
    /// </summary>
    /// <param name="points">List of 2D vertices</param>
    /// <param name="rejectDuplicatePoints">Whether to omit duplicated points</param>
    /// <returns></returns>
    private List<Triad> Triangulation(List<Vertex> points, bool rejectDuplicatePoints)
    {
        List<Triad> triads = new List<Triad>();
        Hull hull = new Hull();

        Analyse(points, hull, triads, rejectDuplicatePoints, false);

        // Now, need to flip any pairs of adjacent triangles not satisfying
        // the Delaunay criterion
        int numt = triads.Count;
        bool[] idsA = new bool[numt];
        bool[] idsB = new bool[numt];

        // We maintain a "list" of the triangles we've flipped in order to propogate any
        // consequent changes
        // When the number of changes is large, this is best maintained as a vector of bools
        // When the number becomes small, it's best maintained as a set
        // We switch between these regimes as the number flipped decreases
        //
        // the iteration cycle limit is included to prevent degenerate cases 'oscillating'
        // and the algorithm failing to stop.
        int flipped = FlipTriangles(triads, idsA);

        int iterations = 1;
        while (flipped > (int)(fraction * (float)numt) && iterations < 1000)
        {
            if ((iterations & 1) == 1)
                flipped = FlipTriangles(triads, idsA, idsB);
            else
                flipped = FlipTriangles(triads, idsB, idsA);

            iterations++;
        }

        Set<int> idSetA = new Set<int>(), idSetB = new Set<int>();
        flipped = FlipTriangles(triads,
            ((iterations & 1) == 1) ? idsA : idsB, idSetA);

        iterations = 1;
        while (flipped > 0 && iterations < 2000)
        {
            if ((iterations & 1) == 1)
                flipped = FlipTriangles(triads, idSetA, idSetB);
            else
                flipped = FlipTriangles(triads, idSetB, idSetA);

            iterations++;
        }

        return triads;
    }

    public float fraction = 0.3f;

    /// <summary>
    /// Test the triad against its 3 neighbours and flip it with any neighbour whose opposite point
    /// is inside the circumcircle of the triad
    /// </summary>
    /// <param name="triads">The triads</param>
    /// <param name="triadIndexToTest">The index of the triad to test</param>
    /// <param name="triadIndexFlipped">Index of adjacent triangle it was flipped with (if any)</param>
    /// <returns>true iff the triad was flipped with any of its neighbours</returns>
    bool FlipTriangle(List<Triad> triads, int triadIndexToTest, out int triadIndexFlipped)
    {
        int oppositeVertex = 0, edge1, edge2, edge3 = 0, edge4 = 0;
        triadIndexFlipped = 0;

        Triad tri = triads[triadIndexToTest];
        // test all 3 neighbours of tri 

        if (tri.bc >= 0)
        {
            triadIndexFlipped = tri.bc;
            Triad t2 = triads[triadIndexFlipped];
            // find relative orientation (shared limb).
            t2.FindAdjacency(tri.b, triadIndexToTest, out oppositeVertex, out edge3, out edge4);
            if (tri.InsideCircumcircle(points[oppositeVertex]))
            {  // not valid in the Delaunay sense.
                edge1 = tri.ab;
                edge2 = tri.ac;
                if (edge1 != edge3 && edge2 != edge4)
                {
                    int tria = tri.a, trib = tri.b, tric = tri.c;
                    tri.Initialize(tria, trib, oppositeVertex, edge1, edge3, triadIndexFlipped, points);
                    t2.Initialize(tria, tric, oppositeVertex, edge2, edge4, triadIndexToTest, points);

                    // change knock on triangle labels.
                    if (edge3 >= 0)
                        triads[edge3].ChangeAdjacentIndex(triadIndexFlipped, triadIndexToTest);
                    if (edge2 >= 0)
                        triads[edge2].ChangeAdjacentIndex(triadIndexToTest, triadIndexFlipped);
                    return true;
                }
            }
        }


        if (tri.ab >= 0)
        {
            triadIndexFlipped = tri.ab;
            Triad t2 = triads[triadIndexFlipped];
            // find relative orientation (shared limb).
            t2.FindAdjacency(tri.a, triadIndexToTest, out oppositeVertex, out edge3, out edge4);
            if (tri.InsideCircumcircle(points[oppositeVertex]))
            {  // not valid in the Delaunay sense.
                edge1 = tri.ac;
                edge2 = tri.bc;
                if (edge1 != edge3 && edge2 != edge4)
                {
                    int tria = tri.a, trib = tri.b, tric = tri.c;
                    tri.Initialize(tric, tria, oppositeVertex, edge1, edge3, triadIndexFlipped, points);
                    t2.Initialize(tric, trib, oppositeVertex, edge2, edge4, triadIndexToTest, points);

                    // change knock on triangle labels.
                    if (edge3 >= 0)
                        triads[edge3].ChangeAdjacentIndex(triadIndexFlipped, triadIndexToTest);
                    if (edge2 >= 0)
                        triads[edge2].ChangeAdjacentIndex(triadIndexToTest, triadIndexFlipped);
                    return true;
                }
            }
        }

        if (tri.ac >= 0)
        {
            triadIndexFlipped = tri.ac;
            Triad t2 = triads[triadIndexFlipped];
            // find relative orientation (shared limb).
            t2.FindAdjacency(tri.a, triadIndexToTest, out oppositeVertex, out edge3, out edge4);
            if (tri.InsideCircumcircle(points[oppositeVertex]))
            {  // not valid in the Delaunay sense.
                edge1 = tri.ab;   // .ac shared limb
                edge2 = tri.bc;
                if (edge1 != edge3 && edge2 != edge4)
                {
                    int tria = tri.a, trib = tri.b, tric = tri.c;
                    tri.Initialize(trib, tria, oppositeVertex, edge1, edge3, triadIndexFlipped, points);
                    t2.Initialize(trib, tric, oppositeVertex, edge2, edge4, triadIndexToTest, points);

                    // change knock on triangle labels.
                    if (edge3 >= 0)
                        triads[edge3].ChangeAdjacentIndex(triadIndexFlipped, triadIndexToTest);
                    if (edge2 >= 0)
                        triads[edge2].ChangeAdjacentIndex(triadIndexToTest, triadIndexFlipped);
                    return true;
                }
            }
        }

        return false;
    }

    /// <summary>
    /// Flip triangles that do not satisfy the Delaunay condition
    /// </summary>
    private int FlipTriangles(List<Triad> triads, bool[] idsFlipped)
    {
        int numt = (int)triads.Count;
        Array.Clear(idsFlipped, 0, numt);

        int flipped = 0;
        for (int t = 0; t < numt; t++)
        {
            int t2;
            if (FlipTriangle(triads, t, out t2))
            {
                flipped += 2;
                idsFlipped[t] = true;
                idsFlipped[t2] = true;

            }
        }

        return flipped;
    }

    private int FlipTriangles(List<Triad> triads, bool[] idsToTest, bool[] idsFlipped)
    {
        int numt = (int)triads.Count;
        Array.Clear(idsFlipped, 0, numt);

        int flipped = 0;
        for (int t = 0; t < numt; t++)
        {
            if (idsToTest[t])
            {
                int t2;
                if (FlipTriangle(triads, t, out t2))
                {
                    flipped += 2;
                    idsFlipped[t] = true;
                    idsFlipped[t2] = true;
                }
            }
        }

        return flipped;
    }

    private int FlipTriangles(List<Triad> triads, bool[] idsToTest, Set<int> idsFlipped)
    {
        int numt = (int)triads.Count;
        idsFlipped.Clear();

        int flipped = 0;
        for (int t = 0; t < numt; t++)
        {
            if (idsToTest[t])
            {
                int t2;
                if (FlipTriangle(triads, t, out t2))
                {
                    flipped += 2;
                    idsFlipped.Add(t);
                    idsFlipped.Add(t2);
                }
            }
        }

        return flipped;
    }

    private int FlipTriangles(List<Triad> triads, Set<int> idsToTest, Set<int> idsFlipped)
    {
        int flipped = 0;
        idsFlipped.Clear();

        foreach (int t in idsToTest)
        {
            int t2;
            if (FlipTriangle(triads, t, out t2))
            {
                flipped += 2;
                idsFlipped.Add(t);
                idsFlipped.Add(t2);
            }
        }

        return flipped;
    }



    /// <summary>
    /// Vertices belonging to the convex hull need to maintain a point and triad index
    /// </summary>
    internal class HullVertex : Vertex
    {
        public int pointsIndex;
        public int triadIndex;

        public HullVertex(List<Vertex> points, int pointIndex)
        {
            x = points[pointIndex].x;
            y = points[pointIndex].y;
            pointsIndex = pointIndex;
            triadIndex = 0;
        }
    }

    /// <summary>
    /// Hull represents a list of vertices in the convex hull, and keeps track of
    /// their indices (into the associated points list) and triads
    /// </summary>
    class Hull : List<HullVertex>
    {
        private int NextIndex(int index)
        {
            if (index == Count - 1)
                return 0;
            else
                return index + 1;
        }

        /// <summary>
        /// Return vector from the hull point at index to next point
        /// </summary>
        public void VectorToNext(int index, out float dx, out float dy)
        {
            Vertex et = this[index], en = this[NextIndex(index)];

            dx = en.x - et.x;
            dy = en.y - et.y;
        }

        /// <summary>
        /// Return whether the hull vertex at index is visible from the supplied coordinates
        /// </summary>
        public bool EdgeVisibleFrom(int index, float dx, float dy)
        {
            float idx, idy;
            VectorToNext(index, out idx, out idy);

            float crossProduct = -dy * idx + dx * idy;
            return crossProduct < 0;
        }

        /// <summary>
        /// Return whether the hull vertex at index is visible from the point
        /// </summary>
        public bool EdgeVisibleFrom(int index, Vertex point)
        {
            float idx, idy;
            VectorToNext(index, out idx, out idy);

            float dx = point.x - this[index].x;
            float dy = point.y - this[index].y;

            float crossProduct = -dy * idx + dx * idy;
            return crossProduct < 0;
        }
    }



}
