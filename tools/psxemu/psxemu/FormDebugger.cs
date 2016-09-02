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
    public partial class FormDebugger : Form
    {
        public FormDebugger()
        {
            InitializeComponent();
        }

        private void closeDebuggerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
