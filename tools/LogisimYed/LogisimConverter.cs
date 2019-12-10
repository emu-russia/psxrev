using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml;
using System.Drawing;
using System.Text.RegularExpressions;
using System.Xml.Serialization;

namespace LogisimYed
{
    public class LogisimConverter
    {
        public const int CompSize = 16;            // Size of standard component (e.g. pad, transistor) in pixels

        public class LogisimModel
        {
            public string name = "";

            public List<LogisimComp> comps = new List<LogisimComp>();
            public List<LogisimWire> wires = new List<LogisimWire>();
            public List<LogisimVias> viases = new List<LogisimVias>();

            public bool ViasExists (LogisimVias vias)
            {
                foreach (var v in viases)
                {
                    if (v.loc.Equals(vias.loc))
                        return true;
                }
                return false;
            }
        }

        /// <summary>
        /// Parse Logisim Xml to model.
        /// </summary>
        /// <param name="source">Logisim Circuit</param>
        /// <returns>Model</returns>
        public static LogisimModel LoadModel(XmlDocument source)
        {
            // Get circuit

            XmlNode circuit = null;

            foreach (XmlNode node in source.DocumentElement.ChildNodes)
            {
                if (node.Name == "circuit")
                {
                    circuit = node;
                    break;
                }
            }

            if (circuit == null)
            {
                throw new Exception("Invalid format. Missing circuit.");
            }

            // Processing

            LogisimModel model = new LogisimModel();

            if ( circuit.Attributes["name"] != null)
            {
                model.name = circuit.Attributes["name"].Value;
            }

            // Get components

            foreach ( XmlNode node in circuit)
            {
                if (node.Name == "comp")
                {
                    LogisimComp comp = new LogisimComp();

                    if (node.Attributes["lib"] != null)
                    {
                        comp.lib = int.Parse(node.Attributes["lib"].Value);
                    }
                    comp.loc = ParseLoc(node.Attributes["loc"].Value);
                    comp.name = node.Attributes["name"].Value;
                    comp.id = GetNextId(model);

                    foreach (XmlNode a in node)
                    {
                        if (a.Name == "a")
                        {
                            string k = a.Attributes["name"].Value;
                            string v = a.Attributes["val"].Value;
                            comp.props[k] = v;
                        }
                    }

                    model.comps.Add(comp);
                }
            }

            // Get wires

            foreach (XmlNode node in circuit)
            {
                if (node.Name == "wire")
                {
                    Point start = ParseLoc(node.Attributes["from"].Value);
                    Point end = ParseLoc(node.Attributes["to"].Value);

                    LogisimWire wire = new LogisimWire(start, end);

                    model.wires.Add(wire);
                }
            }

            // Connect wires with components

            PreProcessing(model);

            // Get wires interconnections (produce viases)

            foreach (var wire in model.wires)
            {
                foreach (var another in model.wires)
                {
                    if (wire != another)
                    {
                        if (wire.From().Equals(another.From()) || 
                            wire.From().Equals(another.To()) )
                        {
                            LogisimVias vias = new LogisimVias(wire.From());

                            if (!model.ViasExists(vias))
                            {
                                vias.id = GetNextId(model);
                                model.viases.Add(vias);
                            }
                        }

                        if ( wire.To().Equals(another.To()) ||
                            wire.To().Equals(another.From()) )
                        {
                            LogisimVias vias = new LogisimVias(wire.To());

                            if (!model.ViasExists(vias))
                            {
                                vias.id = GetNextId(model);
                                model.viases.Add(vias);
                            }
                        }
                    }
                }
            }

            PostProcessing(model);

            Reduce(model);

            DumpLogisimModel(model);

            return model;
        }

        private static Point ParseLoc (string text)
        {
            Regex regex = new Regex(@"\(\s*(?<from>\d+)\s*,\s*(?<to>\d+)\s*\)");

            var match = regex.Matches(text);

            if (match.Count != 1)
            {
                throw new Exception("Invalid location format!");
            }

            Point point = new Point();

            point.X = int.Parse(match[0].Groups["from"].Value);
            point.Y = int.Parse(match[0].Groups["to"].Value);

            return point;
        }

        /// <summary>
        /// Connect wires with components
        /// </summary>
        /// <param name="model"></param>
        private static void PreProcessing(LogisimModel model)
        {
            foreach(var wire in model.wires)
            {
                foreach (var comp in model.comps)
                {
                    if (comp.loc.Equals(wire.From()) && wire.source.id < 0)
                    {
                        wire.source = comp;
                    }

                    if (comp.loc.Equals(wire.To()) && wire.dest.id < 0)
                    {
                        wire.dest = comp;
                    }
                }
            }
        }

