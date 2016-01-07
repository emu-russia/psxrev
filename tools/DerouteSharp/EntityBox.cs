using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;

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

        public EntityBox()
        {
            BackColor = SystemColors.MenuHighlight;

            _entities = new List<Entity>();

            Lambda = 5.0F;

            Zoom = 100;

            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

        public event EventHandler<EventArgs> ImageChanged;

        //
        // Coordinate space convertion
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

            gr.DrawLine(linePen,
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

            DrawLambdaScale(gr);
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

            switch ( entity.Type )
            {
                case EntityType.ViasConnect:
                case EntityType.ViasFloating:
                case EntityType.ViasGround:
                case EntityType.ViasInout:
                case EntityType.ViasInput:
                case EntityType.ViasOutput:
                case EntityType.ViasPower:

                    viasColor = Color.Indigo;

                    Point point = LambdaToScreen(entity.LambdaX, entity.LambdaY);

                    centerX = point.X;
                    centerY = point.Y;
                    radius = (int)Lambda;

                    gr.FillEllipse( new SolidBrush(viasColor),
                                    centerX - radius, centerY - radius,
                                    radius + radius, radius + radius );

                    break;

                case EntityType.WireGround:
                case EntityType.WirePower:
                case EntityType.WireInterconnect:

                    wireColor = Color.BlueViolet;

                    Point point1 = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point point2 = LambdaToScreen(entity.LambdaEndX, entity.LambdaEndY);

                    startX = point1.X;
                    startY = point1.Y;
                    endX = point2.X;
                    endY = point2.Y;

                    gr.DrawLine( new Pen(wireColor, (int)Lambda),
                                 startX, startY,
                                 endX, endY);

                    break;
            }
        }

        private void DrawScene (Graphics gr, int width, int height)
        {
            //
            // Background
            //

            Region region = new Region(new Rectangle(0, 0, width, height));

            gr.FillRegion(new SolidBrush(BackColor), region);

            //
            // Image
            //

            if (Image != null)
            {
                gr.DrawImage( Image,
                              ScrollX, ScrollY,
                              Image.Width * Zoom / 100,
                              Image.Height * Zoom / 100);
            }

            //
            // Grid
            //

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

            DrawScene(gfx.Graphics, this.Width, this.Height);

            gfx.Render(e.Graphics);
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

                    this.OnImageChanged(EventArgs.Empty);
                }
            }
        }

        [Category("Logic")]
        public float Lambda
        {
            get { return _lambda; }
            set { _lambda = value; Invalidate(); }
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
    }
}
