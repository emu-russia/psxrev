using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace RubberTool
{
    public partial class FormAbout : Form
    {
        private int stepCurrent = 0;
        private const int stepsAmount = 25;
        private const int msPerStep = 20;

        public FormAbout()
        {
            InitializeComponent();

            SpawnKeypoints();

            backgroundWorker1.RunWorkerAsync();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private static float Lerp(float firstFloat, float secondFloat, float by)
        {
            return firstFloat + by * (secondFloat - firstFloat);
        }

        PointF Lerp(PointF firstVector, PointF secondVector, float by)
        {
            float retX = Lerp(firstVector.X, secondVector.X, by);
            float retY = Lerp(firstVector.X, secondVector.Y, by);
            return new PointF(retX, retY);
        }

        private void SpawnKeypoints ()
        {
            Random rnd = new Random();

            int maxPoints = rnd.Next(10, 25);

            for(int i=0;i<maxPoints; i++)
            {
                int x = rnd.Next(0, entityBox1.Width - 1);
                int y = rnd.Next(0, entityBox1.Height - 1);

                Entity vias = entityBox1.AddVias(EntityType.ViasConnect, x, y, entityBox1.ViasConnectColor);
            }
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                Thread.Sleep(msPerStep);

                backgroundWorker1.ReportProgress(0);
            }
        }

        private void GenMesh ()
        {
            Delaunay delaunay = new Delaunay();
            List<Point2D> points = new List<Point2D>();

            foreach (Entity entity in entityBox1._entities)
            {
                Point screen = entityBox1.LambdaToScreen(entity.LambdaX, entity.LambdaY);
                points.Add(new Point2D(screen.X, screen.Y));
            }

            //
            // Триангулируем
            //

            List<Triangle> mesh = delaunay.GenMesh(points);

            //
            // Отобразить треугольную сетку
            //

            foreach (Triangle tri in mesh)
            {
                AddTriangle(entityBox1, tri);
            }
        }

        private void AddTriangle(EntityBox box, Triangle tri)
        {
            box.AddWire(EntityType.WireInterconnect, (int)tri.a.X, (int)tri.a.Y, (int)tri.b.X, (int)tri.b.Y);
            box.AddWire(EntityType.WireInterconnect, (int)tri.b.X, (int)tri.b.Y, (int)tri.c.X, (int)tri.c.Y);
            box.AddWire(EntityType.WireInterconnect, (int)tri.c.X, (int)tri.c.Y, (int)tri.a.X, (int)tri.a.Y);
        }

        private void ClearMesh()
        {
            List<Entity> ents = new List<Entity>();

            foreach (Entity entity in entityBox1._entities)
            {
                if (EntityBox.IsEntityWire(entity))
                {
                    ents.Add(entity);
                }
            }

            foreach (Entity entity in ents)
            {
                entityBox1._entities.Remove(entity);
            }

            entityBox1.Invalidate();
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (stepCurrent == 0)
            {
                CreatePath();
            }

            LerpPath();

            stepCurrent++;

            if (stepCurrent >= stepsAmount)
            {
                stepCurrent = 0;
            }
        }

        private void LerpPath ()
        {
            foreach (Entity entity in entityBox1._entities)
            {
                entity.LambdaX = Lerp(entity.LambdaX, entity.LambdaEndX, 1F / (float)stepsAmount);
                entity.LambdaY = Lerp(entity.LambdaY, entity.LambdaEndY, 1F / (float)stepsAmount);
            }

            ClearMesh();
            GenMesh();
        }

        private void CreatePath ()
        {
            Random rnd = new Random();

            foreach (Entity entity in entityBox1._entities)
            {
                entity.LambdaEndX = entity.LambdaX + ( -10 + (float)rnd.NextDouble() * 20) ;
                entity.LambdaEndY = entity.LambdaY + ( -10 + (float)rnd.NextDouble() * 20) ;
            }
        }
    }
}