        /// <summary>
        /// Modify model to look nice
        /// </summary>
        /// <param name="model"></param>
        private static void PostProcessing(LogisimModel model)
        {
            int nfetCounter = 1;
            int pfetCounter = 1;

            // Comps

            foreach (var comp in model.comps)
            {
                string facingDefault = "east";

                if (comp.name == "Transistor")
                {
                    if (comp.props.ContainsKey("type"))
                    {
                        if (comp.props["type"] == "n")
                        {
                            comp.name = "nfet_" + nfetCounter.ToString();
                            nfetCounter++;
                        }
                        else
                        {
                            comp.name = "pfet_" + pfetCounter.ToString();
                            pfetCounter++;
                        }
                    }
                    else
                    {
                        comp.name = "pfet_" + pfetCounter.ToString();
                        pfetCounter++;
                    }
                    if (!comp.props.ContainsKey("facing"))
                    {
                        comp.props["facing"] = facingDefault;
                    }
                    switch (comp.props["facing"])
                    {
                        case "north":
                            comp.loc = new Point(
                                comp.loc.X,
                                comp.loc.Y + CompSize - 4);
                            break;
                        case "south":
                            comp.loc = new Point(
                                comp.loc.X,
                                comp.loc.Y - CompSize + 4);
                            break;
                        case "west":
                            comp.loc = new Point(
                                comp.loc.X + CompSize - 4,
                                comp.loc.Y);
                            break;
                        case "east":
                            comp.loc = new Point(
                                comp.loc.X - CompSize + 4,
                                comp.loc.Y);
                            break;
                    }
                }
                else if (comp.name == "Power")
                {
                    comp.name = "1";
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
                    comp.name = "0";
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
                        comp.name = comp.props["label"];
                    }
                    if (!comp.props.ContainsKey("facing"))
                    {
                        comp.props["facing"] = facingDefault;
                    }
                }

                if (comp.props.ContainsKey("facing"))
                {
                    switch (comp.props["facing"])
                    {
                        case "north":
                            comp.loc = new Point(
                                comp.loc.X - CompSize / 2,
                                comp.loc.Y);
                            break;
                        case "south":
                            comp.loc = new Point(
                                comp.loc.X - CompSize / 2,
                                comp.loc.Y - CompSize);
                            break;
                        case "west":
                            comp.loc = new Point(
                                comp.loc.X,
                                comp.loc.Y - CompSize / 2);
                            break;
                        case "east":
                            comp.loc = new Point(
                                comp.loc.X - CompSize,
                                comp.loc.Y - CompSize / 2);
                            break;
                    }
                }

                if (comp.name.Contains("nfet") || comp.name.Contains("pfet"))
                {
                    AddTrans( new Rectangle(comp.loc.X, comp.loc.Y, CompSize, CompSize), comp, model);
                }
            }

            // Link wires with viases

            foreach (var wire in model.wires)
            {
                foreach (var vias in model.viases)
                {
                    if (vias.loc.Equals(wire.From()) && wire.source.id < 0)
                    {
                        wire.source = vias;
                    }

                    if (vias.loc.Equals(wire.To()) && wire.dest.id < 0)
                    {
                        wire.dest = vias;
                    }
                }
            }

        }

