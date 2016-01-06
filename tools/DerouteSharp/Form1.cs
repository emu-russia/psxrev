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
    }
}
