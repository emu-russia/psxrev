using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using NeuralNetwork;

namespace MachineLearningKit
{
    public partial class FormRenameNetwork : Form
    {
        private EntityNetwork.State state;

        public FormRenameNetwork(EntityNetwork.State state)
        {
            InitializeComponent();

            this.state = state;

            textBox1.Text = state.name;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            state.name = textBox1.Text;

            DialogResult = DialogResult.OK;
            Close();
        }
    }
}
