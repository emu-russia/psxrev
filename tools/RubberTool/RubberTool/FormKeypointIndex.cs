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
    public partial class FormKeypointIndex : Form
    {
        public bool OkPressed;
        public int indexLeft;
        public int indexRight;

        public FormKeypointIndex(int leftIndex, int rightIndex)
        {
            InitializeComponent();

            OkPressed = false;

            textBox1.Text = leftIndex.ToString();
            textBox2.Text = rightIndex.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if ( textBox1.Text != "")
            {
                indexLeft = (int)Strtoul(textBox1.Text);
            }
            else
            {
                indexLeft = 0;
            }

            if (textBox2.Text != "")
            {
                indexRight = (int)Strtoul(textBox2.Text);
            }
            else
            {
                indexRight = 0;
            }

            OkPressed = true;

            Close();
        }

        private UInt32 Strtoul (string text)
        {
            return text.StartsWith("0x") ? Convert.ToUInt32(text.Substring(2), 16)
                                         : Convert.ToUInt32(text, 10);
        }

    }
}
