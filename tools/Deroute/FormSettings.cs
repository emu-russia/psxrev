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

        public FormSettings(EntityBox entityBox)
        {
            InitializeComponent();

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


    }
}
