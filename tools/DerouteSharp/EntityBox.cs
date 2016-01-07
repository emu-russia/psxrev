using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Serialization;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {
        private System.Drawing.Image _image;
        private float _lambda;
        private int _zoom;
        private int _ScrollX;
        private int _ScrollY;
        private int SavedScrollX;
        private int SavedScrollY;
        private int SavedMouseX;
        private int SavedMouseY;
        private int LastMouseX;
        private int LastMouseY;
        private bool ScrollingBegin = false;
        private bool DrawingBegin = false;
        private List <Entity> _entities;
        private EntityType drawMode = EntityType.Selection;
        bool hideImage;

        public EntityBox()
        {
            BackColor = SystemColors.MenuHighlight;

            _entities = new List<Entity>();

            Lambda = 5.0F;
            Zoom = 100;
            HideImage = false;

            DefaultEntityAppearance();

            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

        public event EventHandler<EventArgs> ImageChanged;

        private bool IsEntityWire(Entity entity)
        {
            return (entity.Type == EntityType.WireGround ||
                     entity.Type == EntityType.WireInterconnect ||
                     entity.Type == EntityType.WirePower);
        }

        private bool IsEntityVias(Entity entity)
        {
            return (entity.Type == EntityType.ViasConnect ||
                     entity.Type == EntityType.ViasFloating ||
                     entity.Type == EntityType.ViasGround ||
                     entity.Type == EntityType.ViasInout ||
                     entity.Type == EntityType.ViasInput ||
                     entity.Type == EntityType.ViasOutput ||
                     entity.Type == EntityType.ViasPower);
        }

        private bool IsEntityCell(Entity entity)
        {
            return (entity.Type == EntityType.CellNot ||
                     entity.Type == EntityType.CellBuffer ||
                     entity.Type == EntityType.CellMux ||
                     entity.Type == EntityType.CellLogic ||
                     entity.Type == EntityType.CellAdder ||
                     entity.Type == EntityType.CellBusSupp ||
                     entity.Type == EntityType.CellFlipFlop ||
                     entity.Type == EntityType.CellLatch ||
                     entity.Type == EntityType.UnitRegfile ||
                     entity.Type == EntityType.UnitMemory ||
                     entity.Type == EntityType.UnitCustom);
        }

        //
        // Coordinate space convertion
        //
        // lx = (sx - scroll) / (zoom * lambda)
        //
        // sx = lx * zoom * lambda + scroll
        //

        private PointF ScreenToLambda ( int ScreenX, int ScreenY)
        {
            PointF point = new PointF (0.0F, 0.0F);
            float zf = (float)Zoom / 100;

            point.X = (float)(ScreenX - ScrollX) / (zf * Lambda);
            point.Y = (float)(ScreenY - ScrollY) / (zf * Lambda);

            return point;
        }

        private Point LambdaToScreen ( float LambdaX, float LambdaY )
        {
            Point point = new Point(0, 0);
            float zf = (float)Zoom / 100;

            float x = LambdaX * Lambda * zf + (float)ScrollX;
            float y = LambdaY * Lambda * zf + (float)ScrollY;

            point.X = (int)x;
            point.Y = (int)y;

            return point;
        }

        //
        // Mouse hit test
        //

        private Entity EntityHitTest ( int MouseX, int MouseY )
        {
            PointF point = ScreenToLambda(MouseX, MouseY);

            foreach ( Entity entity in _entities )
            {

            }

            return null;
        }

        //
        // Mouse events handling
        //

        protected override void OnMouseDown(MouseEventArgs e)
        {
            //
            // Scrolling
            //

            if (e.Button == MouseButtons.Right && ScrollingBegin == false && DrawingBegin == false)
            {
                SavedMouseX = e.X;
                SavedMouseY = e.Y;
                SavedScrollX = _ScrollX;
                SavedScrollY = _ScrollY;
                ScrollingBegin = true;
            }

            //
            // Drawing
            //

            if (e.Button == MouseButtons.Left && Mode != EntityType.Selection && 
                 DrawingBegin == false && ScrollingBegin == false )
            {
                Entity entity;
                bool Okay;

                //
                // Cannot draw cells / custom blocks over other entites
                //

                Okay = true;

                entity = EntityHitTest(e.X, e.Y);
                if (entity != null && (Mode == EntityType.CellAdder ||
                     Mode == EntityType.CellBuffer || Mode == EntityType.CellBusSupp ||
                     Mode == EntityType.CellFlipFlop || Mode == EntityType.CellLatch ||
                     Mode == EntityType.CellLogic || Mode == EntityType.CellMux ||
                     Mode == EntityType.CellNot || Mode == EntityType.UnitCustom ||
                     Mode == EntityType.UnitMemory || Mode == EntityType.UnitRegfile ) )
                {
                    Okay = false;
                }

                if (Okay == true)
                {
                    SavedMouseX = e.X;
                    SavedMouseY = e.Y;
                    SavedScrollX = _ScrollX;
                    SavedScrollY = _ScrollY;
                    DrawingBegin = true;
                }
            }

            base.OnMouseDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            if ( e.Button == MouseButtons.Right && ScrollingBegin)
            {
                ScrollingBegin = false;
                this.Invalidate();
            }

            //
            // Select entity
            //

            if ( e.Button == MouseButtons.Left && Mode == EntityType.Selection )
            {
                Entity entity = EntityHitTest(e.X, e.Y);

                if (entity != null)
                    MessageBox.Show("Selected " + entity.Label);
            }

            //
            // Add vias
            //

            if ( e.Button == MouseButtons.Left && 
                 (Mode == EntityType.ViasConnect || Mode == EntityType.ViasFloating || Mode == EntityType.ViasGround ||
                  Mode == EntityType.ViasInout || Mode == EntityType.ViasInput || Mode == EntityType.ViasOutput ||
                  Mode == EntityType.ViasPower ) && DrawingBegin )
            {
                AddVias(Mode, e.X, e.Y);

                DrawingBegin = false;
            }

            //
            // Add wire
            //

            if ( e.Button == MouseButtons.Left && ( Mode == EntityType.WireGround || 
                  Mode == EntityType.WireInterconnect || Mode == EntityType.WirePower ) && DrawingBegin )
            {
                AddWire(Mode, SavedMouseX, SavedMouseY, e.X, e.Y);

                DrawingBegin = false;
            }

            base.OnMouseUp(e);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            //
            // Scroll animation
            //

            if ( ScrollingBegin )
            {
                ScrollX = SavedScrollX + e.X - SavedMouseX;
                ScrollY = SavedScrollY + e.Y - SavedMouseY;
                this.Invalidate();
            }

            //
            // Wire drawing animation
            //

            if ( DrawingBegin && (Mode == EntityType.WireGround || 
                   Mode == EntityType.WireInterconnect || Mode == EntityType.WirePower ))
            {
                LastMouseX = e.X;
                LastMouseY = e.Y;
                this.Invalidate();
            }

            base.OnMouseMove(e);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            if ( e.Delta > 0 )
                Zoom += 10;
            else
                Zoom -= 10;

            base.OnMouseWheel(e);
        }

        //
        // Drawing
        //

        private void DrawLambdaScale (Graphics gr)
        {
            float scaleWidth = (int)Lambda * 5;

            scaleWidth *= (float)Zoom / 100.0F;

            Pen linePen = new Pen(Color.LightGray, 3);

            gr.DrawLine( linePen,
                         this.Width - scaleWidth - 5,
                         this.Height - 5,
                         this.Width - 5,
                         this.Height - 5);

            string label = "5λ";
            int labelWidth = (int)gr.MeasureString(label, this.Font).Width;

            gr.DrawString( label, this.Font, Brushes.Black,
                           this.Width - labelWidth - scaleWidth / 2,
                           this.Height - this.Font.Height - linePen.Width - 5);
        }

        private void DrawLambdaGrid (Graphics gr)
        {
            float x, y;

            float scaleWidth = (int)Lambda * 5;
            scaleWidth *= (float)Zoom / 100.0F;

            for ( y=0; y<this.Height; y+= scaleWidth)
            {
                for (x=0; x<this.Width; x+= scaleWidth)
                {
                    gr.FillRectangle(Brushes.LightGray, x, y, 1, 1);
                }
            }
        }

        private void DrawEntity ( Entity entity, Graphics gr)
        {
            Color viasColor;
            int centerX;
            int centerY;
            int radius;
            Color wireColor;
            int startX;
            int startY;
            int endX;
            int endY;
            float zf = (float)Zoom / 100.0F;

            switch ( entity.Type )
            {
                case EntityType.ViasConnect:
                case EntityType.ViasFloating:
                case EntityType.ViasGround:
                case EntityType.ViasInout:
                case EntityType.ViasInput:
                case EntityType.ViasOutput:
                case EntityType.ViasPower:

                    if (entity.Type == EntityType.ViasConnect)
                        viasColor = ViasConnectColor;
                    else if (entity.Type == EntityType.ViasFloating)
                        viasColor = ViasFloatingColor;
                    else if (entity.Type == EntityType.ViasGround)
                        viasColor = ViasGroundColor;
                    else if (entity.Type == EntityType.ViasInout)
                        viasColor = ViasInoutColor;
                    else if (entity.Type == EntityType.ViasInput)
                        viasColor = ViasInputColor;
                    else if (entity.Type == EntityType.ViasOutput)
                        viasColor = ViasOutputColor;
                    else if (entity.Type == EntityType.ViasPower)
                        viasColor = ViasPowerColor;
                    else
                        viasColor = Color.Black;

                    Point point = LambdaToScreen(entity.LambdaX, entity.LambdaY);

                    centerX = point.X;
                    centerY = point.Y;
                    radius = (int)((float)ViasBaseSize * zf);

                    gr.FillEllipse( new SolidBrush(viasColor),
                                    centerX - radius, centerY - radius,
                                    radius + radius, radius + radius );

                    break;

                case EntityType.WireGround:
                case EntityType.WirePower:
                case EntityType.WireInterconnect:

                    if (entity.Type == EntityType.WireGround)
                        wireColor = WireGroundColor;
                    else if (entity.Type == EntityType.WirePower)
                        wireColor = WirePowerColor;
                    else if (entity.Type == EntityType.WireInterconnect)
                        wireColor = WireInterconnectColor;
                    else
                        wireColor = Color.Blue;

                    Point point1 = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point point2 = LambdaToScreen(entity.LambdaEndX, entity.LambdaEndY);

                    startX = point1.X;
                    startY = point1.Y;
                    endX = point2.X;
                    endY = point2.Y;

                    gr.DrawLine( new Pen(wireColor, (float)WireBaseSize * zf),
                                 startX, startY,
                                 endX, endY);

                    break;
            }
        }

        private void DrawScene (Graphics gr, int width, int height, bool WholeScene, Point origin)
        {
            int savedScrollX = 0, savedScrollY = 0, savedZoom = 0;

            if ( WholeScene == true )
            {
                savedScrollX = _ScrollX;
                savedScrollY = _ScrollY;
                savedZoom = _zoom;

                _ScrollX = -origin.X;
                _ScrollY = -origin.Y;
                _zoom = 100;
            }

            //
            // Background
            //

            Region region = new Region(new Rectangle(0, 0, width - origin.X, height - origin.Y));

            gr.FillRegion(new SolidBrush(BackColor), region);

            //
            // Image
            //

            if (Image != null && hideImage == false)
            {
                gr.DrawImage( Image,
                              ScrollX, ScrollY,
                              Image.Width * Zoom / 100,
                              Image.Height * Zoom / 100);
            }

            //
            // Grid
            //

            if (WholeScene == false)
                DrawLambdaGrid(gr);

            //
            // Entities
            //

            if (Lambda > 0.0F)
            {
                foreach (Entity entity in _entities)
                {
                    DrawEntity(entity, gr);
                }

                //
                // Wire drawing animation
                //

                if (DrawingBegin && (Mode == EntityType.WireGround ||
                       Mode == EntityType.WireInterconnect || Mode == EntityType.WirePower))
                {
                    Entity virtualEntity = new Entity();

                    PointF point1 = ScreenToLambda(SavedMouseX, SavedMouseY);
                    PointF point2 = ScreenToLambda(LastMouseX, LastMouseY);

                    virtualEntity.LambdaX = point1.X;
                    virtualEntity.LambdaY = point1.Y;
                    virtualEntity.LambdaEndX = point2.X;
                    virtualEntity.LambdaEndY = point2.Y;
                    virtualEntity.Type = Mode;

                    DrawEntity(virtualEntity, gr);
                }

                //
                // Lambda Scale
                //

                if (WholeScene == false)
                    DrawLambdaScale(gr);
            }

            if (WholeScene == true )
            {
                _ScrollX = savedScrollX;
                _ScrollY = savedScrollY;
                _zoom = savedZoom;
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            BufferedGraphicsContext context;
            BufferedGraphics gfx;

            context = BufferedGraphicsManager.Current;

            context.MaximumBuffer = new Size(this.Width + 1, this.Height + 1);

            gfx = context.Allocate(this.CreateGraphics(),
                 new Rectangle(0, 0, this.Width, this.Height));

            Point origin = new Point(0, 0);
            DrawScene(gfx.Graphics, this.Width, this.Height, false, origin);

            gfx.Render(e.Graphics);

            gfx.Dispose();
        }

        [Browsable(false), EditorBrowsable(EditorBrowsableState.Never), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public override Image BackgroundImage
        {
            get { return base.BackgroundImage; }
            set { base.BackgroundImage = value; }
        }

        [Browsable(false), EditorBrowsable(EditorBrowsableState.Never), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public override ImageLayout BackgroundImageLayout
        {
            get { return base.BackgroundImageLayout; }
            set { base.BackgroundImageLayout = value; }
        }

        [Category("Appearance"), DefaultValue(null)]
        public System.Drawing.Image Image
        {
            get { return _image; }
            set
            {
                if (_image != value)
                {
                    _image = value;

                    ScrollX = 0;
                    ScrollY = 0;
                    ScrollingBegin = false;
                    Zoom = 100;

                    this.OnImageChanged(EventArgs.Empty);
                }
            }
        }

        [Category("Logic")]
        public float Lambda
        {
            get { return _lambda; }
            set
            {
                _lambda = value;

                ViasBaseSize = Math.Max(1, (int)Lambda - 1);
                WireBaseSize = (int)Lambda;

                Invalidate();
            }
        }

        [Category("Logic")]
        public EntityType Mode
        {
            get { return drawMode; }
            set
            {
                drawMode = value;

                if (drawMode == EntityType.Selection)
                    DrawingBegin = false;
            }
        }

        [Category("Appearance")]
        public int ScrollX
        {
            get { return _ScrollX; }
            set { _ScrollX = value; Invalidate(); }
        }

        [Category("Appearance")]
        public int ScrollY
        {
            get { return _ScrollY; }
            set { _ScrollY = value; Invalidate(); }
        }

        [Category("Appearance")]
        public bool HideImage
        {
            get { return hideImage; }
            set { hideImage = value; Invalidate(); }
        }

        protected virtual void OnImageChanged(EventArgs e)
        {
            this.Invalidate();

            if (this.ImageChanged != null)
                this.ImageChanged(this, e);
        }

        private void AddVias ( EntityType Type, int ScreenX, int ScreenY )
        {
            Entity item = new Entity();

            PointF point = ScreenToLambda(ScreenX, ScreenY);

            item.Label = "vias";
            item.LambdaX = point.X;
            item.LambdaY = point.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;

            _entities.Add(item);

            this.Invalidate();
        }

        private void AddWire ( EntityType Type, int StartX, int StartY, int EndX, int EndY )
        {
            Entity item = new Entity();

            PointF point1 = ScreenToLambda(StartX, StartY);
            PointF point2 = ScreenToLambda(EndX, EndY);

            item.Label = "wire";
            item.LambdaX = point1.X;
            item.LambdaY = point1.Y;
            item.LambdaEndX = point2.X;
            item.LambdaEndY = point2.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;

            _entities.Add(item);

            this.Invalidate();
        }

        [Category("Appearance")]
        public int Zoom
        {
            get { return _zoom; }
            set
            {
                _zoom = value;

                if (_zoom < 30)
                    _zoom = 30;

                if (_zoom > 400)
                    _zoom = 400;

                Invalidate();
            }
        }

        public void DeleteAllEntites ()
        {
            _entities.Clear();
            this.Invalidate();
        }

        private Point DetermineSceneSize (out Point origin)
        {
            Point point = new Point(0, 0);
            Point originOut = new Point(0, 0);

            int savedScrollX = 0, savedScrollY = 0, savedZoom = 0;

            savedScrollX = _ScrollX;
            savedScrollY = _ScrollY;
            savedZoom = _zoom;

            _ScrollX = 0;
            _ScrollY = 0;
            _zoom = 100;

            if ( Image != null && HideImage == false )
            {
                point.X = Image.Width;
                point.Y = Image.Height;
            }

            if (Lambda > 0)
            {
                foreach (Entity entity in _entities)
                {
                    Point screenCoords;

                    //
                    // Bottom Right Bounds
                    //

                    if (IsEntityWire(entity))
                    {
                        screenCoords = LambdaToScreen(Math.Max(entity.LambdaX, entity.LambdaEndX),
                                                        Math.Max(entity.LambdaY, entity.LambdaEndY));
                        screenCoords.X += WireBaseSize;
                        screenCoords.Y += WireBaseSize;
                    }
                    else if (IsEntityCell(entity))
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX + entity.LambdaWidth,
                                                        entity.LambdaY + entity.LambdaHeight);
                    }
                    else
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                        screenCoords.X += ViasBaseSize;
                        screenCoords.Y += ViasBaseSize;
                    }

                    if (screenCoords.X > point.X)
                        point.X = screenCoords.X;

                    if (screenCoords.Y > point.Y)
                        point.Y = screenCoords.Y;

                    //
                    // Top Left Bounds
                    //

                    if (IsEntityWire(entity))
                    {
                        screenCoords = LambdaToScreen(Math.Min(entity.LambdaX, entity.LambdaEndX),
                                                        Math.Min(entity.LambdaY, entity.LambdaEndY));
                        screenCoords.X -= WireBaseSize;
                        screenCoords.Y -= WireBaseSize;
                    }
                    else if (IsEntityCell(entity))
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX,
                                                        entity.LambdaY);
                    }
                    else
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                        screenCoords.X -= ViasBaseSize;
                        screenCoords.Y -= ViasBaseSize;
                    }

                    if (screenCoords.X < originOut.X)
                        originOut.X = screenCoords.X;

                    if (screenCoords.Y < originOut.Y)
                        originOut.Y = screenCoords.Y;
                }
            }

            _ScrollX = savedScrollX;
            _ScrollY = savedScrollY;
            _zoom = savedZoom;

            origin = originOut;

            return point;
        }

        public void SaveSceneAsImage (string FileName)
        {
            ImageFormat imageFormat;
            string ext;
            Point origin;
            Point sceneSize = DetermineSceneSize(out origin);

            Bitmap bitmap = new Bitmap(sceneSize.X - origin.X, sceneSize.Y - origin.Y);

            Graphics gr = Graphics.FromImage(bitmap);

            //origin.X = -100;
            //origin.Y = -100;

            DrawScene(gr, sceneSize.X, sceneSize.Y, true, origin);

            ext = Path.GetExtension(FileName);

            if (ext.ToLower() == ".jpg" || ext.ToLower() == ".jpeg")
                imageFormat = ImageFormat.Jpeg;
            if (ext.ToLower() == ".png" )
                imageFormat = ImageFormat.Png;
            if (ext.ToLower() == ".bmp" )
                imageFormat = ImageFormat.Bmp;
            else
                imageFormat = ImageFormat.Jpeg;

            bitmap.Save(FileName, imageFormat);
        }

        //
        // Serialization
        //

        public void Serialize (string FileName)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            using (FileStream fs = new FileStream(FileName, FileMode.Create))
            {
                ser.Serialize(fs, _entities);
            }
        }

        public void Unserialize (string FileName, bool Append)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            using (FileStream fs = new FileStream(FileName, FileMode.Open))
            {
                if (Append == true)
                {
                    List<Entity> list = (List<Entity>)ser.Deserialize(fs);

                    foreach (Entity entity in list)
                        _entities.Add(entity);
                }
                else
                {
                    _entities.Clear();

                    _entities = (List<Entity>)ser.Deserialize(fs);
                }

                Invalidate();
            }
        }

        //
        // Entity properties
        //

        private Color _ViasInputColor;
        private Color _ViasOutputColor;
        private Color _ViasInoutColor;
        private Color _ViasConnectColor;
        private Color _ViasFloatingColor;
        private Color _ViasPowerColor;
        private Color _ViasGroundColor;
        private Color _WireInterconnectColor;
        private Color _WirePowerColor;
        private Color _WireGroundColor;
        private Color _CellNotColor;
        private Color _CellBufferColor;
        private Color _CellMuxColor;
        private Color _CellLogicColor;
        private Color _CellAdderColor;
        private Color _CellBusSuppColor;
        private Color _CellFlipFlopColor;
        private Color _CellLatchColor;
        private Color _UnitRegfileColor;
        private Color _UnitMemoryColor;
        private Color _UnitCustomColor;
        private ViasShape _viasShape;
        private int _viasBaseSize;
        private int _wireBaseSize;
        private CellTextAlignment _cellTextAlignment;

        private void DefaultEntityAppearance()
        {
            ViasShape = ViasShape.Round;
            ViasBaseSize = Math.Max(1, (int)Lambda - 1);
            WireBaseSize = (int)Lambda;
            CellTextAlignment = CellTextAlignment.TopLeft;

            ViasInputColor = Color.Green;
            ViasOutputColor = Color.Red;
            ViasInoutColor = Color.Yellow;
            ViasConnectColor = Color.Black;
            ViasFloatingColor = Color.Gray;
            ViasPowerColor = Color.Black;
            ViasGroundColor = Color.Black;

            WireInterconnectColor = Color.Blue;
            WirePowerColor = Color.Red;
            WireGroundColor = Color.Green;
        }

        [Category("Entity Appearance")]
        public ViasShape ViasShape
        {
            get { return _viasShape; }
            set { _viasShape = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public int ViasBaseSize
        {
            get { return _viasBaseSize; }
            set { _viasBaseSize = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public int WireBaseSize
        {
            get { return _wireBaseSize; }
            set { _wireBaseSize = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public CellTextAlignment CellTextAlignment
        {
            get { return _cellTextAlignment; }
            set { _cellTextAlignment = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasInputColor
        {
            get { return _ViasInputColor; }
            set { _ViasInputColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasOutputColor
        {
            get { return _ViasOutputColor; }
            set { _ViasOutputColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasInoutColor
        {
            get { return _ViasInoutColor; }
            set { _ViasInoutColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasConnectColor
        {
            get { return _ViasConnectColor; }
            set { _ViasConnectColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasFloatingColor
        {
            get { return _ViasFloatingColor; }
            set { _ViasFloatingColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasPowerColor
        {
            get { return _ViasPowerColor; }
            set { _ViasPowerColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color ViasGroundColor
        {
            get { return _ViasGroundColor; }
            set { _ViasGroundColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color WireInterconnectColor
        {
            get { return _WireInterconnectColor; }
            set { _WireInterconnectColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color WirePowerColor
        {
            get { return _WirePowerColor; }
            set { _WirePowerColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color WireGroundColor
        {
            get { return _WireGroundColor; }
            set { _WireGroundColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellNotColor
        {
            get { return _CellNotColor; }
            set { _CellNotColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellBufferColor
        {
            get { return _CellBufferColor; }
            set { _CellBufferColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellMuxColor
        {
            get { return _CellMuxColor; }
            set { _CellMuxColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellLogicColor
        {
            get { return _CellLogicColor; }
            set { _CellLogicColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellAdderColor
        {
            get { return _CellAdderColor; }
            set { _CellAdderColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellBusSuppColor
        {
            get { return _CellBusSuppColor; }
            set { _CellBusSuppColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellFlipFlopColor
        {
            get { return _CellFlipFlopColor; }
            set { _CellFlipFlopColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellLatchColor
        {
            get { return _CellLatchColor; }
            set { _CellLatchColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color UnitRegfileColor
        {
            get { return _UnitRegfileColor; }
            set { _UnitRegfileColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color UnitMemoryColor
        {
            get { return _UnitMemoryColor; }
            set { _UnitMemoryColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color UnitCustomColor
        {
            get { return _UnitCustomColor; }
            set { _UnitCustomColor = value; Invalidate(); }
        }

    }
}
