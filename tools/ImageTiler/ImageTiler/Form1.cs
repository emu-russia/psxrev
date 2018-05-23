using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace ImageTiler
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult res = openFileDialog1.ShowDialog();

            if ( res == DialogResult.OK)
            {
                string filename = openFileDialog1.FileName;

                pictureBox1.Image = Image.FromFile(filename);
            }
        }

        private void SplitImageOnTiles ( Image image, int gap, int tileSize )
        {
            Bitmap bitmap = new Bitmap(image);
            PixelFormat pixelFormat = image.PixelFormat;

            for (int y = 0; y <image.Height; y += tileSize)
            {
                for (int x = 0; x < image.Width; x += tileSize)
                {
                    int w = Math.Min(tileSize + 2 * gap, image.Width - x);
                    int h = Math.Min(tileSize + 2 * gap, image.Height - y);

                    int gapX = Math.Max(0, x - gap);
                    int gapY = Math.Max(0, y - gap);

                    if (gapX == 0)
                        w -= gap;
                    if (gapY == 0)
                        h -= gap;

                    Rectangle rect = new Rectangle(gapX, gapY, w, h);
                    Bitmap tile = bitmap.Clone(rect, pixelFormat);
                    Image tileImage = (Image)tile;
                    string tileFileName =
                        "col_" + (y / tileSize).ToString() + "_" +
                        "row_" + (x / tileSize).ToString() + ".jpg";
                    tileImage.Save(tileFileName, ImageFormat.Jpeg);

                    tile = null;
                    GC.Collect();
                }
            }

            MessageBox.Show("Operation Completed", "Notification");
        }

        private void splitImageOnTilesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SplitImageOnTiles(pictureBox1.Image, 100, 1000);
        }
    }
}
