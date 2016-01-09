using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Serialization;
using System.Linq;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {
        private Image [] _image = new Image[3];
        private Image[] _imageOrig = new Image[3];
        private float _lambda;
        private int [] _imageZoom = new int[3];
        private int _zoom;
        private Point[] _imageScroll = new Point[3];
        private Point[] _savedImageScroll = new Point[3];
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
        private int SelectStartMouseX;
        private int SelectStartMouseY;
        private bool ScrollingBegin = false;
        private bool DrawingBegin = false;
        private bool DraggingBegin = false;
        private bool SelectionBegin = false;
        private List <Entity> _entities;
        private EntityType drawMode = EntityType.Selection;
        private bool hideImage;
        private bool hideVias;
        private bool hideWires;
        private bool hideCells;
        private PropertyGrid entityGrid;
        private List<Entity> selected;
        private float draggingDist;
        private Color selectionBoxColor;
        private int[] _imageOpacity = new int[3];

        public EntityBox()
        {
            BackColor = SystemColors.WindowFrame;

            _entities = new List<Entity>();

            Lambda = 5.0F;
            Zoom = 100;
            _imageZoom[0] = _imageZoom[1] = _imageZoom[2] = 100;
            _imageOpacity[0] = _imageOpacity[1] = _imageOpacity[2] = 100;
            hideImage = false;
            hideVias = false;
            hideWires = false;
            hideCells = false;
            selectionBoxColor = Color.Red;
            entityGrid = null;

            DefaultEntityAppearance();

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

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
                     entity.Type == EntityType.CellOther ||
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
                _savedImageScroll[0] = _imageScroll[0];
                _savedImageScroll[1] = _imageScroll[1];
                _savedImageScroll[2] = _imageScroll[2];
                ScrollingBegin = true;
            }

            //
            // Drawing
            //

            if (e.Button == MouseButtons.Left && Mode != EntityType.Selection &&
                 Mode != EntityType.ImageLayer0 && Mode != EntityType.ImageLayer1 && Mode != EntityType.ImageLayer2 && 
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
                     Mode == EntityType.CellNot || Mode == EntityType.CellOther || Mode == EntityType.UnitCustom ||
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
            // Dragging / selection
            //

            if ( e.Button == MouseButtons.Left && Mode == EntityType.Selection
                 && DraggingBegin == false && SelectionBegin == false )
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
                else
                {
                    SelectStartMouseX = e.X;
                    SelectStartMouseY = e.Y;
                    SelectionBegin = true;
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
                //
                // Catch entities overlapping selection box
                //

                bool CatchSomething = false;

                if (SelectionBegin)
                {
                    //
                    // Selection box area
                    //

                    PointF selectionStart = ScreenToLambda(SelectStartMouseX, SelectStartMouseY);
                    PointF selectionEnd = ScreenToLambda(e.X, e.Y);

                    PointF selectionOrigin = new PointF();
                    PointF selectionSize = new PointF();

                    selectionSize.X = Math.Abs(selectionEnd.X - selectionStart.X);
                    selectionSize.Y = Math.Abs(selectionEnd.Y - selectionStart.Y);

                    if ( selectionEnd.X > selectionStart.X )
                    {
                        if ( selectionEnd.Y > selectionStart.Y )
                        {
                            selectionOrigin.X = selectionStart.X;
                            selectionOrigin.Y = selectionStart.Y;
                        }
                        else
                        {
                            selectionOrigin.X = selectionStart.X;
                            selectionOrigin.Y = selectionEnd.Y;
                        }
                    }
                    else
                    {
                        if (selectionEnd.Y > selectionStart.Y)
                        {
                            selectionOrigin.X = selectionEnd.X;
                            selectionOrigin.Y = selectionStart.Y;
                        }
                        else
                        {
                            selectionOrigin.X = selectionEnd.X;
                            selectionOrigin.Y = selectionEnd.Y;
                        }
                    }

                    RectangleF rect = new RectangleF(selectionOrigin.X,
                                                      selectionOrigin.Y,
                                                      selectionSize.X,
                                                      selectionSize.Y);

                    //
                    // Estimate area. Doesn't count selection below 4 lamda square
                    //

                    float square = selectionSize.X * selectionSize.Y;

                    if (square >= 4.0F)
                    {
                        foreach (Entity ent in _entities)
                        {
                            if ( IsEntityCell(ent) )
                            {
                                RectangleF rect2 = new RectangleF(ent.LambdaX, ent.LambdaY,
                                                                   ent.LambdaWidth, ent.LambdaHeight);

                                if ( rect.IntersectsWith(rect2) && ent.Selected == false )
                                {
                                    ent.Selected = true;
                                    CatchSomething = true;
                                }
                            }
                            else if ( IsEntityWire(ent) )
                            {
                                PointF point1 = new PointF(ent.LambdaX, ent.LambdaY);
                                PointF point2 = new PointF(ent.LambdaEndX, ent.LambdaEndY);

                                if (LineIntersectsRect(point1, point2, rect) && ent.Selected == false)
                                {
                                    ent.Selected = true;
                                    CatchSomething = true;
                                }
                            }
                            else    // Vias
                            {
                                PointF point1 = new PointF(ent.LambdaX, ent.LambdaY);

                                if ( rect.Contains ( point1 ) && ent.Selected == false )
                                {
                                    ent.Selected = true;
                                    CatchSomething = true;
                                }
                            }
                        }

                        if (CatchSomething == true)
                            Invalidate();
                    }
                }

                Entity entity = EntityHitTest(e.X, e.Y);

                if (entity != null && CatchSomething == false)
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
                    if (draggingDist < 1.0F && CatchSomething == false )
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
            // Add cell
            //

            if (e.Button == MouseButtons.Left && (
                    Mode == EntityType.CellNot ||
                    Mode == EntityType.CellBuffer ||
                    Mode == EntityType.CellMux ||
                    Mode == EntityType.CellLogic ||
                    Mode == EntityType.CellAdder ||
                    Mode == EntityType.CellBusSupp ||
                    Mode == EntityType.CellFlipFlop ||
                    Mode == EntityType.CellLatch ||
                    Mode == EntityType.CellOther ||
                    Mode == EntityType.UnitRegfile ||
                    Mode == EntityType.UnitMemory ||
                    Mode == EntityType.UnitCustom
                    ) && DrawingBegin)
            {
                AddCell(Mode, SavedMouseX, SavedMouseY, e.X, e.Y);

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

            //
            // Clear selection box
            //

            if (SelectionBegin)
            {
                SelectionBegin = false;
                Invalidate();
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
                switch (Mode)
                {
                    case EntityType.Selection:
                    default:
                        ScrollX = SavedScrollX + e.X - SavedMouseX;
                        ScrollY = SavedScrollY + e.Y - SavedMouseY;
                        _imageScroll[0].X = _savedImageScroll[0].X + e.X - SavedMouseX;
                        _imageScroll[0].Y = _savedImageScroll[0].Y + e.Y - SavedMouseY;
                        _imageScroll[1].X = _savedImageScroll[1].X + e.X - SavedMouseX;
                        _imageScroll[1].Y = _savedImageScroll[1].Y + e.Y - SavedMouseY;
                        _imageScroll[2].X = _savedImageScroll[2].X + e.X - SavedMouseX;
                        _imageScroll[2].Y = _savedImageScroll[2].Y + e.Y - SavedMouseY;
                        break;

                    case EntityType.ImageLayer0:
                        _imageScroll[0].X = _savedImageScroll[0].X + e.X - SavedMouseX;
                        _imageScroll[0].Y = _savedImageScroll[0].Y + e.Y - SavedMouseY;
                        break;

                    case EntityType.ImageLayer1:
                        _imageScroll[1].X = _savedImageScroll[1].X + e.X - SavedMouseX;
                        _imageScroll[1].Y = _savedImageScroll[1].Y + e.Y - SavedMouseY;
                        break;

                    case EntityType.ImageLayer2:
                        _imageScroll[2].X = _savedImageScroll[2].X + e.X - SavedMouseX;
                        _imageScroll[2].Y = _savedImageScroll[2].Y + e.Y - SavedMouseY;
                        break;
                }

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
            // Cell drawing animation
            //

            if (DrawingBegin && (
                    Mode == EntityType.CellNot ||
                    Mode == EntityType.CellBuffer ||
                    Mode == EntityType.CellMux ||
                    Mode == EntityType.CellLogic ||
                    Mode == EntityType.CellAdder ||
                    Mode == EntityType.CellBusSupp ||
                    Mode == EntityType.CellFlipFlop ||
                    Mode == EntityType.CellLatch ||
                    Mode == EntityType.CellOther ||
                    Mode == EntityType.UnitRegfile ||
                    Mode == EntityType.UnitMemory ||
                    Mode == EntityType.UnitCustom ))
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

            //
            // Selection box animation
            //

            if ( SelectionBegin )
            {
                LastMouseX = e.X;
                LastMouseY = e.Y;
                Invalidate();
            }

            base.OnMouseMove(e);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            int delta;

            if (e.Delta > 0)
                delta = +10;
            else
                delta = -10;

            switch ( Mode )
            {
                case EntityType.Selection:
                default:
                    ZoomImage0 += delta;
                    ZoomImage1 += delta;
                    ZoomImage2 += delta;
                    Zoom += delta;
                    break;

                case EntityType.ImageLayer0:
                    ZoomImage0 += delta;
                    Invalidate();
                    break;

                case EntityType.ImageLayer1:
                    ZoomImage1 += delta;
                    Invalidate();
                    break;

                case EntityType.ImageLayer2:
                    ZoomImage2 += delta;
                    Invalidate();
                    break;
            }

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
            Color cellColor;

            switch (entity.Type)
            {
                //
                // Vias
                //

                case EntityType.ViasConnect:
                case EntityType.ViasFloating:
                case EntityType.ViasGround:
                case EntityType.ViasInout:
                case EntityType.ViasInput:
                case EntityType.ViasOutput:
                case EntityType.ViasPower:

                    if (hideVias == true)
                        break;

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

                    if (entity.ColorOverride != Color.Black)
                        viasColor = entity.ColorOverride;

                    viasColor = Color.FromArgb(ViasOpacity, viasColor);

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

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0)
                    {
                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = ViasTextAlignment;

                        SizeF textSize = gr.MeasureString(entity.Label, Font);

                        Point origin = new Point(centerX, centerY);

                        switch (align)
                        {
                            case TextAlignment.Top:
                            case TextAlignment.TopLeft:
                            case TextAlignment.TopRight:
                            default:
                                origin.Y = centerY - radius - (int)(textSize.Height * zf);
                                break;

                            case TextAlignment.Bottom:
                            case TextAlignment.BottomLeft:
                            case TextAlignment.BottomRight:
                                origin.Y = centerY + radius;
                                break;
                        }

                        switch (align)
                        {
                            case TextAlignment.Top:
                            case TextAlignment.Bottom:
                            default:
                                origin.X = centerX - (int)(textSize.Width * zf / 2);
                                break;

                            case TextAlignment.TopLeft:
                            case TextAlignment.BottomLeft:
                                origin.X = centerX - radius - (int)(textSize.Width * zf);
                                break;

                            case TextAlignment.TopRight:
                            case TextAlignment.BottomRight:
                                origin.X = centerX + radius;
                                break;
                        }

                        gr.TranslateTransform(origin.X, origin.Y);
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, Font, Brushes.Black, 0, 0);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Wires
                //

                case EntityType.WireGround:
                case EntityType.WirePower:
                case EntityType.WireInterconnect:

                    if (hideWires == true)
                        break;

                    if (entity.Type == EntityType.WireGround)
                        wireColor = WireGroundColor;
                    else if (entity.Type == EntityType.WirePower)
                        wireColor = WirePowerColor;
                    else if (entity.Type == EntityType.WireInterconnect)
                        wireColor = WireInterconnectColor;
                    else
                        wireColor = Color.Blue;

                    if (entity.ColorOverride != Color.Black)
                        wireColor = entity.ColorOverride;

                    wireColor = Color.FromArgb(WireOpacity, wireColor);

                    Point point1 = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point point2 = LambdaToScreen(entity.LambdaEndX, entity.LambdaEndY);

                    startX = point1.X;
                    startY = point1.Y;
                    endX = point2.X;
                    endY = point2.Y;

                    if (entity.Selected == true)
                    {
                        gr.DrawLine(new Pen(SelectionColor, (float)WireBaseSize * zf + (int)Lambda),
                                     startX, startY,
                                     endX, endY);
                    }

                    gr.DrawLine( new Pen(wireColor, (float)WireBaseSize * zf),
                                 startX, startY,
                                 endX, endY);

                    //
                    // Label
                    //

                    if ( entity.Label != null && entity.Label.Length > 0 )
                    {
                        Point start = new Point(startX, startY);
                        Point end = new Point(endX, endY);
                        Point temp;

                        if (startX == endX && startY == endY)
                            break;

                        if ( end.X < start.X )
                        {
                            temp = start;
                            start = end;
                            end = temp;
                        }

                        int a = end.Y - start.Y;
                        int b = end.X - start.X;
                        float Tga = (float)a / (float)b;
                        float alpha = (float)Math.Atan(Tga);

                        int wireLength = (int)(Math.Sqrt( Math.Pow(end.X - start.X, 2) +
                                                         Math.Pow(end.Y - start.Y, 2)) / (double)zf);

                        SizeF textSize = gr.MeasureString(entity.Label, Font);

                        int origin;

                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = WireTextAlignment;

                        switch (align)
                        {
                            case TextAlignment.BottomLeft:
                            case TextAlignment.TopLeft:
                            default:
                                origin = (int)(textSize.Width / entity.Label.Length);
                                break;
                            case TextAlignment.Top:
                            case TextAlignment.Bottom:
                                origin = wireLength / 2 - (int)textSize.Width / 2;
                                break;
                            case TextAlignment.BottomRight:
                            case TextAlignment.TopRight:
                                origin = wireLength - (int)textSize.Width - (int)(textSize.Width / entity.Label.Length);
                                break;
                        }

                        gr.TranslateTransform(start.X, start.Y);
                        gr.RotateTransform((float)(180.0F * alpha / Math.PI));
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, Font, Brushes.Black, origin, -textSize.Height / 2);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Cells and Units
                //

                case EntityType.CellNot:
                case EntityType.CellBuffer:
                case EntityType.CellMux:
                case EntityType.CellLogic:
                case EntityType.CellAdder:
                case EntityType.CellBusSupp:
                case EntityType.CellFlipFlop:
                case EntityType.CellLatch:
                case EntityType.CellOther:
                case EntityType.UnitRegfile:
                case EntityType.UnitMemory:
                case EntityType.UnitCustom:

                    if (hideCells == true)
                        break;

                    if (entity.Type == EntityType.CellNot)
                        cellColor = CellNotColor;
                    else if (entity.Type == EntityType.CellBuffer)
                        cellColor = CellBufferColor;
                    else if (entity.Type == EntityType.CellMux)
                        cellColor = CellMuxColor;
                    else if (entity.Type == EntityType.CellLogic)
                        cellColor = CellLogicColor;
                    else if (entity.Type == EntityType.CellAdder)
                        cellColor = CellAdderColor;
                    else if (entity.Type == EntityType.CellBusSupp)
                        cellColor = CellBusSuppColor;
                    else if (entity.Type == EntityType.CellFlipFlop)
                        cellColor = CellFlipFlopColor;
                    else if (entity.Type == EntityType.CellLatch)
                        cellColor = CellLatchColor;
                    else if (entity.Type == EntityType.CellOther)
                        cellColor = CellOtherColor;
                    else if (entity.Type == EntityType.UnitRegfile)
                        cellColor = UnitRegfileColor;
                    else if (entity.Type == EntityType.UnitMemory)
                        cellColor = UnitMemoryColor;
                    else if (entity.Type == EntityType.UnitCustom)
                        cellColor = UnitCustomColor;
                    else
                        cellColor = Color.Gray;

                    if (entity.ColorOverride != Color.Black)
                        cellColor = entity.ColorOverride;

                    cellColor = Color.FromArgb(CellOpacity, cellColor);

                    Point topLeft = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point bottomRight = LambdaToScreen( entity.LambdaX + entity.LambdaWidth,
                                                        entity.LambdaY + entity.LambdaHeight);
                    Point rectSize = new Point(bottomRight.X - topLeft.X,
                                               bottomRight.Y - topLeft.Y );

                    if ( entity.Selected )
                    {
                        gr.FillRectangle(new SolidBrush(SelectionColor),
                                           topLeft.X - (int)Lambda, topLeft.Y - (int)Lambda,
                                           rectSize.X + 2*(int)Lambda, rectSize.Y + 2*(int)Lambda);
                    }

                    gr.FillRectangle( new SolidBrush(cellColor), topLeft.X, topLeft.Y,
                                       rectSize.X, rectSize.Y);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0)
                    {
                        SizeF textSize = gr.MeasureString(entity.Label, Font);

                        Point origin = new Point(0, 0);

                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = CellTextAlignment;

                        rectSize.X = (int)((float)rectSize.X / zf);
                        rectSize.Y = (int)((float)rectSize.Y / zf);

                        switch (align)
                        {
                            case TextAlignment.TopLeft:
                            default:
                                origin.X = 0;
                                origin.Y = 0;
                                break;

                            case TextAlignment.BottomLeft:
                                origin.X = 0;
                                origin.Y = rectSize.Y - (int)(textSize.Height);
                                break;

                            case TextAlignment.Top:
                                origin.X = rectSize.X / 2 - (int)(textSize.Width / 2);
                                origin.Y = 0;
                                break;

                            case TextAlignment.Bottom:
                                origin.X = rectSize.X / 2 - (int)(textSize.Width / 2);
                                origin.Y = rectSize.Y - (int)textSize.Height;
                                break;

                            case TextAlignment.TopRight:
                                origin.X = rectSize.X - (int)textSize.Width;
                                origin.Y = 0;
                                break;

                            case TextAlignment.BottomRight:
                                origin.X = rectSize.X - (int)textSize.Width;
                                origin.Y = rectSize.Y - (int)textSize.Height;
                                break;
                        }

                        gr.TranslateTransform(topLeft.X, topLeft.Y);
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, Font, Brushes.Black, origin.X, origin.Y);
                        gr.ResetTransform();
                    }

                    break;
            }
        }
        
        private static Bitmap ChangeOpacity(Image img, float opacityvalue)
        {
            Bitmap bmp = new Bitmap(img.Width, img.Height); // Determining Width and Height of Source Image
            Graphics graphics = Graphics.FromImage(bmp);
            ColorMatrix colormatrix = new ColorMatrix();
            colormatrix.Matrix33 = opacityvalue;
            ImageAttributes imgAttribute = new ImageAttributes();
            imgAttribute.SetColorMatrix(colormatrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            graphics.DrawImage(img, new Rectangle(0, 0, bmp.Width, bmp.Height), 0, 0, img.Width, img.Height, GraphicsUnit.Pixel, imgAttribute);
            graphics.Dispose();   // Releasing all resource used by graphics 
            return bmp;
        }

        //
        // width / height - Scene viewport size
        // WholeScene - draw Scene for saving to bitmap
        // origin - Scene offset.
        //

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
            // Image Layers
            //

            for (int n = 2; n >= 0; n--)
            {
                if (_image[n] != null && hideImage == false)
                {
                    int zoom = _imageZoom[n];
                    int imageWidth = _image[n].Width * zoom / 100;
                    int imageHeight = _image[n].Height * zoom / 100;
                    int sx = _imageScroll[n].X;
                    int sy = _imageScroll[n].Y;

                    gr.DrawImage(_image[n],
                                  sx, sy,
                                  imageWidth,
                                  imageHeight);
                }
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

                if (WholeScene == false && DrawingBegin && (Mode == EntityType.WireGround ||
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
                    virtualEntity.Priority = WirePriority;
                    virtualEntity.ColorOverride = Color.Black;

                    DrawEntity(virtualEntity, gr);
                }

                //
                // Cell drawing animation
                //

                if (WholeScene == false && DrawingBegin && (
                        Mode == EntityType.CellNot ||
                        Mode == EntityType.CellBuffer ||
                        Mode == EntityType.CellMux ||
                        Mode == EntityType.CellLogic ||
                        Mode == EntityType.CellAdder ||
                        Mode == EntityType.CellBusSupp ||
                        Mode == EntityType.CellFlipFlop ||
                        Mode == EntityType.CellLatch ||
                        Mode == EntityType.CellOther ||
                        Mode == EntityType.UnitRegfile ||
                        Mode == EntityType.UnitMemory ||
                        Mode == EntityType.UnitCustom
                       ))
                {
                    Entity virtualEntity = new Entity();

                    PointF point1 = ScreenToLambda(SavedMouseX, SavedMouseY);
                    PointF point2 = ScreenToLambda(LastMouseX, LastMouseY);
                    PointF originPos = new PointF();
                    PointF size = new PointF();

                    size.X = Math.Abs(point2.X - point1.X);
                    size.Y = Math.Abs(point2.Y - point1.Y);

                    if (point2.X > point1.X)
                    {
                        if ( point2.Y > point1.Y )
                        {
                            originPos.X = point1.X;
                            originPos.Y = point1.Y;
                        }
                        else
                        {
                            originPos.X = point1.X;
                            originPos.Y = point2.Y;
                        }
                    }
                    else
                    {
                        if (point2.Y > point1.Y)
                        {
                            originPos.X = point2.X;
                            originPos.Y = point1.Y;
                        }
                        else
                        {
                            originPos.X = point2.X;
                            originPos.Y = point2.Y;
                        }
                    }

                    virtualEntity.LambdaX = originPos.X;
                    virtualEntity.LambdaY = originPos.Y;
                    virtualEntity.LambdaWidth = size.X;
                    virtualEntity.LambdaHeight = size.Y;
                    virtualEntity.Type = Mode;
                    virtualEntity.Priority = CellPriority;
                    virtualEntity.ColorOverride = Color.Black;
                    virtualEntity.Label = Mode.ToString();

                    DrawEntity(virtualEntity, gr);
                }

                //
                // Lambda Scale
                //

                if (WholeScene == false)
                    DrawLambdaScale(gr);
            }

            //
            // Select box animation
            //

            if (SelectionBegin && WholeScene == false)
            {
                Point[] points = new Point[5];

                points[0] = new Point(SelectStartMouseX, SelectStartMouseY);
                points[1] = new Point(SelectStartMouseX, LastMouseY);
                points[2] = new Point(LastMouseX, LastMouseY);
                points[3] = new Point(LastMouseX, SelectStartMouseY);
                points[4] = new Point(SelectStartMouseX, SelectStartMouseY);

                Pen pen = new Pen(SelectionBoxColor, 2);

                gr.DrawLines(pen, points);
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

        protected override void OnSizeChanged(EventArgs e)
        {
            Invalidate();
            base.OnSizeChanged(e);
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
        public Image Image0
        {
            get { return _image[0]; }
            set
            {
                if (_image[0] != value)
                {
                    _image[0] = _imageOrig[0] = value;
                    ScrollingBegin = false;
                    Invalidate();
                }
            }
        }

        [Category("Appearance"), DefaultValue(null)]
        public Image Image1
        {
            get { return _image[1]; }
            set
            {
                if (_image[1] != value)
                {
                    _image[1] = _imageOrig[1] = value;
                    ScrollingBegin = false;
                    Invalidate();
                }
            }
        }

        [Category("Appearance"), DefaultValue(null)]
        public Image Image2
        {
            get { return _image[2]; }
            set
            {
                if (_image[2] != value)
                {
                    _image[2] = _imageOrig[2] = value;
                    ScrollingBegin = false;
                    Invalidate();
                }
            }
        }

        [Category("Appearance")]
        public int ImageOpacity0
        {
            get { return _imageOpacity[0]; }
            set
            {
                if (_image[0] != null)
                {
                    _imageOpacity[0] = Math.Max(0, Math.Min(100, value));
                    _image[0] = ChangeOpacity(_imageOrig[0], (float)_imageOpacity[0] / 100);
                    Invalidate();
                }
            }
        }

        [Category("Appearance")]
        public int ImageOpacity1
        {
            get { return _imageOpacity[1]; }
            set
            {
                if (_image[1] != null)
                {
                    _imageOpacity[1] = Math.Max(0, Math.Min(100, value));
                    _image[1] = ChangeOpacity(_imageOrig[1], (float)_imageOpacity[1] / 100);
                    Invalidate();
                }
            }
        }

        [Category("Appearance")]
        public int ImageOpacity2
        {
            get { return _imageOpacity[2]; }
            set
            {
                if (_image[2] != null)
                {
                    _imageOpacity[2] = Math.Max(0, Math.Min(100, value));
                    _image[2] = ChangeOpacity(_imageOrig[2], (float)_imageOpacity[2] / 100);
                    Invalidate();
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

                if (drawMode == EntityType.Selection || 
                    drawMode == EntityType.ImageLayer0 || 
                    drawMode == EntityType.ImageLayer1 || 
                    drawMode == EntityType.ImageLayer2 )
                    DrawingBegin = false;
            }
        }

        [Category("Appearance")]
        public Point ScrollImage0
        {
            get { return _imageScroll[0]; }
            set { _imageScroll[0] = value; Invalidate(); }
        }

        [Category("Appearance")]
        public Point ScrollImage1
        {
            get { return _imageScroll[1]; }
            set { _imageScroll[1] = value; Invalidate(); }
        }

        [Category("Appearance")]
        public Point ScrollImage2
        {
            get { return _imageScroll[2]; }
            set { _imageScroll[2] = value; Invalidate(); }
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

        [Category("Appearance")]
        public bool HideVias
        {
            get { return hideVias; }
            set { hideVias = value; Invalidate(); }
        }

        [Category("Appearance")]
        public bool HideWires
        {
            get { return hideWires; }
            set { hideWires = value; Invalidate(); }
        }

        [Category("Appearance")]
        public bool HideCells
        {
            get { return hideCells; }
            set { hideCells = value; Invalidate(); }
        }

        [Category("Appearance")]
        public Color SelectionBoxColor
        {
            get { return selectionBoxColor; }
            set { selectionBoxColor = value; Invalidate(); }
        }

        private void AddVias ( EntityType Type, int ScreenX, int ScreenY )
        {
            Entity item = new Entity();

            PointF point = ScreenToLambda(ScreenX, ScreenY);

            item.Label = "";
            item.LambdaX = point.X;
            item.LambdaY = point.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = Color.Black;
            item.Priority = ViasPriority;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();
        }

        private void AddWire ( EntityType Type, int StartX, int StartY, int EndX, int EndY )
        {
            Entity item = new Entity();

            PointF point1 = ScreenToLambda(StartX, StartY);
            PointF point2 = ScreenToLambda(EndX, EndY);

            float len = (float)Math.Sqrt( Math.Pow(point2.X - point1.X, 2) + 
                                          Math.Pow(point2.Y - point1.Y, 2));

            if (len < 1.0F)
            {
                Invalidate();
                return;
            }

            item.Label = "";
            item.LambdaX = point1.X;
            item.LambdaY = point1.Y;
            item.LambdaEndX = point2.X;
            item.LambdaEndY = point2.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = Color.Black;
            item.Priority = WirePriority;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();
        }

        private void AddCell(EntityType Type, int StartX, int StartY, int EndX, int EndY)
        {
            Entity item = new Entity();

            PointF point1 = ScreenToLambda(StartX, StartY);
            PointF point2 = ScreenToLambda(EndX, EndY);

            PointF originPos = new PointF();
            PointF size = new PointF();

            size.X = Math.Abs(point2.X - point1.X);
            size.Y = Math.Abs(point2.Y - point1.Y);

            float square = size.X * size.Y;

            if (square < 4.0F)
            {
                Invalidate();
                return;
            }

            if (point2.X > point1.X)
            {
                if (point2.Y > point1.Y)
                {
                    originPos.X = point1.X;
                    originPos.Y = point1.Y;
                }
                else
                {
                    originPos.X = point1.X;
                    originPos.Y = point2.Y;
                }
            }
            else
            {
                if (point2.Y > point1.Y)
                {
                    originPos.X = point2.X;
                    originPos.Y = point1.Y;
                }
                else
                {
                    originPos.X = point2.X;
                    originPos.Y = point2.Y;
                }
            }

            item.Label = "";
            item.LambdaX = originPos.X;
            item.LambdaY = originPos.Y;
            item.LambdaEndX = 1F;
            item.LambdaEndY = 1F;
            item.LambdaWidth = size.X;
            item.LambdaHeight = size.Y;
            item.Type = Type;
            item.ColorOverride = Color.Black;
            item.Priority = CellPriority;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();
        }

        [Category("Appearance")]
        public int Zoom
        {
            get { return _zoom; }
            set
            {
                int oldZoom = _zoom;
                float oldzf = (float)oldZoom / 100.0F;

                if (value < 30)
                    value = 30;

                if (value > 400)
                    value = 400;

                _zoom = value;
                
/*
                float zf = (float)Zoom / 100.0F;

                Point origin;
                Point sceneSize = DetermineSceneSize(out origin);

                float deltaX = Math.Abs(sceneSize.X * zf - sceneSize.X * oldzf) / 2;
                float deltaY = Math.Abs(sceneSize.Y * zf - sceneSize.Y * oldzf) / 2;

                if (_zoom < oldZoom)  // Zoom out
                {
                    _ScrollX += (int)deltaX;
                    _ScrollY += (int)deltaY;
                }
                else if (_zoom > oldZoom) // Zoom in
                {
                    _ScrollX -= (int)deltaX;
                    _ScrollY -= (int)deltaY;
                }
*/

                Invalidate();
            }
        }

        [Category("Appearance")]
        public int ZoomImage0
        {
            get { return _imageZoom[0]; }
            set
            {
                if (value < 30)
                    value = 30;

                if (value > 400)
                    value = 400;

                _imageZoom[0] = value;

                Invalidate();
            }
        }

        [Category("Appearance")]
        public int ZoomImage1
        {
            get { return _imageZoom[1]; }
            set
            {
                if (value < 30)
                    value = 30;

                if (value > 400)
                    value = 400;

                _imageZoom[1] = value;

                Invalidate();
            }
        }

        [Category("Appearance")]
        public int ZoomImage2
        {
            get { return _imageZoom[2]; }
            set
            {
                if (value < 30)
                    value = 30;

                if (value > 400)
                    value = 400;

                _imageZoom[2] = value;

                Invalidate();
            }
        }

        public void DeleteAllEntites ()
        {
            _entities.Clear();
            Invalidate();
        }

        //
        // Scene size and origin in screen coordinate space
        //

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

            //
            // Space occupied by Image layers
            //

            for (int n = 2; n >= 0; n--)
            {
                if (_image[n] != null && HideImage == false)
                {
                    float zf = (float)_imageZoom[n] / 100F;

                    if (_imageScroll[n].X < originOut.X)
                        originOut.X = _imageScroll[n].X;

                    if (_imageScroll[n].Y < originOut.Y)
                        originOut.Y = _imageScroll[n].Y;

                    int rightSide = (int)((float)_image[n].Width * zf) + _imageScroll[n].X;
                    int bottomSide = (int)((float)_image[n].Height * zf) + _imageScroll[n].Y;

                    if (rightSide > point.X )
                        point.X = rightSide;

                    if (bottomSide > point.Y)
                        point.Y = bottomSide;
                }
            }

            //
            // Space occupied by Entities
            //

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

        private void WipeGarbage ()
        {
            //
            // Wipe small wires (< 1 lambda)
            //

            List<Entity> pendingDelete = new List<Entity>();

            foreach (Entity entity in _entities)
            {
                if (IsEntityWire(entity))
                {
                    float len = (float)Math.Sqrt(Math.Pow(entity.LambdaEndX - entity.LambdaX, 2) +
                                                   Math.Pow(entity.SavedLambdaEndY - entity.LambdaY, 2));

                    if (len < 1.0F)
                        pendingDelete.Add(entity);
                }
            }

            if (pendingDelete.Count > 0)
            {
                foreach (Entity entity in pendingDelete)
                {
                    _entities.Remove(entity);
                }
            }
        }

        public void Serialize (string FileName)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            WipeGarbage();

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

                WipeGarbage();

                _entities = _entities.OrderBy(o => o.Priority).ToList();

                Invalidate();
            }
        }

        //
        // Entity Selection-related
        //

        public static bool LineIntersectsRect(PointF p1, PointF p2, RectangleF r)
        {
            return LineIntersectsLine(p1, p2, new PointF(r.X, r.Y), new PointF(r.X + r.Width, r.Y)) ||
                   LineIntersectsLine(p1, p2, new PointF(r.X + r.Width, r.Y), new PointF(r.X + r.Width, r.Y + r.Height)) ||
                   LineIntersectsLine(p1, p2, new PointF(r.X + r.Width, r.Y + r.Height), new PointF(r.X, r.Y + r.Height)) ||
                   LineIntersectsLine(p1, p2, new PointF(r.X, r.Y + r.Height), new PointF(r.X, r.Y)) ||
                   (r.Contains(p1) && r.Contains(p2));
        }

        private static bool LineIntersectsLine(PointF l1p1, PointF l1p2, PointF l2p1, PointF l2p2)
        {
            float q = (l1p1.Y - l2p1.Y) * (l2p2.X - l2p1.X) - (l1p1.X - l2p1.X) * (l2p2.Y - l2p1.Y);
            float d = (l1p2.X - l1p1.X) * (l2p2.Y - l2p1.Y) - (l1p2.Y - l1p1.Y) * (l2p2.X - l2p1.X);

            if (d == 0)
            {
                return false;
            }

            float r = q / d;

            q = (l1p1.Y - l2p1.Y) * (l1p2.X - l1p1.X) - (l1p1.X - l2p1.X) * (l1p2.Y - l1p1.Y);
            float s = q / d;

            if (r < 0 || r > 1 || s < 0 || s > 1)
            {
                return false;
            }

            return true;
        }

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

        public void RemoveSelection ()
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

        public void DeleteSelected ()
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

        #region Entity Props

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
        private Color _CellOtherColor;
        private Color _UnitRegfileColor;
        private Color _UnitMemoryColor;
        private Color _UnitCustomColor;
        private Color _SelectionColor;
        private ViasShape _viasShape;
        private int _viasBaseSize;
        private int _wireBaseSize;
        private TextAlignment _cellTextAlignment;
        private TextAlignment _viasTextAlignment;
        private TextAlignment _wireTextAlignment;
        private int _ViasOpacity;
        private int _WireOpacity;
        private int _CellOpacity;
        private int _ViasPriority;
        private int _WirePriority;
        private int _CellPriority;
        private bool _AutoPriority;

        private void DefaultEntityAppearance()
        {
            _viasShape = ViasShape.Round;
            _viasBaseSize = Math.Max(1, (int)Lambda - 1);
            _wireBaseSize = (int)Lambda;
            _cellTextAlignment = TextAlignment.TopLeft;
            _viasTextAlignment = TextAlignment.Top;
            _wireTextAlignment = TextAlignment.TopLeft;

            _ViasInputColor = Color.Green;
            _ViasOutputColor = Color.Red;
            _ViasInoutColor = Color.Yellow;
            _ViasConnectColor = Color.Black;
            _ViasFloatingColor = Color.Gray;
            _ViasPowerColor = Color.Black;
            _ViasGroundColor = Color.Black;

            _WireInterconnectColor = Color.Blue;
            _WirePowerColor = Color.Red;
            _WireGroundColor = Color.Green;

            _CellNotColor = Color.Navy;
            _CellBufferColor = Color.Navy;
            _CellMuxColor = Color.DarkOrange;
            _CellLogicColor = Color.Yellow;
            _CellAdderColor = Color.Red;
            _CellBusSuppColor = Color.DarkViolet;
            _CellFlipFlopColor = Color.Lime;
            _CellLatchColor = Color.SpringGreen;
            _CellOtherColor = Color.Snow;

            _UnitRegfileColor = Color.Snow;
            _UnitMemoryColor = Color.Snow;
            _UnitCustomColor = Color.Snow;

            _SelectionColor = Color.LimeGreen;

            _ViasOpacity = 255;
            _WireOpacity = 128;
            _CellOpacity = 128;

            _ViasPriority = 3;
            _WirePriority = 2;
            _CellPriority = 1;
            _AutoPriority = true;
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
        public TextAlignment CellTextAlignment
        {
            get { return _cellTextAlignment; }
            set { _cellTextAlignment = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public TextAlignment WireTextAlignment
        {
            get { return _wireTextAlignment; }
            set { _wireTextAlignment = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public TextAlignment ViasTextAlignment
        {
            get { return _viasTextAlignment; }
            set { _viasTextAlignment = value; Invalidate(); }
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
        public Color CellOtherColor
        {
            get { return _CellOtherColor; }
            set { _CellOtherColor = value; Invalidate(); }
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

        [Category("Entity Appearance")]
        public int ViasOpacity
        {
            get { return _ViasOpacity; }
            set
            {
                _ViasOpacity = Math.Max (0, Math.Min(255, value));
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public int WireOpacity
        {
            get { return _WireOpacity; }
            set
            {
                _WireOpacity = Math.Max(0, Math.Min(255, value));
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public int CellOpacity
        {
            get { return _CellOpacity; }
            set
            {
                _CellOpacity = Math.Max(0, Math.Min(255, value));
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public int ViasPriority
        {
            get { return _ViasPriority; }
            set
            {
                _ViasPriority = value;
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public int WirePriority
        {
            get { return _WirePriority; }
            set
            {
                _WirePriority = value;
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public int CellPriority
        {
            get { return _CellPriority; }
            set
            {
                _CellPriority = value;
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public bool AutoPriority
        {
            get { return _AutoPriority; }
            set
            {
                _AutoPriority = value;
                SortEntities();
                Invalidate();
            }
        }

        #endregion Entity Props

        //
        // Key input handling
        //

        protected override void OnKeyUp(KeyEventArgs e)
        {
            if ( e.KeyCode == Keys.Delete )
                DeleteSelected();

            else if (e.KeyCode == Keys.Escape)
                RemoveSelection();

            else if ( ( e.KeyCode == Keys.Right ||
                        e.KeyCode == Keys.Left ||
                        e.KeyCode == Keys.Up ||
                        e.KeyCode == Keys.Down ) && Mode == EntityType.Selection)
            {
                bool NeedUpdate = false;
                float deltaX = 0;
                float deltaY = 0;

                switch (e.KeyCode)
                {
                    case Keys.Right:
                        deltaX = +0.1F;
                        deltaY = 0;
                        break;
                    case Keys.Left:
                        deltaX = -0.1F;
                        deltaY = 0;
                        break;
                    case Keys.Up:
                        deltaX = 0;
                        deltaY = -0.1F;
                        break;
                    case Keys.Down:
                        deltaX = 0;
                        deltaY = +0.1F;
                        break;
                }

                foreach (Entity entity in _entities)
                {
                    if (entity.Selected)
                    {
                        entity.LambdaX += deltaX;
                        entity.LambdaY += deltaY;
                        entity.LambdaEndX += deltaX;
                        entity.LambdaEndY += deltaY;
                        NeedUpdate = true;
                    }
                }

                if (NeedUpdate)
                    Invalidate();
            }

            base.OnKeyUp(e);
        }

        //
        // Priority stuff
        //

        public void SortEntities()
        {
            if ( AutoPriority == true )
                _entities = _entities.OrderBy(o => o.Priority).ToList();
        }

        //
        // Wire merger. This operation is potentially dangerous!!!
        //

        public void MergeSelectedWires (bool Vertical)
        {
            List<Entity> selectedWires = new List<Entity>();

            //
            // Grab selected 
            //

            foreach ( Entity entity in _entities )
            {
                if ( IsEntityWire(entity) && entity.Selected == true)
                {
                    selectedWires.Add(entity);
                }
            }

            if (selectedWires.Count == 0)
                return;

            //
            // Find left-most and right-most points + Determine average wire type
            //

            PointF left = new PointF(float.MaxValue, float.MaxValue);
            PointF right = new PointF(float.MinValue, float.MinValue);
            int [] types = { 0, 0, 0 };

            foreach ( Entity entity in selectedWires )
            {
                if (Vertical == true)
                {
                    if (entity.LambdaY < left.Y)
                    {
                        left.X = entity.LambdaX;
                        left.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndY < left.Y)
                    {
                        left.X = entity.LambdaEndX;
                        left.Y = entity.LambdaEndY;
                    }
                    if (entity.LambdaY > right.Y)
                    {
                        right.X = entity.LambdaX;
                        right.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndY > right.Y)
                    {
                        right.X = entity.LambdaEndX;
                        right.Y = entity.LambdaEndY;
                    }
                }
                else
                {
                    if (entity.LambdaX < left.X)
                    {
                        left.X = entity.LambdaX;
                        left.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndX < left.X)
                    {
                        left.X = entity.LambdaEndX;
                        left.Y = entity.LambdaEndY;
                    }
                    if (entity.LambdaX > right.X)
                    {
                        right.X = entity.LambdaX;
                        right.Y = entity.LambdaY;
                    }
                    if (entity.LambdaEndX > right.X)
                    {
                        right.X = entity.LambdaEndX;
                        right.Y = entity.LambdaEndY;
                    }
                }

                switch ( entity.Type )
                {
                    case EntityType.WireGround:
                        types[0]++;
                        break;
                    case EntityType.WireInterconnect:
                        types[1]++;
                        break;
                    case EntityType.WirePower:
                        types[2]++;
                        break;
                }
            }

            //
            // Remove selected
            //

            foreach (Entity entity in selectedWires)
            {
                _entities.Remove(entity);
            }

            //
            // Add merger
            //

            Point start = LambdaToScreen(left.X, left.Y);
            Point end = LambdaToScreen(right.X, right.Y);

            int maxValue = types.Max();
            int index = types.ToList().IndexOf(maxValue);

            EntityType type;

            switch(index)
            {
                case 0:
                    type = EntityType.WireGround;
                    break;
                case 1:
                default:
                    type = EntityType.WireInterconnect;
                    break;
                case 2:
                    type = EntityType.WirePower;
                    break;
            }

            AddWire(type, start.X, start.Y, end.X, end.Y);

            Invalidate();
        }

        public void LoadImage (Image image)
        {
            switch ( drawMode )
            {
                case EntityType.ImageLayer0:
                default:
                    _image[0] = _imageOrig[0] = image;
                    Invalidate();
                    break;
                case EntityType.ImageLayer1:
                    _image[1] = _imageOrig[1] = image;
                    Invalidate();
                    break;
                case EntityType.ImageLayer2:
                    _image[2] = _imageOrig[2] = image;
                    Invalidate();
                    break;
            }
        }
    }
}
