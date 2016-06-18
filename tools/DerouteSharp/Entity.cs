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

public enum EntityType
{
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
    Region,
}

public enum ViasShape
{
    Square = 1,
    Round,
}

public enum TextAlignment
{
    GlobalSettings,
    Top,
    TopLeft,
    TopRight,
    BottomLeft,
    Bottom,
    BottomRight,
}

public class Entity
{
    private string _Label;
    private float _LambdaWidth;
    private float _LambdaHeight;
    private float _LambdaX;
    private float _LambdaY;
    private float _LambdaEndX;
    private float _LambdaEndY;
    private EntityType _Type;
    private bool _Selected;
    private EntityBox parentBox = null;
    private Color _ColorOverride;
    private TextAlignment labelAlignment;
    private int _Priority;
    private List<PointF> _pathPoints = null;
    [XmlIgnore]
    public List<PointF> SavedPathPoints = null;

    [XmlIgnore]
    public long SelectTimeStamp;

    [XmlIgnore]
    public float SavedLambdaX;
    [XmlIgnore]
    public float SavedLambdaY;
    [XmlIgnore]
    public float SavedLambdaEndX;
    [XmlIgnore]
    public float SavedLambdaEndY;

    [Category("Entity Properties")]
    public string Label
    {
        get { return _Label; }
        set
        {
            _Label = value;

            if (parentBox != null)
            {
                parentBox.LabelEdited(this);
                parentBox.Invalidate();
            }
        }
    }

    [Category("Entity Properties")]
    public float LambdaWidth
    {
        get { return _LambdaWidth; }
        set { _LambdaWidth = value;
            if (parentBox != null) parentBox.Invalidate(); }
    }

    [Category("Entity Properties")]
    public float LambdaHeight
    {
        get { return _LambdaHeight; }
        set { _LambdaHeight = value;
            if (parentBox != null) parentBox.Invalidate(); }
    }

    [Category("Entity Properties")]
    public float LambdaX
    {
        get { return _LambdaX; }
        set { _LambdaX = value;
            if (parentBox != null) parentBox.Invalidate();  }
    }

    [Category("Entity Properties")]
    public float LambdaY
    {
        get { return _LambdaY; }
        set { _LambdaY = value;
            if (parentBox != null) parentBox.Invalidate();     }
    }

    [Category("Entity Properties")]
    public float LambdaEndX
    {
        get { return _LambdaEndX; }
        set { _LambdaEndX = value;
            if (parentBox != null) parentBox.Invalidate();    }
    }

    [Category("Entity Properties")]
    public float LambdaEndY
    {
        get { return _LambdaEndY; }
        set { _LambdaEndY = value;
            if (parentBox != null) parentBox.Invalidate();    }
    }

    [Category("Entity Properties")]
    public EntityType Type
    {
        get { return _Type; }
        set { _Type = value;
            if (parentBox != null) parentBox.Invalidate();    }
    }

    [Category("Entity Properties")]
    [XmlIgnore]
    public bool Selected
    {
        get { return _Selected; }
        set
        {
            _Selected = value;

            SelectTimeStamp = DateTime.Now.Ticks;

            if (parentBox != null)
                parentBox.Invalidate();
        }
    }

    [Category("Entity Properties")]
    [Description("Set color other than Black to override it.")]
    [XmlIgnore]
    public Color ColorOverride
    {
        get { return _ColorOverride; }
        set
        {
            _ColorOverride = value;
            if (parentBox != null) parentBox.Invalidate();
        }
    }

    [XmlElement("ColorOverride")]
    [Browsable(false)]
    public string ClrGridHtml
    {
        get { return ColorTranslator.ToHtml(_ColorOverride); }
        set { _ColorOverride = ColorTranslator.FromHtml(value); }
    }

    [Category("Entity Properties")]
    public TextAlignment LabelAlignment
    {
        get { return labelAlignment; }
        set
        {
            labelAlignment = value;
            if (parentBox != null) parentBox.Invalidate();
        }
    }

    [Category("Entity Properties")]
    public int Priority
    {
        get { return _Priority; }
        set
        {
            _Priority = value;
            if (parentBox != null)
            {
                parentBox.SortEntities();
                parentBox.Invalidate();
            }
        }
    }

    [Category("Entity Properties")]
    public List<PointF> PathPoints
    {
        get { return _pathPoints; }
        set
        {
            _pathPoints = value;
            if (parentBox != null) parentBox.Invalidate();
        }
    }

    public void SetParent ( EntityBox parent )
    {
        parentBox = parent;
    }
}
