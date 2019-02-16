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
    public partial class FormCoverage : Form
    {
        private Bitmap sourceBitmap;
        private EntityNetwork nn;

        public FormCoverage(Image image, EntityNetwork nn)
        {
            InitializeComponent();

            /// Load source image

            sourceBitmap = new Bitmap(image);
            this.nn = nn;

            entityBox1.Mode = EntityMode.ImageLayer0;
            entityBox1.LoadImage(image);
            entityBox1.Mode = EntityMode.Selection;

            /// Fill combobox by Features

            for(int i=0; i<nn._state.features.Count; i++)
            {
                comboBox1.Items.Add( nn.GetFeatureName(i) );
            }
        }

        /// <summary>
        /// Cover
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            int feature = comboBox1.SelectedIndex;
            if (feature >= 0 && feature < nn._state.features.Count)
            {
                /// Move sliding window over image

                const int deltaStepX = 2;
                const int deltaStepY = 2;

                for (int y = 0; y < sourceBitmap.Height - (16 + deltaStepY - 1); y += deltaStepY)
                {
                    for (int x = 0; x < sourceBitmap.Width - (16 + deltaStepX - 1); x += deltaStepX)
                    {
                        Rectangle rect = new Rectangle(x, y, 16, 16);

                        Bitmap sub = sourceBitmap.Clone(rect, sourceBitmap.PixelFormat);

                        int guessed = nn.Guess(sub);

                        /// If feature is detected -- add vias on its place

                        if ( guessed == feature)
                        {
                            PointF p = entityBox1.ImageToLambda(x + 8, y + 8);
                            Point screenPoint = entityBox1.LambdaToScreen(p.X, p.Y);

                            entityBox1.AddVias(EntityType.ViasConnect, screenPoint.X, screenPoint.Y, 
                                entityBox1.ViasConnectColor );
                        }
                    }

                    Console.WriteLine("y: " + y.ToString());
                }

            }

        }

    }
}
