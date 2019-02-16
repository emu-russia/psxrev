// Wire router

// TODO: Implement local grid refinement to optimize wall check

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

using AStar;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {
        private float boxPadding = 2F;

        //
        // AStar WRAPPER
        //

        /// <summary>
        /// Route wires between two viases bounding shapes(walls)
        /// </summary>
        /// <param name="vias1">start point</param>
        /// <param name="vias2">end point</param>
        /// <param name="shapes">walls</param>
        /// <param name="select"></param>
        /// <returns></returns>

        public List<Entity> Route ( Entity vias1, Entity vias2, 
            List<Entity> shapes,
            bool select )
        {
            long Stamp1;
            long Stamp2;

            List<Entity> wires = new List<Entity>();

            Point start = LambdaToImage(vias1.LambdaX, vias1.LambdaY);
            Point end = LambdaToImage(vias2.LambdaX, vias2.LambdaY);

            //
            // Remove home shapes 
            //

            List<Entity> pendingRemove = new List<Entity>();

            PointF point1 = new PointF(vias1.LambdaX, vias1.LambdaY);
            PointF point2 = new PointF(vias2.LambdaX, vias2.LambdaY);

            foreach ( Entity shape in shapes)
            {
                PointF topLeft;
                PointF bottomRight;

                GetBoundingBox(shape, out topLeft, out bottomRight);

                if ( IsPointInBox ( point1, topLeft, bottomRight ) ||
                    IsPointInBox(point2, topLeft, bottomRight) )
                {
                    pendingRemove.Add(shape);
                }
            }

            foreach ( Entity shape in pendingRemove )
            {
                shapes.Remove(shape);
            }

            //
            // Get global bounding box
            //

            int maxx = Math.Max(start.X, end.X);
            int maxy = Math.Max(start.Y, end.Y);

            foreach ( Entity shape in shapes )
            {
                PointF topLeft;
                PointF bottomRight;

                GetBoundingBox(shape, out topLeft, out bottomRight);

                Point bottomRightScreen = LambdaToImage(bottomRight.X, bottomRight.Y);

                if ( bottomRightScreen.X > maxx )
                {
                    maxx = bottomRightScreen.X;
                }
                if (bottomRightScreen.Y > maxy)
                {
                    maxy = bottomRightScreen.Y;
                }
            }

            //
            // Generate grid
            //

            Stamp1 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            var grid = new Grid2D(this,
                maxx + 1,
                maxy + 1,
                shapes,
                start.X, start.Y,
                end.X, end.Y);

            Stamp2 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            Console.WriteLine("Grid2D: " + (Stamp2 - Stamp1).ToString() + " ms");

            //
            // Dispatch router
            //

            Stamp1 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            var astar = new AStarRouter(grid.Start, grid.Goal);

            var result = astar.Run();

            Stamp2 = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            Console.WriteLine("AStarRouter: " + (Stamp2 - Stamp1).ToString() + " ms");

            //
            // Draw wires
            //

            IEnumerable<INode> path = astar.GetPath();

            GridNode startNode = (GridNode)path.First();
            GridNode prevNode = (GridNode)path.First();

            Entity wire;
            Point startPlane;
            Point prevPlane;

            foreach (INode node in path)
            {
                if ( path.First() == node )
                {
                    continue;
                }

                GridNode gridNode = (GridNode)node;

                if ( gridNode.X != startNode.X && 
                    gridNode.Y != startNode.Y )
                {
                    startPlane = grid.GridToPlane(new Point(startNode.X, startNode.Y));
                    prevPlane = grid.GridToPlane(new Point(prevNode.X, prevNode.Y));

                    wire = AddWireOnImage(EntityType.WireInterconnect,
                        startPlane.X, startPlane.Y,
                        prevPlane.X, prevPlane.Y);

                    if ( wire == null )
                    {
                        Console.WriteLine("!!!");
                    }

                    if (select && wire != null)
                    {
                        wire.Selected = true;
                    }
                    if (wire != null)
                    {
                        wires.Add(wire);
                    }

                    startNode = prevNode;
                }

                prevNode = gridNode;
            }

            startPlane = grid.GridToPlane(new Point(startNode.X, startNode.Y));
            prevPlane = grid.GridToPlane(new Point(prevNode.X, prevNode.Y));

            wire = AddWireOnImage(EntityType.WireInterconnect,
                startPlane.X, startPlane.Y,
                prevPlane.X, prevPlane.Y);

            if (select && wire != null)
            {
                wire.Selected = true;
            }
            if (wire != null)
            {
                wires.Add(wire);
            }

            return wires;
        }

        private static int GCD(int a, int b)
        {
            while (a != 0 && b != 0)
            {
                if (a > b)
                    a %= b;
                else
                    b %= a;
            }

            return a == 0 ? b : a;
        }

        private bool IsPointInBox ( PointF point, PointF topLeft, PointF bottomRight)
        {
            return (point.X >= topLeft.X && point.X <= bottomRight.X &&
                 point.Y >= topLeft.Y && point.Y <= bottomRight.Y);
        }

        private void GetBoundingBox (Entity entity, out PointF topLeft, out PointF bottomRight)
        {
            topLeft = new PointF(0, 0);
            bottomRight = new PointF(0, 0);

            if (entity.IsCell())
            {
                float minx = entity.LambdaX - boxPadding;
                float miny = entity.LambdaY - boxPadding;
                float maxx = entity.LambdaX + entity.LambdaWidth + boxPadding;
                float maxy = entity.LambdaY + entity.LambdaHeight + boxPadding;

                topLeft = new PointF(minx, miny);
                bottomRight = new PointF(maxx, maxy);
            }
            else if (entity.IsRegion())
            {
                float minx = float.MaxValue;
                float miny = float.MaxValue;
                float maxx = 0;
                float maxy = 0;

                foreach (PointF v in entity.PathPoints)
                {
                    if (v.X < minx)
                    {
                        minx = v.X;
                    }
                    if (v.Y < miny)
                    {
                        miny = v.Y;
                    }
                    if (v.X > maxx)
                    {
                        maxx = v.X;
                    }
                    if (v.Y > maxy)
                    {
                        maxy = v.Y;
                    }
                }

                minx -= boxPadding;
                miny -= boxPadding;
                maxx += boxPadding;
                maxy += boxPadding;

                topLeft = new PointF(minx, miny);
                bottomRight = new PointF(maxx, maxy);
            }
        }

        private bool IsPointInCell(PointF point, Entity cell)
        {
            if ( !cell.IsCell())
            {
                return false;
            }

            PointF topLeft;
            PointF bottomRight;

            GetBoundingBox(cell, out topLeft, out bottomRight);

            return IsPointInBox(point, topLeft, bottomRight);
        }

        private bool IsPointInRegion(PointF point, Entity region)
        {
            if (!region.IsRegion())
            {
                return false;
            }

            PointF topLeft;
            PointF bottomRight;

            GetBoundingBox(region, out topLeft, out bottomRight);

            return IsPointInBox(point, topLeft, bottomRight);
        }

        public class Grid2D
        {
            public GridNode[][] Grid;

            public int Width { get { return Grid.Length; } }
            public int Height { get { return Grid[0].Length; } }

            public GridNode Start;
            public GridNode Goal;

            Point planeMax;
            Point planeStart;
            Point planeEnd;

            Point gridMax;
            Point gridStart;
            Point gridEnd;

            public Point PlaneToGrid(Point planePoint)
            {
                Point gridPoint = new Point();

                gridPoint.X = planePoint.X;
                gridPoint.Y = planePoint.Y;

                return gridPoint;
            }

            public Point GridToPlane(Point gridPoint)
            {
                Point planePoint = new Point();

                planePoint.X = gridPoint.X;
                planePoint.Y = gridPoint.Y;

                return planePoint;
            }

            public Grid2D(GridNode[][] grid, GridNode start, GridNode goal)
            {
                Grid = grid;
                Start = start;
                Goal = goal;
            }

            public Grid2D (EntityBox box, int maxX, int maxY, List<Entity> shapes, int startX, int startY, int goalX, int goalY)
            {
                planeMax = new Point(maxX, maxY);
                planeStart = new Point(startX, startY);
                planeEnd = new Point(goalX, goalY);

                gridMax = PlaneToGrid(new Point(maxX, maxY));
                gridStart = PlaneToGrid(new Point(startX, startY));
                gridEnd = PlaneToGrid(new Point(goalX, goalY));

                Start = new GridNode(this, gridStart.X, gridStart.Y, false);
                Goal = new GridNode(this, gridEnd.X, gridEnd.Y, false);

                Grid = new GridNode[gridMax.X][];
                for (var i = 0; i < gridMax.X; i++)
                    Grid[i] = new GridNode[gridMax.Y];

                Grid[Start.X][Start.Y] = Start;
                Grid[Goal.X][Goal.Y] = Goal;

                for (var i = 0; i < gridMax.X; i++)
                {
                    for (var j = 0; j < gridMax.Y; j++)
                    {
                        // don't overwrite start/goal nodes
                        if (Grid[i][j] != null)
                            continue;

                        Point planePoint = GridToPlane(new Point(i, j));

                        PointF pos = box.ImageToLambda(planePoint.X, planePoint.Y);

                        bool wall = false;

                        foreach (Entity entity in shapes)
                        {
                            if (box.IsPointInRegion(pos, entity) ||
                                box.IsPointInCell(pos, entity))
                            {
                                wall = true;
                                break;
                            }
                        }

                        Grid[i][j] = new GridNode(this, i, j, wall);
                    }
                }
            }

            public string Print(IEnumerable<INode> path)
            {
                var output = "";
                for (var i = 0; i < Width; i++)
                {
                    for (var j = 0; j < Height; j++)
                    {
                        output += Grid[i][j].Print(Start, Goal, path);
                    }
                    output += "\n";
                }
                return output;
            }
        }


        public class GridNode : INode
        {
            private bool isOpenList = false;
            private bool isClosedList = false;

            public int X { get; private set; }
            public int Y { get; private set; }

            public bool IsWall { get; set; }

            public Grid2D parentGrid;

            public GridNode(Grid2D grid, int x, int y, bool isWall)
            {
                parentGrid = grid;
                X = x;
                Y = y;
                IsWall = isWall;
            }

            /// <summary>
            /// Gets or sets whether this node is on the open list.
            /// </summary>
            public bool IsOpenList(IEnumerable<INode> openList)
            {
                return isOpenList;
            }

            public void SetOpenList(bool value)
            {
                isOpenList = value;
            }

            /// <summary>
            /// If this is a wall then return as unsearchable without ever checking the node.
            /// </summary>
            public bool IsClosedList(IEnumerable<INode> closedList)
            {
                return IsWall || isClosedList;
            }

            public void SetClosedList(bool value)
            {
                isClosedList = value;
            }

            /// <summary>
            /// Gets the total cost for this node.
            /// f = g + h
            /// TotalCost = MovementCost + EstimatedCost
            /// </summary>
            public int TotalCost { get { return MovementCost + EstimatedCost; } }

            /// <summary>
            /// Gets the movement cost for this node.
            /// This is the movement cost from this node to the starting node, or g.
            /// </summary>
            public int MovementCost { get; private set; }

            /// <summary>
            /// Gets the estimated cost for this node.
            /// This is the heuristic from this node to the goal node, or h.
            /// </summary>
            public int EstimatedCost { get; private set; }

            /// <summary>
            /// Parent.MovementCost + 1
            /// </summary>
            /// <param name="parent">Parent node, for access to the parents movement cost.</param>
            public void SetMovementCost(INode parent)
            {
                this.MovementCost = parent.MovementCost + 1;
            }

            /// <summary>
            /// Simple manhatten.
            /// </summary>
            /// <param name="goal">Goal node, for acces to the goals position.</param>
            public void SetEstimatedCost(INode goal)
            {
                var g = (GridNode)goal;
                this.EstimatedCost = Math.Abs(this.X - g.X) + Math.Abs(this.Y - g.Y);
            }

            /// <summary>
            /// Gets or sets the parent node to this node.
            /// </summary>
            public INode Parent { get; set; }

            // X - 1, Y - 1 | X, Y - 1 | X + 1, Y - 1
            // X - 1, Y     |          | X + 1, Y
            // X - 1, Y + 1 | X, Y + 1 | X + 1, Y + 1
            //private static int[] childXPos = new int[] { -1, 0, 1, -1, 1, -1, 0, 1 };
            //private static int[] childYPos = new int[] { -1, -1, -1, 0, 0, 1, 1, 1 };

            private static int[] childXPos = new int[] { 0, -1, 1, 0, };
            private static int[] childYPos = new int[] { -1, 0, 0, 1, };

            /// <summary>
            /// Gets this node's children.
            /// </summary>
            /// <remarks>The children can be setup in a graph before starting the
            /// A* algorithm or they can be dynamically generated the first time
            /// the A* algorithm calls this property.</remarks>
            public IEnumerable<INode> Children
            {
                get
                {
                    var children = new List<GridNode>();

                    for (int i = 0; i < childXPos.Length; i++)
                    {
                        // skip any nodes out of bounds.
                        if (X + childXPos[i] >= parentGrid.Width || Y + childYPos[i] >= parentGrid.Height)
                            continue;
                        if (X + childXPos[i] < 0 || Y + childYPos[i] < 0)
                            continue;

                        children.Add(parentGrid.Grid[X + childXPos[i]][Y + childYPos[i]]);
                    }

                    return children;
                }
            }

            /// <summary>
            /// Returns true if this node is the goal, false if it is not the goal.
            /// </summary>
            /// <param name="goal">The goal node to compare this node against.</param>
            /// <returns>True if this node is the goal, false if it s not the goal.</returns>
            public bool IsGoal(INode goal)
            {
                return IsEqual((GridNode)goal);
            }

            /// <summary>
            /// Two nodes are equal if they share the same spot in the grid.
            /// </summary>
            /// <param name="node"></param>
            /// <returns></returns>
            public bool IsEqual(GridNode node)
            {
                return (this == node) || (this.X == node.X && this.Y == node.Y);
            }

            public string Print(GridNode start, GridNode goal, IEnumerable<INode> path)
            {
                if (IsWall)
                {
                    return "W";
                }
                else if (IsEqual(start))
                {
                    return "s";
                }
                else if (IsEqual(goal))
                {
                    return "g";
                }
                else if (IsInPath(path))
                {
                    return ".";
                }
                else
                {
                    return " ";
                }
            }

            private bool IsInPath(IEnumerable<INode> path)
            {
                foreach (var node in path)
                {
                    if (IsEqual((GridNode)node))
                        return true;
                }
                return false;
            }
        }




    }
}


