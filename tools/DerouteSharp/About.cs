using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DerouteSharp
{
    public partial class About : Form
    {
        public About()
        {
            InitializeComponent();

            label3.Text = Environment.Version.ToString();

            label5.Text = GC.GetTotalMemory(true).ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void About_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
                Close();
        }
    }
}
