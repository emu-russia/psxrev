using System.ComponentModel;
using System.Windows.Forms;

public enum EntityType
{
    Selection = 0,
    ViasInput = 10,
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
    UnitRegfile,
    UnitMemory,
    UnitCustom,
}

public enum ViasShape
{
    Square = 1,
    Round,
}

public enum CellTextAlignment
{
    Top = 1,
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

    public float SavedLambdaX;
    public float SavedLambdaY;
    public float SavedLambdaEndX;
    public float SavedLambdaEndY;

    [Category("Entity Properties")]
    public string Label
    {
        get { return _Label; }
        set { _Label = value;
              if (parentBox != null) parentBox.Invalidate(); }
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
    public bool Selected
    {
        get { return _Selected; }
        set { _Selected = value;
            if (parentBox != null) parentBox.Invalidate(); }
    }

    public void SetParent ( EntityBox parent )
    {
        parentBox = parent;
    }
}
