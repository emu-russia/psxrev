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
        private int tileSize = 1000;
        private int gapSize = 100;

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

        private void SplitImageOnTiles ( Image image )
        {
            Bitmap bitmap = new Bitmap(image);
            PixelFormat pixelFormat = image.PixelFormat;

            for (int y = 0; y <image.Height; y += tileSize)
            {
                for (int x = 0; x < image.Width; x += tileSize)
                {
                    int w = Math.Min(tileSize + 2 * gapSize, image.Width - x);
                    int h = Math.Min(tileSize + 2 * gapSize, image.Height - y);

                    int gapX = Math.Max(0, x - gapSize);
                    int gapY = Math.Max(0, y - gapSize);

                    if (gapX == 0)
                        w -= gapSize;
                    if (gapY == 0)
                        h -= gapSize;

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

            MessageBox.Show("Operation Completed", "Notification",
                MessageBoxButtons.OK, MessageBoxIcon.Information );
        }

        private void splitImageOnTilesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( pictureBox1.Image == null )
            {
                MessageBox.Show("Load Image", "Notification", 
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            SplitImageOnTiles(pictureBox1.Image);
        }

        private void tilingOptionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormTilingOptions form = new FormTilingOptions(tileSize, gapSize);

            form.FormClosed += Form_FormClosed;
            form.ShowDialog();
        }

        private void Form_FormClosed(object sender, FormClosedEventArgs e)
        {
            FormTilingOptions form = (FormTilingOptions)sender;

            if ( form.OkPressed)
            {
                tileSize = form.tileSize;
                gapSize = form.gapSize;
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout form = new FormAbout();
            form.ShowDialog();
        }
    }
}
