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
    public partial class FormEnterValue : Form
    {
        private UInt32 _value = 0;

        public UInt32 Value
        {
            get { return _value; }
            set {
                _value = value;
                textBox1.Text = value.ToString();
            }
        }

        public UInt32 HexValue
        {
            get { return _value; }
            set
            {
                _value = value;
                textBox1.Text = "0x" + value.ToString("X");
            }
        }

        public FormEnterValue(string tip)
        {
            InitializeComponent();

            label1.Text = tip;
        }

        /// <summary>
        /// Strtoul with dec/hex auto-detection
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        private static UInt32 Strtoul(string str)
        {
            str = str.Trim();

            return (str.Contains("0x") || str.Contains("0X")) ?
                Convert.ToUInt32(str, 16) :
                    str[0] == '0' ? Convert.ToUInt32(str, 8) : Convert.ToUInt32(str, 10);
        }


        private void button1_Click(object sender, EventArgs e)
        {
            _value = Strtoul(textBox1.Text);
            DialogResult = DialogResult.OK;
            Close();
        }

    }
}
