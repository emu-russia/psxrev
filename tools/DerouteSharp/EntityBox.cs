using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Serialization;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {
        private Image _image;
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
        private int DragStartMouseX;
        private int DragStartMouseY;
        private bool ScrollingBegin = false;
        private bool DrawingBegin = false;
        private bool DraggingBegin = false;
        private List <Entity> _entities;
        private EntityType drawMode = EntityType.Selection;
        private bool hideImage;
        private PropertyGrid entityGrid;
        private List<Entity> selected;
        private float draggingDist;

        public EntityBox()
        {
            BackColor = SystemColors.MenuHighlight;

            _entities = new List<Entity>();

            Lambda = 5.0F;
            Zoom = 100;
            HideImage = false;
            entityGrid = null;

            DefaultEntityAppearance();

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
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

        private PointF rotate(PointF point, double angle)
        {
            PointF rotated_point = new Point();
            double rad = angle * Math.PI / 180.0F;
            rotated_point.X = point.X * (float)Math.Cos(rad) - point.Y * (float)Math.Sin(rad);
            rotated_point.Y = point.X * (float)Math.Sin(rad) + point.Y * (float)Math.Cos(rad);
            return rotated_point;
        }

        private Entity EntityHitTest ( int MouseX, int MouseY )
        {
            PointF point = new Point(MouseX, MouseY);
            PointF[] rect = new PointF[4];
            float zf = (float)Zoom / 100.0F;

            foreach ( Entity entity in _entities )
            {
                if ( IsEntityWire(entity) )
                {
                    PointF start = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    PointF end = LambdaToScreen(entity.LambdaEndX, entity.LambdaEndY);
                    
                    if ( end.X < start.X )
                    {
                        PointF temp = start;
                        start = end;
                        end = temp;
                    }

                    PointF ortho = new PointF(end.X - start.X, end.Y - start.Y);

                    float len = (float)Math.Sqrt( Math.Pow(ortho.X, 2) + 
                                                  Math.Pow(ortho.Y, 2));
                    len = Math.Max(1.0F, len);

                    PointF rot = rotate(ortho, -90);
                    PointF normalized = new PointF(rot.X / len, rot.Y / len);
                    PointF baseVect = new PointF(normalized.X * ((WireBaseSize * zf) / 2),
                                                  normalized.Y * ((WireBaseSize * zf) / 2));

                    rect[0].X = baseVect.X + start.X;
                    rect[0].Y = baseVect.Y + start.Y;
                    rect[3].X = baseVect.X + end.X;
                    rect[3].Y = baseVect.Y + end.Y;

                    rot = rotate(ortho, +90);
                    normalized = new PointF(rot.X / len, rot.Y / len);
                    baseVect = new PointF(normalized.X * ((WireBaseSize * zf) / 2),
                                           normalized.Y * ((WireBaseSize * zf) / 2));

                    rect[1].X = baseVect.X + start.X;
                    rect[1].Y = baseVect.Y + start.Y;
                    rect[2].X = baseVect.X + end.X;
                    rect[2].Y = baseVect.Y + end.Y;

                    if (PointInPoly(rect, point) == true)
                        return entity;
                }
                else if ( IsEntityCell(entity))
                {
                    rect[0] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[1] = LambdaToScreen(entity.LambdaX, entity.LambdaY + entity.LambdaHeight);
                    rect[2] = LambdaToScreen(entity.LambdaX + entity.LambdaWidth, entity.LambdaY + entity.LambdaHeight);
                    rect[3] = LambdaToScreen(entity.LambdaX + entity.LambdaWidth, entity.LambdaY);

                    if (PointInPoly(rect, point) == true)
                        return entity;
                }
                else        // Vias
                {
                    rect[0] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[0].X -= ((float)ViasBaseSize * zf);
                    rect[0].Y -= ((float)ViasBaseSize * zf);

                    rect[1] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[1].X += ((float)ViasBaseSize * zf);
                    rect[1].Y -= ((float)ViasBaseSize * zf);

                    rect[2] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[2].X += ((float)ViasBaseSize * zf);
                    rect[2].Y += ((float)ViasBaseSize * zf);

                    rect[3] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[3].X -= ((float)ViasBaseSize * zf);
                    rect[3].Y += ((float)ViasBaseSize * zf);

                    if (PointInPoly(rect, point) == true)
                        return entity;
                }
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

            //
            // Dragging
            //

            if ( e.Button == MouseButtons.Left && Mode == EntityType.Selection && DraggingBegin == false )
            {
                selected = GetSelected();

                if ( selected.Count > 0 )
                {
                    foreach ( Entity entity in selected )
                    {
                        entity.SavedLambdaX = entity.LambdaX;
                        entity.SavedLambdaY = entity.LambdaY;

                        entity.SavedLambdaEndX = entity.LambdaEndX;
                        entity.SavedLambdaEndY = entity.LambdaEndY;
                    }

                    DragStartMouseX = e.X;
                    DragStartMouseY = e.Y;
                    DraggingBegin = true;
                }
            }

            base.OnMouseDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            Focus();

            if ( e.Button == MouseButtons.Right && ScrollingBegin)
            {
                ScrollingBegin = false;
                Invalidate();
            }

            //
            // Select entity
            //

            if (e.Button == MouseButtons.Left && Mode == EntityType.Selection)
            {
                Entity entity = EntityHitTest(e.X, e.Y);

                if (entity != null)
                {
                    if (entity.Selected == true && draggingDist < 1.0F)
                    {
                        entity.Selected = false;
                        Invalidate();

                        if (entityGrid != null)
                            entityGrid.SelectedObject = null;
                    }
                    else
                    {
                        entity.Selected = true;
                        Invalidate();

                        if (entityGrid != null)
                            entityGrid.SelectedObject = entity;
                    }
                }
                else
                {
                    if (draggingDist < 1.0F )
                        RemoveSelection();
                }
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

            //
            // End Drag
            //

            if (e.Button == MouseButtons.Left && DraggingBegin)
            {
                selected.Clear();
                draggingDist = 0.0F;
                DraggingBegin = false;
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
                Invalidate();
            }

            //
            // Wire drawing animation
            //

            if ( DrawingBegin && (Mode == EntityType.WireGround || 
                   Mode == EntityType.WireInterconnect || Mode == EntityType.WirePower ))
            {
                LastMouseX = e.X;
                LastMouseY = e.Y;
                Invalidate();
            }

            //
            // Drag animation
            //

            if (DraggingBegin && selected.Count > 0)
            {
                foreach ( Entity entity in selected )
                {
                    Point point = LambdaToScreen(entity.SavedLambdaX, entity.SavedLambdaY);

                    point.X += e.X - DragStartMouseX;
                    point.Y += e.Y - DragStartMouseY;

                    PointF lambda = ScreenToLambda(point.X, point.Y);

                    entity.LambdaX = lambda.X;
                    entity.LambdaY = lambda.Y;

                    point = LambdaToScreen(entity.SavedLambdaEndX, entity.SavedLambdaEndY);

                    point.X += e.X - DragStartMouseX;
                    point.Y += e.Y - DragStartMouseY;

                    lambda = ScreenToLambda(point.X, point.Y);

                    entity.LambdaEndX = lambda.X;
                    entity.LambdaEndY = lambda.Y;

                    draggingDist = (float)Math.Sqrt( Math.Pow(Math.Abs(e.X - DragStartMouseX), 2) +
                                                     Math.Pow(Math.Abs(e.Y - DragStartMouseY), 2) );
                }

                Invalidate();
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

        #region Drawing

        //
        // Drawing
        //

        private void DrawLambdaScale (Graphics gr)
        {
            float scaleWidth = (int)Lambda * 5;

            scaleWidth *= (float)Zoom / 100.0F;

            Pen linePen = new Pen(Color.LightGray, 3);

            gr.DrawLine( linePen,
                         Width - scaleWidth - 5,
                         Height - 5,
                         Width - 5,
                         Height - 5);

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

            for ( y=0; y<Height; y+= scaleWidth)
            {
                for (x=0; x<Width; x+= scaleWidth)
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

            switch (entity.Type)
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

                    if (ViasShape == ViasShape.Round)
                    {
                        if (entity.Selected == true)
                        {
                            radius += (int)Lambda;

                            gr.FillEllipse(new SolidBrush(SelectionColor),
                                            centerX - radius, centerY - radius,
                                            radius + radius, radius + radius);

                            radius -= (int)Lambda;
                        }

                        gr.FillEllipse(new SolidBrush(viasColor),
                                        centerX - radius, centerY - radius,
                                        radius + radius, radius + radius);
                    }
                    else
                    {
                        if (entity.Selected == true)
                        {
                            radius += (int)Lambda;

                            gr.FillRectangle(new SolidBrush(SelectionColor),
                                               centerX - radius, centerY - radius,
                                               2 * radius, 2 * radius);

                            radius -= (int)Lambda;
                        }

                        gr.FillRectangle(new SolidBrush(viasColor),
                                           centerX - radius, centerY - radius,
                                           2 * radius, 2 * radius);
                    }

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

                    if (entity.Selected == true)
                    {
                        gr.DrawLine(new Pen(Color.LimeGreen, (float)WireBaseSize * zf + (int)Lambda),
                                     startX, startY,
                                     endX, endY);
                    }

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

            context.MaximumBuffer = new Size(Width + 1, Height + 1);

            gfx = context.Allocate(CreateGraphics(),
                 new Rectangle(0, 0, Width, Height));

            Point origin = new Point(0, 0);
            DrawScene(gfx.Graphics, Width, Height, false, origin);

            gfx.Render(e.Graphics);

            gfx.Dispose();
        }

        #endregion Drawing

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

                    OnImageChanged(EventArgs.Empty);
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
            Invalidate();

            if (ImageChanged != null)
                ImageChanged(this, e);
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
            item.SetParent(this);

            _entities.Add(item);

            Invalidate();
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
            item.SetParent(this);

            _entities.Add(item);

            Invalidate();
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
            Invalidate();
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
        // Entity Selection-related
        //

        private bool PointInPoly ( PointF[] poly, PointF point )
        {
            int max_point = poly.Length - 1;
            float total_angle = GetAngle(
                poly[max_point].X, poly[max_point].Y,
                point.X, point.Y,
                poly[0].X, poly[0].Y);

            for (int i = 0; i < max_point; i++)
            {
                total_angle += GetAngle(
                    poly[i].X, poly[i].Y,
                    point.X, point.Y,
                    poly[i + 1].X, poly[i + 1].Y);
            }

            return (Math.Abs(total_angle) > 0.000001);
        }

        private float GetAngle(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float dot_product = DotProduct(Ax, Ay, Bx, By, Cx, Cy);

            float cross_product = CrossProductLength(Ax, Ay, Bx, By, Cx, Cy);

            return (float)Math.Atan2(cross_product, dot_product);
        }

        private float DotProduct(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float BAx = Ax - Bx;
            float BAy = Ay - By;
            float BCx = Cx - Bx;
            float BCy = Cy - By;

            return (BAx * BCx + BAy * BCy);
        }

        private float CrossProductLength(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float BAx = Ax - Bx;
            float BAy = Ay - By;
            float BCx = Cx - Bx;
            float BCy = Cy - By;

            return (BAx * BCy - BAy * BCx);
        }

        private void RemoveSelection ()
        {
            bool UpdateRequired = false;

            foreach ( Entity entity in _entities )
            {
                if (entity.Selected == true)
                {
                    entity.Selected = false;
                    UpdateRequired = true;
                }
            }

            if (UpdateRequired == true)
                Invalidate();

            if (entityGrid != null)
                entityGrid.SelectedObject = null;
        }

        public void AssociateSelectionPropertyGrid ( PropertyGrid propertyGrid )
        {
            entityGrid = propertyGrid;
        }

        private void DeleteSelected ()
        {
            bool UpdateRequired = false;
            List<Entity> pendingDelete = new List<Entity>();

            foreach (Entity entity in _entities)
            {
                if (entity.Selected == true)
                {
                    pendingDelete.Add(entity);
                    UpdateRequired = true;
                }
            }

            foreach (Entity entity in pendingDelete)
            {
                _entities.Remove(entity);
            }

            if (UpdateRequired == true)
                Invalidate();

            if (entityGrid != null)
                entityGrid.SelectedObject = null;
        }

        private List<Entity> GetSelected ()
        {
            List<Entity> _selected = new List<Entity>();

            foreach (Entity entity in _entities)
            {
                if (entity.Selected == true)
                {
                    _selected.Add(entity);
                }
            }

            return _selected;
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
        private Color _SelectionColor;
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

            SelectionColor = Color.LimeGreen;
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

        [Category("Entity Appearance")]
        public Color SelectionColor
        {
            get { return _SelectionColor; }
            set { _SelectionColor = value; Invalidate(); }
        }

        //
        // Key input handling
        //

        protected override void OnKeyUp(KeyEventArgs e)
        {
            if ( e.KeyCode == Keys.Delete )
                DeleteSelected();

            if (e.KeyCode == Keys.Escape)
                RemoveSelection();

            base.OnKeyUp(e);
        }
    }
}
