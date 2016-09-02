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

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout aboutDlg = new FormAbout();
            aboutDlg.Show();
        }

        private void openDebuggerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormDebugger debugDlg = new FormDebugger();
            debugDlg.Show();
        }
    }
}
