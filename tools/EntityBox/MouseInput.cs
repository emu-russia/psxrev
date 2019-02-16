// Mouse stuff

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
        public Point GetLastRightMouseButton()
        {
            return LastRMB;
        }

        public float GetDragDistance()
        {
            return draggingDist;
        }

        //
        // Mouse hit test
        //

        private Entity EntityHitTest(int MouseX, int MouseY)
        {
            PointF point = new Point(MouseX, MouseY);
            PointF[] rect = new PointF[4];
            float zf = (float)Zoom / 100.0F;

            List<Entity> reversed = SortEntitiesReverse();

            foreach (Entity entity in reversed)
            {
                if (entity.IsWire() && HideWires == false)
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

                    PointF rot = RotatePoint(ortho, -90);
                    PointF normalized = new PointF(rot.X / len, rot.Y / len);
                    PointF baseVect = new PointF(normalized.X * ((WireBaseSize * zf) / 2),
                                                  normalized.Y * ((WireBaseSize * zf) / 2));

                    rect[0].X = baseVect.X + start.X;
                    rect[0].Y = baseVect.Y + start.Y;
                    rect[3].X = baseVect.X + end.X;
                    rect[3].Y = baseVect.Y + end.Y;

                    rot = RotatePoint(ortho, +90);
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
                else if (entity.IsCell() && HideCells == false)
                {
                    rect[0] = LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    rect[1] = LambdaToScreen(entity.LambdaX, entity.LambdaY + entity.LambdaHeight);
                    rect[2] = LambdaToScreen(entity.LambdaX + entity.LambdaWidth, entity.LambdaY + entity.LambdaHeight);
                    rect[3] = LambdaToScreen(entity.LambdaX + entity.LambdaWidth, entity.LambdaY);

                    if (PointInPoly(rect, point) == true)
                        return entity;
                }
                else if (entity.Type == EntityType.Beacon)
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
                else if (entity.IsVias() && HideVias == false)
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
                else if (entity.IsRegion() && HideRegions == false)
                {
                    PointF[] poly = new PointF[entity.PathPoints.Count];

                    int idx = 0;
                    foreach (PointF pathPoint in entity.PathPoints)
                    {
                        poly[idx++] = (PointF)LambdaToScreen(pathPoint.X, pathPoint.Y);
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
                 && DraggingBegin == false && SelectionBegin == false)
            {
                selected = GetSelected();

                if (selected.Count > 0)
                {
                    foreach (Entity entity in selected)
                    {
                        if (entity.IsRegion())
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

            if (e.Button == MouseButtons.Right)
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
                        foreach (Entity ent in GetEntities())
                        {
                            if (ent.Selected)
                                continue;

                            if (ent.IsCell())
                            {
                                RectangleF rect2 = new RectangleF(ent.LambdaX, ent.LambdaY,
                                                                   ent.LambdaWidth, ent.LambdaHeight);

                                if (rect.IntersectsWith(rect2))
                                {
                                    SelectEntity(ent);
                                    CatchSomething = true;
                                }
                            }
                            else if (ent.IsWire())
                            {
                                PointF point1 = new PointF(ent.LambdaX, ent.LambdaY);
                                PointF point2 = new PointF(ent.LambdaEndX, ent.LambdaEndY);

                                if (LineIntersectsRect(point1, point2, rect))
                                {
                                    SelectEntity(ent);
                                    CatchSomething = true;
                                }
                            }
                            else if (ent.IsVias())
                            {
                                PointF point1 = new PointF(ent.LambdaX, ent.LambdaY);

                                if (rect.Contains(point1))
                                {
                                    SelectEntity(ent);
                                    CatchSomething = true;
                                }
                            }
                            else if (ent.IsRegion())
                            {
                                foreach (PointF point in ent.PathPoints)
                                {
                                    if (rect.Contains(point))
                                    {
                                        SelectEntity(ent);
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
                        SelectEntity(entity);
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
                AddVias((EntityType)Mode, e.X, e.Y, Color.Black);

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

                if (draggingDist < 2 * Lambda)
                {
                    foreach (Entity entity in selected)
                    {
                        entity.LambdaX = entity.SavedLambdaX;
                        entity.LambdaY = entity.SavedLambdaY;
                        entity.LambdaEndX = entity.SavedLambdaEndX;
                        entity.LambdaEndY = entity.SavedLambdaEndY;

                        if (OnEntityScroll != null)
                            OnEntityScroll(this, entity, EventArgs.Empty);
                    }
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
                    if (entity.IsRegion())
                    {
                        for (int i = 0; i < entity.PathPoints.Count; i++)
                        {
                            Point point = LambdaToScreen(entity.SavedPathPoints[i].X,
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

                        if (OnEntityScroll != null)
                            OnEntityScroll(this, entity, EventArgs.Empty);
                    }

                    Point dist = new Point(Math.Abs(e.X - DragStartMouseX),
                                             Math.Abs(e.Y - DragStartMouseY));

                    draggingDist = (float)Math.Sqrt(Math.Pow(dist.X, 2) +
                                                     Math.Pow(dist.Y, 2));
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


    }
}