// astar-1.0.cs may be freely distributed under the MIT license.
// Copyright(c) 2013 Josh Baldwin
// https://github.com/jbaldwin/astar.cs

#region AStar Implementation

namespace AStar
{

    /// <summary>
    /// The A* algorithm takes a starting node and a goal node and searchings from
    /// start to the goal.
    /// 
    /// The nodes can be setup in a graph ahead of running the algorithm or the children
    /// nodes can be generated on the fly when the A* algorithm requests the Children property.
    /// 
    /// See the square puzzle implementation to see the children being generated on the fly instead
    /// of the classical image/graph search with walls.
    /// </summary>
    public interface INode
    {
        /// <summary>
        /// Determines if this node is on the open list.
        /// </summary>
        bool IsOpenList(IEnumerable<INode> openList);

        /// <summary>
        /// Sets this node to be on the open list.
        /// </summary>
        void SetOpenList(bool value);

        /// <summary>
        /// Determines if this node is on the closed list.
        /// </summary>
        bool IsClosedList(IEnumerable<INode> closedList);

        /// <summary>
        /// Sets this node to be on the open list.
        /// </summary>
        void SetClosedList(bool value);

        /// <summary>
        /// Gets the total cost for this node.
        /// f = g + h
        /// TotalCost = MovementCost + EstimatedCost
        /// </summary>
        int TotalCost { get; }

