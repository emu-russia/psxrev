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
            bool found = false;

            do
            {
                found = false;

                foreach ( var vias in model.viases)
                {
                    // Count inputs/outputs for vias

                    int inputCount = 0;
                    int outputCount = 0;

                    LogisimWire wireIn = null;
                    LogisimWire wireOut = null;

                    foreach (var wire in model.wires)
                    {
                        if (wire.dest == vias)
                        {
                            if (wireIn == null)
                                wireIn = wire;
                            inputCount++;
                        }
                        if (wire.source == vias)
                        {
                            if (wireOut == null)
                                wireOut = wire;
                            outputCount++;
                        }
                    }

                    // If exactly 1/1 - delete vias and reduce wire

                    if (inputCount == 1 && outputCount == 1)
                    {
                        Point last = wireIn.To();
                        wireIn.path.Remove(last);
                        wireIn.path.AddRange(wireOut.path);
                        wireIn.dest = wireOut.dest;
                        model.wires.Remove(wireOut);
                        model.viases.Remove(vias);
                        found = true;
                        break;
                    }
                }

            } while (found);
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

        /// <summary>
        /// Convert logisim model to yEd Xml
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        public static XmlDocument ToYed(LogisimModel model)
        {
            XmlDocument doc = new XmlDocument();

            // Create an XML declaration
            XmlDeclaration xmldecl;
            xmldecl = doc.CreateXmlDeclaration("1.0", "UTF-8", "no");

            // Add the new node to the document
            XmlElement root = doc.DocumentElement;
            doc.InsertBefore(xmldecl, root);

            // Convert Logisim Model to Yed Model

            YedModel yed = new YedModel();

            AddYedKeys(yed);

            YedData graphData = new YedData();

            graphData.key = "d0";
            graphData.xmlSpace = "preserve";

            yed.graph.data.Add(graphData);

            foreach (var comp in model.comps)
            {
                YedNode node = new YedNode();
                node.id = "n" + comp.id.ToString();

                YedData d6 = new YedData();

                node.data.Add(d6);

                d6.key = "d6";

                d6.shape = new YedShapeNode();
                d6.shape.geometry = new YedGeometry();
                d6.shape.shape = new YedShape();

                d6.shape.geometry.x = comp.loc.X;
                d6.shape.geometry.y = comp.loc.Y;
                d6.shape.geometry.width = CompSize;
                d6.shape.geometry.height = CompSize;

                d6.shape.shape.type = "rectangle";

                yed.graph.nodes.Add(node);
            }

            foreach (var vias in model.viases)
            {
                YedNode node = new YedNode();
                node.id = "n" + vias.id.ToString();

                YedData d6 = new YedData();

                node.data.Add(d6);

                d6.key = "d6";

                d6.shape = new YedShapeNode();
                d6.shape.geometry = new YedGeometry();
                d6.shape.shape = new YedShape();

                d6.shape.geometry.x = vias.loc.X;
                d6.shape.geometry.y = vias.loc.Y;
                d6.shape.geometry.width = 5;
                d6.shape.geometry.height = 5;

                d6.shape.shape.type = "ellipse";

                yed.graph.nodes.Add(node);
            }

            int edgeId = 0;     // Doesn't matter..

            foreach (var wire in model.wires)
            {
                YedEdge edge = new YedEdge();
                edge.id = "e" + edgeId.ToString();
                edge.source = "n" + wire.source.id.ToString();
                edge.target = "n" + wire.dest.id.ToString();
                edgeId++;
                yed.graph.edges.Add(edge);
            }

            // Convert Yed Model to Xml

            var nav = doc.CreateNavigator();
            var ns = new XmlSerializerNamespaces();
            ns.Add("java", "http://www.yworks.com/xml/yfiles-common/1.0/java");
            ns.Add("sys", "http://www.yworks.com/xml/yfiles-common/markup/primitives/2.0");
            ns.Add("x", "http://www.yworks.com/xml/yfiles-common/markup/2.0");
            ns.Add("xsi", "http://www.w3.org/2001/XMLSchema-instance");
            ns.Add("y", "http://www.yworks.com/xml/graphml");
            ns.Add("yed", "http://www.yworks.com/xml/yed/3");

            using (var writer = nav.AppendChild())
            {
                var ser = new XmlSerializer(typeof(YedModel));
                ser.Serialize(writer, yed, ns);
            }

            return doc;
        }

        private static void AddYedKeys(YedModel yed)
        {
            yed.keys.Add(new YedKey("d0", "Description", "string", "graph", null));
            yed.keys.Add(new YedKey("d1", null, null, "port", "portgraphics"));
            yed.keys.Add(new YedKey("d2", null, null, "port", "portgeometry"));
            yed.keys.Add(new YedKey("d3", null, null, "port", "portuserdata"));
            yed.keys.Add(new YedKey("d4", "url", "string", "node", null));
            yed.keys.Add(new YedKey("d5", "description", "string", "node", null));
            yed.keys.Add(new YedKey("d6", null, null, "node", "nodegraphics"));
            yed.keys.Add(new YedKey("d7", null, null, "graphml", "resources"));
            yed.keys.Add(new YedKey("d8", "url", "string", "edge", null));
            yed.keys.Add(new YedKey("d9", "description", "string", "edge", null));
            yed.keys.Add(new YedKey("d10", null, null, "edge", "edgegraphics"));
        }

        [XmlRoot("graphml")]
        public class YedModel
        {
            [XmlElement(ElementName ="key")]
            public List<YedKey> keys = new List<YedKey>();
            [XmlElement]
            public YedGraph graph = new YedGraph();
        }

        public class YedGraph
        {
            [XmlAttribute]
            public string edgedefault = "directed";
            [XmlAttribute]
            public string id = "G";
            [XmlElement(ElementName = "data")]
            public List<YedData> data = new List<YedData>();
            [XmlElement(ElementName = "node")]
            public List<YedNode> nodes = new List<YedNode>();
            [XmlElement(ElementName = "edge")]
            public List<YedEdge> edges = new List<YedEdge>();
        }

        public class YedKey
        {
            [XmlAttribute]
            public string id;
            [XmlAttribute(AttributeName = "attr.name")]
            public string attrName;
            [XmlAttribute(AttributeName = "attr.type")]
            public string attrType;
            [XmlAttribute(AttributeName = "for")]
            public string For;
            [XmlAttribute(AttributeName = "yfiles.type")]
            public string yfilesType;

            public YedKey() {}

            public YedKey (string _id, string _attrName, string _attrType, string _for, string _yfilesType)
            {
                id = _id;
                attrName = _attrName;
                attrType = _attrType;
                For = _for;
                yfilesType = _yfilesType;
            }
        }

        public class YedNode
        {
            [XmlAttribute]
            public string id;
            [XmlElement(ElementName = "data")]
            public List<YedData> data = new List<YedData>();
        }

        public class YedEdge
        {
            [XmlAttribute]
            public string id;
            [XmlAttribute]
            public string source;
            [XmlAttribute]
            public string target;
        }

        public class YedData
        {
            [XmlAttribute]
            public string key;
            [XmlAttribute(AttributeName = "xml:space")]
            public string xmlSpace;
            [XmlElement(ElementName = "ShapeNode", Namespace = "http://www.yworks.com/xml/graphml")]
            public YedShapeNode shape;
        }

        public class YedShapeNode
        {
            [XmlElement(ElementName = "Geometry", Namespace = "http://www.yworks.com/xml/graphml")]
            public YedGeometry geometry;
            [XmlElement(ElementName = "Shape", Namespace = "http://www.yworks.com/xml/graphml")]
            public YedShape shape;
        }

        public class YedGeometry
        {
            [XmlAttribute]
            public float x;
            [XmlAttribute]
            public float y;
            [XmlAttribute]
            public float width;
            [XmlAttribute]
            public float height;
        }

        public class YedShape
        {
            [XmlAttribute]
            public string type;
        }





    }
}
