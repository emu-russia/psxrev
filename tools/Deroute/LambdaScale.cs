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
    public partial class LambdaScale : Form
    {
        public float ScaleValue = float.NaN;

        public LambdaScale()
        {
            InitializeComponent();
        }

        private void LambdaScale_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
                Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string value = textBox1.Text;
            value = value.Replace('.', ',');
            ScaleValue = Convert.ToSingle(value);
            Close ();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            textBox1.Text = "0.25";
        }

        private void button4_Click(object sender, EventArgs e)
        {
            textBox1.Text = "0.5";
        }

        private void button5_Click(object sender, EventArgs e)
        {
            textBox1.Text = "0.75";
        }

        private void button6_Click(object sender, EventArgs e)
        {
            textBox1.Text = "1.25";
        }

        private void button10_Click(object sender, EventArgs e)
        {
            textBox1.Text = "1.5";
        }

        private void button9_Click(object sender, EventArgs e)
        {
            textBox1.Text = "1.75";
        }

        private void button8_Click(object sender, EventArgs e)
        {
            textBox1.Text = "2";
        }

        private void button7_Click(object sender, EventArgs e)
        {
            textBox1.Text = "4";
        }
    }
}
