using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MachineLearningKit
{
    public partial class FormCreateNetwork : Form
    {
        public int hiddenLayer = 1000;
        public float learningRate = 0.3F;

        public FormCreateNetwork()
        {
            InitializeComponent();

            textBox1.Text = hiddenLayer.ToString();
            textBox2.Text = learningRate.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;

            /// Limit between 10 ... 100000

            hiddenLayer = Math.Min( 100000, Math.Max (100, Convert.ToInt32(textBox1.Text)));

            /// Learning rate between 0.1 ... 10.0

            learningRate = Math.Min(10.0F, Math.Max(0.1F, float.Parse(textBox2.Text)));

            Close();
        }
    }
}
