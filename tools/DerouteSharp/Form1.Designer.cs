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
            this.components = new System.ComponentModel.Container();
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
            this.propertyGrid2 = new System.Windows.Forms.PropertyGrid();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog2 = new System.Windows.Forms.SaveFileDialog();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.viasPowerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viasGroundToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viasInputToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viasOutputToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viasInoutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viasFloatingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripDropDownButton2 = new System.Windows.Forms.ToolStripDropDownButton();
            this.wireInterconnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.wirePowerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.wireGroundToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripDropDownButton3 = new System.Windows.Forms.ToolStripDropDownButton();
            this.cellNotToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellBufferToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellMuxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellLogicToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellAdderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellBusSupportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellFlipFlopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellLatchToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cellOtherToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.unitRegisterFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unitMemoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unitCustomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.entityBox1 = new System.Windows.Forms.EntityBox();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
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
            this.menuStrip1.Size = new System.Drawing.Size(1040, 24);
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
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer1.Location = new System.Drawing.Point(0, 66);
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
            this.splitContainer1.Size = new System.Drawing.Size(1040, 480);
            this.splitContainer1.SplitterDistance = 743;
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
            this.splitContainer2.Size = new System.Drawing.Size(743, 480);
            this.splitContainer2.SplitterDistance = 441;
            this.splitContainer2.TabIndex = 0;
            // 
            // propertyGrid2
            // 
            this.propertyGrid2.Dock = System.Windows.Forms.DockStyle.Top;
            this.propertyGrid2.Location = new System.Drawing.Point(0, 0);
            this.propertyGrid2.Name = "propertyGrid2";
            this.propertyGrid2.Size = new System.Drawing.Size(293, 321);
            this.propertyGrid2.TabIndex = 1;
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
            // toolStrip1
            // 
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripDropDownButton1,
            this.toolStripDropDownButton2,
            this.toolStripDropDownButton3,
            this.toolStripButton2,
            this.toolStripButton3});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1040, 39);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.BackColor = System.Drawing.SystemColors.Control;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(91, 36);
            this.toolStripButton1.Text = "Selection";
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1,
            this.viasPowerToolStripMenuItem,
            this.viasGroundToolStripMenuItem,
            this.viasInputToolStripMenuItem,
            this.viasOutputToolStripMenuItem,
            this.viasInoutToolStripMenuItem,
            this.viasFloatingToolStripMenuItem});
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(45, 36);
            this.toolStripDropDownButton1.Text = "Vias";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripMenuItem1.Image")));
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(169, 22);
            this.toolStripMenuItem1.Text = "Vias Connection";
            this.toolStripMenuItem1.Click += new System.EventHandler(this.toolStripMenuItem1_Click);
            // 
            // viasPowerToolStripMenuItem
            // 
            this.viasPowerToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viasPowerToolStripMenuItem.Image")));
            this.viasPowerToolStripMenuItem.Name = "viasPowerToolStripMenuItem";
            this.viasPowerToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.viasPowerToolStripMenuItem.Text = "Vias Power";
            this.viasPowerToolStripMenuItem.Click += new System.EventHandler(this.viasPowerToolStripMenuItem_Click);
            // 
            // viasGroundToolStripMenuItem
            // 
            this.viasGroundToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viasGroundToolStripMenuItem.Image")));
            this.viasGroundToolStripMenuItem.Name = "viasGroundToolStripMenuItem";
            this.viasGroundToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.viasGroundToolStripMenuItem.Text = "Vias Ground";
            this.viasGroundToolStripMenuItem.Click += new System.EventHandler(this.viasGroundToolStripMenuItem_Click);
            // 
            // viasInputToolStripMenuItem
            // 
            this.viasInputToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viasInputToolStripMenuItem.Image")));
            this.viasInputToolStripMenuItem.Name = "viasInputToolStripMenuItem";
            this.viasInputToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.viasInputToolStripMenuItem.Text = "Vias Input";
            this.viasInputToolStripMenuItem.Click += new System.EventHandler(this.viasInputToolStripMenuItem_Click);
            // 
            // viasOutputToolStripMenuItem
            // 
            this.viasOutputToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viasOutputToolStripMenuItem.Image")));
            this.viasOutputToolStripMenuItem.Name = "viasOutputToolStripMenuItem";
            this.viasOutputToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.viasOutputToolStripMenuItem.Text = "Vias Output";
            this.viasOutputToolStripMenuItem.Click += new System.EventHandler(this.viasOutputToolStripMenuItem_Click);
            // 
            // viasInoutToolStripMenuItem
            // 
            this.viasInoutToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viasInoutToolStripMenuItem.Image")));
            this.viasInoutToolStripMenuItem.Name = "viasInoutToolStripMenuItem";
            this.viasInoutToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.viasInoutToolStripMenuItem.Text = "Vias Input/Output";
            this.viasInoutToolStripMenuItem.Click += new System.EventHandler(this.viasInoutToolStripMenuItem_Click);
            // 
            // viasFloatingToolStripMenuItem
            // 
            this.viasFloatingToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("viasFloatingToolStripMenuItem.Image")));
            this.viasFloatingToolStripMenuItem.Name = "viasFloatingToolStripMenuItem";
            this.viasFloatingToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.viasFloatingToolStripMenuItem.Text = "Vias Floating";
            this.viasFloatingToolStripMenuItem.Click += new System.EventHandler(this.viasFloatingToolStripMenuItem_Click);
            // 
            // toolStripDropDownButton2
            // 
            this.toolStripDropDownButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.wireInterconnectToolStripMenuItem,
            this.wirePowerToolStripMenuItem,
            this.wireGroundToolStripMenuItem});
            this.toolStripDropDownButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton2.Image")));
            this.toolStripDropDownButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton2.Name = "toolStripDropDownButton2";
            this.toolStripDropDownButton2.Size = new System.Drawing.Size(45, 36);
            this.toolStripDropDownButton2.Text = "Wires";
            // 
            // wireInterconnectToolStripMenuItem
            // 
            this.wireInterconnectToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("wireInterconnectToolStripMenuItem.Image")));
            this.wireInterconnectToolStripMenuItem.Name = "wireInterconnectToolStripMenuItem";
            this.wireInterconnectToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.wireInterconnectToolStripMenuItem.Text = "Wire Interconnect";
            this.wireInterconnectToolStripMenuItem.Click += new System.EventHandler(this.wireInterconnectToolStripMenuItem_Click);
            // 
            // wirePowerToolStripMenuItem
            // 
            this.wirePowerToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("wirePowerToolStripMenuItem.Image")));
            this.wirePowerToolStripMenuItem.Name = "wirePowerToolStripMenuItem";
            this.wirePowerToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.wirePowerToolStripMenuItem.Text = "Wire Power";
            this.wirePowerToolStripMenuItem.Click += new System.EventHandler(this.wirePowerToolStripMenuItem_Click);
            // 
            // wireGroundToolStripMenuItem
            // 
            this.wireGroundToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("wireGroundToolStripMenuItem.Image")));
            this.wireGroundToolStripMenuItem.Name = "wireGroundToolStripMenuItem";
            this.wireGroundToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.wireGroundToolStripMenuItem.Text = "Wire Ground";
            this.wireGroundToolStripMenuItem.Click += new System.EventHandler(this.wireGroundToolStripMenuItem_Click);
            // 
            // toolStripDropDownButton3
            // 
            this.toolStripDropDownButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton3.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cellNotToolStripMenuItem,
            this.cellBufferToolStripMenuItem,
            this.cellMuxToolStripMenuItem,
            this.cellLogicToolStripMenuItem,
            this.cellAdderToolStripMenuItem,
            this.cellBusSupportToolStripMenuItem,
            this.cellFlipFlopToolStripMenuItem,
            this.cellLatchToolStripMenuItem,
            this.cellOtherToolStripMenuItem,
            this.toolStripSeparator3,
            this.unitRegisterFileToolStripMenuItem,
            this.unitMemoryToolStripMenuItem,
            this.unitCustomToolStripMenuItem});
            this.toolStripDropDownButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton3.Image")));
            this.toolStripDropDownButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton3.Name = "toolStripDropDownButton3";
            this.toolStripDropDownButton3.Size = new System.Drawing.Size(45, 36);
            this.toolStripDropDownButton3.Text = "toolStripDropDownButton3";
            this.toolStripDropDownButton3.ToolTipText = "Cells / Units";
            // 
            // cellNotToolStripMenuItem
            // 
            this.cellNotToolStripMenuItem.Name = "cellNotToolStripMenuItem";
            this.cellNotToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellNotToolStripMenuItem.Text = "Cell Not";
            this.cellNotToolStripMenuItem.Click += new System.EventHandler(this.cellNotToolStripMenuItem_Click);
            // 
            // cellBufferToolStripMenuItem
            // 
            this.cellBufferToolStripMenuItem.Name = "cellBufferToolStripMenuItem";
            this.cellBufferToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellBufferToolStripMenuItem.Text = "Cell Buffer";
            this.cellBufferToolStripMenuItem.Click += new System.EventHandler(this.cellBufferToolStripMenuItem_Click);
            // 
            // cellMuxToolStripMenuItem
            // 
            this.cellMuxToolStripMenuItem.Name = "cellMuxToolStripMenuItem";
            this.cellMuxToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellMuxToolStripMenuItem.Text = "Cell Mux";
            this.cellMuxToolStripMenuItem.Click += new System.EventHandler(this.cellMuxToolStripMenuItem_Click);
            // 
            // cellLogicToolStripMenuItem
            // 
            this.cellLogicToolStripMenuItem.Name = "cellLogicToolStripMenuItem";
            this.cellLogicToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellLogicToolStripMenuItem.Text = "Cell Logic";
            this.cellLogicToolStripMenuItem.Click += new System.EventHandler(this.cellLogicToolStripMenuItem_Click);
            // 
            // cellAdderToolStripMenuItem
            // 
            this.cellAdderToolStripMenuItem.Name = "cellAdderToolStripMenuItem";
            this.cellAdderToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellAdderToolStripMenuItem.Text = "Cell Adder";
            this.cellAdderToolStripMenuItem.Click += new System.EventHandler(this.cellAdderToolStripMenuItem_Click);
            // 
            // cellBusSupportToolStripMenuItem
            // 
            this.cellBusSupportToolStripMenuItem.Name = "cellBusSupportToolStripMenuItem";
            this.cellBusSupportToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellBusSupportToolStripMenuItem.Text = "Cell Bus Support";
            this.cellBusSupportToolStripMenuItem.Click += new System.EventHandler(this.cellBusSupportToolStripMenuItem_Click);
            // 
            // cellFlipFlopToolStripMenuItem
            // 
            this.cellFlipFlopToolStripMenuItem.Name = "cellFlipFlopToolStripMenuItem";
            this.cellFlipFlopToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellFlipFlopToolStripMenuItem.Text = "Cell Flip-Flop";
            this.cellFlipFlopToolStripMenuItem.Click += new System.EventHandler(this.cellFlipFlopToolStripMenuItem_Click);
            // 
            // cellLatchToolStripMenuItem
            // 
            this.cellLatchToolStripMenuItem.Name = "cellLatchToolStripMenuItem";
            this.cellLatchToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellLatchToolStripMenuItem.Text = "Cell Latch";
            this.cellLatchToolStripMenuItem.Click += new System.EventHandler(this.cellLatchToolStripMenuItem_Click);
            // 
            // cellOtherToolStripMenuItem
            // 
            this.cellOtherToolStripMenuItem.Name = "cellOtherToolStripMenuItem";
            this.cellOtherToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.cellOtherToolStripMenuItem.Text = "Cell Other";
            this.cellOtherToolStripMenuItem.Click += new System.EventHandler(this.cellOtherToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(159, 6);
            // 
            // unitRegisterFileToolStripMenuItem
            // 
            this.unitRegisterFileToolStripMenuItem.Name = "unitRegisterFileToolStripMenuItem";
            this.unitRegisterFileToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.unitRegisterFileToolStripMenuItem.Text = "Unit Register File";
            this.unitRegisterFileToolStripMenuItem.Click += new System.EventHandler(this.unitRegisterFileToolStripMenuItem_Click);
            // 
            // unitMemoryToolStripMenuItem
            // 
            this.unitMemoryToolStripMenuItem.Name = "unitMemoryToolStripMenuItem";
            this.unitMemoryToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.unitMemoryToolStripMenuItem.Text = "Unit Memory";
            this.unitMemoryToolStripMenuItem.Click += new System.EventHandler(this.unitMemoryToolStripMenuItem_Click);
            // 
            // unitCustomToolStripMenuItem
            // 
            this.unitCustomToolStripMenuItem.Name = "unitCustomToolStripMenuItem";
            this.unitCustomToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.unitCustomToolStripMenuItem.Text = "Unit Custom";
            this.unitCustomToolStripMenuItem.Click += new System.EventHandler(this.unitCustomToolStripMenuItem_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.BackColor = System.Drawing.SystemColors.Control;
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(36, 36);
            this.toolStripButton2.Text = "Merge Horizontal Wires";
            this.toolStripButton2.Click += new System.EventHandler(this.toolStripButton2_Click);
            // 
            // toolStripButton3
            // 
            this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(36, 36);
            this.toolStripButton3.Text = "Merge Vertical Wires";
            this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "deroute.ico");
            this.imageList1.Images.SetKeyName(1, "vias.ico");
            this.imageList1.Images.SetKeyName(2, "vias_power.ico");
            this.imageList1.Images.SetKeyName(3, "vias_ground.ico");
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 524);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1040, 22);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(72, 17);
            this.toolStripStatusLabel1.Text = "Some Status";
            // 
            // entityBox1
            // 
            this.entityBox1.AutoPriority = true;
            this.entityBox1.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.entityBox1.CellAdderColor = System.Drawing.Color.Red;
            this.entityBox1.CellBufferColor = System.Drawing.Color.Navy;
            this.entityBox1.CellBusSuppColor = System.Drawing.Color.DarkViolet;
            this.entityBox1.CellFlipFlopColor = System.Drawing.Color.Lime;
            this.entityBox1.CellLatchColor = System.Drawing.Color.SpringGreen;
            this.entityBox1.CellLogicColor = System.Drawing.Color.Yellow;
            this.entityBox1.CellMuxColor = System.Drawing.Color.DarkOrange;
            this.entityBox1.CellNotColor = System.Drawing.Color.Navy;
            this.entityBox1.CellOpacity = 128;
            this.entityBox1.CellOtherColor = System.Drawing.Color.Snow;
            this.entityBox1.CellPriority = 1;
            this.entityBox1.CellTextAlignment = TextAlignment.TopLeft;
            this.entityBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.entityBox1.HideCells = false;
            this.entityBox1.HideImage = false;
            this.entityBox1.HideVias = false;
            this.entityBox1.HideWires = false;
            this.entityBox1.Lambda = 5F;
            this.entityBox1.Location = new System.Drawing.Point(0, 0);
            this.entityBox1.Mode = EntityType.Selection;
            this.entityBox1.Name = "entityBox1";
            this.entityBox1.ScrollX = 0;
            this.entityBox1.ScrollY = 0;
            this.entityBox1.SelectionBoxColor = System.Drawing.Color.Red;
            this.entityBox1.SelectionColor = System.Drawing.Color.LimeGreen;
            this.entityBox1.Size = new System.Drawing.Size(743, 441);
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
            this.entityBox1.ViasPowerColor = System.Drawing.Color.Black;
            this.entityBox1.ViasPriority = 3;
            this.entityBox1.ViasShape = ViasShape.Round;
            this.entityBox1.ViasTextAlignment = TextAlignment.Top;
            this.entityBox1.WireBaseSize = 5;
            this.entityBox1.WireGroundColor = System.Drawing.Color.Green;
            this.entityBox1.WireInterconnectColor = System.Drawing.Color.Blue;
            this.entityBox1.WireOpacity = 128;
            this.entityBox1.WirePowerColor = System.Drawing.Color.Red;
            this.entityBox1.WirePriority = 2;
            this.entityBox1.WireTextAlignment = TextAlignment.TopLeft;
            this.entityBox1.Zoom = 100;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.propertyGrid1.Location = new System.Drawing.Point(0, 132);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.SelectedObject = this.entityBox1;
            this.propertyGrid1.Size = new System.Drawing.Size(293, 348);
            this.propertyGrid1.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1040, 546);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
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
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
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
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem viasPowerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viasGroundToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.ToolStripMenuItem viasInputToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viasOutputToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viasInoutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viasFloatingToolStripMenuItem;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton2;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton3;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.ToolStripButton toolStripButton3;
        private System.Windows.Forms.ToolStripMenuItem wireInterconnectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem wirePowerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem wireGroundToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellNotToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellBufferToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellMuxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellLogicToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellAdderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellBusSupportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellFlipFlopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellLatchToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem unitRegisterFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unitMemoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cellOtherToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unitCustomToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
    }
}

