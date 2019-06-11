// Entity Props

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
        public bool HideLambdaMetrics
        {
            get { return hideLambdaMetrics; }
            set { hideLambdaMetrics = value; Invalidate(); }
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

                float zf = (float)_imageZoom[2] / 100F;
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
            _viasBaseSize = 4;
            _wireBaseSize = 5;
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

    }
}
