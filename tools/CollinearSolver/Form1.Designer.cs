namespace CollinearSolver
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadEntitiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveEntitiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.solveSingleSegmentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.solveAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.entityBox1 = new System.Windows.Forms.EntityBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(688, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadEntitiesToolStripMenuItem,
            this.saveEntitiesToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadEntitiesToolStripMenuItem
            // 
            this.loadEntitiesToolStripMenuItem.Name = "loadEntitiesToolStripMenuItem";
            this.loadEntitiesToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.loadEntitiesToolStripMenuItem.Text = "Add entities...";
            this.loadEntitiesToolStripMenuItem.Click += new System.EventHandler(this.loadEntitiesToolStripMenuItem_Click);
            // 
            // saveEntitiesToolStripMenuItem
            // 
            this.saveEntitiesToolStripMenuItem.Name = "saveEntitiesToolStripMenuItem";
            this.saveEntitiesToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.saveEntitiesToolStripMenuItem.Text = "Save entities...";
            this.saveEntitiesToolStripMenuItem.Click += new System.EventHandler(this.saveEntitiesToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(145, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.solveSingleSegmentToolStripMenuItem,
            this.solveAllToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // solveSingleSegmentToolStripMenuItem
            // 
            this.solveSingleSegmentToolStripMenuItem.Name = "solveSingleSegmentToolStripMenuItem";
            this.solveSingleSegmentToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
            this.solveSingleSegmentToolStripMenuItem.Text = "Solve single segment (Space)";
            this.solveSingleSegmentToolStripMenuItem.Click += new System.EventHandler(this.solveSingleSegmentToolStripMenuItem_Click);
            // 
            // solveAllToolStripMenuItem
            // 
            this.solveAllToolStripMenuItem.Name = "solveAllToolStripMenuItem";
            this.solveAllToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
            this.solveAllToolStripMenuItem.Text = "Solve all";
            this.solveAllToolStripMenuItem.Click += new System.EventHandler(this.solveAllToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
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
            this.entityBox1.Location = new System.Drawing.Point(0, 24);
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
            this.entityBox1.Size = new System.Drawing.Size(688, 370);
            this.entityBox1.TabIndex = 1;
            this.entityBox1.Text = "entityBox1";
            this.entityBox1.UnitCustomColor = System.Drawing.Color.Snow;
            this.entityBox1.UnitMemoryColor = System.Drawing.Color.Snow;
            this.entityBox1.UnitRegfileColor = System.Drawing.Color.Snow;
            this.entityBox1.ViasBaseSize = 2;
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
            this.entityBox1.WireBaseSize = 4;
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
            // openFileDialog1
            // 
            this.openFileDialog1.DefaultExt = "xml";
            this.openFileDialog1.Filter = "XML files|*.xml|All files|*.*";
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "xml";
            this.saveFileDialog1.Filter = "XML files|*.xml|All files|*.*";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(688, 394);
            this.Controls.Add(this.entityBox1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Collinear Solver";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadEntitiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveEntitiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.EntityBox entityBox1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem solveSingleSegmentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem solveAllToolStripMenuItem;
    }
}

