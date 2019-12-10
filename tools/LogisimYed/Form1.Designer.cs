namespace LogisimYed
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
            this.loadLogisimToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveYEdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.controlToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.convertToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.canvasControl1 = new CanvasControl.CanvasControl();
            this.reduceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.flipWiretSpaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.controlToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(682, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadLogisimToolStripMenuItem,
            this.saveYEdToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadLogisimToolStripMenuItem
            // 
            this.loadLogisimToolStripMenuItem.Name = "loadLogisimToolStripMenuItem";
            this.loadLogisimToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.loadLogisimToolStripMenuItem.Text = "Load Logisim...";
            this.loadLogisimToolStripMenuItem.Click += new System.EventHandler(this.loadLogisimToolStripMenuItem_Click);
            // 
            // saveYEdToolStripMenuItem
            // 
            this.saveYEdToolStripMenuItem.Name = "saveYEdToolStripMenuItem";
            this.saveYEdToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.saveYEdToolStripMenuItem.Text = "Save yEd...";
            this.saveYEdToolStripMenuItem.Click += new System.EventHandler(this.saveYEdToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(151, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // controlToolStripMenuItem
            // 
            this.controlToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.flipWiretSpaceToolStripMenuItem,
            this.reduceToolStripMenuItem,
            this.convertToolStripMenuItem});
            this.controlToolStripMenuItem.Name = "controlToolStripMenuItem";
            this.controlToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.controlToolStripMenuItem.Text = "Control";
            // 
            // convertToolStripMenuItem
            // 
            this.convertToolStripMenuItem.Name = "convertToolStripMenuItem";
            this.convertToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.convertToolStripMenuItem.Text = "Convert";
            this.convertToolStripMenuItem.Click += new System.EventHandler(this.convertToolStripMenuItem_Click);
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
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.DefaultExt = "circ";
            this.openFileDialog1.Filter = "Logisim Circuits|*.circ|All files|*.*";
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "graphml";
            this.saveFileDialog1.Filter = "yEd Diagrams|*.graphml|All files|*.*";
            // 
            // canvasControl1
            // 
            this.canvasControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.canvasControl1.GridColor = System.Drawing.Color.Black;
            this.canvasControl1.GridSize = 20;
            this.canvasControl1.Location = new System.Drawing.Point(0, 24);
            this.canvasControl1.Name = "canvasControl1";
            this.canvasControl1.OriginColor = System.Drawing.Color.Red;
            this.canvasControl1.ShowGrid = true;
            this.canvasControl1.ShowOrigin = true;
            this.canvasControl1.Size = new System.Drawing.Size(682, 319);
            this.canvasControl1.TabIndex = 1;
            this.canvasControl1.Text = "canvasControl1";
            this.canvasControl1.Zoom = 100;
            // 
            // reduceToolStripMenuItem
            // 
            this.reduceToolStripMenuItem.Name = "reduceToolStripMenuItem";
            this.reduceToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.reduceToolStripMenuItem.Text = "Reduce";
            this.reduceToolStripMenuItem.Click += new System.EventHandler(this.reduceToolStripMenuItem_Click);
            // 
            // flipWiretSpaceToolStripMenuItem
            // 
            this.flipWiretSpaceToolStripMenuItem.Name = "flipWiretSpaceToolStripMenuItem";
            this.flipWiretSpaceToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.flipWiretSpaceToolStripMenuItem.Text = "Flip Wire (Space)";
            this.flipWiretSpaceToolStripMenuItem.Click += new System.EventHandler(this.flipWiretSpaceToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(682, 343);
            this.Controls.Add(this.canvasControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Logisim yEd Converter";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadLogisimToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveYEdToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem controlToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem convertToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private CanvasControl.CanvasControl canvasControl1;
        private System.Windows.Forms.ToolStripMenuItem reduceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem flipWiretSpaceToolStripMenuItem;
    }
}

