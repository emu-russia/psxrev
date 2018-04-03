using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RubberTool
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout about = new FormAbout();
            about.ShowDialog();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void loadLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if ( result == DialogResult.OK)
            {
                entityBox1.Image0 = Image.FromFile(openFileDialog1.FileName);
            }
        }

        private void loadRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox2.Image0 = Image.FromFile(openFileDialog1.FileName);
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityMode.ViasConnect;
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityMode.Selection;
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            entityBox2.Mode = EntityMode.ViasConnect;
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            entityBox2.Mode = EntityMode.Selection;
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            entityBox1.DrawRegionBetweenSelectedViases();
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            entityBox2.DrawRegionBetweenSelectedViases();
        }
    }
}
