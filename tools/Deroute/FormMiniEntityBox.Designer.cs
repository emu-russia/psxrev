
namespace DerouteSharp
{
    partial class FormMiniEntityBox
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMiniEntityBox));
            this.entityBox1 = new System.Windows.Forms.EntityBox();
            this.SuspendLayout();
            // 
            // entityBox1
            // 
            this.entityBox1.AutoPriority = true;
            this.entityBox1.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.entityBox1.BeaconPriority = 4;
            this.entityBox1.CellAdderColor = System.Drawing.Color.Red;
            this.entityBox1.CellBufferColor = System.Drawing.Color.Navy;
            this.entityBox1.CellBusSuppColor = System.Drawing.Color.DarkViolet;
            this.entityBox1.CellFlipFlopColor = System.Drawing.Color.Lime;
            this.entityBox1.CellLatchColor = System.Drawing.Color.Cyan;
            this.entityBox1.CellLogicColor = System.Drawing.Color.Yellow;
            this.entityBox1.CellMuxColor = System.Drawing.Color.DarkOrange;
            this.entityBox1.CellNotColor = System.Drawing.Color.Navy;
            this.entityBox1.CellOpacity = 128;
            this.entityBox1.CellOtherColor = System.Drawing.Color.Snow;
            this.entityBox1.CellOverrideColor = System.Drawing.Color.Black;
            this.entityBox1.CellPriority = 1;
            this.entityBox1.CellTextAlignment = TextAlignment.TopLeft;
            this.entityBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.entityBox1.ForeColor = System.Drawing.Color.Snow;
            this.entityBox1.Grayscale = false;
            this.entityBox1.HideCells = false;
            this.entityBox1.HideGrid = false;
            this.entityBox1.HideImage = false;
            this.entityBox1.HideLambdaMetrics = false;
            this.entityBox1.HideRegions = false;
            this.entityBox1.HideVias = false;
            this.entityBox1.HideWires = false;
            this.entityBox1.ImageOpacity0 = 100;
            this.entityBox1.ImageOpacity1 = 100;
            this.entityBox1.ImageOpacity2 = 100;
            this.entityBox1.Lambda = 5F;
            this.entityBox1.Location = new System.Drawing.Point(0, 0);
            this.entityBox1.LockScroll0 = false;
            this.entityBox1.LockScroll1 = false;
            this.entityBox1.LockScroll2 = false;
            this.entityBox1.LockZoom0 = false;
            this.entityBox1.LockZoom1 = false;
            this.entityBox1.LockZoom2 = false;
            this.entityBox1.Mode = System.Windows.Forms.EntityMode.Selection;
            this.entityBox1.Name = "entityBox1";
            this.entityBox1.RegionOpacity = 128;
            this.entityBox1.RegionOverrideColor = System.Drawing.Color.Black;
            this.entityBox1.RegionPriority = 0;
            this.entityBox1.ScrollImage0 = ((System.Drawing.PointF)(resources.GetObject("entityBox1.ScrollImage0")));
            this.entityBox1.ScrollImage1 = ((System.Drawing.PointF)(resources.GetObject("entityBox1.ScrollImage1")));
            this.entityBox1.ScrollImage2 = ((System.Drawing.PointF)(resources.GetObject("entityBox1.ScrollImage2")));
            this.entityBox1.ScrollX = 0F;
            this.entityBox1.ScrollY = 0F;
            this.entityBox1.SelectEntitiesAfterAdd = true;
            this.entityBox1.SelectionBoxColor = System.Drawing.Color.Red;
            this.entityBox1.SelectionColor = System.Drawing.Color.LimeGreen;
            this.entityBox1.Size = new System.Drawing.Size(717, 347);
            this.entityBox1.TabIndex = 0;
            this.entityBox1.Text = "entityBox1";
            this.entityBox1.UnitCustomColor = System.Drawing.Color.Snow;
            this.entityBox1.UnitMemoryColor = System.Drawing.Color.Snow;
            this.entityBox1.UnitRegfileColor = System.Drawing.Color.Snow;
            this.entityBox1.ViasBaseSize = 4;
            this.entityBox1.ViasConnectColor = System.Drawing.Color.Black;
            this.entityBox1.ViasFloatingColor = System.Drawing.Color.Gray;
            this.entityBox1.ViasGroundColor = System.Drawing.Color.Black;
            this.entityBox1.ViasInoutColor = System.Drawing.Color.Yellow;
            this.entityBox1.ViasInputColor = System.Drawing.Color.Green;
            this.entityBox1.ViasOpacity = 255;
            this.entityBox1.ViasOutputColor = System.Drawing.Color.Red;
            this.entityBox1.ViasOverrideColor = System.Drawing.Color.Black;
            this.entityBox1.ViasPowerColor = System.Drawing.Color.Black;
            this.entityBox1.ViasPriority = 3;
            this.entityBox1.ViasShape = ViasShape.Round;
            this.entityBox1.ViasTextAlignment = TextAlignment.Top;
            this.entityBox1.WireBaseSize = 3;
            this.entityBox1.WireGroundColor = System.Drawing.Color.Green;
            this.entityBox1.WireInterconnectColor = System.Drawing.Color.Blue;
            this.entityBox1.WireOpacity = 128;
            this.entityBox1.WireOverrideColor = System.Drawing.Color.Black;
            this.entityBox1.WirePowerColor = System.Drawing.Color.Red;
            this.entityBox1.WirePriority = 2;
            this.entityBox1.WireTextAlignment = TextAlignment.TopLeft;
            this.entityBox1.Zoom = 100;
            this.entityBox1.ZoomImage0 = 100;
            this.entityBox1.ZoomImage1 = 100;
            this.entityBox1.ZoomImage2 = 100;
            // 
            // FormMiniEntityBox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(717, 347);
            this.Controls.Add(this.entityBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.Name = "FormMiniEntityBox";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Mini EntityBox (F1 - Select, F2 - Vias, F3 - Wire)";
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.FormMiniEntityBox_KeyUp);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.EntityBox entityBox1;
    }
}