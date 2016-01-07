using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DerouteSharp
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

        private void button1_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityType.Selection;
            propertyGrid1.Refresh();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityType.ViasConnect;
            propertyGrid1.Refresh();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityType.WireInterconnect;
            propertyGrid1.Refresh();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            About aboutDialog = new About();
            aboutDialog.ShowDialog();
        }

        private void deleteAllEntitiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            entityBox1.DeleteAllEntites();
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if ( result == DialogResult.OK )
            {
                Image image = Image.FromFile(openFileDialog1.FileName);
                entityBox1.Image = image;
            }
        }

        private void saveSceneAsImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox1.SaveSceneAsImage(saveFileDialog1.FileName);
            }
        }

        private void loadEntitiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog2.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox1.Unserialize(openFileDialog2.FileName, true);
            }
        }

        private void saveEntitiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog2.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox1.Serialize(saveFileDialog2.FileName);
            }
        }

    }
}