        private static void AddTrans(Rectangle bbox, LogisimComp comp, LogisimModel model)
        {
            LogisimWire source = null;
            LogisimWire drain = null;
            LogisimWire gate = null;
            LogisimVias sourcePad = null;
            LogisimVias drainPad = null;
            LogisimVias gatePad = null;

            int delta = 12;

            bool br = false;

            // Gate direction (br? wtf is br?)

            if (comp.props.ContainsKey("gate"))
            {
                br = comp.props["gate"] == "br" ? true : false;
            }

            // Facing of drain
            // TODO: Some translational symmetry here.. make code more compact.. and complicated

            switch (comp.props["facing"])
            {
                case "north":
                    source = new LogisimWire(
                        new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height + delta),
                        new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height));
                    drain = new LogisimWire(
                        new Point(bbox.X + bbox.Width / 2, bbox.Y),
                        new Point(bbox.X + bbox.Width / 2, bbox.Y - delta));
                    if (br)
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X + bbox.Width + delta, bbox.Y + bbox.Height / 2),
                            new Point(bbox.X + bbox.Width, bbox.Y + bbox.Height / 2));
                    }
                    else
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X - delta, bbox.Y + bbox.Height / 2),
                            new Point(bbox.X, bbox.Y + bbox.Height / 2));
                    }

                    break;
                case "south":
                    source = new LogisimWire(
                        new Point(bbox.X + bbox.Width / 2, bbox.Y - delta),
                        new Point(bbox.X + bbox.Width / 2, bbox.Y));
                    drain = new LogisimWire(
                        new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height),
                        new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height + delta));
                    if (br)
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X + bbox.Width + delta, bbox.Y + bbox.Height / 2),
                            new Point(bbox.X + bbox.Width, bbox.Y + bbox.Height / 2));
                    }
                    else
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X - delta, bbox.Y + bbox.Height / 2),
                            new Point(bbox.X, bbox.Y + bbox.Height / 2));
                    }
                    break;
                case "west":
                    source = new LogisimWire(
                        new Point(bbox.X + bbox.Width + delta, bbox.Y + bbox.Height / 2),
                        new Point(bbox.X + bbox.Width, bbox.Y + bbox.Height / 2));
                    drain = new LogisimWire(
                        new Point(bbox.X, bbox.Y + bbox.Height / 2),
                        new Point(bbox.X - delta, bbox.Y + bbox.Height / 2));
                    if (br)
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height + delta),
                            new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height));
                    }
                    else
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X + bbox.Width / 2, bbox.Y - delta),
                            new Point(bbox.X + bbox.Width / 2, bbox.Y));
                    }
                    break;
                case "east":
                    source = new LogisimWire(
                        new Point(bbox.X - delta, bbox.Y + bbox.Height / 2),
                        new Point(bbox.X, bbox.Y + bbox.Height / 2));
                    drain = new LogisimWire(
                        new Point(bbox.X + bbox.Width, bbox.Y + bbox.Height / 2),
                        new Point(bbox.X + bbox.Width + delta, bbox.Y + bbox.Height / 2));
                    if (br)
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height + delta),
                            new Point(bbox.X + bbox.Width / 2, bbox.Y + bbox.Height));
                    }
                    else
                    {
                        gate = new LogisimWire(
                            new Point(bbox.X + bbox.Width / 2, bbox.Y - delta),
                            new Point(bbox.X + bbox.Width / 2, bbox.Y));
                    }
                    break;
            }

            // Create actual nodes/edges and link them

            sourcePad = GetViasByLoc(model, source.From());
            if (sourcePad == null)
                sourcePad = new LogisimVias(source.From());

            drainPad = GetViasByLoc(model, drain.To());
            if (drainPad == null)
                drainPad = new LogisimVias(drain.To());

            gatePad = GetViasByLoc(model, gate.From());
            if (gatePad == null)
                gatePad = new LogisimVias(gate.From());

            int nextId = GetNextId(model);

            sourcePad.id = nextId;
            drainPad.id = nextId + 1;
            gatePad.id = nextId + 2;

            gate.name = "g";        // Required by GraphFlow

            // Source

            LogisimComp sourceComp = ViasIntersectComp(model, sourcePad);
            if (sourceComp != null)
            {
                source.source = sourceComp;
                sourcePad = null;
            }
            else
            {
                source.source = sourcePad;
            }
            source.dest = comp;

            model.wires.Add(source);
            if (sourcePad != null)
                model.viases.Add(sourcePad);

            // Drain

            LogisimComp drainComp = ViasIntersectComp(model, drainPad);
            if (drainComp != null)
            {
                drain.dest = drainComp;
                drainPad = null;
            }
            else
            {
                drain.dest = drainPad;
            }
            drain.source = comp;

            model.wires.Add(drain);
            if (drainPad != null)
                model.viases.Add(drainPad);

            // Gate

            LogisimComp gateComp = ViasIntersectComp(model, gatePad);
            if (gateComp != null)
            {
                gate.source = gateComp;
                gatePad = null;
            }
            else
            {
                gate.source = gatePad;
            }
            gate.dest = comp;

            model.wires.Add(gate);
            if (gatePad != null)
                model.viases.Add(gatePad);
        }

        /// <summary>
        /// Reduce simple vias connections
        /// </summary>
        /// <param name="model"></param>

        public static void Reduce(LogisimModel model)
        {

        }

        public static int GetNextId (LogisimModel model)
        {
            int id = -1;
            
            foreach (var comp in model.comps)
            {
                if (comp.id > id)
                {
                    id = comp.id;
                }
            }

            foreach (var vias in model.viases)
            {
                if (vias.id > id)
                {
                    id = vias.id;
                }
            }

            return id + 1;
        }

        /// <summary>
        /// Convert logisim model to yEd Xml
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        public static XmlDocument ToYed (LogisimModel model)
        {
            XmlDocument doc = new XmlDocument();

            return doc;
        }

        public static void DumpLogisimModel(LogisimModel model)
        {
            Console.WriteLine(model.name + ":");

            foreach (var comp in model.comps)
            {
                comp.Dump();
            }

            foreach(var wire in model.wires)
            {
                wire.Dump();
            }

            foreach (var vias in model.viases)
            {
                vias.Dump();
            }
        }

        /// <summary>
        /// Restore wire linkage
        /// </summary>
        /// <param name="model"></param>
        public static void RestoreLinkage(LogisimModel model)
        {
            foreach (var wire in model.wires)
            {
                foreach(var comp in model.comps)
                {
                    if (wire.sourceId == comp.id)
                    {
                        wire.source = comp;
                    }
                    if (wire.destId == comp.id)
                    {
                        wire.dest = comp;
                    }
                }

                foreach (var vias in model.viases)
                {
                    if (wire.sourceId == vias.id)
                    {
                        wire.source = vias;
                    }
                    if (wire.destId == vias.id)
                    {
                        wire.dest = vias;
                    }
                }
            }
        }

        private static LogisimVias GetViasByLoc(LogisimModel model, Point loc)
        {
            foreach (var vias in model.viases)
            {
                if (vias.loc.Equals(loc))
                    return vias;
            }
            return null;
        }

        private static LogisimComp ViasIntersectComp (LogisimModel model, LogisimVias vias)
        {
            foreach (var comp in model.comps)
            {
                Rectangle rect = new Rectangle( comp.loc.X, comp.loc.Y, CompSize + 2, CompSize + 2);

                if (rect.Contains(vias.loc))
                {
                    return comp;
                }
            }

            return null;
        }

        public class LogisimNode
        {
            public int id = -1;
            public virtual string GetName()
            {
                return "<Unassigned>";
            }
        }

        public class LogisimEdge
        {
            private LogisimNode _source = new LogisimNode();
            private LogisimNode _dest = new LogisimNode();

            public int sourceId = -1;
            public int destId = -1;

            [XmlIgnore]
            public LogisimNode source
            {
                set
                {
                    _source = value;
                    sourceId = _source.id;
                }
                get
                {
                    return _source;
                }
            }

            [XmlIgnore]
            public LogisimNode dest
            {
                set
                {
                    _dest = value;
                    destId = _dest.id;
                }
                get
                {
                    return _dest;
                }
            }

        }

        public class LogisimWire : LogisimEdge
        {
            public List<Point> path = new List<Point>();
            public string name = "";

            public LogisimWire() {}

            public LogisimWire(Point s, Point e)
            {
                path.Add(s);
                path.Add(e);
            }

            public Point From()
            {
                return path[0];
            }

            public Point To()
            {
                return path[path.Count - 1];
            }

            public void Flip()
            {
                List<Point> pathrev = new List<Point>();

                for (int i=path.Count-1; i>=0; i--)
                {
                    pathrev.Add(new Point(path[i].X, path[i].Y));
                }

                path.Clear();
                path = pathrev;

                var temp = source;
                source = dest;
                dest = temp;
            }

            public void Dump()
            {
                Console.Write("wire {0} ", name);

                foreach(var p in path)
                {
                    Console.Write("({0}, {1})", p.X, p.Y);
                }

                Console.WriteLine(", {0} -> {1}", source.GetName(), dest.GetName());
            }
        }

        public class LogisimComp : LogisimNode
        {
            [XmlIgnore]
            public int lib = 0;
            public Point loc = new Point();
            public string name;
            [XmlIgnore]
            public Dictionary<string, string> props = new Dictionary<string, string>();

            public override string GetName()
            {
                return name + "(" + id.ToString() + ")";
            }

            public void Dump()
            {
                Console.WriteLine("comp: lib={0}, loc=({1},{2}), name={3}({4})", lib, loc.X, loc.Y, name, id);
                foreach (var prop in props)
                {
                    Console.WriteLine("   a {0} = {1}", prop.Key, prop.Value);
                }
            }
        }

        public class LogisimVias : LogisimNode
        {
            public Point loc = new Point();

            public LogisimVias() {}

            public LogisimVias(Point p)
            {
                loc = p;
            }

            public override string GetName()
            {
                return "Vias(" + id.ToString() + ")";
            }

            public void Dump()
            {
                Console.WriteLine("vias ({0},{1})",
                    loc.X, loc.Y );
            }
        }

    }
}