        /// <summary>
        /// Gets the movement cost for this node.
        /// This is the movement cost from this node to the starting node, or g.
        /// </summary>
        int MovementCost { get; }

        /// <summary>
        /// Gets the estimated cost for this node.
        /// This is the heuristic from this node to the goal node, or h.
        /// </summary>
        int EstimatedCost { get; }

        /// <summary>
        /// Sets the movement cost for the current node, or g.
        /// </summary>
        /// <param name="parent">Parent node, for access to the parents movement cost.</param>
        void SetMovementCost(INode parent);

        /// <summary>
        /// Sets the estimated cost for the current node, or h.
        /// </summary>
        /// <param name="goal">Goal node, for acces to the goals position.</param>
        void SetEstimatedCost(INode goal);

        /// <summary>
        /// Gets or sets the parent node to this node.
        /// </summary>
        INode Parent { get; set; }

        /// <summary>
        /// Gets this node's children.
        /// </summary>
        /// <remarks>The children can be setup in a graph before starting the
        /// A* algorithm or they can be dynamically generated the first time
        /// the A* algorithm calls this property.</remarks>
        IEnumerable<INode> Children { get; }

        /// <summary>
        /// Returns true if this node is the goal, false if it is not the goal.
        /// </summary>
        /// <param name="goal">The goal node to compare this node against.</param>
        /// <returns>True if this node is the goal, false if it s not the goal.</returns>
        bool IsGoal(INode goal);
    }


