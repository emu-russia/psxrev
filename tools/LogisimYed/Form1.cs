using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using System.IO;
using System.Xml;
using CanvasControl;

namespace LogisimYed
{
    public partial class Form1 : Form
    {
        [DllImport("kernel32.dll",
                    EntryPoint = "AllocConsole",
                    SetLastError = true,
                    CharSet = CharSet.Auto,
                    CallingConvention = CallingConvention.StdCall)]
        private static extern int AllocConsole();

        private XmlDocument LogisimCirc = null;
        private XmlDocument YedGraphml = null;

        CanvasInputAdapter input;

        private string SavedText;

        public Form1()
        {
            InitializeComponent();

#if DEBUG
            AllocConsole();
#endif

            input = new CanvasInputAdapter(canvasControl1);

            SavedText = Text;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void loadLogisimToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string text = File.ReadAllText(openFileDialog1.FileName, Encoding.UTF8);
                LogisimCirc = new XmlDocument();
                LogisimCirc.LoadXml(text);

                LogisimConverter.LogisimModel model = LogisimConverter.LoadModel(LogisimCirc);
                VisualizeLogisim(model);

                //Console.WriteLine("");
                //Console.WriteLine(text);

                this.Text = SavedText + " - " + Path.GetFileName(openFileDialog1.FileName);
            }
        }

        private void saveYEdToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (YedGraphml == null)
            {
                MessageBox.Show("Convert first.");
                return;
            }

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string text = YedGraphml.OuterXml;
                File.WriteAllText(saveFileDialog1.FileName, text, Encoding.UTF8);
            }
        }

        private void convertToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (LogisimCirc == null)
            {
                MessageBox.Show("Load Logisim Circuit first.");
                return;
            }

            LogisimConverter.LogisimModel model = LogisimConverter.LoadModel(LogisimCirc);
            YedGraphml = LogisimConverter.ToYed(model);

            Console.WriteLine("");
            Console.WriteLine(YedGraphml.OuterXml);
        }

        private void VisualizeLogisim(LogisimConverter.LogisimModel model)
        {
            canvasControl1.RemoveAllItems();

            // Comps

            foreach (var comp in model.comps)
            {
                CanvasRect rect = new CanvasRect((PointF)comp.loc, 16, 16, Color.Gold);

                rect.Text = comp.name;

                if (comp.name == "Text")
                {
                    // Skip labels
                    continue;
                }
                else if (comp.name == "1")
                {
                    rect.FrontColor = Color.Tomato;
                }
                else if (comp.name == "0")
                {
                    rect.FrontColor = Color.LimeGreen;
                }

                canvasControl1.AddItem(rect);
            }

            // Wires

            foreach (var wire in model.wires)
            {
                List<PointF> path = new List<PointF>();

                foreach (var p in wire.path)
                {
                    path.Add(p);
                }

                CanvasPolyLine line = new CanvasPolyLine(path, 1, Color.Black);
                line.Text = wire.name;
                canvasControl1.AddItem(line);
            }

            // Vias

            foreach (var vias in model.viases)
            {
                CanvasPoint point = new CanvasPoint(vias.loc, 5, Color.Gold);
                canvasControl1.AddItem(point);
            }

            canvasControl1.Invalidate();
        }



    }
}
