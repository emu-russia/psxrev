using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Xml.Serialization;
using System.IO;

using NeuralNetwork;

namespace MachineLearningKit
{
    public partial class Form1 : Form
    {
        [DllImport("kernel32")]
        static extern bool AllocConsole();

        private EntityNetwork nn = null;
        private Image sourceImage = null;
        private Image neighbourImage = null;
        private Bitmap sourceBitmap;
        private Random rnd = new Random(DateTime.Now.Millisecond);
        private List<EntityNetwork.Feature> features = new List<EntityNetwork.Feature>();

        public Form1()
        {
            InitializeComponent();

            AllocConsole();

            LoadFeatureCollection();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            SaveFeatureCollection();
        }

        private void LoadFeatureCollection ()
        {
            try
            {
                XmlSerializer ser = new XmlSerializer(typeof(List<EntityNetwork.Feature>));

                using (FileStream fs = new FileStream("features.xml", FileMode.Open))
                {
                    features = (List<EntityNetwork.Feature>)ser.Deserialize(fs);
                }
            }
            catch
            {
                Console.WriteLine("Can't load features collection!");
            }

            PopulateListView();
        }

        private void PopulateListView()
        {
            listView1.Items.Clear();

            foreach (var feature in features)
            {
                listView1.Items.Add(feature.name);
            }
        }

        private void SaveFeatureCollection ()
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<EntityNetwork.Feature>));

            using (FileStream fs = new FileStream("features.xml", FileMode.Create))
            {
                ser.Serialize(fs, features);
            }
        }

        /// <summary>
        /// Check everything is ok
        /// </summary>
        /// <returns></returns>
        private bool CheckEnvironment(bool featureCheck=true)
        {
            if (nn == null)
            {
                MessageBox.Show("Load or create network first", "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            if (sourceImage == null)
            {
                MessageBox.Show("Load image to guess or train", "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            if (pictureBox2.Image == null && featureCheck)
            {
                MessageBox.Show("Extract feature first", "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout about = new FormAbout();
            about.ShowDialog();
        }

        private void loadSourceImageToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if ( openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                sourceImage = Image.FromFile(openFileDialog1.FileName);
                sourceBitmap = new Bitmap(sourceImage);
            }
        }

        private void createNetworkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormCreateNetwork form = new FormCreateNetwork();

            form.FormClosed += Form_FormClosed;

            form.ShowDialog();
        }

        private void Form_FormClosed(object sender, FormClosedEventArgs e)
        {
            FormCreateNetwork form = (FormCreateNetwork)sender;

            if (form.DialogResult == DialogResult.OK)
            {
                nn = new EntityNetwork(features, form.hiddenLayer, form.learningRate);
                toolStripStatusLabel2.Text = nn._state.name;
                toolStripStatusLabel4.Text = nn._state._weightInputHidden.GetLength(0).ToString();
                toolStripStatusLabel6.Text = nn._state._learningRate.ToString("0.00");
            }
        }

        private void loadNetworkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult res = openFileDialog2.ShowDialog();

            if (res == DialogResult.OK)
            {
                string filename = openFileDialog2.FileName;

                XmlSerializer ser = new XmlSerializer(typeof(EntityNetwork.State));

                using (FileStream fs = new FileStream(filename, FileMode.Open))
                {
                    EntityNetwork.State state = (EntityNetwork.State)ser.Deserialize(fs);

                    nn = new EntityNetwork(state);

                    features = nn._state.features;

                    toolStripStatusLabel2.Text = state.name;
                    toolStripStatusLabel4.Text = state._weightInputHidden.GetLength(0).ToString();
                    toolStripStatusLabel6.Text = state._learningRate.ToString("0.00");
                }
            }
        }

        private void saveNetworkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (nn == null)
            {
                MessageBox.Show("Load or create network first", "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            DialogResult res = saveFileDialog1.ShowDialog();

            if (res == DialogResult.OK)
            {
                string filename = saveFileDialog1.FileName;

                XmlSerializer ser = new XmlSerializer(typeof(EntityNetwork.State));

                using (FileStream fs = new FileStream(filename, FileMode.Create))
                {
                    ser.Serialize(fs, nn._state);
                }
            }
        }

        /// <summary>
        /// Guess
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            Guess();
        }

        private void Guess()
        {
            if (!CheckEnvironment())
            {
                return;
            }

            int id = nn.Guess(pictureBox2.Image, true);

            EntityNetwork.Feature feature = nn.GetFeature(id);

            if (feature != null)
            {
                label2.Text = feature.description + " (" + feature.name + ")";
                pictureBox4.Image = feature.image;
            }
            else
            {
                label2.Text = "Не могу распознать";
                pictureBox4.Image = null;
            }
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

        /// <summary>
        /// Extract feature
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            if (sourceImage == null)
            {
                MessageBox.Show("Load image to guess or train", "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

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

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            if (neighbourImage == null)
                return;

            MouseEventArgs me = (MouseEventArgs)e;
            Point coords = me.Location;

            //Console.WriteLine(coords.X.ToString() + ", " + coords.Y.ToString());

            Rectangle rect = new Rectangle();

            rect.Width = pictureBox2.Width;
            rect.Height = pictureBox2.Height;

            rect.X = Math.Min( pictureBox1.Width - rect.Width, Math.Max(0, coords.X - rect.Width / 2));
            rect.Y = Math.Min(pictureBox1.Height - rect.Height, Math.Max(0, coords.Y - rect.Height / 2));

            ExtracLocalFeature(rect);

            if (checkBox1.Checked)
                Guess();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (!CheckEnvironment())
                return;

            if (listView1.SelectedIndices.Count > 0)
            {
                int id = listView1.SelectedIndices[0];

                nn.Train(pictureBox2.Image, id);
            }
        }

        private void featureCoverageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!CheckEnvironment(false))
                return;

            FormCoverage coverage = new FormCoverage(sourceImage, nn);

            coverage.ShowDialog();
        }

        private void featureCollectionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormCollection collection = new FormCollection(features);

            collection.FormClosed += Collection_FormClosed;

            collection.ShowDialog();
        }

        private void Collection_FormClosed(object sender, FormClosedEventArgs e)
        {
            FormCollection form = (FormCollection)sender;

            if (form.DialogResult == DialogResult.OK && form.features != null)
            {
                features = form.features;

                if (form.features.Count != features.Count)
                {
                    MessageBox.Show("Feature collection has been heavily modified, network will be deleted!", "Attention",
                    MessageBoxButtons.OK, MessageBoxIcon.Asterisk);

                    nn = null;
                    toolStripStatusLabel2.Text = "---";
                    toolStripStatusLabel4.Text = "0";
                    toolStripStatusLabel6.Text = "0";
                }
                else
                {
                    if (nn != null)
                    {
                        nn._state.features = features;
                    }
                }

                PopulateListView();
            }
        }

        /// <summary>
        /// Rename network
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripStatusLabel2_Click(object sender, EventArgs e)
        {
            if (nn == null)
            {
                MessageBox.Show("Load or create network first", "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            FormRenameNetwork form = new FormRenameNetwork(nn._state);

            form.FormClosed += Form_FormClosed1;

            form.ShowDialog();
        }

        private void Form_FormClosed1(object sender, FormClosedEventArgs e)
        {
            FormRenameNetwork form = (FormRenameNetwork)sender;

            if ( form.DialogResult == DialogResult.OK)
            {
                toolStripStatusLabel2.Text = nn._state.name;
            }
        }

        /// <summary>
        /// Change learning rate
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripStatusLabel6_Click(object sender, EventArgs e)
        {

        }

    }
}
