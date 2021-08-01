using NeuralNetwork;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static NeuralNetwork.EntityNetwork;

namespace DerouteSharp
{
    public partial class FormTrainMLModel : Form
    {
        NeuralNetwork.EntityNetwork nn;
        private Image sourceImage = null;
        private Image neighbourImage = null;
        private Bitmap sourceBitmap;
        private Random rnd = new Random(DateTime.Now.Millisecond);

        public FormTrainMLModel(NeuralNetwork.EntityNetwork nn, Image image)
        {
            InitializeComponent();

            this.nn = nn;

            PopulateListView(nn._state.features);

            sourceImage = image;
            sourceBitmap = new Bitmap(sourceImage);

            ExtractNextFeature();
        }

        private void PopulateListView(List<Feature> features)
        {
            listView1.Items.Clear();

            foreach (var feature in features)
            {
                listView1.Items.Add(feature.name);
            }
        }

        private void ExtractNextFeature()
        {
            // Extract neighbour field 

            Rectangle rect = new Rectangle();

            rect.Width = pictureBox1.Width;
            rect.Height = pictureBox1.Height;

            rect.X = rnd.Next(0, sourceImage.Width - rect.Width - 1);
            rect.Y = rnd.Next(0, sourceImage.Height - rect.Height - 1);

            neighbourImage = sourceBitmap.Clone(rect, sourceBitmap.PixelFormat);

            // Extract local feature

            rect.Width = pictureBox2.Width;
            rect.Height = pictureBox2.Height;

            rect.X = rnd.Next(0, pictureBox1.Width - rect.Width - 1);
            rect.Y = rnd.Next(0, pictureBox1.Height - rect.Height - 1);

            ExtracLocalFeature(rect);
        }

        private void ExtracLocalFeature(Rectangle rect)
        {
            Bitmap sub = new Bitmap(neighbourImage);
            pictureBox2.Image = sub.Clone(rect, sub.PixelFormat);

            // Draw feature box

            pictureBox1.Image = (Image)neighbourImage.Clone();

            using (Graphics gr = Graphics.FromImage(pictureBox1.Image))
            {
                gr.DrawRectangle(new Pen(Color.Lime), rect);
                pictureBox1.Invalidate();
            }

            // Scale local feature

            pictureBox3.Image = ResizeImage(pictureBox2.Image, pictureBox3.Width, pictureBox3.Height);
        }

        private void Guess()
        {
            int id = nn.Guess(pictureBox2.Image, true);

            EntityNetwork.Feature feature = nn.GetFeature(id);

            if (feature != null)
            {
                label2.Text = feature.description + " (" + feature.name + ")";
                pictureBox4.Image = feature.image;
            }
            else
            {
                label2.Text = "I can't recognize";
                pictureBox4.Image = null;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Guess();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ExtractNextFeature();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count > 0)
            {
                int id = listView1.SelectedIndices[0];

                nn.Train(pictureBox2.Image, id);
            }
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            if (neighbourImage == null)
                return;

            MouseEventArgs me = (MouseEventArgs)e;
            Point coords = me.Location;

            Rectangle rect = new Rectangle();

            rect.Width = pictureBox2.Width;
            rect.Height = pictureBox2.Height;

            rect.X = Math.Min(pictureBox1.Width - rect.Width, Math.Max(0, coords.X - rect.Width / 2));
            rect.Y = Math.Min(pictureBox1.Height - rect.Height, Math.Max(0, coords.Y - rect.Height / 2));

            ExtracLocalFeature(rect);
        }

        // Source: https://stackoverflow.com/questions/1922040/resize-an-image-c-sharp

        /// <summary>
        /// Resize the image to the specified width and height.
        /// </summary>
        /// <param name="image">The image to resize.</param>
        /// <param name="width">The width to resize to.</param>
        /// <param name="height">The height to resize to.</param>
        /// <returns>The resized image.</returns>
        public static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.HighQuality;
                graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                graphics.SmoothingMode = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }
            }

            return destImage;
        }

    }
}
