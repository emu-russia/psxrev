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
    public partial class FormSettings : Form
    {
        private GlobalSettings globalSettings;
        private ColorSettings colorSettings;
        private PrioritySettings prioritySettings;
        private OpacitySettings opacitySettings;
        private SizeSettings sizeSettings;
        private ShapeSettings shapeSettings;

        private EntityBox savedEntityBox;

        public FormSettings(EntityBox entityBox)
        {
            InitializeComponent();

            savedEntityBox = entityBox;

            propertyGridEntityBox.SelectedObject = entityBox;

            globalSettings = new GlobalSettings(entityBox);
            propertyGridGlobal.SelectedObject = globalSettings;

            colorSettings = new ColorSettings(entityBox);
            propertyGridColors.SelectedObject = colorSettings;

            prioritySettings = new PrioritySettings(entityBox);
            propertyGridPriority.SelectedObject = prioritySettings;

            opacitySettings = new OpacitySettings(entityBox);
            propertyGridOpacity.SelectedObject = opacitySettings;

            sizeSettings = new SizeSettings(entityBox);
            propertyGridSize.SelectedObject = sizeSettings;

            shapeSettings = new ShapeSettings(entityBox);
            propertyGridShape.SelectedObject = shapeSettings;
        }

        private void FormSettings_KeyDown(object sender, KeyEventArgs e)
        {
            if ( e.KeyCode == Keys.Escape)
            {
                DialogResult = DialogResult.Cancel;
                Close();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            globalSettings.Save();
            colorSettings.Save();
            prioritySettings.Save();
            opacitySettings.Save();
            sizeSettings.Save();
            shapeSettings.Save();

            SaveSettings(savedEntityBox);

            DialogResult = DialogResult.OK;
            Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }


        internal class GlobalSettings
        {
            public bool SelectEntitiesAfterAdd { get; set; }
            public bool Grayscale { get; set; }
            public float Lambda { get; set; }
            public bool LockScroll0 { get; set; }
            public bool LockScroll1 { get; set; }
            public bool LockScroll2 { get; set; }
            public bool LockZoom0 { get; set; }
            public bool LockZoom1 { get; set; }
            public bool LockZoom2 { get; set; }
            public bool HideGrid { get; set; }
            public bool HideLambdaMetrics { get; set; }
            public TextAlignment CellTextAlignment { get; set; }
            public TextAlignment ViasTextAlignment { get; set; }
            public TextAlignment WireTextAlignment { get; set; }

            private EntityBox savedEntityBox;

            public GlobalSettings(EntityBox entityBox)
            {
                savedEntityBox = entityBox;

                SelectEntitiesAfterAdd = entityBox.SelectEntitiesAfterAdd;
                Grayscale = entityBox.Grayscale;
                Lambda = entityBox.Lambda;
                LockScroll0 = entityBox.LockScroll0;
                LockScroll1 = entityBox.LockScroll1;
                LockScroll2 = entityBox.LockScroll2;
                LockZoom0 = entityBox.LockZoom0;
                LockZoom1 = entityBox.LockZoom1;
                LockZoom2 = entityBox.LockZoom2;
                HideGrid = entityBox.HideGrid;
                HideLambdaMetrics = entityBox.HideLambdaMetrics;
                CellTextAlignment = entityBox.CellTextAlignment;
                ViasTextAlignment = entityBox.ViasTextAlignment;
                WireTextAlignment = entityBox.WireTextAlignment;
            }

            public void Save()
            {
                savedEntityBox.SelectEntitiesAfterAdd = SelectEntitiesAfterAdd;
                savedEntityBox.Grayscale = Grayscale;
                savedEntityBox.Lambda = Lambda;
                savedEntityBox.LockScroll0 = LockScroll0;
                savedEntityBox.LockScroll1 = LockScroll1;
                savedEntityBox.LockScroll2 = LockScroll2;
                savedEntityBox.LockZoom0 = LockZoom0;
                savedEntityBox.LockZoom1 = LockZoom1;
                savedEntityBox.LockZoom2 = LockZoom2;
                savedEntityBox.HideGrid = HideGrid;
                savedEntityBox.HideLambdaMetrics = HideLambdaMetrics;
                savedEntityBox.CellTextAlignment = CellTextAlignment;
                savedEntityBox.ViasTextAlignment = ViasTextAlignment;
                savedEntityBox.WireTextAlignment = WireTextAlignment;
            }
        }



        internal class ColorSettings
        {
            public Color SelectionBoxColor { get; set; }
            public Color ViasInputColor { get; set; }
            public Color ViasOutputColor { get; set; }
            public Color ViasInoutColor { get; set; }
            public Color ViasConnectColor { get; set; }
            public Color ViasFloatingColor { get; set; }
            public Color ViasPowerColor { get; set; }
            public Color ViasGroundColor { get; set; }
            public Color WireInterconnectColor { get; set; }
            public Color WirePowerColor { get; set; }
            public Color WireGroundColor { get; set; }
            public Color CellNotColor { get; set; }
            public Color CellBufferColor { get; set; }
            public Color CellMuxColor { get; set; }
            public Color CellLogicColor { get; set; }
            public Color CellAdderColor { get; set; }
            public Color CellBusSuppColor { get; set; }
            public Color CellFlipFlopColor { get; set; }
            public Color CellLatchColor { get; set; }
            public Color CellOtherColor { get; set; }
            public Color UnitRegfileColor { get; set; }
            public Color UnitMemoryColor { get; set; }
            public Color UnitCustomColor { get; set; }
            public Color SelectionColor { get; set; }
            public Color ViasOverrideColor { get; set; }
            public Color WireOverrideColor { get; set; }
            public Color CellOverrideColor { get; set; }
            public Color RegionOverrideColor { get; set; }

            private EntityBox savedEntityBox;

            public ColorSettings(EntityBox entityBox)
            {
                savedEntityBox = entityBox;

                SelectionBoxColor = entityBox.SelectionBoxColor;
                ViasInputColor = entityBox.ViasInputColor;
                ViasOutputColor = entityBox.ViasOutputColor;
                ViasInoutColor = entityBox.ViasInoutColor;
                ViasConnectColor = entityBox.ViasConnectColor;
                ViasFloatingColor = entityBox.ViasFloatingColor;
                ViasPowerColor = entityBox.ViasPowerColor;
                ViasGroundColor = entityBox.ViasGroundColor;
                WireInterconnectColor = entityBox.WireInterconnectColor;
                WirePowerColor = entityBox.WirePowerColor;
                WireGroundColor = entityBox.WireGroundColor;
                CellNotColor = entityBox.CellNotColor;
                CellBufferColor = entityBox.CellBufferColor;
                CellMuxColor = entityBox.CellMuxColor;
                CellLogicColor = entityBox.CellLogicColor;
                CellAdderColor = entityBox.CellAdderColor;
                CellBusSuppColor = entityBox.CellBusSuppColor;
                CellFlipFlopColor = entityBox.CellFlipFlopColor;
                CellLatchColor = entityBox.CellLatchColor;
                CellOtherColor = entityBox.CellOtherColor;
                UnitRegfileColor = entityBox.UnitRegfileColor;
                UnitMemoryColor = entityBox.UnitMemoryColor;
                UnitCustomColor = entityBox.UnitCustomColor;
                SelectionColor = entityBox.SelectionColor;
                ViasOverrideColor = entityBox.ViasOverrideColor;
                WireOverrideColor = entityBox.WireOverrideColor;
                CellOverrideColor = entityBox.CellOverrideColor;
                RegionOverrideColor = entityBox.RegionOverrideColor;
            }

            public void Save()
            {
                savedEntityBox.SelectionBoxColor = SelectionBoxColor;
                savedEntityBox.ViasInputColor = ViasInputColor;
                savedEntityBox.ViasOutputColor = ViasOutputColor;
                savedEntityBox.ViasInoutColor = ViasInoutColor;
                savedEntityBox.ViasConnectColor = ViasConnectColor;
                savedEntityBox.ViasFloatingColor = ViasFloatingColor;
                savedEntityBox.ViasPowerColor = ViasPowerColor;
                savedEntityBox.ViasGroundColor = ViasGroundColor;
                savedEntityBox.WireInterconnectColor = WireInterconnectColor;
                savedEntityBox.WirePowerColor = WirePowerColor;
                savedEntityBox.WireGroundColor = WireGroundColor;
                savedEntityBox.CellNotColor = CellNotColor;
                savedEntityBox.CellBufferColor = CellBufferColor;
                savedEntityBox.CellMuxColor = CellMuxColor;
                savedEntityBox.CellLogicColor = CellLogicColor;
                savedEntityBox.CellAdderColor = CellAdderColor;
                savedEntityBox.CellBusSuppColor = CellBusSuppColor;
                savedEntityBox.CellFlipFlopColor = CellFlipFlopColor;
                savedEntityBox.CellLatchColor = CellLatchColor;
                savedEntityBox.CellOtherColor = CellOtherColor;
                savedEntityBox.UnitRegfileColor = UnitRegfileColor;
                savedEntityBox.UnitMemoryColor = UnitMemoryColor;
                savedEntityBox.UnitCustomColor = UnitCustomColor;
                savedEntityBox.SelectionColor = SelectionColor;
                savedEntityBox.ViasOverrideColor = ViasOverrideColor;
                savedEntityBox.WireOverrideColor = WireOverrideColor;
                savedEntityBox.CellOverrideColor = CellOverrideColor;
                savedEntityBox.RegionOverrideColor = RegionOverrideColor;
            }

        }


        internal class PrioritySettings
        {
            public int ViasPriority { get; set; }
            public int WirePriority { get; set; }
            public int CellPriority { get; set; }
            public int BeaconPriority { get; set; }
            public int RegionPriority { get; set; }
            public bool AutoPriority { get; set; }

            private EntityBox savedEntityBox;

            public PrioritySettings(EntityBox entityBox)
            {
                savedEntityBox = entityBox;

                ViasPriority = entityBox.ViasPriority;
                WirePriority = entityBox.WirePriority;
                CellPriority = entityBox.CellPriority;
                BeaconPriority = entityBox.BeaconPriority;
                RegionPriority = entityBox.RegionPriority;
                AutoPriority = entityBox.AutoPriority;
            }

            public void Save()
            {
                savedEntityBox.ViasPriority = ViasPriority;
                savedEntityBox.WirePriority = WirePriority;
                savedEntityBox.CellPriority = CellPriority;
                savedEntityBox.BeaconPriority = BeaconPriority;
                savedEntityBox.RegionPriority = RegionPriority;
                savedEntityBox.AutoPriority = AutoPriority;
            }
        }


        internal class OpacitySettings
        {
            public int ViasOpacity { get; set; }
            public int WireOpacity { get; set; }
            public int CellOpacity { get; set; }

            private EntityBox savedEntityBox;

            public OpacitySettings(EntityBox entityBox)
            {
                savedEntityBox = entityBox;

                ViasOpacity = entityBox.ViasOpacity;
                WireOpacity = entityBox.WireOpacity;
                CellOpacity = entityBox.CellOpacity;
            }

            public void Save()
            {
                savedEntityBox.ViasOpacity = ViasOpacity;
                savedEntityBox.WireOpacity = WireOpacity;
                savedEntityBox.CellOpacity = CellOpacity;
            }
        }


        internal class SizeSettings
        {
            public int ViasBaseSize { get; set; }
            public int WireBaseSize { get; set; }

            private EntityBox savedEntityBox;

            public SizeSettings(EntityBox entityBox)
            {
                savedEntityBox = entityBox;

                ViasBaseSize = entityBox.ViasBaseSize;
                WireBaseSize = entityBox.WireBaseSize;
            }

            public void Save()
            {
                savedEntityBox.ViasBaseSize = ViasBaseSize;
                savedEntityBox.WireBaseSize = WireBaseSize;
            }
        }


        internal class ShapeSettings
        {
            public ViasShape ViasShape { get; set; }

            private EntityBox savedEntityBox;

            public ShapeSettings(EntityBox entityBox)
            {
                savedEntityBox = entityBox;

                ViasShape = entityBox.ViasShape;
            }

            public void Save()
            {
                savedEntityBox.ViasShape = ViasShape;
            }
        }


        public static void LoadSettings (EntityBox entityBox)
        {
            Properties.Settings settings = Properties.Settings.Default;

            /// Load global settings

            GlobalSettings global = new GlobalSettings(entityBox);

            global.SelectEntitiesAfterAdd = settings.SelectEntitiesAfterAdd;
            global.Grayscale = settings.Grayscale;
            global.Lambda = settings.Lambda;
            global.LockScroll0 = settings.LockScroll0;
            global.LockScroll1 = settings.LockScroll1;
            global.LockScroll2 = settings.LockScroll2;
            global.LockZoom0 = settings.LockZoom0;
            global.LockZoom1 = settings.LockZoom1;
            global.LockZoom2 = settings.LockZoom2;
            global.HideGrid = settings.HideGrid;
            global.HideLambdaMetrics = settings.HideLambdaMetrics;
            global.CellTextAlignment = (TextAlignment)settings.CellTextAlignment;
            global.ViasTextAlignment = (TextAlignment)settings.ViasTextAlignment;
            global.WireTextAlignment = (TextAlignment)settings.WireTextAlignment;

            global.Save();

            /// Load color settings
            /// 

            ColorSettings color = new ColorSettings(entityBox);

            color.SelectionBoxColor = settings.SelectionBoxColor;
            color.ViasInputColor = settings.ViasInputColor;
            color.ViasOutputColor = settings.ViasOutputColor;
            color.ViasInoutColor = settings.ViasInoutColor;
            color.ViasConnectColor = settings.ViasConnectColor;
            color.ViasFloatingColor = settings.ViasFloatingColor;
            color.ViasPowerColor = settings.ViasPowerColor;
            color.ViasGroundColor = settings.ViasGroundColor;
            color.WireInterconnectColor = settings.WireInterconnectColor;
            color.WirePowerColor = settings.WirePowerColor;
            color.WireGroundColor = settings.WireGroundColor;
            color.CellNotColor = settings.CellNotColor;
            color.CellBufferColor = settings.CellBufferColor;
            color.CellMuxColor = settings.CellMuxColor;
            color.CellLogicColor = settings.CellLogicColor;
            color.CellAdderColor = settings.CellAdderColor;
            color.CellBusSuppColor = settings.CellBusSuppColor;
            color.CellFlipFlopColor = settings.CellFlipFlopColor;
            color.CellLatchColor = settings.CellLatchColor;
            color.CellOtherColor = settings.CellOtherColor;
            color.UnitRegfileColor = settings.UnitRegfileColor;
            color.UnitMemoryColor = settings.UnitMemoryColor;
            color.UnitCustomColor = settings.UnitCustomColor;
            color.SelectionColor = settings.SelectionColor;
            color.ViasOverrideColor = settings.ViasOverrideColor;
            color.WireOverrideColor = settings.WireOverrideColor;
            color.CellOverrideColor = settings.CellOverrideColor;
            color.RegionOverrideColor = settings.RegionOverrideColor;

            color.Save();

            /// Load priority settings
            /// 

            PrioritySettings priority = new PrioritySettings(entityBox);

            priority.ViasPriority = settings.ViasPriority;
            priority.WirePriority = settings.WirePriority;
            priority.CellPriority = settings.CellPriority;
            priority.BeaconPriority = settings.BeaconPriority;
            priority.RegionPriority = settings.RegionPriority;
            priority.AutoPriority = settings.AutoPriority;

            priority.Save();

            /// Load opacity settings
            /// 

            OpacitySettings opacity = new OpacitySettings(entityBox);

            opacity.ViasOpacity = settings.ViasOpacity;
            opacity.WireOpacity = settings.WireOpacity;
            opacity.CellOpacity = settings.CellOpacity;

            opacity.Save();

            /// Load shape settings
            /// 

            ShapeSettings shape = new ShapeSettings(entityBox);

            shape.ViasShape = (ViasShape)settings.ViasShape;

            shape.Save();

            entityBox.Invalidate();
        }

        public static void SaveSettings (EntityBox entityBox)
        {
            Properties.Settings settings = Properties.Settings.Default;

            /// Save global settings

            GlobalSettings global = new GlobalSettings(entityBox);

            settings.SelectEntitiesAfterAdd = global.SelectEntitiesAfterAdd;
            settings.Grayscale = global.Grayscale;
            settings.Lambda = global.Lambda;
            settings.LockScroll0 = global.LockScroll0;
            settings.LockScroll1 = global.LockScroll1;
            settings.LockScroll2 = global.LockScroll2;
            settings.LockZoom0 = global.LockZoom0;
            settings.LockZoom1 = global.LockZoom1;
            settings.LockZoom2 = global.LockZoom2;
            settings.HideGrid = global.HideGrid;
            settings.HideLambdaMetrics = global.HideLambdaMetrics;
            settings.CellTextAlignment = (int)global.CellTextAlignment;
            settings.ViasTextAlignment = (int)global.ViasTextAlignment;
            settings.WireTextAlignment = (int)global.WireTextAlignment;

            /// Save color settings
            /// 

            ColorSettings color = new ColorSettings(entityBox);

            settings.SelectionBoxColor = color.SelectionBoxColor;
            settings.ViasInputColor = color.ViasInputColor;
            settings.ViasOutputColor = color.ViasOutputColor;
            settings.ViasInoutColor = color.ViasInoutColor;
            settings.ViasConnectColor = color.ViasConnectColor;
            settings.ViasFloatingColor = color.ViasFloatingColor;
            settings.ViasPowerColor = color.ViasPowerColor;
            settings.ViasGroundColor = color.ViasGroundColor;
            settings.WireInterconnectColor = color.WireInterconnectColor;
            settings.WirePowerColor = color.WirePowerColor;
            settings.WireGroundColor = color.WireGroundColor;
            settings.CellNotColor = color.CellNotColor;
            settings.CellBufferColor = color.CellBufferColor;
            settings.CellMuxColor = color.CellMuxColor;
            settings.CellLogicColor = color.CellLogicColor;
            settings.CellAdderColor = color.CellAdderColor;
            settings.CellBusSuppColor = color.CellBusSuppColor;
            settings.CellFlipFlopColor = color.CellFlipFlopColor;
            settings.CellLatchColor = color.CellLatchColor;
            settings.CellOtherColor = color.CellOtherColor;
            settings.UnitRegfileColor = color.UnitRegfileColor;
            settings.UnitMemoryColor = color.UnitMemoryColor;
            settings.UnitCustomColor = color.UnitCustomColor;
            settings.SelectionColor = color.SelectionColor;
            settings.ViasOverrideColor = color.ViasOverrideColor;
            settings.WireOverrideColor = color.WireOverrideColor;
            settings.CellOverrideColor = color.CellOverrideColor;
            settings.RegionOverrideColor = color.RegionOverrideColor;

            /// Save priority settings
            /// 

            PrioritySettings priority = new PrioritySettings(entityBox);

            settings.ViasPriority = priority.ViasPriority;
            settings.WirePriority = priority.WirePriority;
            settings.CellPriority = priority.CellPriority;
            settings.BeaconPriority = priority.BeaconPriority;
            settings.RegionPriority = priority.RegionPriority;
            settings.AutoPriority = priority.AutoPriority;

            /// Save opacity settings
            /// 

            OpacitySettings opacity = new OpacitySettings(entityBox);

            settings.ViasOpacity = opacity.ViasOpacity;
            settings.WireOpacity = opacity.WireOpacity;
            settings.CellOpacity = opacity.CellOpacity;

            /// Save shape settings
            /// 

            ShapeSettings shape = new ShapeSettings(entityBox);

            settings.ViasShape = (int)shape.ViasShape;

            settings.Save();
        }


    }
}
