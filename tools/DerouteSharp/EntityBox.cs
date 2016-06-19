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
    public delegate void EntityBoxEventHandler(object sender, EventArgs e);
    public delegate void EntityBoxEntityEventHandler(object sender, Entity entity, EventArgs e);
    public delegate void EntityBoxFrameDoneHandler(object sender, long ms_time, EventArgs e);

    public enum EntitySelection
    {
        Vias,
        Wire,
        Cell,
        All,
    }

    public enum EntityMode
    {
        Selection = 0,

        ImageLayer0,
        ImageLayer1,
        ImageLayer2,

        ViasInput = 0x80,
        ViasOutput,
        ViasInout,
        ViasConnect,
        ViasFloating,
        ViasPower,
        ViasGround,
        WireInterconnect,
        WirePower,
        WireGround,
        CellNot,
        CellBuffer,
        CellMux,
        CellLogic,
        CellAdder,
        CellBusSupp,
        CellFlipFlop,
        CellLatch,
        CellOther,
        UnitRegfile,
        UnitMemory,
        UnitCustom,
        Beacon,
    }

    public enum EntityBoxOperation
    {
        Unknown = 0,
        ImageScroll0,
        ImageScroll1,
        ImageScroll2,
        ImageZoom0,
        ImageZoom1,
        ImageZoom2,
        EntityMove,
        EntityAddSingle,
        EntityAddBulk,
        EntityDelete,
        EntityMergeWires,
    }

    public partial class EntityBox : Control
    {
        private Image beaconImage = null;
        private Image[] _imageOrig = new Image[3];
        private float _lambda;
        private int[] _imageZoom = new int[3];
        private int _zoom;
        private PointF[] _imageScroll = new PointF[3];
        private PointF[] _savedImageScroll = new PointF[3];
        private float _ScrollX;
        private float _ScrollY;
        private float SavedScrollX;
        private float SavedScrollY;
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
        public List<Entity> _entities;
        private EntityMode drawMode = EntityMode.Selection;
        private bool hideImage;
        private bool hideVias;
        private bool hideWires;
        private bool hideCells;
        private bool hideGrid;
        private bool hideRegions;
        private PropertyGrid entityGrid;
        private List<Entity> selected;
        private float draggingDist;
        private Color selectionBoxColor;
        private int[] _imageOpacity = new int[3];
        private BufferedGraphics gfx = null;
        private BufferedGraphicsContext context;
        private EntityBoxOperation lastOp = EntityBoxOperation.Unknown;
        private object lastOpData;
        private List<Entity> lastEntities = new List<Entity>();
        private EntityBoxOperation cancelledOp = EntityBoxOperation.Unknown;
        private object cancelledOpData;
        private List<Entity> cancelledEntities = new List<Entity>();
        private bool[] lockScroll = new bool[3];
        private bool[] lockZoom = new bool[3];
        private bool selectEntitiesAfterAdd;
        private long UnserializeLastStamp = 0;
        private bool grayscale = false;
        private Point LastRMB = new Point(-1, -1);

        public event EntityBoxEventHandler OnScrollChanged = null;
        public event EntityBoxEventHandler OnZoomChanged = null;
        public event EntityBoxEventHandler OnEntityCountChanged = null;
        public event EntityBoxEventHandler OnLastOperation = null;
        public event EntityBoxEntityEventHandler OnEntityLabelEdit = null;
        public event EntityBoxFrameDoneHandler OnFrameDone = null;

        public EntityBox()
        {
            BackColor = SystemColors.WindowFrame;
            ForeColor = Color.Snow;

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
            SelectEntitiesAfterAdd = true;

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

        private bool IsEntityRegion(Entity entity)
        {
            return (entity.Type == EntityType.Region );
        }

        //
        // Coordinate space convertion
        //
        // lx = (sx - scroll) / (zoom * lambda)
        //
        // sx = lx * zoom * lambda + scroll
        //

        private PointF ScreenToLambda(int ScreenX, int ScreenY)
        {
            PointF point = new PointF(0.0F, 0.0F);
            float zf = (float)Zoom / 100F;

            point.X = (float)ScreenX / (zf * Lambda) - ScrollX;
            point.Y = (float)ScreenY / (zf * Lambda) - ScrollY;

            return point;
        }

        private Point LambdaToScreen(float LambdaX, float LambdaY)
        {
            Point point = new Point(0, 0);
            float zf = (float)Zoom / 100F;

            float x = (LambdaX + ScrollX) * (zf * Lambda);
            float y = (LambdaY + ScrollY) * (zf * Lambda);

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

        private Entity EntityHitTest(int MouseX, int MouseY)
        {
            PointF point = new Point(MouseX, MouseY);
            PointF[] rect = new PointF[4];
            float zf = (float)Zoom / 100.0F;

            List<Entity> reversed = new List<Entity>();

            reversed = _entities.OrderByDescending(o => o.Priority).ToList();

            foreach (Entity entity in reversed)
            {
                if (IsEntityWire(entity) && HideWires == false)
                {
                    PointF start = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    PointF end = LambdaToScreen(entity.LambdaEndX, entity.LambdaEndY);

                    if (end.X < start.X)
                    {
                        PointF temp = start;
                        start = end;
                        end = temp;
                    }

                    PointF ortho = new PointF(end.X - start.X, end.Y - start.Y);

                    float len = (float)Math.Sqrt(Math.Pow(ortho.X, 2) +
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
                else if (IsEntityCell(entity) && HideCells == false)
                {
                    rect[0] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[1] = LambdaToScreen(entity.LambdaX, entity.LambdaY + entity.LambdaHeight);
                    rect[2] = LambdaToScreen(entity.LambdaX + entity.LambdaWidth, entity.LambdaY + entity.LambdaHeight);
                    rect[3] = LambdaToScreen(entity.LambdaX + entity.LambdaWidth, entity.LambdaY);

                    if (PointInPoly(rect, point) == true)
                        return entity;
                }
                else if ( entity.Type == EntityType.Beacon )
                {
                    Point beaconOrigin = LambdaToScreen(entity.LambdaX, entity.LambdaY);

                    Point imageOrigin = new Point(beaconOrigin.X - (int)((float)(beaconImage.Width * zf) / 2),
                                                    beaconOrigin.Y - (int)((float)beaconImage.Height * zf));

                    rect[0] = new PointF(imageOrigin.X, imageOrigin.Y);
                    rect[1] = new PointF(imageOrigin.X, imageOrigin.Y + beaconImage.Height * zf);
                    rect[2] = new PointF(imageOrigin.X + beaconImage.Width * zf, imageOrigin.Y + beaconImage.Height * zf);
                    rect[3] = new PointF(imageOrigin.X + beaconImage.Width * zf, imageOrigin.Y);

                    if (PointInPoly(rect, point) == true)
                        return entity;
                }
                else if ( IsEntityVias(entity) && HideVias == false )
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
                else if ( IsEntityRegion(entity))
                {
                    PointF[] poly = new PointF[entity.PathPoints.Count];

                    int idx = 0;
                    foreach ( PointF pathPoint in entity.PathPoints )
                    {
                        poly[idx++] = (PointF) LambdaToScreen ( pathPoint.X, pathPoint.Y );
                    }

                    if (PointInPoly(poly, point) == true)
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

            if (e.Button == MouseButtons.Left && Mode != EntityMode.Selection &&
                 Mode != EntityMode.ImageLayer0 && Mode != EntityMode.ImageLayer1 && Mode != EntityMode.ImageLayer2 &&
                 DrawingBegin == false && ScrollingBegin == false)
            {
                Entity entity;
                bool Okay;

                //
                // Cannot draw cells / custom blocks over other entites
                //

                Okay = true;

                entity = EntityHitTest(e.X, e.Y);
                if (entity != null && (Mode == EntityMode.CellAdder ||
                     Mode == EntityMode.CellBuffer || Mode == EntityMode.CellBusSupp ||
                     Mode == EntityMode.CellFlipFlop || Mode == EntityMode.CellLatch ||
                     Mode == EntityMode.CellLogic || Mode == EntityMode.CellMux ||
                     Mode == EntityMode.CellNot || Mode == EntityMode.CellOther || Mode == EntityMode.UnitCustom ||
                     Mode == EntityMode.UnitMemory || Mode == EntityMode.UnitRegfile))
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

            if (e.Button == MouseButtons.Left && Mode == EntityMode.Selection
                 && DraggingBegin == false && SelectionBegin == false )
            {
                selected = GetSelected();

                if (selected.Count > 0)
                {
                    foreach (Entity entity in selected)
                    {
                        if (IsEntityRegion(entity))
                        {
                            entity.SavedPathPoints = new List<PointF>(entity.PathPoints);
                        }
                        else
                        {
                            entity.SavedLambdaX = entity.LambdaX;
                            entity.SavedLambdaY = entity.LambdaY;

                            entity.SavedLambdaEndX = entity.LambdaEndX;
                            entity.SavedLambdaEndY = entity.LambdaEndY;
                        }
                    }

                    lastEntities = new List<Entity>();
                    foreach (Entity entity in _entities)
                    {
                        lastEntities.Add(entity);
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

            long timeStampNow = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            if ((timeStampNow - UnserializeLastStamp) < 500)
                return;

            if ( e.Button == MouseButtons.Right )
            {
                LastRMB.X = e.X;
                LastRMB.Y = e.Y;
            }
            else
            {
                LastRMB.X = -1;
                LastRMB.Y = -1;
            }

            if (e.Button == MouseButtons.Right && ScrollingBegin)
            {
                switch ( Mode )
                {
                    case EntityMode.ImageLayer0:
                        LastOpWrapper = EntityBoxOperation.ImageScroll0;
                        lastOpData = (object)_savedImageScroll[0];
                        cancelledOp = EntityBoxOperation.Unknown;
                        break;
                    case EntityMode.ImageLayer1:
                        LastOpWrapper = EntityBoxOperation.ImageScroll1;
                        lastOpData = (object)_savedImageScroll[1];
                        cancelledOp = EntityBoxOperation.Unknown;
                        break;
                    case EntityMode.ImageLayer2:
                        LastOpWrapper = EntityBoxOperation.ImageScroll2;
                        lastOpData = (object)_savedImageScroll[2];
                        cancelledOp = EntityBoxOperation.Unknown;
                        break;
                }

                ScrollingBegin = false;
                Invalidate();
            }

            //
            // Select entity
            //

            if (e.Button == MouseButtons.Left && Mode == EntityMode.Selection)
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

                    if (selectionEnd.X > selectionStart.X)
                    {
                        if (selectionEnd.Y > selectionStart.Y)
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
                    // Estimate area. Doesn't count selection below 4 lambda square
                    //

                    float square = selectionSize.X * selectionSize.Y;

                    if (square >= 4.0F)
                    {
                        foreach (Entity ent in _entities)
                        {
                            if (IsEntityCell(ent))
                            {
                                RectangleF rect2 = new RectangleF(ent.LambdaX, ent.LambdaY,
                                                                   ent.LambdaWidth, ent.LambdaHeight);

                                if (rect.IntersectsWith(rect2) && ent.Selected == false)
                                {
                                    ent.Selected = true;
                                    CatchSomething = true;
                                }
                            }
                            else if (IsEntityWire(ent))
                            {
                                PointF point1 = new PointF(ent.LambdaX, ent.LambdaY);
                                PointF point2 = new PointF(ent.LambdaEndX, ent.LambdaEndY);

                                if (LineIntersectsRect(point1, point2, rect) && ent.Selected == false)
                                {
                                    ent.Selected = true;
                                    CatchSomething = true;
                                }
                            }
                            else if (IsEntityVias(ent))
                            {
                                PointF point1 = new PointF(ent.LambdaX, ent.LambdaY);

                                if (rect.Contains(point1) && ent.Selected == false)
                                {
                                    ent.Selected = true;
                                    CatchSomething = true;
                                }
                            }
                            else if (IsEntityRegion(ent) && ent.Selected == false)
                            {
                                foreach ( PointF point in ent.PathPoints )
                                {
                                    if ( rect.Contains(point))
                                    {
                                        ent.Selected = true;
                                        CatchSomething = true;
                                    }
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
                    if (entity.Selected == true && draggingDist < 1F)
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
                    if (draggingDist < 1F && CatchSomething == false)
                        RemoveSelection();
                }
            }

            //
            // Add vias
            //

            if (e.Button == MouseButtons.Left &&
                 (Mode == EntityMode.ViasConnect || Mode == EntityMode.ViasFloating || Mode == EntityMode.ViasGround ||
                  Mode == EntityMode.ViasInout || Mode == EntityMode.ViasInput || Mode == EntityMode.ViasOutput ||
                  Mode == EntityMode.ViasPower) && DrawingBegin)
            {
                AddVias((EntityType)Mode, e.X, e.Y);

                DrawingBegin = false;
            }

            //
            // Add beacon
            //

            if (e.Button == MouseButtons.Left &&
                Mode == EntityMode.Beacon && DrawingBegin)
            {
                AddBeacon(e.X, e.Y);

                DrawingBegin = false;
            }

            //
            // Add wire
            //

            if (e.Button == MouseButtons.Left && (Mode == EntityMode.WireGround ||
                  Mode == EntityMode.WireInterconnect || Mode == EntityMode.WirePower) && DrawingBegin)
            {
                AddWire((EntityType)Mode, SavedMouseX, SavedMouseY, e.X, e.Y);

                DrawingBegin = false;
            }

            //
            // Add cell
            //

            if (e.Button == MouseButtons.Left && (
                    Mode == EntityMode.CellNot ||
                    Mode == EntityMode.CellBuffer ||
                    Mode == EntityMode.CellMux ||
                    Mode == EntityMode.CellLogic ||
                    Mode == EntityMode.CellAdder ||
                    Mode == EntityMode.CellBusSupp ||
                    Mode == EntityMode.CellFlipFlop ||
                    Mode == EntityMode.CellLatch ||
                    Mode == EntityMode.CellOther ||
                    Mode == EntityMode.UnitRegfile ||
                    Mode == EntityMode.UnitMemory ||
                    Mode == EntityMode.UnitCustom
                    ) && DrawingBegin)
            {
                AddCell((EntityType)Mode, SavedMouseX, SavedMouseY, e.X, e.Y);

                DrawingBegin = false;
            }

            //
            // End Drag
            //

            if (e.Button == MouseButtons.Left && DraggingBegin)
            {
                //
                // Fix move jitter
                //

                if ( draggingDist < 2 * Lambda )
                {
                    foreach ( Entity entity in selected )
                    {
                        entity.LambdaX = entity.SavedLambdaX;
                        entity.LambdaY = entity.SavedLambdaY;
                        entity.LambdaEndX = entity.SavedLambdaEndX;
                        entity.LambdaEndY = entity.SavedLambdaEndY;
                    }
                }
                else
                {
                    LastOpWrapper = EntityBoxOperation.EntityMove;
                    cancelledOp = EntityBoxOperation.Unknown;
                }

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

            Point screenCoord = new Point();

            if (ScrollingBegin)
            {
                switch (Mode)
                {
                    case EntityMode.Selection:
                    default:
                        screenCoord = LambdaToScreen(SavedScrollX, SavedScrollY);

                        PointF lambdaCoord = ScreenToLambda(screenCoord.X + e.X - SavedMouseX,
                                                          screenCoord.Y + e.Y - SavedMouseY);

                        ScrollX = lambdaCoord.X;
                        ScrollY = lambdaCoord.Y;
                        break;

                    case EntityMode.ImageLayer0:
                        if (LockScroll0 == false)
                        {
                            screenCoord = LambdaToScreen(_savedImageScroll[0].X, _savedImageScroll[0].Y);

                            _imageScroll[0] = ScreenToLambda(screenCoord.X + e.X - SavedMouseX,
                                                              screenCoord.Y + e.Y - SavedMouseY);
                        }
                        break;

                    case EntityMode.ImageLayer1:
                        if (LockScroll1 == false)
                        {
                            screenCoord = LambdaToScreen(_savedImageScroll[1].X, _savedImageScroll[1].Y);

                            _imageScroll[1] = ScreenToLambda(screenCoord.X + e.X - SavedMouseX,
                                                              screenCoord.Y + e.Y - SavedMouseY);
                        }
                        break;

                    case EntityMode.ImageLayer2:
                        if (LockScroll2 == false)
                        {
                            screenCoord = LambdaToScreen(_savedImageScroll[2].X, _savedImageScroll[2].Y);

                            _imageScroll[2] = ScreenToLambda(screenCoord.X + e.X - SavedMouseX,
                                                              screenCoord.Y + e.Y - SavedMouseY);
                        }
                        break;
                }

                Invalidate();
            }

            //
            // Wire drawing animation
            //

            if (DrawingBegin && (Mode == EntityMode.WireGround ||
                   Mode == EntityMode.WireInterconnect || Mode == EntityMode.WirePower))
            {
                LastMouseX = e.X;
                LastMouseY = e.Y;
                Invalidate();
            }

            //
            // Cell drawing animation
            //

            if (DrawingBegin && (
                    Mode == EntityMode.CellNot ||
                    Mode == EntityMode.CellBuffer ||
                    Mode == EntityMode.CellMux ||
                    Mode == EntityMode.CellLogic ||
                    Mode == EntityMode.CellAdder ||
                    Mode == EntityMode.CellBusSupp ||
                    Mode == EntityMode.CellFlipFlop ||
                    Mode == EntityMode.CellLatch ||
                    Mode == EntityMode.CellOther ||
                    Mode == EntityMode.UnitRegfile ||
                    Mode == EntityMode.UnitMemory ||
                    Mode == EntityMode.UnitCustom))
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
                foreach (Entity entity in selected)
                {
                    if (IsEntityRegion(entity))
                    {
                        for (int i = 0; i < entity.PathPoints.Count; i++ )
                        {
                            Point point = LambdaToScreen( entity.SavedPathPoints[i].X,
                                                          entity.SavedPathPoints[i].Y);

                            point.X += e.X - DragStartMouseX;
                            point.Y += e.Y - DragStartMouseY;

                            entity.PathPoints[i] = ScreenToLambda(point.X, point.Y);
                        }
                    }
                    else
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
                    }

                    Point dist = new Point(Math.Abs(e.X - DragStartMouseX),
                                             Math.Abs(e.Y - DragStartMouseY));

                    draggingDist = (float)Math.Sqrt( Math.Pow(dist.X, 2) +
                                                     Math.Pow(dist.Y, 2) );
                }

                Invalidate();
            }

            //
            // Selection box animation
            //

            if (SelectionBegin)
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

            LastRMB.X = -1;
            LastRMB.Y = -1;

            if (e.Delta > 0)
                delta = +10;
            else
                delta = -10;

            switch (Mode)
            {
                case EntityMode.Selection:
                default:

                    // Get old mouse pos in lambda space
                    PointF oldMouse = ScreenToLambda(e.X, e.Y);

                    // Teh zoom
                    Zoom += delta;

                    // Get new mouse pos in lambda
                    PointF mousePos = ScreenToLambda(e.X, e.Y);

                    // Adjust Scroll
                    _ScrollX += mousePos.X - oldMouse.X;
                    _ScrollY += mousePos.Y - oldMouse.Y;

                    Invalidate();

                    break;

                case EntityMode.ImageLayer0:
                    if (LockZoom0 == false)
                    {
                        ZoomImage0 += delta;
                        Invalidate();
                    }
                    break;

                case EntityMode.ImageLayer1:
                    if (LockZoom1 == false)
                    {
                        ZoomImage1 += delta;
                        Invalidate();
                    }
                    break;

                case EntityMode.ImageLayer2:
                    if (LockZoom2 == false)
                    {
                        ZoomImage2 += delta;
                        Invalidate();
                    }
                    break;
            }

            base.OnMouseWheel(e);
        }

        #region Drawing

        //
        // Drawing
        //

        private void DrawLambdaScale(Graphics gr)
        {
            float scaleWidth = (int)Lambda * 5;

            scaleWidth *= (float)Zoom / 100.0F;

            Pen linePen = new Pen(Color.LightGray, 3);

            gr.DrawLine(linePen,
                         Width - scaleWidth - 5,
                         Height - 5,
                         Width - 5,
                         Height - 5);

            string label = "5λ";
            int labelWidth = (int)gr.MeasureString(label, this.Font).Width;

            gr.DrawString(label, this.Font, Brushes.Black,
                           this.Width - labelWidth - scaleWidth / 2,
                           this.Height - this.Font.Height - linePen.Width - 5);
        }

        private void DrawLambdaGrid(Graphics gr)
        {
            float x, y;

            if (Zoom <= 50)
                return;

            float scaleWidth = (int)Lambda * 5;
            scaleWidth *= (float)Zoom / 100.0F;

            for (y = 0; y < Height; y += scaleWidth)
            {
                for (x = 0; x < Width; x += scaleWidth)
                {
                    gr.FillRectangle(Brushes.LightGray, x, y, 1, 1);
                }
            }
        }

        private void DrawOrigin (Graphics gr)
        {
            float zf = (float)Zoom / 100.0F;
            Point point = LambdaToScreen(0, 0);

            int centerX = point.X;
            int centerY = point.Y;
            int radius = (int)((float)ViasBaseSize * zf);

            gr.FillEllipse( new SolidBrush(Color.Red),
                            centerX - radius, centerY - radius,
                            radius + radius, radius + radius);
        }

        private void DrawRightMouseCross(Graphics gr)
        {
            int crossWidth = 16;

            if (LastRMB.X < 0 || LastRMB.Y < 0)
                return;

            Pen pen = new Pen(new SolidBrush(Color.Black));

            gr.DrawLine(pen, new Point(LastRMB.X - crossWidth / 2, LastRMB.Y - crossWidth / 2),
                            new Point(LastRMB.X + crossWidth / 2, LastRMB.Y + crossWidth / 2));
            gr.DrawLine(pen, new Point(LastRMB.X - crossWidth / 2, LastRMB.Y + crossWidth / 2),
                            new Point(LastRMB.X + crossWidth / 2, LastRMB.Y - crossWidth / 2));
        }

        private void DrawEntity(Entity entity, Graphics gr)
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
                    int viasSize = ViasBaseSize;
                    if (entity.WidthOverride > 0)
                        viasSize = entity.WidthOverride;
                    radius = (int)((float)viasSize * zf);

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

                    if (entity.Label != null && entity.Label.Length > 0 && Zoom > 50 )
                    {
                        TextAlignment align = entity.LabelAlignment;

                        if (align == TextAlignment.GlobalSettings)
                            align = ViasTextAlignment;

                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

                        Point origin = new Point(centerX, centerY);

                        switch (align)
                        {
                            case TextAlignment.Top:
                            default:
                                origin.Y = centerY - radius - (int)(textSize.Height * zf);
                                break;

                            case TextAlignment.Bottom:
                                origin.Y = centerY + radius;
                                break;

                            case TextAlignment.TopLeft:
                            case TextAlignment.BottomLeft:
                                origin.Y = centerY + 2 * radius - (int)(textSize.Height * zf);
                                break;

                            case TextAlignment.TopRight:
                            case TextAlignment.BottomRight:
                                origin.Y = centerY + 2 * radius - (int)(textSize.Height * zf);
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
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor), 0, 0);
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

                    int wireSize = WireBaseSize;
                    if (entity.WidthOverride > 0)
                        wireSize = entity.WidthOverride;

                    if (entity.Selected == true)
                    {
                        gr.DrawLine(new Pen(SelectionColor, (float)wireSize * zf + (int)Lambda),
                                     startX, startY,
                                     endX, endY);
                    }

                    //gr.CompositingQuality = CompositingQuality;
                    //gr.CompositingMode = CompositingMode.SourceCopy;


                    gr.DrawLine(new Pen(wireColor, (float)wireSize * zf),
                                 startX, startY,
                                 endX, endY);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0 && Zoom > 50)
                    {
                        Point start = new Point(startX, startY);
                        Point end = new Point(endX, endY);
                        Point temp;

                        if (startX == endX && startY == endY)
                            break;

                        if (end.X < start.X)
                        {
                            temp = start;
                            start = end;
                            end = temp;
                        }

                        int a = end.Y - start.Y;
                        int b = end.X - start.X;
                        float Tga = (float)a / (float)b;
                        float alpha = (float)Math.Atan(Tga);

                        int wireLength = (int)(Math.Sqrt(Math.Pow(end.X - start.X, 2) +
                                                         Math.Pow(end.Y - start.Y, 2)) / (double)zf);

                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

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
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor),
                                       origin, -textSize.Height / 2);
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
                    Point bottomRight = LambdaToScreen(entity.LambdaX + entity.LambdaWidth,
                                                        entity.LambdaY + entity.LambdaHeight);
                    Point rectSize = new Point(bottomRight.X - topLeft.X,
                                               bottomRight.Y - topLeft.Y);

                    if (entity.Selected)
                    {
                        gr.FillRectangle(new SolidBrush(SelectionColor),
                                           topLeft.X - (int)Lambda, topLeft.Y - (int)Lambda,
                                           rectSize.X + 2 * (int)Lambda, rectSize.Y + 2 * (int)Lambda);
                    }

                    gr.FillRectangle(new SolidBrush(cellColor), topLeft.X, topLeft.Y,
                                       rectSize.X, rectSize.Y);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0 && Zoom > 50)
                    {
                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

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
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor), origin.X, origin.Y);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Beacon
                //

                case EntityType.Beacon:

                    Point beaconOrigin = LambdaToScreen(entity.LambdaX, entity.LambdaY);

                    Point imageOrigin = new Point(beaconOrigin.X - (int)((float)(beaconImage.Width * zf) / 2),
                                                    beaconOrigin.Y - (int)((float)beaconImage.Height * zf) );

                    if ( entity.Selected )
                    {
                        ColorMatrix colorMatrix = new ColorMatrix(
                            new float[][]
                            {
                                new float[] {.0f, 1f, .0f, 0, 0},
                                new float[] {.0f, 1f, .0f, 0, 0},
                                new float[] {.0f, 1f, .0f, 0, 0},
                                new float[] {0, 0, 0, 1, 0},
                                new float[] {0, 0, 0, 0, 1}
                            });

                        ImageAttributes attributes = new ImageAttributes();

                        attributes.SetColorMatrix(colorMatrix);

                        float borderSize = 3F * zf;

                        gr.DrawImage(beaconImage,
                            new Rectangle(
                                imageOrigin.X - (int)borderSize,
                                imageOrigin.Y - (int)borderSize,
                                (int)((float)beaconImage.Width * zf) + (int)(borderSize * 2F),
                                (int)((float)beaconImage.Height * zf) + (int)(borderSize * 2F)),
                            0, 0, beaconImage.Width, beaconImage.Height, GraphicsUnit.Pixel, attributes);
                    }

                    gr.DrawImage(beaconImage,
                                  imageOrigin.X, imageOrigin.Y,
                                  beaconImage.Width * zf,
                                  beaconImage.Height * zf);

                    // Debug
                    //Rectangle rect = new Rectangle(beaconOrigin.X, beaconOrigin.Y, 1, 1);
                    //gr.DrawRectangle(Pens.White, rect);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0)
                    {
                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        SizeF textSize = gr.MeasureString(entity.Label, font);

                        Point textOrigin = new Point(beaconOrigin.X - (int)(textSize.Width * zf / 2F),
                                                       imageOrigin.Y - (int)(textSize.Height * zf));

                        gr.TranslateTransform(textOrigin.X, textOrigin.Y);
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor), 0, 0);
                        gr.ResetTransform();
                    }

                    break;

                //
                // Region
                //

                case EntityType.Region:

                    if (hideRegions)
                        break;

                    Brush brush = new SolidBrush(Color.FromArgb(RegionOpacity, entity.ColorOverride));

                    GraphicsPath gp = new GraphicsPath();

                    PointF prev = (PointF) LambdaToScreen ( entity.PathPoints[0].X, entity.PathPoints[0].Y );
                    PointF first = prev;

                    foreach (PointF pathPoint in entity.PathPoints)
                    {
                        PointF translated = (PointF)LambdaToScreen(pathPoint.X, pathPoint.Y);

                        if (translated == prev)
                            continue;

                        gp.AddLine(prev, translated);

                        prev = translated;
                    }

                    gp.AddLine(prev, first);

                    if ( entity.Selected == true )
                    {
                        gr.DrawPath(new Pen(SelectionColor, (float)WireBaseSize * zf + (int)Lambda),
                                      gp);
                    }

                    gr.FillPath(brush, gp);

                    //
                    // Label
                    //

                    if (entity.Label != null && entity.Label.Length > 0)
                    {
                        Point p0 = LambdaToScreen(entity.PathPoints[0].X, entity.PathPoints[0].Y);
                        Point p1 = LambdaToScreen(entity.PathPoints[1].X, entity.PathPoints[1].Y);

                        Point start = new Point(p0.X, p0.Y);
                        Point end = new Point(p1.X, p1.Y);
                        Point temp;

                        if (p0.X == p1.X && p0.Y == p1.Y)
                            break;

                        if (end.X < start.X)
                        {
                            temp = start;
                            start = end;
                            end = temp;
                        }

                        int a = end.Y - start.Y;
                        int b = end.X - start.X;
                        float Tga = (float)a / (float)b;
                        float alpha = (float)Math.Atan(Tga);

                        Font font = Font;
                        if (entity.FontOverride != null)
                            font = entity.FontOverride;

                        gr.TranslateTransform(start.X, start.Y);
                        gr.RotateTransform((float)(180.0F * alpha / Math.PI));
                        gr.ScaleTransform(zf, zf);
                        gr.DrawString(entity.Label, font, new SolidBrush(ForeColor),
                                       0, 0);
                        gr.ResetTransform();
                    }

                    break;
            }
        }

        //
        // width / height - Scene viewport size
        // WholeScene - draw Scene for saving to bitmap
        // origin - Scene offset (px).
        //

        private void DrawScene(Graphics gr, int width, int height, bool WholeScene, Point origin)
        {
            float savedScrollX = 0, savedScrollY = 0;
            int savedZoom = 0;

            if (WholeScene == true)
            {
                savedScrollX = _ScrollX;
                savedScrollY = _ScrollY;
                savedZoom = _zoom;

                PointF originLambda = ScreenToLambda(origin.X, origin.Y);

                _ScrollX = -originLambda.X - ScrollX;
                _ScrollY = -originLambda.Y - ScrollY;
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

            float zf = (float)Zoom / 100F;
            bool EnableOpacity = true;

            for (int n = 2; n >= 0; n--)
            {
                if (_imageOrig[n] != null && hideImage == false)
                {
                    Point imageOffset = LambdaToScreen(_imageScroll[n].X, _imageScroll[n].Y);
                    float imageWidth = (float)_imageOrig[n].Width;
                    float imageHeight = (float)_imageOrig[n].Height;
                    float sx = imageOffset.X;
                    float sy = imageOffset.Y;

                    float imgZf = (float)_imageZoom[n] / 100F;

                    ColorMatrix colorMatrix = new ColorMatrix();
                    colorMatrix.Matrix33 = _imageOpacity[n] / 100F;

                    ImageAttributes imageAtt = new ImageAttributes();
                    imageAtt.SetColorMatrix(
                       colorMatrix,
                       ColorMatrixFlag.Default,
                       ColorAdjustType.Bitmap);

                    if (EnableOpacity == false)
                    {
                        gr.DrawImage(_imageOrig[n],
                                      sx, sy,
                                      imageWidth * zf / imgZf,
                                      imageHeight * zf / imgZf);
                    }
                    else
                    {
                        gr.InterpolationMode = InterpolationMode.HighQualityBilinear;

                        gr.DrawImage(_imageOrig[n],
                            new Rectangle(
                                (int)sx, (int)sy,
                                (int)(imageWidth * zf), (int)(imageHeight * zf)),
                            0, 0,
                            imageWidth / imgZf,
                            imageHeight / imgZf,
                            GraphicsUnit.Pixel,
                            imageAtt);
                    }
                }
            }

            //
            // Grid
            //

            if (WholeScene == false && HideGrid == false)
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

                if (WholeScene == false && DrawingBegin && (Mode == EntityMode.WireGround ||
                       Mode == EntityMode.WireInterconnect || Mode == EntityMode.WirePower))
                {
                    Entity virtualEntity = new Entity();

                    PointF point1 = ScreenToLambda(SavedMouseX, SavedMouseY);
                    PointF point2 = ScreenToLambda(LastMouseX, LastMouseY);

                    virtualEntity.LambdaX = point1.X;
                    virtualEntity.LambdaY = point1.Y;
                    virtualEntity.LambdaEndX = point2.X;
                    virtualEntity.LambdaEndY = point2.Y;
                    virtualEntity.Type = (EntityType)Mode;
                    virtualEntity.Priority = WirePriority;
                    virtualEntity.ColorOverride = Color.Black;

                    DrawEntity(virtualEntity, gr);
                }

                //
                // Cell drawing animation
                //

                if (WholeScene == false && DrawingBegin && (
                        Mode == EntityMode.CellNot ||
                        Mode == EntityMode.CellBuffer ||
                        Mode == EntityMode.CellMux ||
                        Mode == EntityMode.CellLogic ||
                        Mode == EntityMode.CellAdder ||
                        Mode == EntityMode.CellBusSupp ||
                        Mode == EntityMode.CellFlipFlop ||
                        Mode == EntityMode.CellLatch ||
                        Mode == EntityMode.CellOther ||
                        Mode == EntityMode.UnitRegfile ||
                        Mode == EntityMode.UnitMemory ||
                        Mode == EntityMode.UnitCustom
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

                    virtualEntity.LambdaX = originPos.X;
                    virtualEntity.LambdaY = originPos.Y;
                    virtualEntity.LambdaWidth = size.X;
                    virtualEntity.LambdaHeight = size.Y;
                    virtualEntity.Type = (EntityType)Mode;
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

                //
                // Origin
                //

#if DEBUG
                DrawOrigin (gr);
#endif

                //
                // RMB
                //

                if (WholeScene == false)
                    DrawRightMouseCross(gr);
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

            if (WholeScene == true)
            {
                _ScrollX = savedScrollX;
                _ScrollY = savedScrollY;
                _zoom = savedZoom;
            }
        }

        private void ReallocateGraphics ()
        {
            context = BufferedGraphicsManager.Current;
            context.MaximumBuffer = new Size(Width + 1, Height + 1);

            gfx = context.Allocate(CreateGraphics(),
                 new Rectangle(0, 0, Width, Height));
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (gfx == null)
                ReallocateGraphics();

            long beginTime = DateTime.Now.Ticks;

            Point origin = new Point(0, 0);
            DrawScene(gfx.Graphics, Width, Height, false, origin);

            gfx.Render(e.Graphics);

            long endTime = DateTime.Now.Ticks;

            if (OnFrameDone != null)
                OnFrameDone(this, (endTime - beginTime) / 10000, EventArgs.Empty);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            if (gfx != null)
            {
                gfx.Dispose();
                ReallocateGraphics();
            }

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

        private Image ToGrayscale(Image source)
        {
            if (grayscale == true)
            {
                Bitmap grayscaleBitmap = ColorToGrayscale(new Bitmap(source));
                source.Dispose();
                GC.Collect();
                return (Image)grayscaleBitmap;
            }
            else
                return source;
        }

        [Category("Appearance"), DefaultValue(null)]
        [Browsable(false)]
        public Image BeaconImage
        {
            get { return beaconImage; }
            set
            {
                if (beaconImage != value)
                {
                    if (beaconImage != null)
                    {
                        beaconImage.Dispose();
                        GC.Collect();
                    }

                    beaconImage = new Bitmap(value);
                    Invalidate();
                }
            }
        }

        [Category("Appearance"), DefaultValue(null)]
        [Browsable(false)]
        public Image Image0
        {
            get { return _imageOrig[0]; }
            set
            {
                if (_imageOrig[0] != value)
                {
                    if (_imageOrig[0] != null)
                    {
                        _imageOrig[0].Dispose();
                        GC.Collect();
                    }

                    _imageOrig[0] = ToGrayscale(value);

                    ScrollingBegin = false;
                    Invalidate();
                }
            }
        }

        [Category("Appearance"), DefaultValue(null)]
        [Browsable(false)]
        public Image Image1
        {
            get { return _imageOrig[1]; }
            set
            {
                if (_imageOrig[1] != value)
                {
                    if (_imageOrig[1] != null)
                    {
                        _imageOrig[1].Dispose();
                    }

                    _imageOrig[1] = ToGrayscale(value);

                    ScrollingBegin = false;
                    Invalidate();
                }
            }
        }

        [Category("Appearance"), DefaultValue(null)]
        [Browsable(false)]
        public Image Image2
        {
            get { return _imageOrig[2]; }
            set
            {
                if (_imageOrig[2] != value)
                {
                    if (_imageOrig[2] != null)
                    {
                        _imageOrig[2].Dispose();
                        GC.Collect();
                    }

                    _imageOrig[2] = ToGrayscale(value);

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
                _imageOpacity[0] = Math.Max(0, Math.Min(100, value));
                Invalidate();
            }
        }

        [Category("Appearance")]
        public int ImageOpacity1
        {
            get { return _imageOpacity[1]; }
            set
            {
                _imageOpacity[1] = Math.Max(0, Math.Min(100, value));
                Invalidate();
            }
        }

        [Category("Appearance")]
        public int ImageOpacity2
        {
            get { return _imageOpacity[2]; }
            set
            {
                _imageOpacity[2] = Math.Max(0, Math.Min(100, value));
                Invalidate();
            }
        }

        [Category("Logic")]
        public bool SelectEntitiesAfterAdd
        {
            get { return selectEntitiesAfterAdd; }
            set { selectEntitiesAfterAdd = value; }
        }

        [Category("Logic")]
        public bool Grayscale
        {
            get { return grayscale; }
            set { grayscale = value; }
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
        public EntityMode Mode
        {
            get { return drawMode; }
            set
            {
                drawMode = value;

                if (drawMode == EntityMode.Selection ||
                    drawMode == EntityMode.ImageLayer0 ||
                    drawMode == EntityMode.ImageLayer1 ||
                    drawMode == EntityMode.ImageLayer2)
                    DrawingBegin = false;
            }
        }

        [Category("Appearance")]
        [TypeConverter(typeof(PointFConverter))]
        public PointF ScrollImage0
        {
            get { return _imageScroll[0]; }
            set { _imageScroll[0] = value; Invalidate(); }
        }

        [Category("Appearance")]
        [TypeConverter(typeof(PointFConverter))]
        public PointF ScrollImage1
        {
            get { return _imageScroll[1]; }
            set { _imageScroll[1] = value; Invalidate(); }
        }

        [Category("Appearance")]
        [TypeConverter(typeof(PointFConverter))]
        public PointF ScrollImage2
        {
            get { return _imageScroll[2]; }
            set { _imageScroll[2] = value; Invalidate(); }
        }

        [Category("Appearance")]
        public float ScrollX
        {
            get { return _ScrollX; }
            set
            {
                _ScrollX = value;
                Invalidate();

                if (OnScrollChanged != null)
                    OnScrollChanged(this, EventArgs.Empty);
            }
        }

        [Category("Appearance")]
        public float ScrollY
        {
            get { return _ScrollY; }
            set
            {
                _ScrollY = value;
                Invalidate();

                if (OnScrollChanged != null)
                    OnScrollChanged(this, EventArgs.Empty);
            }
        }

        [Category("Appearance")]
        public int Zoom
        {
            get { return _zoom; }
            set
            {
                int oldZoom = _zoom;

                if (value < 10)
                    value = 10;

                if (value > 400)
                    value = 400;

                _zoom = value;

                if (oldZoom != _zoom)
                {
                    Invalidate();

                    if (OnZoomChanged != null)
                        OnZoomChanged(this, EventArgs.Empty);
                }
            }
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
        public bool HideGrid
        {
            get { return hideGrid; }
            set { hideGrid = value; Invalidate(); }
        }

        [Category("Appearance")]
        public bool HideRegions
        {
            get { return hideRegions; }
            set { hideRegions = value; Invalidate(); }
        }

        [Category("Appearance")]
        public Color SelectionBoxColor
        {
            get { return selectionBoxColor; }
            set { selectionBoxColor = value; Invalidate(); }
        }

        [Category("Appearance")]
        public bool LockScroll0
        {
            get { return lockScroll[0]; }
            set { lockScroll[0] = value; }
        }

        [Category("Appearance")]
        public bool LockScroll1
        {
            get { return lockScroll[1]; }
            set { lockScroll[1] = value; }
        }

        [Category("Appearance")]
        public bool LockScroll2
        {
            get { return lockScroll[2]; }
            set { lockScroll[2] = value; }
        }

        [Category("Appearance")]
        public bool LockZoom0
        {
            get { return lockZoom[0]; }
            set { lockZoom[0] = value; }
        }

        [Category("Appearance")]
        public bool LockZoom1
        {
            get { return lockZoom[1]; }
            set { lockZoom[1] = value; }
        }

        [Category("Appearance")]
        public bool LockZoom2
        {
            get { return lockZoom[2]; }
            set { lockZoom[2] = value; }
        }

        private string GenBeaconName ()
        {
            int NumBeacons = GetBeaconCount() + 1;
            return "Beacon" + NumBeacons.ToString();
        }

        private Entity AddBeacon(int ScreenX, int ScreenY)
        {
            Entity item = new Entity();

            PointF point = ScreenToLambda(ScreenX, ScreenY);

            item.Label = GenBeaconName ();
            item.LambdaX = point.X;
            item.LambdaY = point.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = EntityType.Beacon;
            item.ColorOverride = Color.Black;
            item.Priority = BeaconPriority;
            item.FontOverride = null;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);

            LastOpWrapper = EntityBoxOperation.EntityAddSingle;
            lastOpData = (object)item;
            cancelledOp = EntityBoxOperation.Unknown;

            return item;
        }

        private Entity AddVias(EntityType Type, int ScreenX, int ScreenY)
        {
            Entity item = new Entity();

            PointF point = ScreenToLambda(ScreenX, ScreenY);

            //
            // Get rid of clutching viases
            //

            foreach ( Entity entity in _entities )
            {
                if ( IsEntityVias(entity) )
                {
                    float dist = (float)Math.Sqrt( Math.Pow(entity.LambdaX - point.X, 2) + 
                                                   Math.Pow(entity.LambdaY - point.Y, 2));

                    if (dist <= 1.5F)
                        return null;
                }
            }

            item.Label = "";
            item.LambdaX = point.X;
            item.LambdaY = point.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = ViasOverrideColor;
            item.Priority = ViasPriority;
            item.FontOverride = null;
            item.WidthOverride = 0;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);

            LastOpWrapper = EntityBoxOperation.EntityAddSingle;
            lastOpData = (object)item;
            cancelledOp = EntityBoxOperation.Unknown;

            return item;
        }

        private Entity AddWire(EntityType Type, int StartX, int StartY, int EndX, int EndY)
        {
            Entity item = new Entity();

            PointF point1 = ScreenToLambda(StartX, StartY);
            PointF point2 = ScreenToLambda(EndX, EndY);

            float len = (float)Math.Sqrt(Math.Pow(point2.X - point1.X, 2) +
                                          Math.Pow(point2.Y - point1.Y, 2));

            if (len < 1.0F)
            {
                Invalidate();
                return null;
            }

            item.Label = "";
            item.LambdaX = point1.X;
            item.LambdaY = point1.Y;
            item.LambdaEndX = point2.X;
            item.LambdaEndY = point2.Y;
            item.LambdaWidth = 1;
            item.LambdaHeight = 1;
            item.Type = Type;
            item.ColorOverride = WireOverrideColor;
            item.Priority = WirePriority;
            item.FontOverride = null;
            item.WidthOverride = 0;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);

            LastOpWrapper = EntityBoxOperation.EntityAddSingle;
            lastOpData = (object)item;
            cancelledOp = EntityBoxOperation.Unknown;

            return item;
        }

        private Entity AddCell(EntityType Type, int StartX, int StartY, int EndX, int EndY)
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
                return null;
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
            item.ColorOverride = CellOverrideColor;
            item.Priority = CellPriority;
            item.FontOverride = null;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);

            LastOpWrapper = EntityBoxOperation.EntityAddSingle;
            lastOpData = (object)item;
            cancelledOp = EntityBoxOperation.Unknown;

            return item;
        }

        public static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.HighQuality;
                graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                graphics.SmoothingMode = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }

                graphics.Dispose();
            }

            return destImage;
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

                if (LastOpWrapper != EntityBoxOperation.ImageZoom0)
                {
                    LastOpWrapper = EntityBoxOperation.ImageZoom0;
                    lastOpData = (object)_imageZoom[0];
                    cancelledOp = EntityBoxOperation.Unknown;
                }

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

                if (LastOpWrapper != EntityBoxOperation.ImageZoom1)
                {
                    LastOpWrapper = EntityBoxOperation.ImageZoom1;
                    lastOpData = (object)_imageZoom[1];
                    cancelledOp = EntityBoxOperation.Unknown;
                }

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

                if (LastOpWrapper != EntityBoxOperation.ImageZoom2)
                {
                    LastOpWrapper = EntityBoxOperation.ImageZoom2;
                    lastOpData = (object)_imageZoom[2];
                    cancelledOp = EntityBoxOperation.Unknown;
                }

                _imageZoom[2] = value;

                float zf = (float)_imageZoom[2] / 100F;
                Invalidate();
            }
        }

        public void DeleteAllEntites()
        {
            lastEntities = new List<Entity>();
            foreach (Entity entity in _entities)
            {
                lastEntities.Add(entity);
            }

            LastOpWrapper = EntityBoxOperation.EntityDelete;
            cancelledOp = EntityBoxOperation.Unknown;

            _entities.Clear();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);
        }

        //
        // Scene size and origin in screen coordinate space
        //

        private Point DetermineSceneSize(out Point origin)
        {
            Point point = new Point(0, 0);
            Point originOut = new Point(0, 0);

            float savedScrollX = 0, savedScrollY = 0;
            int savedZoom = 0;

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
                if (_imageOrig[n] != null && HideImage == false)
                {
                    Point offset = LambdaToScreen(_imageScroll[n].X, _imageScroll[n].Y);

                    if (offset.X < originOut.X)
                        originOut.X = offset.X;

                    if (offset.Y < originOut.Y)
                        originOut.Y = offset.Y;

                    float imgZf = (float)_imageZoom[n] / 100F;

                    int rightSide = (int)((float)_imageOrig[n].Width * imgZf) + offset.X;
                    int bottomSide = (int)((float)_imageOrig[n].Height * imgZf) + offset.Y;

                    if (rightSide > point.X)
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
                        screenCoords.X -= 2 * WireBaseSize;
                        screenCoords.Y -= 2 * WireBaseSize;
                    }
                    else if (IsEntityCell(entity))
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX,
                                                        entity.LambdaY);
                    }
                    else
                    {
                        screenCoords = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                        screenCoords.X -= 2 * ViasBaseSize;
                        screenCoords.Y -= 2 * ViasBaseSize;
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

        public void SaveSceneAsImage(string FileName)
        {
            ImageFormat imageFormat;
            string ext;
            Point origin;
            Point sceneSize = DetermineSceneSize(out origin);

            int bitmapWidth = sceneSize.X - origin.X;
            int bitmapHeight = sceneSize.Y - origin.Y;

            Bitmap bitmap = new Bitmap(bitmapWidth, bitmapHeight, PixelFormat.Format16bppRgb565);

            Graphics gr = Graphics.FromImage(bitmap);

            DrawScene(gr, sceneSize.X, sceneSize.Y, true, origin);

            ext = Path.GetExtension(FileName);

            if (ext.ToLower() == ".jpg" || ext.ToLower() == ".jpeg")
                imageFormat = ImageFormat.Jpeg;
            else if (ext.ToLower() == ".png")
                imageFormat = ImageFormat.Png;
            else if (ext.ToLower() == ".bmp")
                imageFormat = ImageFormat.Bmp;
            else
                imageFormat = ImageFormat.Jpeg;

            bitmap.Save(FileName, imageFormat);

            bitmap.Dispose();
            gr.Dispose();
        }

        //
        // Serialization
        //

        private void WipeGarbage()
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
                                                   Math.Pow(entity.LambdaEndY - entity.LambdaY, 2));

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

        public void Serialize(string FileName)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            WipeGarbage();

            using (FileStream fs = new FileStream(FileName, FileMode.Create))
            {
                ser.Serialize(fs, _entities);
            }
        }

        public void Unserialize(string FileName, bool Append)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            using (FileStream fs = new FileStream(FileName, FileMode.Open))
            {
                lastEntities = new List<Entity>();
                foreach (Entity entity in _entities)
                {
                    lastEntities.Add(entity);
                }

                LastOpWrapper = EntityBoxOperation.EntityAddBulk;
                cancelledOp = EntityBoxOperation.Unknown;

                List<Entity> list = (List<Entity>)ser.Deserialize(fs);

                if (Append == false)
                    _entities.Clear();

                foreach (Entity entity in list)
                {
                    entity.Selected = SelectEntitiesAfterAdd;
                    _entities.Add(entity);
                }

                WipeGarbage();

                _entities = _entities.OrderBy(o => o.Priority).ToList();

                Invalidate();

                if (OnEntityCountChanged != null)
                    OnEntityCountChanged(this, EventArgs.Empty);

                UnserializeLastStamp = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
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

        private bool PointInPoly(PointF[] poly, PointF point)
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

        public void RemoveSelection()
        {
            bool UpdateRequired = false;

            foreach (Entity entity in _entities)
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

        public void AssociateSelectionPropertyGrid(PropertyGrid propertyGrid)
        {
            entityGrid = propertyGrid;
        }

        public void DeleteSelected()
        {
            bool UpdateRequired = false;
            List<Entity> pendingDelete = new List<Entity>();

            lastEntities = new List<Entity>();
            foreach (Entity entity in _entities)
            {
                lastEntities.Add(entity);
            }

            LastOpWrapper = EntityBoxOperation.EntityDelete;
            cancelledOp = EntityBoxOperation.Unknown;

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
            {
                Invalidate();

                if (OnEntityCountChanged != null)
                    OnEntityCountChanged(this, EventArgs.Empty);
            }

            if (entityGrid != null)
                entityGrid.SelectedObject = null;
        }

        private List<Entity> GetSelected()
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
        private Color _ViasOverrideColor;
        private Color _WireOverrideColor;
        private Color _CellOverrideColor;
        private Color _RegionOverrideColor;
        private ViasShape _viasShape;
        private int _viasBaseSize;
        private int _wireBaseSize;
        private TextAlignment _cellTextAlignment;
        private TextAlignment _viasTextAlignment;
        private TextAlignment _wireTextAlignment;
        private int _ViasOpacity;
        private int _WireOpacity;
        private int _CellOpacity;
        private int _RegionOpacity;
        private int _ViasPriority;
        private int _WirePriority;
        private int _CellPriority;
        private int _BeaconPriority;
        private int _RegionPriority;
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
            _CellLatchColor = Color.Cyan;
            _CellOtherColor = Color.Snow;

            _UnitRegfileColor = Color.Snow;
            _UnitMemoryColor = Color.Snow;
            _UnitCustomColor = Color.Snow;

            _SelectionColor = Color.LimeGreen;

            _ViasOverrideColor = Color.Black;
            _WireOverrideColor = Color.Black;
            _CellOverrideColor = Color.Black;
            _RegionOverrideColor = Color.Black;

            _ViasOpacity = 255;
            _WireOpacity = 128;
            _CellOpacity = 128;
            _RegionOpacity = 128;

            _BeaconPriority = 4;
            _ViasPriority = 3;
            _WirePriority = 2;
            _CellPriority = 1;
            _RegionPriority = 0;
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
        public Color ViasOverrideColor
        {
            get { return _ViasOverrideColor; }
            set { _ViasOverrideColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color WireOverrideColor
        {
            get { return _WireOverrideColor; }
            set { _WireOverrideColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color CellOverrideColor
        {
            get { return _CellOverrideColor; }
            set { _CellOverrideColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public Color RegionOverrideColor
        {
            get { return _RegionOverrideColor; }
            set { _RegionOverrideColor = value; Invalidate(); }
        }

        [Category("Entity Appearance")]
        public int ViasOpacity
        {
            get { return _ViasOpacity; }
            set
            {
                _ViasOpacity = Math.Max(0, Math.Min(255, value));
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
        public int RegionOpacity
        {
            get { return _RegionOpacity; }
            set
            {
                _RegionOpacity = Math.Max(0, Math.Min(255, value));
                Invalidate();
            }
        }

        [Category("Entity Appearance")]
        public int BeaconPriority
        {
            get { return _BeaconPriority; }
            set
            {
                _BeaconPriority = value;
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

        [Category("Entity Appearance")]
        public int RegionPriority
        {
            get { return _RegionPriority; }
            set
            {
                _RegionPriority = value;
                Invalidate();
            }
        }

        #endregion Entity Props

        //
        // Key input handling
        //

        protected override void OnKeyUp(KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
                DeleteSelected();

            else if (e.KeyCode == Keys.Escape)
                RemoveSelection();

            else if (e.KeyCode == Keys.Home)
            {
                _ScrollX = 0;
                _ScrollY = 0;
                Invalidate();

                if (OnScrollChanged != null)
                    OnScrollChanged(this, EventArgs.Empty);
            }

            else if ((e.KeyCode == Keys.Right ||
                        e.KeyCode == Keys.Left ||
                        e.KeyCode == Keys.Up ||
                        e.KeyCode == Keys.Down) && Mode == EntityMode.Selection)
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
            if (AutoPriority == true)
                _entities = _entities.OrderBy(o => o.Priority).ToList();
        }

        //
        // Wire merger. This operation is potentially dangerous!!!
        //

        public void MergeSelectedWires(bool Vertical)
        {
            List<Entity> selectedWires = new List<Entity>();

            //
            // Grab selected 
            //

            foreach (Entity entity in _entities)
            {
                if (IsEntityWire(entity) && entity.Selected == true)
                {
                    selectedWires.Add(entity);
                }
            }

            if (selectedWires.Count <= 1)
                return;

            lastEntities = new List<Entity>();
            foreach ( Entity entity in _entities )
            {
                lastEntities.Add(entity);
            }

            //
            // Find left-most and right-most points + Determine average wire type
            //

            PointF left = new PointF(float.MaxValue, float.MaxValue);
            PointF right = new PointF(float.MinValue, float.MinValue);
            int[] types = { 0, 0, 0 };

            foreach (Entity entity in selectedWires)
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

                switch (entity.Type)
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
            // Remove selected / join label names
            //

            string Label = "";

            foreach (Entity entity in selectedWires)
            {
                Label += entity.Label;
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

            switch (index)
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

            Entity mergedWire = AddWire(type, start.X, start.Y, end.X, end.Y);

            if (mergedWire != null)
            {
                mergedWire.Label = Label;
            }

            LastOpWrapper = EntityBoxOperation.EntityMergeWires;
            cancelledOp = EntityBoxOperation.Unknown;

            Invalidate();
        }

        public static Bitmap ColorToGrayscale(Bitmap bmp)
        {
            int w = bmp.Width,
            h = bmp.Height,
            r, ic, oc, bmpStride, outputStride, bytesPerPixel;
            PixelFormat pfIn = bmp.PixelFormat;
            ColorPalette palette;
            Bitmap output;
            BitmapData bmpData, outputData;

            //Create the new bitmap
            output = new Bitmap(w, h, PixelFormat.Format8bppIndexed);

            //Build a grayscale color Palette
            palette = output.Palette;
            for (int i = 0; i < 256; i++)
            {
                Color tmp = Color.FromArgb(255, i, i, i);
                palette.Entries[i] = Color.FromArgb(255, i, i, i);
            }
            output.Palette = palette;

            //No need to convert formats if already in 8 bit
            if (pfIn == PixelFormat.Format8bppIndexed)
            {
                output = (Bitmap)bmp.Clone();

                //Make sure the palette is a grayscale palette and not some other
                //8-bit indexed palette
                output.Palette = palette;

                return output;
            }

            //Get the number of bytes per pixel
            switch (pfIn)
            {
                case PixelFormat.Format24bppRgb: bytesPerPixel = 3; break;
                case PixelFormat.Format32bppArgb: bytesPerPixel = 4; break;
                case PixelFormat.Format32bppRgb: bytesPerPixel = 4; break;
                default: throw new InvalidOperationException("Image format not supported");
            }

            //Lock the images
            bmpData = bmp.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadOnly,
            pfIn);
            outputData = output.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.WriteOnly,
            PixelFormat.Format8bppIndexed);
            bmpStride = bmpData.Stride;
            outputStride = outputData.Stride;

            //Traverse each pixel of the image
            unsafe
            {
                byte* bmpPtr = (byte*)bmpData.Scan0.ToPointer(),
                outputPtr = (byte*)outputData.Scan0.ToPointer();

                if (bytesPerPixel == 3)
                {
                    for (r = 0; r < h; r++)
                    {
                        for (ic = oc = 0; oc < w; ic += 3, ++oc)
                        {
                            int yc = (int)
                            (0.2126f * (float)bmpPtr[r * bmpStride + ic + 1] +
                            0.752f * (float)bmpPtr[r * bmpStride + ic + 2] +
                            0.0722f * (float)bmpPtr[r * bmpStride + ic + 3]);

                            yc = Math.Max(0, Math.Min(yc, 255));

                            outputPtr[r * outputStride + oc] = (byte)yc;
                        }
                    }
                }
                else //bytesPerPixel == 4
                {
                    for (r = 0; r < h; r++)
                    {
                        for (ic = oc = 0; oc < w; ic += 4, ++oc)
                        {
                            int yc = (int)
                            (0.2126f * (float)bmpPtr[r * bmpStride + ic + 1] +
                            0.752f * (float)bmpPtr[r * bmpStride + ic + 2] +
                            0.0722f * (float)bmpPtr[r * bmpStride + ic + 3]);

                            yc = Math.Max(0, Math.Min(yc, 255));

                            outputPtr[r * outputStride + oc] = (byte)yc;
                        }
                    }
                }
            }

            //Unlock the images
            bmp.UnlockBits(bmpData);
            output.UnlockBits(outputData);

            return output;
        }

        public void LoadImage(Image image)
        {
            switch (drawMode)
            {
                case EntityMode.ImageLayer0:
                default:
                    Image0 = image;
                    break;
                case EntityMode.ImageLayer1:
                    Image1 = image;
                    break;
                case EntityMode.ImageLayer2:
                    Image2 = image;
                    break;
            }
        }

        public void UnloadImage ()
        {
            switch (drawMode)
            {
                case EntityMode.ImageLayer0:
                default:
                    _imageOrig[0].Dispose();
                    _imageOrig[0] = null;
                    GC.Collect();
                    Invalidate();
                    break;
                case EntityMode.ImageLayer1:
                    _imageOrig[1].Dispose();
                    _imageOrig[1] = null;
                    GC.Collect();
                    Invalidate();
                    break;
                case EntityMode.ImageLayer2:
                    _imageOrig[2].Dispose();
                    _imageOrig[2] = null;
                    GC.Collect();
                    Invalidate();
                    break;
            }
        }

        public int GetViasCount()
        {
            int Count = 0;
            foreach (Entity entity in _entities)
            {
                if (IsEntityVias(entity))
                    Count++;
            }
            return Count;
        }

        public int GetWireCount()
        {
            int Count = 0;
            foreach (Entity entity in _entities)
            {
                if (IsEntityWire(entity))
                    Count++;
            }
            return Count;
        }

        public int GetCellCount()
        {
            int Count = 0;
            foreach (Entity entity in _entities)
            {
                if (IsEntityCell(entity))
                    Count++;
            }
            return Count;
        }

        public float GetDragDistance ()
        {
            return draggingDist;
        }

        //
        // Last operation
        //

        private void DispatchLastOperation (bool Undo)
        {
            switch (lastOp)
            {
                case EntityBoxOperation.EntityAddSingle:
                    if (Undo)
                    {
                        cancelledOpData = lastOpData;
                        _entities.Remove((Entity)lastOpData);
                    }
                    else
                        _entities.Add((Entity)cancelledOpData);

                    if (OnEntityCountChanged != null)
                        OnEntityCountChanged(this, EventArgs.Empty);

                    Invalidate();
                    break;

                case EntityBoxOperation.ImageScroll0:
                    if (Undo)
                    {
                        cancelledOpData = _imageScroll[0];
                        _imageScroll[0] = (PointF)lastOpData;
                    }
                    else
                        _imageScroll[0] = (PointF)cancelledOpData;
                    Invalidate();
                    break;

                case EntityBoxOperation.ImageScroll1:
                    if (Undo)
                    {
                        cancelledOpData = _imageScroll[1];
                        _imageScroll[1] = (PointF)lastOpData;
                    }
                    else
                        _imageScroll[1] = (PointF)cancelledOpData;
                    Invalidate();
                    break;

                case EntityBoxOperation.ImageScroll2:
                    if (Undo)
                    {
                        cancelledOpData = _imageScroll[2];
                        _imageScroll[2] = (PointF)lastOpData;
                    }
                    else
                        _imageScroll[2] = (PointF)cancelledOpData;
                    Invalidate();
                    break;

                case EntityBoxOperation.ImageZoom0:
                    if (Undo)
                    {
                        cancelledOpData = _imageZoom[0];
                        ZoomImage0 = (int)lastOpData;
                    }
                    else
                        ZoomImage0 = (int)cancelledOpData;
                    Invalidate();
                    break;

                case EntityBoxOperation.ImageZoom1:
                    if (Undo)
                    {
                        cancelledOpData = _imageZoom[1];
                        ZoomImage1 = (int)lastOpData;
                    }
                    else
                        ZoomImage1 = (int)cancelledOpData;
                    Invalidate();
                    break;

                case EntityBoxOperation.ImageZoom2:
                    if (Undo)
                    {
                        cancelledOpData = _imageZoom[2];
                        ZoomImage2 = (int)lastOpData;
                    }
                    else
                        ZoomImage2 = (int)cancelledOpData;
                    Invalidate();
                    break;

                case EntityBoxOperation.EntityMergeWires:
                case EntityBoxOperation.EntityAddBulk:
                case EntityBoxOperation.EntityDelete:
                case EntityBoxOperation.EntityMove:
                    if (Undo == true)
                    {
                        cancelledEntities = new List<Entity>();
                        foreach ( Entity entity in _entities )
                        {
                            cancelledEntities.Add(entity);
                        }

                        _entities = new List<Entity>();
                        foreach (Entity entity in lastEntities)
                        {
                            if (lastOp == EntityBoxOperation.EntityMove)
                            {
                                entity.LambdaX = entity.SavedLambdaX;
                                entity.LambdaY = entity.SavedLambdaY;
                                entity.LambdaEndX = entity.SavedLambdaEndX;
                                entity.LambdaEndY = entity.SavedLambdaEndY;
                            }

                            _entities.Add(entity);
                        }
                    }
                    else
                    {
                        _entities = new List<Entity>();
                        foreach (Entity entity in cancelledEntities)
                        {
                            _entities.Add(entity);
                        }
                    }

                    Invalidate();

                    if (OnEntityCountChanged != null)
                        OnEntityCountChanged(this, EventArgs.Empty);

                    break;
            }
        }

        public void CancelLastOperation()
        {
            if (LastOpWrapper == EntityBoxOperation.Unknown)
                return;

            cancelledOp = LastOpWrapper;

            DispatchLastOperation(true);

            LastOpWrapper = EntityBoxOperation.Unknown;
        }

        public void RetryCancelledOperation()
        {
            if (cancelledOp == EntityBoxOperation.Unknown)
                return;

            LastOpWrapper = cancelledOp;

            DispatchLastOperation(false);

            cancelledOp = EntityBoxOperation.Unknown;
        }

        public EntityBoxOperation GetLastOperation()
        {
            return LastOpWrapper;
        }

        private EntityBoxOperation LastOpWrapper
        {
            get { return lastOp; }
            set
            {
                lastOp = value;

                if (OnLastOperation != null)
                    OnLastOperation(this, EventArgs.Empty);
            }
        }

        //
        // Workspace
        //

        public void SaveWorkspace(string FileName)
        {
            WipeGarbage();

            EntityBoxWorkspace wrk = new EntityBoxWorkspace(this);
            XmlSerializer ser = new XmlSerializer(typeof(EntityBoxWorkspace));

            using (FileStream fs = new FileStream(FileName, FileMode.Create))
            {
                ser.Serialize(fs, wrk);
            }
        }

        public void LoadWorkspace(string FileName)
        {
            XmlSerializer ser = new XmlSerializer(typeof(EntityBoxWorkspace));

            using (FileStream fs = new FileStream(FileName, FileMode.Open))
            {
                EntityBoxWorkspace wrk = (EntityBoxWorkspace)ser.Deserialize(fs);

                wrk.LoadSnapshot(this);

                if (OnEntityCountChanged != null)
                    OnEntityCountChanged(this, EventArgs.Empty);

                if (OnScrollChanged != null)
                    OnScrollChanged(this, EventArgs.Empty);

                if (OnZoomChanged != null)
                    OnZoomChanged(this, EventArgs.Empty);

                LastOpWrapper = EntityBoxOperation.Unknown;
                cancelledOp = EntityBoxOperation.Unknown;
            }
        }

        public void DrawWireBetweenSelectedViases ()
        {
            List<Entity> selectedVias = new List<Entity>();

            //
            // Grab selected viases
            //

            foreach ( Entity entity in _entities )
            {
                if ( IsEntityVias(entity) && entity.Selected )
                {
                    selectedVias.Add(entity);
                }
            }

            //
            // Sort by select timestamp
            //

            selectedVias = selectedVias.OrderBy(o => o.SelectTimeStamp).ToList();

            //
            // Connect viases by wires
            //

            if ( selectedVias.Count >= 2 )
            {
                Entity prevVias = null;

                foreach ( Entity vias in selectedVias )
                {
                    if (prevVias == null)
                    {
                        prevVias = vias;
                        continue;
                    }

                    //
                    // Connect prevVias with current
                    //

                    Point Start = LambdaToScreen(prevVias.LambdaX, prevVias.LambdaY);
                    Point End = LambdaToScreen(vias.LambdaX, vias.LambdaY);

                    AddWire( EntityType.WireInterconnect,
                             Start.X, Start.Y, End.X, End.Y);

                    //
                    // Replace prev vias
                    //

                    prevVias = vias;
                }

                Invalidate();
            }
        }

        public int GetBeaconCount ()
        {
            int NumBeacons = 0;
            foreach (Entity entity in _entities)
            {
                if (entity.Type == EntityType.Beacon)
                    NumBeacons++;
            }
            return NumBeacons;
        }

        public List<Entity> GetBeacons ()
        {
            List<Entity> beacons = new List<Entity>();
            foreach (Entity entity in _entities)
            {
                if (entity.Type == EntityType.Beacon)
                    beacons.Add(entity);
            }
            return beacons;
        }

        public void ScrollToBeacon (Entity beacon)
        {
            _ScrollX = 0;
            _ScrollY = 0;

            Point screen = LambdaToScreen(beacon.LambdaX, beacon.LambdaY);

            screen.X -= Width / 2;
            screen.Y -= Height / 2;

            PointF lambda = ScreenToLambda(screen.X, screen.Y);

            _ScrollX = -lambda.X;
            _ScrollY = -lambda.Y;

            Invalidate();

            if (OnScrollChanged != null)
                OnScrollChanged(this, EventArgs.Empty);
        }

        public void LabelEdited (Entity entity)
        {
            if (OnEntityLabelEdit != null)
                OnEntityLabelEdit(this, entity, EventArgs.Empty);
        }

        //
        // Traverse selection
        //

        private bool IsViasInWire (Entity vias, Entity wire)
        {
            float delta = 1F;       // Lambdas

            PointF start = new PointF(wire.LambdaX, wire.LambdaY);
            PointF end = new PointF(wire.LambdaEndX, wire.LambdaEndY);

            RectangleF rect = new RectangleF(
                vias.LambdaX - delta, vias.LambdaY - delta,
                2 * delta, 2 * delta );

            return LineIntersectsRect(start, end, rect);
        }

        private void SelectTraverse ( Entity source,
                                      int Tier,
                                      int TierMax,
                                      int Depth,
                                      bool Debug )
        {
            PointF[] rect1 = new PointF[4];
            PointF[] rect2 = new PointF[4];
            PointF restrictedStart = new PointF(0, 0);
            PointF restrictedEnd = new PointF(0, 0);

            if (Tier >= TierMax)
                return;

            if (Debug)
                source.Label = Depth.ToString();

            //
            // Wire joint Vias/Wire
            //

            if ( IsEntityWire(source) )
            {
                float maxDist = 1F;
                float dist;
                List<Entity> viases = new List<Entity>();

                //
                // Get not selected entities in delta range for Start point
                //
                // Get not selected entities in delta range for End point
                //

                float dx = Math.Abs (source.LambdaEndX - source.LambdaX);
                float dy = Math.Abs(source.LambdaEndY - source.LambdaY);
                bool Vert = dx < dy;

                foreach ( Entity entity in _entities )
                {
                    if (entity.Selected == false)
                    {
                        //
                        // Wire -> Vias
                        // 

                        if (IsEntityVias(entity) )
                        {
                            if (IsViasInWire(entity, source))
                                viases.Add(entity);
                        }

                        //
                        // Wire -> Wire
                        //

                        else if (IsEntityWire(entity))
                        {
                            PointF pointStart = new PointF(entity.LambdaX, entity.LambdaY);
                            PointF pointEnd = new PointF(entity.LambdaEndX, entity.LambdaEndY);

                            dist = (float)Math.Sqrt( Math.Pow(entity.LambdaX - source.LambdaX, 2) +
                                                     Math.Pow(entity.LambdaY - source.LambdaY, 2));

                            if ( dist < maxDist )
                            {
                                entity.Selected = true;
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                            dist = (float)Math.Sqrt(Math.Pow(entity.LambdaX - source.LambdaEndX, 2) +
                                                     Math.Pow(entity.LambdaY - source.LambdaEndY, 2));

                            if (dist < maxDist && entity.Selected == false)
                            {
                                entity.Selected = true;
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                            dist = (float)Math.Sqrt( Math.Pow(entity.LambdaEndX - source.LambdaEndX, 2) +
                                                     Math.Pow(entity.LambdaEndY - source.LambdaEndY, 2));

                            if ( dist < maxDist && entity.Selected == false)
                            {
                                entity.Selected = true;
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                            dist = (float)Math.Sqrt( Math.Pow(entity.LambdaEndX - source.LambdaX, 2) +
                                                     Math.Pow(entity.LambdaEndY - source.LambdaY, 2));

                            if ( dist < maxDist && entity.Selected == false)
                            {
                                entity.Selected = true;
                                SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                            }

                        }
                    }
                }           // foreach


                //
                // Process viases
                //

                foreach (Entity entity in viases)
                {
                    entity.Selected = true;
                    SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                }

            }

            //
            // Vias joint Cell/Wire
            //

            else if ( IsEntityVias(source) )
            {
                PointF point = new PointF(source.LambdaX, source.LambdaY);
                PointF[] rect = new PointF[4];
                List<Entity> wires = new List<Entity>();
                List<Entity> cells = new List<Entity>();

                //
                // Collect all wires/cells by vias intersections
                //

                foreach ( Entity entity in _entities )
                {
                    if ( entity.Selected == false )
                    {
                        //
                        // Vias -> Wire
                        //

                        if ( IsEntityWire (entity) )
                        {
                            if (IsViasInWire (source, entity))
                                wires.Add(entity);
                        }

                        //
                        // Vias -> Cell
                        //

                        else if ( IsEntityCell (entity) )
                        {
                            rect[0].X = entity.LambdaX;
                            rect[0].Y = entity.LambdaY;

                            rect[1].X = entity.LambdaX;
                            rect[1].Y = entity.LambdaY + entity.LambdaHeight;

                            rect[2].X = entity.LambdaX + entity.LambdaWidth;
                            rect[2].Y = entity.LambdaY + entity.LambdaHeight;

                            rect[3].X = entity.LambdaX + entity.LambdaWidth;
                            rect[3].Y = entity.LambdaY;

                            if (PointInPoly(rect, point))
                                cells.Add(entity);
                        }
                    }
                }

                //
                // Process
                //

                foreach ( Entity entity in wires )
                {
                    entity.Selected = true;
                    SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);

                    //
                    // Only single child
                    //

                    break;
                }

                foreach (Entity entity in cells)
                {
                    entity.Selected = true;
                    SelectTraverse(entity, Tier+1, TierMax, Depth + 1, Debug);

                    //
                    // Only single child
                    //

                    break;
                }

            }

            //
            // Cell joint Vias
            //

            else if ( IsEntityCell(source) )
            {
                PointF[] rect = new PointF[4];
                List<Entity> viases = new List<Entity>();

                rect[0].X = source.LambdaX;
                rect[0].Y = source.LambdaY;

                rect[1].X = source.LambdaX;
                rect[1].Y = source.LambdaY + source.LambdaHeight;

                rect[2].X = source.LambdaX + source.LambdaWidth;
                rect[2].Y = source.LambdaY + source.LambdaHeight;

                rect[3].X = source.LambdaX + source.LambdaWidth;
                rect[3].Y = source.LambdaY;

                //
                // Add Output vias intersecting given cell
                //

                foreach ( Entity entity in _entities )
                {
                    if ( entity.Selected == false && 
                        (entity.Type == EntityType.ViasOutput || entity.Type == EntityType.ViasInout) )
                    {
                        PointF point = new PointF(entity.LambdaX, entity.LambdaY);

                        if (PointInPoly(rect, point))
                            viases.Add(entity);
                    }
                }

                //
                // Process added viases
                //

                foreach ( Entity entity in viases )
                {
                    entity.Selected = true;
                    SelectTraverse(entity, Tier, TierMax, Depth + 1, Debug);
                }
            }
        }

        public void TraversalSelection (int TierMax)
        {
            List<Entity> selectedEnts = new List<Entity>();

            foreach ( Entity entity in _entities )
            {
                if ( (IsEntityCell(entity) || IsEntityVias(entity) || IsEntityWire(entity)) 
                     && entity.Selected )
                    selectedEnts.Add(entity);
            }

            foreach ( Entity entity in selectedEnts)
            {
                SelectTraverse(entity, 0, TierMax, 0, false);
            }

            Invalidate();
        }

        //
        // Wire extend
        //

        private Entity FirstSelectedWire ()
        {
            Entity wire = null;

            foreach (Entity entity in _entities)
            {
                if (IsEntityWire(entity) && entity.Selected)
                {
                    wire = entity;
                    break;
                }
            }

            return wire;
        }

        private float WireTangentInclination (Entity wire)
        {
            PointF start = new PointF(wire.LambdaX, wire.LambdaY);
            PointF end = new PointF(wire.LambdaEndX, wire.LambdaEndY);

            //
            // Calculate tangent inclination 
            //

            float a = end.Y - start.Y;
            float b = end.X - start.X;
            return (float)Math.Atan(a / b);
        }

        public void WireExtendHead ()
        {
            Entity wire = FirstSelectedWire ();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = WireTangentInclination(wire);

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaX += (float)Math.Cos(alpha);
                wire.LambdaY += (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaX -= (float)Math.Cos(alpha);
                wire.LambdaY -= (float)Math.Sin(alpha);
            }
        }

        public void WireExtendTail ()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = WireTangentInclination(wire);

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaEndX -= (float)Math.Cos(alpha);
                wire.LambdaEndY -= (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaEndX += (float)Math.Cos(alpha);
                wire.LambdaEndY += (float)Math.Sin(alpha);
            }
        }

        public void WireShortenHead()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = WireTangentInclination(wire);

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaX -= (float)Math.Cos(alpha);
                wire.LambdaY -= (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaX += (float)Math.Cos(alpha);
                wire.LambdaY += (float)Math.Sin(alpha);
            }
        }

        public void WireShortenTail()
        {
            Entity wire = FirstSelectedWire();

            if (wire == null)
                return;

            //
            // Extend
            //

            float alpha = WireTangentInclination(wire);

            if (wire.LambdaEndX < wire.LambdaX)
            {
                wire.LambdaEndX += (float)Math.Cos(alpha);
                wire.LambdaEndY += (float)Math.Sin(alpha);
            }
            else
            {
                wire.LambdaEndX -= (float)Math.Cos(alpha);
                wire.LambdaEndY -= (float)Math.Sin(alpha);
            }
        }

        //
        // Wire Recognition
        //

        private int AverageSubImageColor ( Image image, Rectangle srcRect)
        {
            Bitmap bmp = new Bitmap(1, 1);
            Color pixel = new Color();

            if (srcRect.X + srcRect.Width > image.Width)
                return 0;

            if (srcRect.Y + srcRect.Height > image.Height)
                return 0;

            if (srcRect.X < 0 || srcRect.Y < 0)
                return 0;

            using ( Graphics gr = Graphics.FromImage(bmp) )
            {
                gr.InterpolationMode = InterpolationMode.NearestNeighbor;
                gr.DrawImage(image, new Rectangle(0, 0, 1, 1), srcRect, GraphicsUnit.Pixel);
                pixel = bmp.GetPixel(0, 0);
                gr.Dispose();
            }

            int yc = (int) (0.2126f * (float)pixel.R +
                            0.752f * (float)pixel.G +
                            0.0722f * (float)pixel.B);
            yc = Math.Max(0, Math.Min(yc, 255));
            return yc;
        }

        private Point LambdaToImage(PointF imageScroll, PointF origin)
        {
            Point dest = new Point();
            dest.X = (int)((origin.X - imageScroll.X) * Lambda);
            dest.Y = (int)((origin.Y - imageScroll.Y) * Lambda);
            return dest;
        }

        private PointF ImageToLamda(PointF imageScroll, Point origin)
        {
            PointF dest = new PointF();
            dest.X = (float)origin.X / Lambda + imageScroll.X;
            dest.Y = (float)origin.Y / Lambda + imageScroll.Y;
            return dest;
        }

        public void WireRecognize ()
        {
            Image sourceImage = null;
            PointF imageScroll = new PointF();
            int imageZoom;
            int gate = 10;

            //
            // Lock current image
            //

            switch ( Mode )
            {
                default:
                    sourceImage = _imageOrig[0];
                    imageScroll = _imageScroll[0];
                    imageZoom = _imageZoom[0];
                    break;
                case EntityMode.ImageLayer1:
                    sourceImage = _imageOrig[1];
                    imageScroll = _imageScroll[1];
                    imageZoom = _imageZoom[1];
                    break;
                case EntityMode.ImageLayer2:
                    sourceImage = _imageOrig[2];
                    imageScroll = _imageScroll[2];
                    imageZoom = _imageZoom[2];
                    break;
            }

            if ( sourceImage == null )
            {
                MessageBox.Show("Image not loaded");
                return;
            }

            //
            // Grab first selected wire
            //

            Entity wire = FirstSelectedWire();

            if (wire == null)
            {
                MessageBox.Show("No wire selected");
                return;
            }

            //
            // Determine initial conditions
            //

            float dx = Math.Abs(wire.LambdaEndX - wire.LambdaX);
            float dy = Math.Abs(wire.LambdaEndY - wire.LambdaY);
            bool Horz = dx > dy;

            PointF startLambda = new PointF(wire.LambdaX, wire.LambdaY);
            Point start = LambdaToImage(imageScroll, startLambda);

            PointF endLambda = new PointF(wire.LambdaEndX, wire.LambdaEndY);
            Point end = LambdaToImage(imageScroll, endLambda);

            Rectangle sourceRect = new Rectangle(start.X,
                                         start.Y - 1,
                                         end.X - start.X,
                                         2);

            int avgColor = AverageSubImageColor(sourceImage, sourceRect);

            //MessageBox.Show("Rect: " + sourceRect.ToString() + "\n" + "Avg Color: " + avgColor.ToString());

            //
            // Propagate end
            //

            int saved_x = sourceRect.X;

            while (true)
            {
                sourceRect.X += WireBaseSize;

                int col = AverageSubImageColor(sourceImage, sourceRect);

                int diff = Math.Abs(avgColor - col);
                if (diff > gate)
                    break;

                //
                // Other wire hit test
                //
            }

            Point endPoint = new Point(sourceRect.X, sourceRect.Y);
            PointF lambda = ImageToLamda(imageScroll, endPoint);
            wire.LambdaEndX = lambda.X;

            //
            // Propagate start
            //

            sourceRect.X = saved_x;

            while (true)
            {
                sourceRect.X -= WireBaseSize;

                int col = AverageSubImageColor(sourceImage, sourceRect);

                int diff = Math.Abs(avgColor - col);
                if (diff > gate)
                    break;

                //
                // Other wire hit test
                //
            }

            endPoint.X = sourceRect.X;
            endPoint.Y = sourceRect.Y;
            lambda = ImageToLamda(imageScroll, endPoint);
            wire.LambdaX = lambda.X;

            Invalidate();
        }

        //
        // Lambda Transformation
        //

        public void LambdaScale(float scale)
        {
            List<Entity> sourceList = new List<Entity>();

            //
            // Grab selected entities
            //

            foreach ( Entity entity in _entities )
            {
                if (entity.Selected)
                    sourceList.Add(entity);
            }

            if (sourceList.Count == 0)
                return;

            //
            // Scale
            //

            foreach (Entity entity in sourceList )
            {
                if (IsEntityRegion(entity))
                {
                    for (int i = 0; i < entity.PathPoints.Count; i++ )
                    {
                        PointF scaled = new PointF();

                        scaled.X = entity.PathPoints[i].X * scale;
                        scaled.Y = entity.PathPoints[i].Y * scale;

                        entity.PathPoints[i] = scaled;
                    }
                }
                else
                {
                    entity.LambdaX *= scale;
                    entity.LambdaY *= scale;
                    entity.LambdaEndX *= scale;
                    entity.LambdaEndY *= scale;
                    entity.LambdaWidth *= scale;
                    entity.LambdaHeight *= scale;
                }
            }

            Invalidate();
        }

        //
        // Select All
        //

        public void SelectAll(EntitySelection kind = EntitySelection.All)
        {
            foreach ( Entity entity in _entities )
            {
                switch (kind)
                {
                    case EntitySelection.Vias:
                        if (IsEntityVias(entity))
                            entity.Selected = true;
                        break;

                    case EntitySelection.Wire:
                        if (IsEntityWire(entity))
                            entity.Selected = true;
                        break;

                    case EntitySelection.Cell:
                        if (IsEntityCell(entity))
                            entity.Selected = true;
                        break;

                    default:
                    case EntitySelection.All:
                        entity.Selected = true;
                        break;
                }
            }

            Invalidate();
        }

        //
        // Draw region between selected viases
        //

        public void DrawRegionBetweenSelectedViases ()
        {
            List<Entity> selected = new List<Entity>();

            //
            // Grab selected viases (minimum 3)
            //

            foreach ( Entity entity in _entities)
            {
                if ( IsEntityVias(entity) && entity.Selected )
                {
                    selected.Add(entity);
                }
            }

            if (selected.Count < 3)
                return;

            //
            // Fill path
            //

            List<PointF> path = new List<PointF>();

            foreach ( Entity entity in selected )
            {
                PointF point = new PointF();

                point.X = entity.LambdaX;
                point.Y = entity.LambdaY;

                path.Add ( point );
            }

            //
            // Add new region entity
            //

            Entity item = new Entity();

            item.Type = EntityType.Region;
            item.Label = "";
            item.LabelAlignment = TextAlignment.GlobalSettings;
            item.Priority = RegionPriority;
            item.Selected = false;
            item.PathPoints = path;
            item.ColorOverride = RegionOverrideColor;
            item.FontOverride = null;
            item.SetParent(this);

            _entities.Add(item);
            SortEntities();

            Invalidate ();
        }

    }       // EntityBox

    #region Workspace stuff

    //
    // Workspace snapshot
    //

    public class EntityBoxWorkspace
    {
        private Image[] imageOrig = new Image[3];
        public float lambda;

        [XmlElement("Image0")]
        [Browsable(false)]
        public byte[] Image0
        {
            get
            {
                if (imageOrig[0] != null)
                {
                    MemoryStream ms = new MemoryStream();
                    imageOrig[0].Save(ms, ImageFormat.Jpeg);
                    return ms.ToArray();
                }
                else return null;
            }
            set
            {
                imageOrig[0] = Image.FromStream(new MemoryStream(value) );
            }
        }

        [XmlElement("Image1")]
        [Browsable(false)]
        public byte[] Image1
        {
            get
            {
                if (imageOrig[1] != null)
                {
                    MemoryStream ms = new MemoryStream();
                    imageOrig[1].Save(ms, ImageFormat.Jpeg);
                    return ms.ToArray();
                }
                else return null;
            }
            set
            {
                imageOrig[1] = Image.FromStream(new MemoryStream(value));
            }
        }

        [XmlElement("Image2")]
        [Browsable(false)]
        public byte[] Image2
        {
            get
            {
                if (imageOrig[2] != null)
                {
                    MemoryStream ms = new MemoryStream();
                    imageOrig[2].Save(ms, ImageFormat.Jpeg);
                    return ms.ToArray();
                }
                else return null;
            }
            set
            {
                imageOrig[2] = Image.FromStream(new MemoryStream(value));
            }
        }

        public int[] imageZoom = new int[3];
        public int zoom;
        public PointF[] imageScroll = new PointF[3];
        public float ScrollX;
        public float ScrollY;
        public List<Entity> entities;
        public EntityMode drawMode;
        public bool hideImage;
        public bool hideVias;
        public bool hideWires;
        public bool hideCells;
        public bool hideGrid;
        public bool hideRegions;
        [XmlIgnore] public Color selectionBoxColor;
        [XmlIgnore] public Color ForeColor;
        public int[] imageOpacity = new int[3];
        public bool[] lockScroll = new bool[3];
        public bool[] lockZoom = new bool[3];
        public bool grayscale;

        [XmlIgnore] public Color ViasInputColor;
        [XmlIgnore] public Color ViasOutputColor;
        [XmlIgnore] public Color ViasInoutColor;
        [XmlIgnore] public Color ViasConnectColor;
        [XmlIgnore] public Color ViasFloatingColor;
        [XmlIgnore] public Color ViasPowerColor;
        [XmlIgnore] public Color ViasGroundColor;
        [XmlIgnore] public Color WireInterconnectColor;
        [XmlIgnore] public Color WirePowerColor;
        [XmlIgnore] public Color WireGroundColor;
        [XmlIgnore] public Color CellNotColor;
        [XmlIgnore] public Color CellBufferColor;
        [XmlIgnore] public Color CellMuxColor;
        [XmlIgnore] public Color CellLogicColor;
        [XmlIgnore] public Color CellAdderColor;
        [XmlIgnore] public Color CellBusSuppColor;
        [XmlIgnore] public Color CellFlipFlopColor;
        [XmlIgnore] public Color CellLatchColor;
        [XmlIgnore] public Color CellOtherColor;
        [XmlIgnore] public Color UnitRegfileColor;
        [XmlIgnore] public Color UnitMemoryColor;
        [XmlIgnore] public Color UnitCustomColor;
        [XmlIgnore] public Color SelectionColor;
        [XmlIgnore] public Color ViasOverrideColor;
        [XmlIgnore] public Color WireOverrideColor;
        [XmlIgnore] public Color CellOverrideColor;
        [XmlIgnore] public Color RegionOverrideColor;
        public ViasShape viasShape;
        public int viasBaseSize;
        public int wireBaseSize;
        public TextAlignment cellTextAlignment;
        public TextAlignment viasTextAlignment;
        public TextAlignment wireTextAlignment;
        public int ViasOpacity;
        public int WireOpacity;
        public int CellOpacity;
        public int RegionOpacity;
        public int ViasPriority;
        public int WirePriority;
        public int CellPriority;
        public int BeaconPriority;
        public int RegionPriority;
        public bool AutoPriority;

        [XmlElement("ForeColor")]
        [Browsable(false)]
        public string ForeColorHtml
        {
            get { return ColorTranslator.ToHtml(ForeColor); }
            set { ForeColor = ColorTranslator.FromHtml(value); }
        }


        [XmlElement("SelectionBoxColor")]
        [Browsable(false)]
        public string SelectionBoxColorHtml
        {
            get { return ColorTranslator.ToHtml(selectionBoxColor); }
            set { selectionBoxColor = ColorTranslator.FromHtml(value); }
        }


        [XmlElement("ViasInputColor")]
        [Browsable(false)]
        public string ViasInputColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasInputColor); }
            set { ViasInputColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("ViasOutputColor")]
        [Browsable(false)]
        public string ViasOutputColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasOutputColor); }
            set { ViasOutputColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("ViasInoutColor")]
        [Browsable(false)]
        public string ViasInoutColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasInoutColor); }
            set { ViasInoutColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("ViasConnectColor")]
        [Browsable(false)]
        public string ViasConnectColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasConnectColor); }
            set { ViasConnectColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("ViasFloatingColor")]
        [Browsable(false)]
        public string ViasFloatingColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasFloatingColor); }
            set { ViasFloatingColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("ViasPowerColor")]
        [Browsable(false)]
        public string ViasPowerColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasPowerColor); }
            set { ViasPowerColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("ViasGroundColor")]
        [Browsable(false)]
        public string ViasGroundColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasGroundColor); }
            set { ViasGroundColor = ColorTranslator.FromHtml(value); }
        }


        [XmlElement("WireInterconnectColor")]
        [Browsable(false)]
        public string WireInterconnectColorHtml
        {
            get { return ColorTranslator.ToHtml(WireInterconnectColor); }
            set { WireInterconnectColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("WirePowerColor")]
        [Browsable(false)]
        public string WirePowerColorHtml
        {
            get { return ColorTranslator.ToHtml(WirePowerColor); }
            set { WirePowerColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("WireGroundColor")]
        [Browsable(false)]
        public string WireGroundColorHtml
        {
            get { return ColorTranslator.ToHtml(WireGroundColor); }
            set { WireGroundColor = ColorTranslator.FromHtml(value); }
        }


        [XmlElement("CellNotColor")]
        [Browsable(false)]
        public string CellNotColorHtml
        {
            get { return ColorTranslator.ToHtml(CellNotColor); }
            set { CellNotColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellBufferColor")]
        [Browsable(false)]
        public string CellBufferColorHtml
        {
            get { return ColorTranslator.ToHtml(CellBufferColor); }
            set { CellBufferColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellMuxColor")]
        [Browsable(false)]
        public string CellMuxColorHtml
        {
            get { return ColorTranslator.ToHtml(CellMuxColor); }
            set { CellMuxColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellLogicColor")]
        [Browsable(false)]
        public string CellLogicColorHtml
        {
            get { return ColorTranslator.ToHtml(CellLogicColor); }
            set { CellLogicColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellAdderColor")]
        [Browsable(false)]
        public string CellAdderColorHtml
        {
            get { return ColorTranslator.ToHtml(CellAdderColor); }
            set { CellAdderColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellBusSuppColor")]
        [Browsable(false)]
        public string CellBusSuppColorHtml
        {
            get { return ColorTranslator.ToHtml(CellBusSuppColor); }
            set { CellBusSuppColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellFlipFlopColor")]
        [Browsable(false)]
        public string CellFlipFlopColorHtml
        {
            get { return ColorTranslator.ToHtml(CellFlipFlopColor); }
            set { CellFlipFlopColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellLatchColor")]
        [Browsable(false)]
        public string CellLatchColorHtml
        {
            get { return ColorTranslator.ToHtml(CellLatchColor); }
            set { CellLatchColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellOtherColor")]
        [Browsable(false)]
        public string CellOtherColorHtml
        {
            get { return ColorTranslator.ToHtml(CellOtherColor); }
            set { CellOtherColor = ColorTranslator.FromHtml(value); }
        }


        [XmlElement("UnitRegfileColor")]
        [Browsable(false)]
        public string UnitRegfileColorHtml
        {
            get { return ColorTranslator.ToHtml(UnitRegfileColor); }
            set { UnitRegfileColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("UnitMemoryColor")]
        [Browsable(false)]
        public string UnitMemoryColorHtml
        {
            get { return ColorTranslator.ToHtml(UnitMemoryColor); }
            set { UnitMemoryColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("UnitCustomColor")]
        [Browsable(false)]
        public string UnitCustomColorHtml
        {
            get { return ColorTranslator.ToHtml(UnitCustomColor); }
            set { UnitCustomColor = ColorTranslator.FromHtml(value); }
        }

        [XmlElement("SelectionColor")]
        [Browsable(false)]
        public string SelectionColorHtml
        {
            get { return ColorTranslator.ToHtml(SelectionColor); }
            set { SelectionColor = ColorTranslator.FromHtml(value); }
        }

        [XmlElement("ViasOverrideColor")]
        [Browsable(false)]
        public string ViasOverrideColorHtml
        {
            get { return ColorTranslator.ToHtml(ViasOverrideColor); }
            set { ViasOverrideColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("WireOverrideColor")]
        [Browsable(false)]
        public string WireOverrideColorHtml
        {
            get { return ColorTranslator.ToHtml(WireOverrideColor); }
            set { WireOverrideColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("CellOverrideColor")]
        [Browsable(false)]
        public string CellOverrideColorHtml
        {
            get { return ColorTranslator.ToHtml(CellOverrideColor); }
            set { CellOverrideColor = ColorTranslator.FromHtml(value); }
        }
        [XmlElement("RegionOverrideColor")]
        [Browsable(false)]
        public string RegionOverrideColorHtml
        {
            get { return ColorTranslator.ToHtml(RegionOverrideColor); }
            set { RegionOverrideColor = ColorTranslator.FromHtml(value); }
        }

        public EntityBoxWorkspace()
        {
        }

        public EntityBoxWorkspace(EntityBox parent)
        {
            if (parent.Image0 != null)
                imageOrig[0] = new Bitmap(parent.Image0);
            else
                imageOrig[0] = null;

            if (parent.Image1 != null)
                imageOrig[1] = new Bitmap(parent.Image1);
            else
                imageOrig[1] = null;

            if (parent.Image2 != null)
                imageOrig[2] = new Bitmap(parent.Image2);
            else
                imageOrig[2] = null;

            lambda = parent.Lambda;
            imageZoom[0] = parent.ZoomImage0;
            imageZoom[1] = parent.ZoomImage1;
            imageZoom[2] = parent.ZoomImage2;
            zoom = parent.Zoom;
            imageScroll[0] = parent.ScrollImage0;
            imageScroll[1] = parent.ScrollImage1;
            imageScroll[2] = parent.ScrollImage2;
            ScrollX = parent.ScrollX;
            ScrollY = parent.ScrollY;
            entities = parent._entities;
            drawMode = parent.Mode;
            hideImage = parent.HideImage;
            hideVias = parent.HideVias;
            hideWires = parent.HideWires;
            hideCells = parent.HideCells;
            hideGrid = parent.HideGrid;
            hideRegions = parent.HideRegions;
            selectionBoxColor = parent.SelectionBoxColor;
            ForeColor = parent.ForeColor;
            imageOpacity[0] = parent.ImageOpacity0;
            imageOpacity[1] = parent.ImageOpacity1;
            imageOpacity[2] = parent.ImageOpacity2;
            lockScroll[0] = parent.LockScroll0;
            lockScroll[1] = parent.LockScroll1;
            lockScroll[2] = parent.LockScroll2;
            lockZoom[0] = parent.LockZoom0;
            lockZoom[1] = parent.LockZoom1;
            lockZoom[2] = parent.LockZoom2;
            grayscale = parent.Grayscale;

            ViasInputColor = parent.ViasInputColor;
            ViasOutputColor = parent.ViasOutputColor;
            ViasInoutColor = parent.ViasInoutColor;
            ViasConnectColor = parent.ViasConnectColor;
            ViasFloatingColor = parent.ViasFloatingColor;
            ViasPowerColor = parent.ViasPowerColor;
            ViasGroundColor = parent.ViasGroundColor;
            WireInterconnectColor = parent.WireInterconnectColor;
            WirePowerColor = parent.WirePowerColor;
            WireGroundColor = parent.WireGroundColor;
            CellNotColor = parent.CellNotColor;
            CellBufferColor = parent.CellBufferColor;
            CellMuxColor = parent.CellMuxColor;
            CellLogicColor = parent.CellLogicColor;
            CellAdderColor = parent.CellAdderColor;
            CellBusSuppColor = parent.CellBusSuppColor;
            CellFlipFlopColor = parent.CellFlipFlopColor;
            CellLatchColor = parent.CellLatchColor;
            CellOtherColor = parent.CellOtherColor;
            UnitRegfileColor = parent.UnitRegfileColor;
            UnitMemoryColor = parent.UnitMemoryColor;
            UnitCustomColor = parent.UnitCustomColor;
            SelectionColor = parent.SelectionColor;
            ViasOverrideColor = parent.ViasOverrideColor;
            WireOverrideColor = parent.WireOverrideColor;
            CellOverrideColor = parent.CellOverrideColor;
            RegionOverrideColor = parent.RegionOverrideColor;
            viasShape = parent.ViasShape;
            viasBaseSize = parent.ViasBaseSize;
            wireBaseSize = parent.WireBaseSize;
            cellTextAlignment = parent.CellTextAlignment;
            viasTextAlignment = parent.ViasTextAlignment;
            wireTextAlignment = parent.WireTextAlignment;
            ViasOpacity = parent.ViasOpacity;
            WireOpacity = parent.WireOpacity;
            CellOpacity = parent.CellOpacity;
            RegionOpacity = parent.RegionOpacity;
            ViasPriority = parent.ViasPriority;
            WirePriority = parent.WirePriority;
            CellPriority = parent.CellPriority;
            AutoPriority = parent.AutoPriority;
            BeaconPriority = parent.BeaconPriority;
            RegionPriority = parent.RegionPriority;
        }

        public void LoadSnapshot ( EntityBox parent )
        {
            parent.Image0 = imageOrig[0];
            parent.Image1 = imageOrig[1];
            parent.Image2 = imageOrig[2];

            parent.Lambda = lambda;
            parent.ZoomImage0 = imageZoom[0];
            parent.ZoomImage1 = imageZoom[1];
            parent.ZoomImage2 = imageZoom[2];
            parent.Zoom = zoom;
            parent.ScrollImage0 = imageScroll[0];
            parent.ScrollImage1 = imageScroll[1];
            parent.ScrollImage2 = imageScroll[2];
            parent.ScrollX = ScrollX;
            parent.ScrollY = ScrollY;

            parent._entities.Clear();
            parent._entities = entities;

            parent.Mode = drawMode;
            parent.HideImage = hideImage;
            parent.HideVias = hideVias;
            parent.HideWires = hideWires;
            parent.HideCells = hideCells;
            parent.HideGrid = hideGrid;
            parent.HideRegions = hideRegions;
            parent.SelectionBoxColor = selectionBoxColor;
            parent.ForeColor = ForeColor;
            parent.ImageOpacity0 = imageOpacity[0];
            parent.ImageOpacity1 = imageOpacity[1];
            parent.ImageOpacity2 = imageOpacity[2];
            parent.LockScroll0 = lockScroll[0];
            parent.LockScroll1 = lockScroll[1];
            parent.LockScroll2 = lockScroll[2];
            parent.LockZoom0 = lockZoom[0];
            parent.LockZoom1 = lockZoom[1];
            parent.LockZoom2 = lockZoom[2];
            parent.Grayscale = grayscale;

            parent.ViasInputColor = ViasInputColor;
            parent.ViasOutputColor = ViasOutputColor;
            parent.ViasInoutColor = ViasInoutColor;
            parent.ViasConnectColor = ViasConnectColor;
            parent.ViasFloatingColor = ViasFloatingColor;
            parent.ViasPowerColor = ViasPowerColor;
            parent.ViasGroundColor = ViasGroundColor;
            parent.WireInterconnectColor = WireInterconnectColor;
            parent.WirePowerColor = WirePowerColor;
            parent.WireGroundColor = WireGroundColor;
            parent.CellNotColor = CellNotColor;
            parent.CellBufferColor = CellBufferColor;
            parent.CellMuxColor = CellMuxColor;
            parent.CellLogicColor = CellLogicColor;
            parent.CellAdderColor = CellAdderColor;
            parent.CellBusSuppColor = CellBusSuppColor;
            parent.CellFlipFlopColor = CellFlipFlopColor;
            parent.CellLatchColor = CellLatchColor;
            parent.CellOtherColor = CellOtherColor;
            parent.UnitRegfileColor = UnitRegfileColor;
            parent.UnitMemoryColor = UnitMemoryColor;
            parent.UnitCustomColor = UnitCustomColor;
            parent.SelectionColor = SelectionColor;
            parent.ViasOverrideColor = ViasOverrideColor;
            parent.WireOverrideColor = WireOverrideColor;
            parent.CellOverrideColor = CellOverrideColor;
            parent.RegionOverrideColor = RegionOverrideColor;
            parent.ViasShape = viasShape;
            parent.ViasBaseSize = viasBaseSize;
            parent.WireBaseSize = wireBaseSize;
            parent.CellTextAlignment = cellTextAlignment;
            parent.ViasTextAlignment = viasTextAlignment;
            parent.WireTextAlignment = wireTextAlignment;
            parent.ViasOpacity = ViasOpacity;
            parent.WireOpacity = WireOpacity;
            parent.CellOpacity = CellOpacity;
            parent.RegionOpacity = RegionOpacity;
            parent.ViasPriority = ViasPriority;
            parent.WirePriority = WirePriority;
            parent.CellPriority = CellPriority;
            parent.AutoPriority = AutoPriority;
            parent.BeaconPriority = BeaconPriority;
            parent.RegionPriority = RegionPriority;

            GC.Collect();
            parent.Invalidate();
        }
    }

    #endregion Workspace stuff

}       // namespace
