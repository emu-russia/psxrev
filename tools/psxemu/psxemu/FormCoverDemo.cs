using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace psxemu
{
    public partial class FormCoverDemo : Form
    {
        public FormCoverDemo()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            FetchCDCover cover = new FetchCDCover();

            pictureBox1.Image = cover.GetCover(textBox1.Text);
        }
    }
}
