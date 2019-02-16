// Workspace stuff

using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Serialization;
using System.Linq;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {
        //
        // Workspace
        //

        public void SaveWorkspace(string FileName)
        {
            DeleteGarbage();

            EntityBoxWorkspace wrk = new EntityBoxWorkspace(this);
            XmlSerializer ser = new XmlSerializer(typeof(EntityBoxWorkspace));

            using (FileStream fs = new FileStream(FileName, FileMode.Create))
            {
                ser.Serialize(fs, wrk);
            }
        }

        public void LoadWorkspace(string FileName)
        {
            XmlSerializer ser = new XmlSerializer(typeof(EntityBoxWorkspace));

            using (FileStream fs = new FileStream(FileName, FileMode.Open))
            {
                EntityBoxWorkspace wrk = (EntityBoxWorkspace)ser.Deserialize(fs);

                wrk.LoadSnapshot(this);

                if (OnEntityCountChanged != null)
                    OnEntityCountChanged(this, EventArgs.Empty);

                if (OnScrollChanged != null)
                    OnScrollChanged(this, EventArgs.Empty);

                if (OnZoomChanged != null)
                    OnZoomChanged(this, EventArgs.Empty);
            }
        }


        //
        // Workspace snapshot
        //

        public class EntityBoxWorkspace
        {
            private Image[] imageOrig = new Image[3];
            public float lambda;

            [XmlElement("Image0")]
            [Browsable(false)]
            public byte[] Image0
            {
                get
                {
                    if (imageOrig[0] != null)
                    {
                        MemoryStream ms = new MemoryStream();
                        imageOrig[0].Save(ms, ImageFormat.Jpeg);
                        return ms.ToArray();
                    }
                    else return null;
                }
                set
                {
                    imageOrig[0] = Image.FromStream(new MemoryStream(value));
                }
            }

            [XmlElement("Image1")]
            [Browsable(false)]
            public byte[] Image1
            {
                get
                {
                    if (imageOrig[1] != null)
                    {
                        MemoryStream ms = new MemoryStream();
                        imageOrig[1].Save(ms, ImageFormat.Jpeg);
                        return ms.ToArray();
                    }
                    else return null;
                }
                set
                {
                    imageOrig[1] = Image.FromStream(new MemoryStream(value));
                }
            }

            [XmlElement("Image2")]
            [Browsable(false)]
            public byte[] Image2
            {
                get
                {
                    if (imageOrig[2] != null)
                    {
                        MemoryStream ms = new MemoryStream();
                        imageOrig[2].Save(ms, ImageFormat.Jpeg);
                        return ms.ToArray();
                    }
                    else return null;
                }
                set
                {
                    imageOrig[2] = Image.FromStream(new MemoryStream(value));
                }
            }

            public int[] imageZoom = new int[3];
            public int zoom;
            public PointF[] imageScroll = new PointF[3];
            public float ScrollX;
            public float ScrollY;
            public Entity root;
            public List<Entity> entities;
            public EntityMode drawMode;
            public bool hideImage;
            public bool hideVias;
            public bool hideWires;
            public bool hideCells;
            public bool hideGrid;
            public bool hideLambdaMetrics;
            public bool hideRegions;
            [XmlIgnore] public Color selectionBoxColor;
            [XmlIgnore] public Color ForeColor;
            public int[] imageOpacity = new int[3];
            public bool[] lockScroll = new bool[3];
            public bool[] lockZoom = new bool[3];
            public bool grayscale;

            [XmlIgnore] public Color ViasInputColor;
            [XmlIgnore] public Color ViasOutputColor;
            [XmlIgnore] public Color ViasInoutColor;
            [XmlIgnore] public Color ViasConnectColor;
            [XmlIgnore] public Color ViasFloatingColor;
            [XmlIgnore] public Color ViasPowerColor;
            [XmlIgnore] public Color ViasGroundColor;
            [XmlIgnore] public Color WireInterconnectColor;
            [XmlIgnore] public Color WirePowerColor;
            [XmlIgnore] public Color WireGroundColor;
            [XmlIgnore] public Color CellNotColor;
            [XmlIgnore] public Color CellBufferColor;
            [XmlIgnore] public Color CellMuxColor;
            [XmlIgnore] public Color CellLogicColor;
            [XmlIgnore] public Color CellAdderColor;
            [XmlIgnore] public Color CellBusSuppColor;
            [XmlIgnore] public Color CellFlipFlopColor;
            [XmlIgnore] public Color CellLatchColor;
            [XmlIgnore] public Color CellOtherColor;
            [XmlIgnore] public Color UnitRegfileColor;
            [XmlIgnore] public Color UnitMemoryColor;
            [XmlIgnore] public Color UnitCustomColor;
            [XmlIgnore] public Color SelectionColor;
            [XmlIgnore] public Color ViasOverrideColor;
            [XmlIgnore] public Color WireOverrideColor;
            [XmlIgnore] public Color CellOverrideColor;
            [XmlIgnore] public Color RegionOverrideColor;
            public ViasShape viasShape;
            public int viasBaseSize;
            public int wireBaseSize;
            public TextAlignment cellTextAlignment;
            public TextAlignment viasTextAlignment;
            public TextAlignment wireTextAlignment;
            public int ViasOpacity;
            public int WireOpacity;
            public int CellOpacity;
            public int RegionOpacity;
            public int ViasPriority;
            public int WirePriority;
            public int CellPriority;
            public int BeaconPriority;
            public int RegionPriority;
            public bool AutoPriority;

            [XmlElement("ForeColor")]
            [Browsable(false)]
            public string ForeColorHtml
            {
                get { return ColorTranslator.ToHtml(ForeColor); }
                set { ForeColor = ColorTranslator.FromHtml(value); }
            }


            [XmlElement("SelectionBoxColor")]
            [Browsable(false)]
            public string SelectionBoxColorHtml
            {
                get { return ColorTranslator.ToHtml(selectionBoxColor); }
                set { selectionBoxColor = ColorTranslator.FromHtml(value); }
            }


            [XmlElement("ViasInputColor")]
            [Browsable(false)]
            public string ViasInputColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasInputColor); }
                set { ViasInputColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("ViasOutputColor")]
            [Browsable(false)]
            public string ViasOutputColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasOutputColor); }
                set { ViasOutputColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("ViasInoutColor")]
            [Browsable(false)]
            public string ViasInoutColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasInoutColor); }
                set { ViasInoutColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("ViasConnectColor")]
            [Browsable(false)]
            public string ViasConnectColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasConnectColor); }
                set { ViasConnectColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("ViasFloatingColor")]
            [Browsable(false)]
            public string ViasFloatingColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasFloatingColor); }
                set { ViasFloatingColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("ViasPowerColor")]
            [Browsable(false)]
            public string ViasPowerColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasPowerColor); }
                set { ViasPowerColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("ViasGroundColor")]
            [Browsable(false)]
            public string ViasGroundColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasGroundColor); }
                set { ViasGroundColor = ColorTranslator.FromHtml(value); }
            }


            [XmlElement("WireInterconnectColor")]
            [Browsable(false)]
            public string WireInterconnectColorHtml
            {
                get { return ColorTranslator.ToHtml(WireInterconnectColor); }
                set { WireInterconnectColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("WirePowerColor")]
            [Browsable(false)]
            public string WirePowerColorHtml
            {
                get { return ColorTranslator.ToHtml(WirePowerColor); }
                set { WirePowerColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("WireGroundColor")]
            [Browsable(false)]
            public string WireGroundColorHtml
            {
                get { return ColorTranslator.ToHtml(WireGroundColor); }
                set { WireGroundColor = ColorTranslator.FromHtml(value); }
            }


            [XmlElement("CellNotColor")]
            [Browsable(false)]
            public string CellNotColorHtml
            {
                get { return ColorTranslator.ToHtml(CellNotColor); }
                set { CellNotColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellBufferColor")]
            [Browsable(false)]
            public string CellBufferColorHtml
            {
                get { return ColorTranslator.ToHtml(CellBufferColor); }
                set { CellBufferColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellMuxColor")]
            [Browsable(false)]
            public string CellMuxColorHtml
            {
                get { return ColorTranslator.ToHtml(CellMuxColor); }
                set { CellMuxColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellLogicColor")]
            [Browsable(false)]
            public string CellLogicColorHtml
            {
                get { return ColorTranslator.ToHtml(CellLogicColor); }
                set { CellLogicColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellAdderColor")]
            [Browsable(false)]
            public string CellAdderColorHtml
            {
                get { return ColorTranslator.ToHtml(CellAdderColor); }
                set { CellAdderColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellBusSuppColor")]
            [Browsable(false)]
            public string CellBusSuppColorHtml
            {
                get { return ColorTranslator.ToHtml(CellBusSuppColor); }
                set { CellBusSuppColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellFlipFlopColor")]
            [Browsable(false)]
            public string CellFlipFlopColorHtml
            {
                get { return ColorTranslator.ToHtml(CellFlipFlopColor); }
                set { CellFlipFlopColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellLatchColor")]
            [Browsable(false)]
            public string CellLatchColorHtml
            {
                get { return ColorTranslator.ToHtml(CellLatchColor); }
                set { CellLatchColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellOtherColor")]
            [Browsable(false)]
            public string CellOtherColorHtml
            {
                get { return ColorTranslator.ToHtml(CellOtherColor); }
                set { CellOtherColor = ColorTranslator.FromHtml(value); }
            }


            [XmlElement("UnitRegfileColor")]
            [Browsable(false)]
            public string UnitRegfileColorHtml
            {
                get { return ColorTranslator.ToHtml(UnitRegfileColor); }
                set { UnitRegfileColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("UnitMemoryColor")]
            [Browsable(false)]
            public string UnitMemoryColorHtml
            {
                get { return ColorTranslator.ToHtml(UnitMemoryColor); }
                set { UnitMemoryColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("UnitCustomColor")]
            [Browsable(false)]
            public string UnitCustomColorHtml
            {
                get { return ColorTranslator.ToHtml(UnitCustomColor); }
                set { UnitCustomColor = ColorTranslator.FromHtml(value); }
            }

            [XmlElement("SelectionColor")]
            [Browsable(false)]
            public string SelectionColorHtml
            {
                get { return ColorTranslator.ToHtml(SelectionColor); }
                set { SelectionColor = ColorTranslator.FromHtml(value); }
            }

            [XmlElement("ViasOverrideColor")]
            [Browsable(false)]
            public string ViasOverrideColorHtml
            {
                get { return ColorTranslator.ToHtml(ViasOverrideColor); }
                set { ViasOverrideColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("WireOverrideColor")]
            [Browsable(false)]
            public string WireOverrideColorHtml
            {
                get { return ColorTranslator.ToHtml(WireOverrideColor); }
                set { WireOverrideColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("CellOverrideColor")]
            [Browsable(false)]
            public string CellOverrideColorHtml
            {
                get { return ColorTranslator.ToHtml(CellOverrideColor); }
                set { CellOverrideColor = ColorTranslator.FromHtml(value); }
            }
            [XmlElement("RegionOverrideColor")]
            [Browsable(false)]
            public string RegionOverrideColorHtml
            {
                get { return ColorTranslator.ToHtml(RegionOverrideColor); }
                set { RegionOverrideColor = ColorTranslator.FromHtml(value); }
            }

            public EntityBoxWorkspace()
            {
            }

            public EntityBoxWorkspace(EntityBox parent)
            {
                if (parent.Image0 != null)
                    imageOrig[0] = new Bitmap(parent.Image0);
                else
                    imageOrig[0] = null;

                if (parent.Image1 != null)
                    imageOrig[1] = new Bitmap(parent.Image1);
                else
                    imageOrig[1] = null;

                if (parent.Image2 != null)
                    imageOrig[2] = new Bitmap(parent.Image2);
                else
                    imageOrig[2] = null;

                lambda = parent.Lambda;
                imageZoom[0] = parent.ZoomImage0;
                imageZoom[1] = parent.ZoomImage1;
                imageZoom[2] = parent.ZoomImage2;
                zoom = parent.Zoom;
                imageScroll[0] = parent.ScrollImage0;
                imageScroll[1] = parent.ScrollImage1;
                imageScroll[2] = parent.ScrollImage2;
                ScrollX = parent.ScrollX;
                ScrollY = parent.ScrollY;

                root = parent.root;

                /// For compatibility with older snapshots

                entities = parent.GetEntities();

                drawMode = parent.Mode;
                hideImage = parent.HideImage;
                hideVias = parent.HideVias;
                hideWires = parent.HideWires;
                hideCells = parent.HideCells;
                hideGrid = parent.HideGrid;
                hideLambdaMetrics = parent.HideLambdaMetrics;
                hideRegions = parent.HideRegions;
                selectionBoxColor = parent.SelectionBoxColor;
                ForeColor = parent.ForeColor;
                imageOpacity[0] = parent.ImageOpacity0;
                imageOpacity[1] = parent.ImageOpacity1;
                imageOpacity[2] = parent.ImageOpacity2;
                lockScroll[0] = parent.LockScroll0;
                lockScroll[1] = parent.LockScroll1;
                lockScroll[2] = parent.LockScroll2;
                lockZoom[0] = parent.LockZoom0;
                lockZoom[1] = parent.LockZoom1;
                lockZoom[2] = parent.LockZoom2;
                grayscale = parent.Grayscale;

                ViasInputColor = parent.ViasInputColor;
                ViasOutputColor = parent.ViasOutputColor;
                ViasInoutColor = parent.ViasInoutColor;
                ViasConnectColor = parent.ViasConnectColor;
                ViasFloatingColor = parent.ViasFloatingColor;
                ViasPowerColor = parent.ViasPowerColor;
                ViasGroundColor = parent.ViasGroundColor;
                WireInterconnectColor = parent.WireInterconnectColor;
                WirePowerColor = parent.WirePowerColor;
                WireGroundColor = parent.WireGroundColor;
                CellNotColor = parent.CellNotColor;
                CellBufferColor = parent.CellBufferColor;
                CellMuxColor = parent.CellMuxColor;
                CellLogicColor = parent.CellLogicColor;
                CellAdderColor = parent.CellAdderColor;
                CellBusSuppColor = parent.CellBusSuppColor;
                CellFlipFlopColor = parent.CellFlipFlopColor;
                CellLatchColor = parent.CellLatchColor;
                CellOtherColor = parent.CellOtherColor;
                UnitRegfileColor = parent.UnitRegfileColor;
                UnitMemoryColor = parent.UnitMemoryColor;
                UnitCustomColor = parent.UnitCustomColor;
                SelectionColor = parent.SelectionColor;
                ViasOverrideColor = parent.ViasOverrideColor;
                WireOverrideColor = parent.WireOverrideColor;
                CellOverrideColor = parent.CellOverrideColor;
                RegionOverrideColor = parent.RegionOverrideColor;
                viasShape = parent.ViasShape;
                viasBaseSize = parent.ViasBaseSize;
                wireBaseSize = parent.WireBaseSize;
                cellTextAlignment = parent.CellTextAlignment;
                viasTextAlignment = parent.ViasTextAlignment;
                wireTextAlignment = parent.WireTextAlignment;
                ViasOpacity = parent.ViasOpacity;
                WireOpacity = parent.WireOpacity;
                CellOpacity = parent.CellOpacity;
                RegionOpacity = parent.RegionOpacity;
                ViasPriority = parent.ViasPriority;
                WirePriority = parent.WirePriority;
                CellPriority = parent.CellPriority;
                AutoPriority = parent.AutoPriority;
                BeaconPriority = parent.BeaconPriority;
                RegionPriority = parent.RegionPriority;
            }

            public void LoadSnapshot(EntityBox parent)
            {
                parent.Image0 = imageOrig[0];
                parent.Image1 = imageOrig[1];
                parent.Image2 = imageOrig[2];

                parent.Lambda = lambda;
                parent.ZoomImage0 = imageZoom[0];
                parent.ZoomImage1 = imageZoom[1];
                parent.ZoomImage2 = imageZoom[2];
                parent.Zoom = zoom;
                parent.ScrollImage0 = imageScroll[0];
                parent.ScrollImage1 = imageScroll[1];
                parent.ScrollImage2 = imageScroll[2];
                parent.ScrollX = ScrollX;
                parent.ScrollY = ScrollY;

                /// For compatibility with older snapshots

                if (root == null && entities != null)
                {
                    parent.root.Children = entities;
                }
                else
                {
                    parent.root = root;
                }

                parent.Mode = drawMode;
                parent.HideImage = hideImage;
                parent.HideVias = hideVias;
                parent.HideWires = hideWires;
                parent.HideCells = hideCells;
                parent.HideGrid = hideGrid;
                parent.HideLambdaMetrics = hideLambdaMetrics;
                parent.HideRegions = hideRegions;
                parent.SelectionBoxColor = selectionBoxColor;
                parent.ForeColor = ForeColor;
                parent.ImageOpacity0 = imageOpacity[0];
                parent.ImageOpacity1 = imageOpacity[1];
                parent.ImageOpacity2 = imageOpacity[2];
                parent.LockScroll0 = lockScroll[0];
                parent.LockScroll1 = lockScroll[1];
                parent.LockScroll2 = lockScroll[2];
                parent.LockZoom0 = lockZoom[0];
                parent.LockZoom1 = lockZoom[1];
                parent.LockZoom2 = lockZoom[2];
                parent.Grayscale = grayscale;

                parent.ViasInputColor = ViasInputColor;
                parent.ViasOutputColor = ViasOutputColor;
                parent.ViasInoutColor = ViasInoutColor;
                parent.ViasConnectColor = ViasConnectColor;
                parent.ViasFloatingColor = ViasFloatingColor;
                parent.ViasPowerColor = ViasPowerColor;
                parent.ViasGroundColor = ViasGroundColor;
                parent.WireInterconnectColor = WireInterconnectColor;
                parent.WirePowerColor = WirePowerColor;
                parent.WireGroundColor = WireGroundColor;
                parent.CellNotColor = CellNotColor;
                parent.CellBufferColor = CellBufferColor;
                parent.CellMuxColor = CellMuxColor;
                parent.CellLogicColor = CellLogicColor;
                parent.CellAdderColor = CellAdderColor;
                parent.CellBusSuppColor = CellBusSuppColor;
                parent.CellFlipFlopColor = CellFlipFlopColor;
                parent.CellLatchColor = CellLatchColor;
                parent.CellOtherColor = CellOtherColor;
                parent.UnitRegfileColor = UnitRegfileColor;
                parent.UnitMemoryColor = UnitMemoryColor;
                parent.UnitCustomColor = UnitCustomColor;
                parent.SelectionColor = SelectionColor;
                parent.ViasOverrideColor = ViasOverrideColor;
                parent.WireOverrideColor = WireOverrideColor;
                parent.CellOverrideColor = CellOverrideColor;
                parent.RegionOverrideColor = RegionOverrideColor;
                parent.ViasShape = viasShape;
                parent.ViasBaseSize = viasBaseSize;
                parent.WireBaseSize = wireBaseSize;
                parent.CellTextAlignment = cellTextAlignment;
                parent.ViasTextAlignment = viasTextAlignment;
                parent.WireTextAlignment = wireTextAlignment;
                parent.ViasOpacity = ViasOpacity;
                parent.WireOpacity = WireOpacity;
                parent.CellOpacity = CellOpacity;
                parent.RegionOpacity = RegionOpacity;
                parent.ViasPriority = ViasPriority;
                parent.WirePriority = WirePriority;
                parent.CellPriority = CellPriority;
                parent.AutoPriority = AutoPriority;
                parent.BeaconPriority = BeaconPriority;
                parent.RegionPriority = RegionPriority;

                GC.Collect();
                parent.Invalidate();
            }
        }

    }
}
