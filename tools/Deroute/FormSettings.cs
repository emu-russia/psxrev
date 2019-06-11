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
        private ColorSettings colorSettings;

        public FormSettings(EntityBox entityBox)
        {
            InitializeComponent();

            propertyGridEntityBox.SelectedObject = entityBox;

            colorSettings = new ColorSettings(entityBox);
            propertyGridColors.SelectedObject = colorSettings;
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
            colorSettings.Save();

            DialogResult = DialogResult.OK;
            Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }


        internal class ColorSettings
        {
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

    }
}
