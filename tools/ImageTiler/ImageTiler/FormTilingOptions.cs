using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ImageTiler
{
    public partial class FormTilingOptions : Form
    {
        public int tileSize;
        public int gapSize;
        public bool OkPressed = false;

        public FormTilingOptions(int tileSize, int gapSize)
        {
            InitializeComponent();

            this.tileSize = tileSize;
            this.gapSize = gapSize;

            textBox1.Text = tileSize.ToString();
            textBox2.Text = gapSize.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                tileSize = (int)Strtoul(textBox1.Text);
            }
            else
            {
                tileSize = 0;
            }

            if (textBox2.Text != "")
            {
                gapSize = (int)Strtoul(textBox2.Text);
            }
            else
            {
                gapSize = 0;
            }

            OkPressed = true;

            Close();
        }

        private UInt32 Strtoul(string text)
        {
            return text.StartsWith("0x") ? Convert.ToUInt32(text.Substring(2), 16)
                                         : Convert.ToUInt32(text, 10);
        }
    }
}
