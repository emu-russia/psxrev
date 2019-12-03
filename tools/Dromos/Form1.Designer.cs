namespace Dromos
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addPointToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.add1000RandomPointsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addRectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.add100RectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.addLineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.add100LinesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addPolyLineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.add25PolyLinesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addRegionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.add100RegionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.zoom10ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.zoom10ToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.panX10ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panX10ToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.panY10ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panY10ToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.canvasControl1 = new CanvasControl.CanvasControl();
            this.homeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.debugToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(770, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(92, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // debugToolStripMenuItem
            // 
            this.debugToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addPointToolStripMenuItem,
            this.add1000RandomPointsToolStripMenuItem,
            this.addRectToolStripMenuItem,
            this.add100RectToolStripMenuItem,
            this.addLineToolStripMenuItem,
            this.add100LinesToolStripMenuItem,
            this.addPolyLineToolStripMenuItem,
            this.add25PolyLinesToolStripMenuItem,
            this.addRegionToolStripMenuItem,
            this.add100RegionsToolStripMenuItem,
            this.toolStripSeparator1,
            this.removeAllToolStripMenuItem});
            this.debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            this.debugToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
            this.debugToolStripMenuItem.Text = "Debug";
            // 
            // addPointToolStripMenuItem
            // 
            this.addPointToolStripMenuItem.Name = "addPointToolStripMenuItem";
            this.addPointToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.addPointToolStripMenuItem.Text = "Add Point";
            this.addPointToolStripMenuItem.Click += new System.EventHandler(this.addPointToolStripMenuItem_Click);
            // 
            // add1000RandomPointsToolStripMenuItem
            // 
            this.add1000RandomPointsToolStripMenuItem.Name = "add1000RandomPointsToolStripMenuItem";
            this.add1000RandomPointsToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.add1000RandomPointsToolStripMenuItem.Text = "Add 1000 Random Points";
            this.add1000RandomPointsToolStripMenuItem.Click += new System.EventHandler(this.add1000RandomPointsToolStripMenuItem_Click);
            // 
            // removeAllToolStripMenuItem
            // 
            this.removeAllToolStripMenuItem.Name = "removeAllToolStripMenuItem";
            this.removeAllToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.removeAllToolStripMenuItem.Text = "Remove All";
            this.removeAllToolStripMenuItem.Click += new System.EventHandler(this.removeAllToolStripMenuItem_Click);
            // 
            // addRectToolStripMenuItem
            // 
            this.addRectToolStripMenuItem.Name = "addRectToolStripMenuItem";
            this.addRectToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.addRectToolStripMenuItem.Text = "Add Rect";
            this.addRectToolStripMenuItem.Click += new System.EventHandler(this.addRectToolStripMenuItem_Click);
            // 
            // add100RectToolStripMenuItem
            // 
            this.add100RectToolStripMenuItem.Name = "add100RectToolStripMenuItem";
            this.add100RectToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.add100RectToolStripMenuItem.Text = "Add 100 Rect";
            this.add100RectToolStripMenuItem.Click += new System.EventHandler(this.add100RectToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(204, 6);
            // 
            // addLineToolStripMenuItem
            // 
            this.addLineToolStripMenuItem.Name = "addLineToolStripMenuItem";
            this.addLineToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.addLineToolStripMenuItem.Text = "Add Line";
            this.addLineToolStripMenuItem.Click += new System.EventHandler(this.addLineToolStripMenuItem_Click);
            // 
            // add100LinesToolStripMenuItem
            // 
            this.add100LinesToolStripMenuItem.Name = "add100LinesToolStripMenuItem";
            this.add100LinesToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.add100LinesToolStripMenuItem.Text = "Add 100 Lines";
            this.add100LinesToolStripMenuItem.Click += new System.EventHandler(this.add100LinesToolStripMenuItem_Click);
            // 
            // addPolyLineToolStripMenuItem
            // 
            this.addPolyLineToolStripMenuItem.Name = "addPolyLineToolStripMenuItem";
            this.addPolyLineToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.addPolyLineToolStripMenuItem.Text = "Add PolyLine";
            this.addPolyLineToolStripMenuItem.Click += new System.EventHandler(this.addPolyLineToolStripMenuItem_Click);
            // 
            // add25PolyLinesToolStripMenuItem
            // 
            this.add25PolyLinesToolStripMenuItem.Name = "add25PolyLinesToolStripMenuItem";
            this.add25PolyLinesToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.add25PolyLinesToolStripMenuItem.Text = "Add 25 PolyLines";
            this.add25PolyLinesToolStripMenuItem.Click += new System.EventHandler(this.add25PolyLinesToolStripMenuItem_Click);
            // 
            // addRegionToolStripMenuItem
            // 
            this.addRegionToolStripMenuItem.Name = "addRegionToolStripMenuItem";
            this.addRegionToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.addRegionToolStripMenuItem.Text = "Add Region";
            this.addRegionToolStripMenuItem.Click += new System.EventHandler(this.addRegionToolStripMenuItem_Click);
            // 
            // add100RegionsToolStripMenuItem
            // 
            this.add100RegionsToolStripMenuItem.Name = "add100RegionsToolStripMenuItem";
            this.add100RegionsToolStripMenuItem.Size = new System.Drawing.Size(207, 22);
            this.add100RegionsToolStripMenuItem.Text = "Add 100 Regions";
            this.add100RegionsToolStripMenuItem.Click += new System.EventHandler(this.add100RegionsToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.zoom10ToolStripMenuItem,
            this.zoom10ToolStripMenuItem1,
            this.panX10ToolStripMenuItem,
            this.panX10ToolStripMenuItem1,
            this.panY10ToolStripMenuItem,
            this.panY10ToolStripMenuItem1,
            this.homeToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // zoom10ToolStripMenuItem
            // 
            this.zoom10ToolStripMenuItem.Name = "zoom10ToolStripMenuItem";
            this.zoom10ToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.zoom10ToolStripMenuItem.Text = "Zoom +10";
            this.zoom10ToolStripMenuItem.Click += new System.EventHandler(this.zoom10ToolStripMenuItem_Click);
            // 
            // zoom10ToolStripMenuItem1
            // 
            this.zoom10ToolStripMenuItem1.Name = "zoom10ToolStripMenuItem1";
            this.zoom10ToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.zoom10ToolStripMenuItem1.Text = "Zoom -10";
            this.zoom10ToolStripMenuItem1.Click += new System.EventHandler(this.zoom10ToolStripMenuItem1_Click);
            // 
            // panX10ToolStripMenuItem
            // 
            this.panX10ToolStripMenuItem.Name = "panX10ToolStripMenuItem";
            this.panX10ToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.panX10ToolStripMenuItem.Text = "Pan X +10";
            this.panX10ToolStripMenuItem.Click += new System.EventHandler(this.panX10ToolStripMenuItem_Click);
            // 
            // panX10ToolStripMenuItem1
            // 
            this.panX10ToolStripMenuItem1.Name = "panX10ToolStripMenuItem1";
            this.panX10ToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.panX10ToolStripMenuItem1.Text = "Pan X -10";
            this.panX10ToolStripMenuItem1.Click += new System.EventHandler(this.panX10ToolStripMenuItem1_Click);
            // 
            // panY10ToolStripMenuItem
            // 
            this.panY10ToolStripMenuItem.Name = "panY10ToolStripMenuItem";
            this.panY10ToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.panY10ToolStripMenuItem.Text = "Pan Y +10";
            this.panY10ToolStripMenuItem.Click += new System.EventHandler(this.panY10ToolStripMenuItem_Click);
            // 
            // panY10ToolStripMenuItem1
            // 
            this.panY10ToolStripMenuItem1.Name = "panY10ToolStripMenuItem1";
            this.panY10ToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.panY10ToolStripMenuItem1.Text = "Pan Y -10";
            this.panY10ToolStripMenuItem1.Click += new System.EventHandler(this.panY10ToolStripMenuItem1_Click);
            // 
            // canvasControl1
            // 
            this.canvasControl1.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.canvasControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.canvasControl1.Location = new System.Drawing.Point(0, 24);
            this.canvasControl1.Name = "canvasControl1";
            this.canvasControl1.Size = new System.Drawing.Size(770, 376);
            this.canvasControl1.TabIndex = 0;
            this.canvasControl1.Zoom = 100;
            // 
            // homeToolStripMenuItem
            // 
            this.homeToolStripMenuItem.Name = "homeToolStripMenuItem";
            this.homeToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.homeToolStripMenuItem.Text = "Home";
            this.homeToolStripMenuItem.Click += new System.EventHandler(this.homeToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(770, 400);
            this.Controls.Add(this.canvasControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Canvas Demo";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CanvasControl.CanvasControl canvasControl1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addPointToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem add1000RandomPointsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addRectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem add100RectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem addLineToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem add100LinesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addPolyLineToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem add25PolyLinesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addRegionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem add100RegionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem zoom10ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem zoom10ToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem panX10ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem panX10ToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem panY10ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem panY10ToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem homeToolStripMenuItem;
    }
}

