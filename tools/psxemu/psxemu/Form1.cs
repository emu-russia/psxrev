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
        bool DebugActive = false;

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
            if (DebugActive)
            {
                return;
            }

            FormDebugger debugDlg = new FormDebugger();
            debugDlg.FormClosed += debugDlg_FormClosed;
            debugDlg.Show();
            DebugActive = true;
        }

        void debugDlg_FormClosed(object sender, FormClosedEventArgs e)
        {
            DebugActive = false;
        }

        private void fetchCDCoverDemoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormCoverDemo dlg = new FormCoverDemo();

            dlg.ShowDialog();
        }
    }
}
