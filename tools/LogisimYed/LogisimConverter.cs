using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml;
using System.Drawing;
using System.Text.RegularExpressions;

namespace LogisimYed
{
    public class LogisimConverter
    {
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
                    LogisimWire wire = new LogisimWire();

                    wire.from = ParseLoc(node.Attributes["from"].Value);
                    wire.to = ParseLoc(node.Attributes["to"].Value);

                    model.wires.Add(wire);
                }
            }

            // Get wires interconnections (produce viases)

            foreach (var wire in model.wires)
            {
                foreach (var another in model.wires)
                {
                    if (wire != another)
                    {
                        if (wire.from.Equals(another.from) || 
                            wire.from.Equals(another.to) )
                        {
                            LogisimVias vias = new LogisimVias();

                            vias.loc = wire.from;

                            if (!model.ViasExists(vias))
                                model.viases.Add(vias);
                        }

                        if ( wire.to.Equals(another.to) ||
                            wire.to.Equals(another.from) )
                        {
                            LogisimVias vias = new LogisimVias();

                            vias.loc = wire.to;

                            if (!model.ViasExists(vias))
                                model.viases.Add(vias);
                        }
                    }
                }
            }

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
        /// Convert logisim model to yEd Xml
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        public static XmlDocument ToYed (LogisimModel model)
        {
            XmlDocument doc = new XmlDocument();

            return doc;
        }

        private static void DumpLogisimModel(LogisimModel model)
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

        public class LogisimWire
        {
            public Point from = new Point();
            public Point to = new Point();

            public void Dump()
            {
                Console.WriteLine("wire ({0},{1}) - ({2},{3})",
                    from.X, from.Y,
                    to.X, to.Y);
            }
        }

        public class LogisimComp
        {
            public int lib = 0;
            public Point loc = new Point();
            public string name;
            public Dictionary<string, string> props = new Dictionary<string, string>();

            public void Dump()
            {
                Console.WriteLine("comp: lib={0}, loc=({1},{2}), name={3}", lib, loc.X, loc.Y, name);
                foreach (var prop in props)
                {
                    Console.WriteLine("   a {0} = {1}", prop.Key, prop.Value);
                }
            }
        }

        public class LogisimVias
        {
            public Point loc = new Point();

            public void Dump()
            {
                Console.WriteLine("vias ({0},{1})",
                    loc.X, loc.Y );
            }
        }

    }
}