    /// <summary>
    /// AStar algorithm states while searching for the goal.
    /// </summary>
    public enum State
    {
        /// <summary>
        /// The AStar algorithm is still searching for the goal.
        /// </summary>
        Searching,

        /// <summary>
        /// The AStar algorithm has found the goal.
        /// </summary>
        GoalFound,

        /// <summary>
        /// The AStar algorithm has failed to find a solution.
        /// </summary>
        Failed
    }

    /// <summary>
    /// System.Collections.Generic.SortedList by default does not allow duplicate items.
    /// Since items are keyed by TotalCost there can be duplicate entries per key.
    /// </summary>
    internal class DuplicateComparer : IComparer<int>
    {
        public int Compare(int x, int y)
        {
            return (x <= y) ? -1 : 1;
        }
    }

    /// <summary>
    /// Interface to setup and run the AStar algorithm.
    /// </summary>
    public class AStarRouter
    {
        /// <summary>
        /// The open list.
        /// </summary>
        private SortedList<int, INode> openList;

        /// <summary>
        /// The closed list.
        /// </summary>
        private SortedList<int, INode> closedList;

        /// <summary>
        /// The current node.
        /// </summary>
        private INode current;

        /// <summary>
        /// The goal node.
        /// </summary>
        private INode goal;

        /// <summary>
        /// Gets the current amount of steps that the algorithm has performed.
        /// </summary>
        public int Steps { get; private set; }

