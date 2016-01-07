
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
    public string Label;

    public float LambdaWidth;

    public float LambdaHeight;

    public float LambdaX;

    public float LambdaY;

    public float LambdaEndX;

    public float LambdaEndY;

    public EntityType Type;

    public bool Selected;
}
