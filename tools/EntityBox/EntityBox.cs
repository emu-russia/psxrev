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

        ImageLayer0,        // Topmost
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

    public partial class EntityBox : Control
    {
        public Entity root;
        private Entity insertionNode;            // Destination for add/paste operations (root by default)

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
        private EntityMode drawMode = EntityMode.Selection;
        private bool hideImage;
        private bool hideVias;
        private bool hideWires;
        private bool hideCells;
        private bool hideGrid;
        private bool hideLambdaMetrics;
        private bool hideRegions;
        private PropertyGrid entityGrid;
        private List<Entity> selected;
        private float draggingDist;
        private Color selectionBoxColor;
        private int[] _imageOpacity = new int[3];
        private BufferedGraphics gfx = null;
        private BufferedGraphicsContext context;
        private bool[] lockScroll = new bool[3];
        private bool[] lockZoom = new bool[3];
        private bool selectEntitiesAfterAdd;
        private bool wireSelectionAutoTraverse;
        private long UnserializeLastStamp = 0;
        private bool grayscale = false;
        private Point LastRMB = new Point(-1, -1);
        private bool DrawInProgress;
        private List<Entity> copied = new List<Entity>();
        private PointF TopLeftCopied;
        private bool DrawStats = false;
        private bool EnableOpacity = true;

        public event EntityBoxEventHandler OnScrollChanged = null;
        public event EntityBoxEventHandler OnZoomChanged = null;
        public event EntityBoxEventHandler OnEntityCountChanged = null;
        public event EntityBoxEntityEventHandler OnEntityLabelEdit = null;
        public event EntityBoxEntityEventHandler OnEntitySelect = null;
        public event EntityBoxEntityEventHandler OnEntityAdd = null;
        public event EntityBoxEntityEventHandler OnEntityRemove = null;
        public event EntityBoxEntityEventHandler OnEntityScroll = null;
        public event EntityBoxEntityEventHandler OnDestinationNodeChanged = null;
        public event EntityBoxFrameDoneHandler OnFrameDone = null;

        public EntityBox()
        {
            BackColor = SystemColors.WindowFrame;
            ForeColor = Color.Snow;

            root = new Entity();
            root.Type = EntityType.Root;

            SetDestinationNode(root);

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

        public List<Entity> GetEntities()
        {
            List<Entity> list = new List<Entity>();

            foreach (var entity in root.Children)
            {
                GetEntitiesRecursive(entity, list);
            }

            return list;
        }

        public void SetDestinationNode(Entity node)
        {
            insertionNode = node;
            OnDestinationNodeChanged?.Invoke(this, node, EventArgs.Empty);
        }

        private void GetEntitiesRecursive(Entity parent, List<Entity> list)
        {
            list.Add(parent);

            foreach (var entity in parent.Children)
            {
                GetEntitiesRecursive(entity, list);
            }
        }


        public void EnsureVisible ( Entity entity )
        {
            Point screen = LambdaToScreen(entity.LambdaX, entity.LambdaY);

            if ( screen.X < WireBaseSize * 2 || screen.Y < WireBaseSize * 2 ||
                screen.X >= Width - WireBaseSize * 2 || screen.Y >= Height - WireBaseSize * 2)
            {
                PointF center = ScreenToLambda(Width, Height);
                float zf = (float)Zoom / 100F;

                ScrollX = -entity.LambdaX + (Width/2/ (zf*Lambda));
                ScrollY = -entity.LambdaY + (Height/2 / (zf*Lambda));

                Invalidate();
            }
        }

        public int GetViasCount()
        {
            List<Entity> _entities = GetEntities();

            int Count = 0;
            foreach (Entity entity in _entities)
            {
                if (entity.IsVias())
                    Count++;
            }
            return Count;
        }

        public int GetWireCount()
        {
            List<Entity> _entities = GetEntities();

            int Count = 0;
            foreach (Entity entity in _entities)
            {
                if (entity.IsWire())
                    Count++;
            }
            return Count;
        }

        public int GetCellCount()
        {
            List<Entity> _entities = GetEntities();

            int Count = 0;
            foreach (Entity entity in _entities)
            {
                if (entity.IsCell())
                    Count++;
            }
            return Count;
        }


        public int GetBeaconCount ()
        {
            List<Entity> _entities = GetEntities();

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
            List<Entity> _entities = GetEntities();

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
        // Lambda Transformation
        //

        public void LambdaScale(float scale)
        {
            //
            // Grab selected entities
            //

            List<Entity> sourceList = GetSelected();

            if (sourceList.Count == 0)
                return;

            //
            // Scale
            //

            foreach (Entity entity in sourceList )
            {
                if (entity.IsRegion())
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



    }       // EntityBox


}       // namespace
