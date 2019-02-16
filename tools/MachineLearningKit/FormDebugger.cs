using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using NeuralNetwork;

namespace MachineLearningKit
{
    public partial class FormDebugger : Form
    {
        private EntityNetwork nn;
        private Entity[][] inputHidden;
        private Entity[][] hiddenOutput;

        public FormDebugger(EntityNetwork nn)
        {
            InitializeComponent();

            SelectNetwork(nn);
        }

        public void SelectNetwork (EntityNetwork nn)
        {
            this.nn = nn;

            if (nn != null)
            {
                inputHidden = BuildEntityMatrix(nn._state._weightInputHidden, new Point(0, 0), 16, 16 * 3, 16);
                hiddenOutput = BuildEntityMatrix(nn._state._weightHiddenOutput, new Point(16, 0), 16, 16 * 3, 16);
            }
        }

        public void UpdateNetworkView()
        {
            if (nn == null)
                return;

            DrawMatrix(inputHidden, nn._state._weightInputHidden);
            DrawMatrix(hiddenOutput, nn._state._weightHiddenOutput);
        }

        private Entity[][] BuildEntityMatrix (double[][] matrix, Point point, int size, int deltaX, int deltaY)
        {
            var rows = matrix.Length;
            Entity[][] ematrix = new Entity[rows][];

            for (var row = 0; row < rows; row++)
            {
                ematrix[row] = new Entity[matrix[row].Length];

                for (var col = 0; col < matrix[row].Length; col++)
                {
                    ematrix[row][col] = entityBox1.AddCell(EntityType.CellOther,
                        point.X + col * (size + deltaX), point.Y + row * (size + deltaY),
                        point.X + size + col * (size + deltaX), point.Y + size + row * (size + deltaY),
                        false);
                }
            }

            return ematrix;
        }

        private void DrawMatrix (Entity[][] ematrix, double [][] matrix)
        {
            var rows = matrix.Length;

            for (var row = 0; row < rows; row++)
            {
                for (var col = 0; col < matrix[row].Length; col++)
                {
                    int percentage = (int)((matrix[row][col] + 1.0F) / 2.0F * 100.0F);
                    ematrix[row][col].ColorOverride = GetBlendedColor(percentage);
                }
            }

            entityBox1.Invalidate();
        }

        // https://stackoverflow.com/questions/6394304/algorithm-how-do-i-fade-from-red-to-green-via-yellow-using-rgb-values

        public Color GetBlendedColor(int percentage)
        {
            if (percentage < 50)
                return Interpolate(Color.Red, Color.Yellow, percentage / 50.0);
            return Interpolate(Color.Yellow, Color.Lime, (percentage - 50) / 50.0);
        }

        private Color Interpolate(Color color1, Color color2, double fraction)
        {
            double r = Interpolate(color1.R, color2.R, fraction);
            double g = Interpolate(color1.G, color2.G, fraction);
            double b = Interpolate(color1.B, color2.B, fraction);
            return Color.FromArgb((int)Math.Round(r), (int)Math.Round(g), (int)Math.Round(b));
        }

        private double Interpolate(double d1, double d2, double fraction)
        {
            return d1 + (d2 - d1) * fraction;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            UpdateNetworkView();
        }

    }
}
