namespace DerouteSharp
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
            this.loadImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSceneAsImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.loadEntitiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveEntitiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteAllEntitiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.entityBox1 = new System.Windows.Forms.EntityBox();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.propertyGrid2 = new System.Windows.Forms.PropertyGrid();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog2 = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1106, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadImageToolStripMenuItem,
            this.saveSceneAsImageToolStripMenuItem,
            this.toolStripSeparator1,
            this.loadEntitiesToolStripMenuItem,
            this.saveEntitiesToolStripMenuItem,
            this.toolStripSeparator2,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadImageToolStripMenuItem
            // 
            this.loadImageToolStripMenuItem.Name = "loadImageToolStripMenuItem";
            this.loadImageToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.loadImageToolStripMenuItem.Text = "Load Image...";
            this.loadImageToolStripMenuItem.Click += new System.EventHandler(this.loadImageToolStripMenuItem_Click);
            // 
            // saveSceneAsImageToolStripMenuItem
            // 
            this.saveSceneAsImageToolStripMenuItem.Name = "saveSceneAsImageToolStripMenuItem";
            this.saveSceneAsImageToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.saveSceneAsImageToolStripMenuItem.Text = "Save scene as Image...";
            this.saveSceneAsImageToolStripMenuItem.Click += new System.EventHandler(this.saveSceneAsImageToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(187, 6);
            // 
            // loadEntitiesToolStripMenuItem
            // 
            this.loadEntitiesToolStripMenuItem.Name = "loadEntitiesToolStripMenuItem";
            this.loadEntitiesToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.loadEntitiesToolStripMenuItem.Text = "Add entities...";
            this.loadEntitiesToolStripMenuItem.Click += new System.EventHandler(this.loadEntitiesToolStripMenuItem_Click);
            // 
            // saveEntitiesToolStripMenuItem
            // 
            this.saveEntitiesToolStripMenuItem.Name = "saveEntitiesToolStripMenuItem";
            this.saveEntitiesToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.saveEntitiesToolStripMenuItem.Text = "Save entities...";
            this.saveEntitiesToolStripMenuItem.Click += new System.EventHandler(this.saveEntitiesToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(187, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.deleteAllEntitiesToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // deleteAllEntitiesToolStripMenuItem
            // 
            this.deleteAllEntitiesToolStripMenuItem.Name = "deleteAllEntitiesToolStripMenuItem";
            this.deleteAllEntitiesToolStripMenuItem.Size = new System.Drawing.Size(163, 22);
            this.deleteAllEntitiesToolStripMenuItem.Text = "Delete all entities";
            this.deleteAllEntitiesToolStripMenuItem.Click += new System.EventHandler(this.deleteAllEntitiesToolStripMenuItem_Click);
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
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.propertyGrid2);
            this.splitContainer1.Panel2.Controls.Add(this.propertyGrid1);
            this.splitContainer1.Size = new System.Drawing.Size(1106, 580);
            this.splitContainer1.SplitterDistance = 538;
            this.splitContainer1.TabIndex = 1;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.entityBox1);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.button3);
            this.splitContainer2.Panel2.Controls.Add(this.button2);
            this.splitContainer2.Panel2.Controls.Add(this.button1);
            this.splitContainer2.Size = new System.Drawing.Size(538, 580);
            this.splitContainer2.SplitterDistance = 454;
            this.splitContainer2.TabIndex = 0;
            // 
            // entityBox1
            // 
            this.entityBox1.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.entityBox1.CellAdderColor = System.Drawing.Color.Empty;
            this.entityBox1.CellBufferColor = System.Drawing.Color.Empty;
            this.entityBox1.CellBusSuppColor = System.Drawing.Color.Empty;
            this.entityBox1.CellFlipFlopColor = System.Drawing.Color.Empty;
            this.entityBox1.CellLatchColor = System.Drawing.Color.Empty;
            this.entityBox1.CellLogicColor = System.Drawing.Color.Empty;
            this.entityBox1.CellMuxColor = System.Drawing.Color.Empty;
            this.entityBox1.CellNotColor = System.Drawing.Color.Empty;
            this.entityBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.entityBox1.HideImage = false;
            this.entityBox1.Lambda = 5F;
            this.entityBox1.Location = new System.Drawing.Point(0, 0);
            this.entityBox1.Mode = EntityType.Selection;
            this.entityBox1.Name = "entityBox1";
            this.entityBox1.ScrollX = 0;
            this.entityBox1.ScrollY = 0;
            this.entityBox1.SelectionColor = System.Drawing.Color.LimeGreen;
            this.entityBox1.Size = new System.Drawing.Size(538, 454);
            this.entityBox1.TabIndex = 0;
            this.entityBox1.Text = "entityBox1";
            this.entityBox1.UnitCustomColor = System.Drawing.Color.Empty;
            this.entityBox1.UnitMemoryColor = System.Drawing.Color.Empty;
            this.entityBox1.UnitRegfileColor = System.Drawing.Color.Empty;
            this.entityBox1.ViasBaseSize = 4;
            this.entityBox1.ViasConnectColor = System.Drawing.Color.Black;
            this.entityBox1.ViasFloatingColor = System.Drawing.Color.Gray;
            this.entityBox1.ViasGroundColor = System.Drawing.Color.Black;
            this.entityBox1.ViasInoutColor = System.Drawing.Color.Yellow;
            this.entityBox1.ViasInputColor = System.Drawing.Color.Green;
            this.entityBox1.ViasOutputColor = System.Drawing.Color.Red;
            this.entityBox1.ViasPowerColor = System.Drawing.Color.Black;
            this.entityBox1.ViasShape = ViasShape.Round;
            this.entityBox1.WireBaseSize = 5;
            this.entityBox1.WireGroundColor = System.Drawing.Color.Green;
            this.entityBox1.WireInterconnectColor = System.Drawing.Color.Blue;
            this.entityBox1.WirePowerColor = System.Drawing.Color.Red;
            this.entityBox1.Zoom = 100;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(268, 24);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(102, 44);
            this.button3.TabIndex = 2;
            this.button3.Text = "Wire";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(144, 24);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(102, 44);
            this.button2.TabIndex = 1;
            this.button2.Text = "Vias";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(20, 24);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(102, 44);
            this.button1.TabIndex = 0;
            this.button1.Text = "Selection";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // propertyGrid2
            // 
            this.propertyGrid2.Location = new System.Drawing.Point(305, 4);
            this.propertyGrid2.Name = "propertyGrid2";
            this.propertyGrid2.Size = new System.Drawing.Size(256, 345);
            this.propertyGrid2.TabIndex = 1;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Location = new System.Drawing.Point(17, 3);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.SelectedObject = this.entityBox1;
            this.propertyGrid1.Size = new System.Drawing.Size(281, 523);
            this.propertyGrid1.TabIndex = 0;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.DefaultExt = "jpg";
            this.openFileDialog1.Filter = "JPEG Images|*.jpg|PNG Images|*.png|All files|*.*";
            this.openFileDialog1.Title = "Load Image";
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "jpg";
            this.saveFileDialog1.Filter = "JPEG Images|*.jpg|PNG Images|*.png|All files|*.*";
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.DefaultExt = "xml";
            this.openFileDialog2.Filter = "XML Files|*.xml|All Files|*.*";
            // 
            // saveFileDialog2
            // 
            this.saveFileDialog2.DefaultExt = "xml";
            this.saveFileDialog2.Filter = "XML Files|*.xml|All Files|*.*";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1106, 604);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.Name = "Form1";
            this.Text = "Deroute";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.EntityBox entityBox1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.PropertyGrid propertyGrid2;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteAllEntitiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem loadEntitiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveEntitiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ToolStripMenuItem saveSceneAsImageToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog2;
        private System.Windows.Forms.SaveFileDialog saveFileDialog2;
    }
}

