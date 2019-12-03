using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Dromos
{
    public partial class Form1 : Form
    {
        Random rnd = new Random();
        CanvasInputAdapter input;

        public Form1()
        {
            InitializeComponent();

            input = new CanvasInputAdapter(canvasControl1);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private Color RandomColor()
        {
            return Color.FromArgb(rnd.Next(128,256), rnd.Next(256), rnd.Next(256), rnd.Next(256));
        }

        private void RandomPoint(Color color)
        {
            PointF pos = canvasControl1.ScreenToWorld(new Point(rnd.Next(Width), rnd.Next(Height)));

            canvasControl1.AddItem(
                new CanvasControl.CanvasPoint(
                    pos,
                    rnd.Next(4, 20),
                    color));
        }

        private void addPointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RandomPoint(Color.Beige);

            canvasControl1.Invalidate();
        }

        private void add1000RandomPointsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i=0; i<1000;i++)
            {
                RandomPoint(RandomColor());
            }

            canvasControl1.Invalidate();
        }

        private void removeAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            canvasControl1.RemoveAllItems();
            canvasControl1.Invalidate();
        }

        private void RandomRect(Color color)
        {
            PointF pos = canvasControl1.ScreenToWorld(new Point(rnd.Next(Width), rnd.Next(Height)));
            float width = rnd.Next(10, 100);
            float height = rnd.Next(10, 100);

            canvasControl1.AddItem(
                new CanvasControl.CanvasRect(
                    pos,
                    width,
                    height,
                    color));
        }

        private void addRectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RandomRect(Color.DeepPink);

            canvasControl1.Invalidate();
        }

        private void add100RectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 100; i++)
            {
                RandomRect(RandomColor());
            }

            canvasControl1.Invalidate();
        }

        private void RandomLine(Color color)
        {
            PointF start = canvasControl1.ScreenToWorld(new Point(rnd.Next(Width), rnd.Next(Height)));
            PointF end = canvasControl1.ScreenToWorld(new Point(rnd.Next(Width), rnd.Next(Height)));

            canvasControl1.AddItem(
                new CanvasControl.CanvasLine(start, end, 
                rnd.Next(1, 5),
                color));
                
        }

        private void addLineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RandomLine(Color.MediumOrchid);

            canvasControl1.Invalidate();
        }

        private void add100LinesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 100; i++)
            {
                RandomLine(RandomColor());
            }

            canvasControl1.Invalidate();
        }

        private void RandomPolyLine (Color color)
        {
            int count = rnd.Next(2, 5) + 1;
            List<PointF> points = new List<PointF>();

            while (count-- != 0)
            {
                points.Add(
                    canvasControl1.ScreenToWorld(new Point(rnd.Next(Width), rnd.Next(Height))) );
            }

            canvasControl1.AddItem(
                new CanvasControl.CanvasPolyLine(points,
                rnd.Next(1, 5),
                color));
        }

        private void addPolyLineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RandomPolyLine(RandomColor());

            canvasControl1.Invalidate();
        }

        private void add25PolyLinesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 25; i++)
            {
                RandomPolyLine(RandomColor());
            }

            canvasControl1.Invalidate();
        }

        private void RandomRegion (Color color)
        {
            int count = rnd.Next(2, 5) + 2;
            List<PointF> points = new List<PointF>();

            while (count-- != 0)
            {
                points.Add(canvasControl1.ScreenToWorld(new Point(rnd.Next(Width), rnd.Next(Height))));
            }

            canvasControl1.AddItem(
                new CanvasControl.CanvasPoly(points,
                rnd.Next(1, 5),
                color));
        }

        private void addRegionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RandomRegion(RandomColor());

            canvasControl1.Invalidate();
        }

        private void add100RegionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 100; i++)
            {
                RandomRegion(RandomColor());
            }

            canvasControl1.Invalidate();
        }

        private void zoom10ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            canvasControl1.Zoom += 10;
            canvasControl1.Invalidate();
        }

        private void zoom10ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            canvasControl1.Zoom -= 10;
            canvasControl1.Invalidate();
        }

        private void panX10ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            canvasControl1.Scroll.X += 10F;
            canvasControl1.Invalidate();
        }

        private void panX10ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            canvasControl1.Scroll.X -= 10F;
            canvasControl1.Invalidate();
        }

        private void panY10ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            canvasControl1.Scroll.Y += 10F;
            canvasControl1.Invalidate();
        }

        private void panY10ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            canvasControl1.Scroll.Y -= 10F;
            canvasControl1.Invalidate();
        }

        private void homeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            canvasControl1.Scroll.X = 0;
            canvasControl1.Scroll.Y = 0;
            canvasControl1.Invalidate();
        }
    }
}