        /// <summary>
        /// Gets the current state of the open list.
        /// </summary>
        public IEnumerable<INode> OpenList { get { return openList.Values; } }

        /// <summary>
        /// Gets the current state of the closed list.
        /// </summary>
        public IEnumerable<INode> ClosedList { get { return closedList.Values; } }

        /// <summary>
        /// Gets the current node that the AStar algorithm is at.
        /// </summary>
        public INode CurrentNode { get { return current; } }

        /// <summary>
        /// Creates a new AStar algorithm instance with the provided start and goal nodes.
        /// </summary>
        /// <param name="start">The starting node for the AStar algorithm.</param>
        /// <param name="goal">The goal node for the AStar algorithm.</param>
        public AStarRouter(INode start, INode goal)
        {
            var duplicateComparer = new DuplicateComparer();
            openList = new SortedList<int, INode>(duplicateComparer);
            closedList = new SortedList<int, INode>(duplicateComparer);
            Reset(start, goal);
        }

        /// <summary>
        /// Resets the AStar algorithm with the newly specified start node and goal node.
        /// </summary>
        /// <param name="start">The starting node for the AStar algorithm.</param>
        /// <param name="goal">The goal node for the AStar algorithm.</param>
        public void Reset(INode start, INode goal)
        {
            openList.Clear();
            closedList.Clear();
            current = start;
            this.goal = goal;
            openList.Add(current);
            current.SetOpenList(true);
        }

