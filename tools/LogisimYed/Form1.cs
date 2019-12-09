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

            int nfetCounter = 1;
            int pfetCounter = 1;

            // Comps

            foreach (var comp in model.comps)
            {
                CanvasRect rect = new CanvasRect((PointF)comp.loc, 16, 16, Color.Gold);

                string facingDefault = "east";

                if (comp.name == "Text")
                {
                    // Skip labels
                    continue;
                }
                else if (comp.name == "Transistor")
                {
                    if (comp.props.ContainsKey("type"))
                    {
                        if (comp.props["type"] == "n")
                        {
                            rect.Text = "nfet_" + nfetCounter.ToString();
                            nfetCounter++;
                        }
                        else
                        {
                            rect.Text = "pfet_" + pfetCounter.ToString();
                            pfetCounter++;
                        }
                    }
                    else
                    {
                        rect.Text = "pfet_" + pfetCounter.ToString();
                        pfetCounter++;
                    }
                    if (!comp.props.ContainsKey("facing"))
                    {
                        comp.props["facing"] = facingDefault;
                    }
                    switch (comp.props["facing"])
                    {
                        case "north":
                            rect.Pos = new PointF(
                                rect.Pos.X,
                                rect.Pos.Y + rect.Height - 4);
                            break;
                        case "south":
                            rect.Pos = new PointF(
                                rect.Pos.X,
                                rect.Pos.Y - rect.Height + 4);
                            break;
                        case "west":
                            rect.Pos = new PointF(
                                rect.Pos.X + rect.Width - 4,
                                rect.Pos.Y);
                            break;
                        case "east":
                            rect.Pos = new PointF(
                                rect.Pos.X - rect.Width + 4,
                                rect.Pos.Y);
                            break;
                    }
                }
                else if (comp.name == "Power")
                {
                    rect.FrontColor = Color.Tomato;
                    rect.Text = "1";
                    if (!comp.props.ContainsKey("facing"))
                    {
                        comp.props["facing"] = facingDefault;
                    }
                    if (comp.props["facing"] == "west")
                    {
                        comp.props["facing"] = "east";
                    }
                    else if (comp.props["facing"] == "east")
                    {
                        comp.props["facing"] = "west";
                    }
                }
                else if (comp.name == "Ground")
                {
                    rect.FrontColor = Color.LimeGreen;
                    rect.Text = "0";
                    if (!comp.props.ContainsKey("facing"))
                    {
                        comp.props["facing"] = facingDefault;
                    }
                    if (comp.props["facing"] == "west")
                    {
                        comp.props["facing"] = "east";
                    }
                    else if (comp.props["facing"] == "east")
                    {
                        comp.props["facing"] = "west";
                    }
                }
                else if (comp.name == "Pin")
                {
                    if (comp.props.ContainsKey("label"))
                    {
                        rect.Text = comp.props["label"];
                    }
                    else
                    {
                        rect.Text = comp.name;
                    }
                    if (!comp.props.ContainsKey("facing"))
                    {
                        comp.props["facing"] = facingDefault;
                    }
                }
                else
                {
                    rect.Text = comp.name;
                }

                if (comp.props.ContainsKey("facing"))
                {
                    switch (comp.props["facing"])
                    {
                        case "north":
                            rect.Pos = new PointF(
                                rect.Pos.X - rect.Width / 2,
                                rect.Pos.Y);
                            break;
                        case "south":
                            rect.Pos = new PointF(
                                rect.Pos.X - rect.Width / 2,
                                rect.Pos.Y - rect.Height);
                            break;
                        case "west":
                            rect.Pos = new PointF(
                                rect.Pos.X,
                                rect.Pos.Y - rect.Height / 2);
                            break;
                        case "east":
                            rect.Pos = new PointF(
                                rect.Pos.X - rect.Width,
                                rect.Pos.Y - rect.Height / 2);
                            break;
                    }
                }

                canvasControl1.AddItem(rect);

                if (rect.Text.Contains("nfet") || rect.Text.Contains("pfet"))
                {
                    AddTrans(rect, comp);
                }
            }

            // Wires

            foreach (var wire in model.wires)
            {
                CanvasLine line = new CanvasLine((PointF)wire.from, (PointF)wire.to, Color.Black);
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

        private void AddTrans (CanvasRect bbox, LogisimConverter.LogisimComp comp)
        {
            CanvasLine source = null;
            CanvasLine drain = null;
            CanvasLine gate = null;
            CanvasPoint sourcePad = null;
            CanvasPoint drainPad = null;
            CanvasPoint gatePad = null;

            int delta = 12;

            bool br = false;

            if (comp.props.ContainsKey("gate"))
            {
                br = comp.props["gate"] == "br" ? true : false;
            }

            switch (comp.props["facing"])
            {
                case "north":
                    source = new CanvasLine(
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height),
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height + delta), Color.Black);
                    drain = new CanvasLine(
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y),
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y - delta), Color.Black);
                    if (br)
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X + bbox.Width, bbox.Pos.Y + bbox.Height / 2),
                            new PointF(bbox.Pos.X + bbox.Width + delta, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    }
                    else
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X, bbox.Pos.Y + bbox.Height / 2),
                            new PointF(bbox.Pos.X - delta, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    }
                    sourcePad = new CanvasPoint(source.PosEnd, 5, Color.Gold);
                    drainPad = new CanvasPoint(drain.PosEnd, 5, Color.Gold);
                    gatePad = new CanvasPoint(gate.PosEnd, 5, Color.Gold);
                    break;
                case "south":
                    source = new CanvasLine(
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y - delta),
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y), Color.Black);
                    drain = new CanvasLine(
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height),
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height + delta), Color.Black);
                    if (br)
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X + bbox.Width, bbox.Pos.Y + bbox.Height / 2),
                            new PointF(bbox.Pos.X + bbox.Width + delta, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    }
                    else
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X, bbox.Pos.Y + bbox.Height / 2),
                            new PointF(bbox.Pos.X - delta, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    }
                    sourcePad = new CanvasPoint(source.Pos, 5, Color.Gold);
                    drainPad = new CanvasPoint(drain.PosEnd, 5, Color.Gold);
                    gatePad = new CanvasPoint(gate.PosEnd, 5, Color.Gold);
                    break;
                case "west":
                    source = new CanvasLine(
                        new PointF(bbox.Pos.X + bbox.Width + delta, bbox.Pos.Y + bbox.Height / 2),
                        new PointF(bbox.Pos.X + bbox.Width, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    drain = new CanvasLine(
                        new PointF(bbox.Pos.X, bbox.Pos.Y + bbox.Height / 2),
                        new PointF(bbox.Pos.X - delta, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    if (br)
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height),
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height + delta), Color.Black);
                    }
                    else
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y),
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y - delta), Color.Black);
                    }
                    sourcePad = new CanvasPoint(source.Pos, 5, Color.Gold);
                    drainPad = new CanvasPoint(drain.PosEnd, 5, Color.Gold);
                    gatePad = new CanvasPoint(gate.PosEnd, 5, Color.Gold);
                    break;
                case "east":
                    source = new CanvasLine(
                        new PointF(bbox.Pos.X - delta, bbox.Pos.Y + bbox.Height / 2),
                        new PointF(bbox.Pos.X, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    drain = new CanvasLine(
                        new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height / 2),
                        new PointF(bbox.Pos.X + bbox.Width + delta, bbox.Pos.Y + bbox.Height / 2), Color.Black);
                    if (br)
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height),
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y + bbox.Height + delta), Color.Black);
                    }
                    else
                    {
                        gate = new CanvasLine(
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y),
                            new PointF(bbox.Pos.X + bbox.Width / 2, bbox.Pos.Y - delta), Color.Black);
                    }
                    sourcePad = new CanvasPoint(source.Pos, 5, Color.Gold);
                    drainPad = new CanvasPoint(drain.PosEnd, 5, Color.Gold);
                    gatePad = new CanvasPoint(gate.PosEnd, 5, Color.Gold);
                    break;
            }

            if (source != null)
            {
                canvasControl1.AddItem(source);
                canvasControl1.AddItem(sourcePad);
            }
            if (drain != null)
            {
                canvasControl1.AddItem(drain);
                canvasControl1.AddItem(drainPad);
            }
            if (gate != null)
            {
                gate.Text = "g";
                canvasControl1.AddItem(gate);
                canvasControl1.AddItem(gatePad);
            }
        }

    }
}
