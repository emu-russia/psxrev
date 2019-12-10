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
using System.Xml.Serialization;

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
        LogisimConverter.LogisimModel CircuitModel = null;
        private XmlDocument YedGraphml = null;

        CanvasInputAdapter input;

        private string SavedText;

        public Form1()
        {
            InitializeComponent();

#if DEBUG
            AllocConsole();
#endif

            input = new CanvasInputAdapter(canvasControl1, this);

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

                CircuitModel = LogisimConverter.LoadModel(LogisimCirc);
                VisualizeLogisim(CircuitModel);

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
            if (CircuitModel == null)
            {
                MessageBox.Show("Load Logisim Circuit first.");
                return;
            }

            YedGraphml = LogisimConverter.ToYed(CircuitModel);

            Console.WriteLine("");
            Console.WriteLine(YedGraphml.OuterXml);
        }

        public class WireMeta
        {
            public LogisimConverter.LogisimWire wire;
            public CanvasItem dir1;
            public CanvasItem dir2;
        }

        private void VisualizeLogisim(LogisimConverter.LogisimModel model)
        {
            canvasControl1.RemoveAllItems();

            // Comps

            foreach (var comp in model.comps)
            {
                CanvasRect rect = new CanvasRect((PointF)comp.loc, 
                    LogisimConverter.CompSize, LogisimConverter.CompSize, Color.Gold);

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

#if DEBUG
                //rect.Text += " #" + comp.id.ToString();
#endif

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

                WireMeta meta = new WireMeta();

                meta.wire = wire;
                line.UserData = meta;

                canvasControl1.AddItem(line);
                WireDirection(wire, meta);
            }

            // Vias

            foreach (var vias in model.viases)
            {
                CanvasPoint point = new CanvasPoint(vias.loc, 5, Color.Gold);
#if DEBUG
                point.Text = vias.id.ToString();
                point.TextColor = Color.DeepPink;
#endif
                canvasControl1.AddItem(point);
            }

            canvasControl1.Invalidate();
        }

        public void WireDirection(LogisimConverter.LogisimWire wire, WireMeta meta)
        {
            if (meta.dir1 != null)
                canvasControl1.RemoveItem(meta.dir1);
            if (meta.dir2 != null)
                canvasControl1.RemoveItem(meta.dir2);

            // Direction

            Point last = wire.To();
            Point prev = wire.path[wire.path.Count - 2];

            int dx = 4;

            if (last.X == prev.X)
            {
                if (last.Y > prev.Y)
                {
                    // Down
                    meta.dir1 = new CanvasLine(
                        new PointF(last.X, last.Y - 1), 
                        new PointF(last.X - dx, last.Y - dx), Color.Black);
                    meta.dir2 = new CanvasLine(
                        new PointF(last.X, last.Y - 1), 
                        new PointF(last.X + dx, last.Y - dx), Color.Black);
                    canvasControl1.AddItem(meta.dir1);
                    canvasControl1.AddItem(meta.dir2);
                }
                else
                {
                    // Up
                    meta.dir1 = new CanvasLine(
                        new PointF(last.X, last.Y + 1), 
                        new PointF(last.X - dx, last.Y + dx), Color.Black);
                    meta.dir2 = new CanvasLine(
                        new PointF(last.X, last.Y + 1), 
                        new PointF(last.X + dx, last.Y + dx), Color.Black);
                    canvasControl1.AddItem(meta.dir1);
                    canvasControl1.AddItem(meta.dir2);
                }
            }
            else
            {
                if (last.X > prev.X)
                {
                    // Right
                    meta.dir1 = new CanvasLine(
                        new PointF(last.X - 1, last.Y), 
                        new PointF(last.X - dx, last.Y - dx), Color.Black);
                    meta.dir2 = new CanvasLine(
                        new PointF(last.X - 1, last.Y), 
                        new PointF(last.X - dx, last.Y + dx), Color.Black);
                    canvasControl1.AddItem(meta.dir1);
                    canvasControl1.AddItem(meta.dir2);
                }
                else
                {
                    // Left
                    meta.dir1 = new CanvasLine(
                        new PointF(last.X + 1, last.Y), 
                        new PointF(last.X + dx, last.Y - dx), Color.Black);
                    meta.dir2 = new CanvasLine(
                        new PointF(last.X + 1, last.Y), 
                        new PointF(last.X + dx, last.Y + dx), Color.Black);
                    canvasControl1.AddItem(meta.dir1);
                    canvasControl1.AddItem(meta.dir2);
                }
            }
        }


        private void reduceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CircuitModel == null)
            {
                MessageBox.Show("Load Logisim Circuit first.");
                return;
            }

            LogisimConverter.Reduce(CircuitModel);
        }

        private void flipWiretSpaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FlipSelectedWires();
        }

        public void FlipSelectedWires()
        {
            List<CanvasItem> selected = canvasControl1.GetSelected();

            foreach (var item in selected)
            {
                if (item.UserData is WireMeta)
                {
                    WireMeta meta = (WireMeta)item.UserData;

                    meta.wire.Flip();

                    // Canvas item

                    List<PointF> pathrev = new List<PointF>();

                    for (int i = item.Points.Count - 1; i >= 0; i--)
                    {
                        pathrev.Add(new PointF(item.Points[i].X, item.Points[i].Y));
                    }

                    item.Points.Clear();
                    item.Points = pathrev;

                    WireDirection(meta.wire, meta);
                }
            }

            canvasControl1.Invalidate();
        }

        private void dumpModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CircuitModel != null)
            {
                LogisimConverter.DumpLogisimModel(CircuitModel);
            }
        }

        private void loadModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( openFileDialog2.ShowDialog() == DialogResult.OK)
            {
                XmlSerializer ser = new XmlSerializer(typeof(LogisimConverter.LogisimModel));

                using (FileStream fs = new FileStream(openFileDialog2.FileName, FileMode.Open))
                {
                    CircuitModel = (LogisimConverter.LogisimModel)ser.Deserialize(fs);

                    // Restore wire linkage

                    LogisimConverter.RestoreLinkage(CircuitModel);

                    VisualizeLogisim(CircuitModel);

                    this.Text = SavedText + " - " + Path.GetFileName(openFileDialog2.FileName);
                }
            }
        }

        private void saveModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog2.ShowDialog() == DialogResult.OK)
            {
                XmlSerializer ser = new XmlSerializer(typeof(LogisimConverter.LogisimModel));

                using (FileStream fs = new FileStream(saveFileDialog2.FileName, FileMode.Create))
                {
                    ser.Serialize(fs, CircuitModel);
                }
            }
        }

    }
}
