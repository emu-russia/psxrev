using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CollinearSolver
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void loadEntitiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                entityBox1.Unserialize(openFileDialog1.FileName, true);
            }
        }

        private void saveEntitiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                entityBox1.Serialize(saveFileDialog1.FileName);
            }
        }

        private void solveSingleSegmentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SolveSingle(GetViases());
        }

        private void solveAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            List<Entity> viases = GetViases();

            Cursor.Current = Cursors.WaitCursor;

            while (viases.Count != 0)
            {
                SolveSingle(viases);

                viases = GetViases();
            }

            Cursor.Current = Cursors.Default;
        }

        private List<Entity> GetViases()
        {
            List<Entity> viases = new List<Entity>();

            foreach (Entity entity in entityBox1.GetEntities())
            {
                if (entity.IsVias())
                    viases.Add(entity);
            }

            return viases;
        }

        private void SolveSingle(List<Entity> viases)
        {
            int numPoints = 0;

            const float distanceDelta = 1.0F;
            const float distanceMax = 2.0F;
            const float angleDelta = 0.5F;

            // Just remove viases if there are not enough of them

            if (viases.Count < 2)
            {
                for (int i=0; i<viases.Count; i++)
                {
                    RemoveEntity(viases[i]);
                }
                return;
            }

            // Get first vias

            Entity first = viases[0];
            numPoints++;

            Entity nearest = null;

            // Get nearest

            float distMin = float.MaxValue;

            for(int i=1; i<viases.Count; i++)
            {
                float distance = Distance(first, viases[i]);

                if (distance < distMin && distance < distanceMax)
                {
                    distMin = distance;
                    nearest = viases[i];
                    numPoints++;
                }
            }

            // Just remove first vias if there is no nearest

            if (nearest == null)
            {
                RemoveEntity(first);
                return;
            }

            // Remove nearest from source set

            first.Selected = true;
            nearest.Selected = true;

            viases.Remove(nearest);

            RemoveEntity(first);
            RemoveEntity(nearest);

            float firstAngle = Angle(first, nearest);

            while (viases.Count != 0)
            {
                Entity nextNearest = null;

                for (int i=1; i<viases.Count; i++)
                {
                    float distance = Distance(nearest, viases[i]);
                    float angle = Angle(nearest, viases[i]);

                    if (distance <= (distMin + distanceDelta) )
                    {
                        if (((firstAngle - angleDelta) <= angle && angle <= (firstAngle + angleDelta)))
                        {
                            nextNearest = viases[i];
                            numPoints++;
                            break;
                        }
                    }
                }

                if (nextNearest == null)
                {
                    break;
                }

                nearest = nextNearest;
                nearest.Selected = true;

                viases.Remove(nextNearest);
                RemoveEntity(nextNearest);
            }

            // Draw wire if there is at least 3 points

            if (numPoints > 2)
            {
                Point startPoint = entityBox1.LambdaToScreen(first.LambdaX, first.LambdaY);
                Point endPoint = entityBox1.LambdaToScreen(nearest.LambdaX, nearest.LambdaY);

                entityBox1.AddWire(EntityType.WireInterconnect,
                    startPoint.X, startPoint.Y,
                    endPoint.X, endPoint.Y);
            }

            entityBox1.Invalidate();
        }

        private float Distance (Entity a, Entity b)
        {
            return (float)Math.Sqrt(
                    Math.Pow(a.LambdaX - b.LambdaX, 2) +
                    Math.Pow(a.LambdaY - b.LambdaY, 2));
        }

        private float Angle (Entity a, Entity b)
        {
            return (float)Math.Atan2(b.LambdaY - a.LambdaY, b.LambdaX - a.LambdaX);
        }

        private void RemoveEntity(Entity entity)
        {
            entity.parent.Children.Remove(entity);
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if ( e.KeyCode == Keys.Space)
            {
                SolveSingle(GetViases());
            }
        }


    }
}