        /// <summary>
        /// Steps the AStar algorithm forward until it either fails or finds the goal node.
        /// </summary>
        /// <returns>Returns the state the algorithm finished in, Failed or GoalFound.</returns>
        public State Run()
        {
            // Continue searching until either failure or the goal node has been found.
            while (true)
            {
                State s = Step();
                if (s != State.Searching)
                    return s;
            }
        }

        /// <summary>
        /// Moves the AStar algorithm forward one step.
        /// </summary>
        /// <returns>Returns the state the alorithm is in after the step, either Failed, GoalFound or still Searching.</returns>
        public State Step()
        {
            Steps++;
            while (true)
            {
                // There are no more nodes to search, return failure.
                if (openList.IsEmpty())
                {
                    return State.Failed;
                }

                // Check the next best node in the graph by TotalCost.
                current = openList.Pop();

                // This node has already been searched, check the next one.
                if (current.IsClosedList(ClosedList))
                {
                    continue;
                }

                // An unsearched node has been found, search it.
                break;
            }

            // Remove from the open list and place on the closed list 
            // since this node is now being searched.
            current.SetOpenList(false);
            closedList.Add(current);
            current.SetClosedList(true);

            // Found the goal, stop searching.
            if (current.IsGoal(goal))
            {
                return State.GoalFound;
            }

            // Node was not the goal so add all children nodes to the open list.
            // Each child needs to have its movement cost set and estimated cost.
            foreach (var child in current.Children)
            {
                // If the child has already been searched (closed list) or is on
                // the open list to be searched then do not modify its movement cost
                // or estimated cost since they have already been set previously.
                if (child.IsOpenList(OpenList) || child.IsClosedList(ClosedList))
                {
                    continue;
                }

                child.Parent = current;
                child.SetMovementCost(current);
                child.SetEstimatedCost(goal);
                openList.Add(child);
                child.SetOpenList(true);
            }

            // This step did not find the goal so return status of still searching.
            return State.Searching;
        }

        /// <summary>
        /// Gets the path of the last solution of the AStar algorithm.
        /// Will return a partial path if the algorithm has not finished yet.
        /// </summary>
        /// <returns>Returns null if the algorithm has never been run.</returns>
        public IEnumerable<INode> GetPath()
        {
            if (current != null)
            {
                var next = current;
                var path = new List<INode>();
                while (next != null)
                {
                    path.Add(next);
                    next = next.Parent;
                }
                path.Reverse();
                return path.ToArray();
            }
            return null;
        }
    }

    /// <summary>
    /// Extension methods to make the System.Collections.Generic.SortedList easier to use.
    /// </summary>
    internal static class SortedListExtensions
    {
        /// <summary>
        /// Checks if the SortedList is empty.
        /// </summary>
        /// <param name="sortedList">SortedList to check if it is empty.</param>
        /// <returns>True if sortedList is empty, false if it still has elements.</returns>
        internal static bool IsEmpty<TKey, TValue>(this SortedList<TKey, TValue> sortedList)
        {
            return sortedList.Count == 0;
        }

        /// <summary>
        /// Adds a INode to the SortedList.
        /// </summary>
        /// <param name="sortedList">SortedList to add the node to.</param>
        /// <param name="node">Node to add to the sortedList.</param>
        internal static void Add(this SortedList<int, INode> sortedList, INode node)
        {
            sortedList.Add(node.TotalCost, node);
        }

        /// <summary>
        /// Removes the node from the sorted list with the smallest TotalCost and returns that node.
        /// </summary>
        /// <param name="sortedList">SortedList to remove and return the smallest TotalCost node.</param>
        /// <returns>Node with the smallest TotalCost.</returns>
        internal static INode Pop(this SortedList<int, INode> sortedList)
        {
            var top = sortedList.Values[0];
            sortedList.RemoveAt(0);
            return top;
        }
    }

}

#endregion

