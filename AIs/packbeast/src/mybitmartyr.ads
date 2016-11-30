-- My own variant on the BitMartyr wrapper.

package MyBitMartyr is

    type Allegiance is (NONE, FRIEND, ENEMY, SELF);

    type Action is (BOOM, UP, RIGHT, DOWN, LEFT);
    for Action use (BOOM => 0, UP => 1, RIGHT => 2, DOWN => 3, LEFT => 4);

    subtype Direction is Action range UP .. LEFT;

    type Healthiness is range 0 .. 5;
    type Relative_Coordinate is range -2 .. 2;

    --  These are the raw integers given to the program, negatives and all
    type Relationship is new Integer range -5 .. 5;

    type Unit is
        record
            Health : Healthiness;
            Team : Allegiance;
        end record;

    -- Basic Format:
    -- Call at the start of your AI function.
    procedure Load_Data;

    -- Call at the end to decide what action the unit takes.
    procedure Set_Unit_Action (
        Unit_Action : in Action);

    -- Standard Format (One-Unit-At-A-Time):
    -- Call at the start of the AI function.
    procedure Begin_Turn;

    -- Call to check that there is another unit.
    function Is_There_A_Current_Unit
        return Boolean;

    No_Current_Unit: exception;

    -- Call once per-unit.
    procedure Set_Current_Unit_Action_And_Load_Next_Unit (
        Unit_Action : in Action);

    function Uniform_Random (
        Minimum : in Integer;
        Maximum : in Integer)
        return Integer;

    --  Get_Unit (0, 0) returns self
    function Get_Unit (
        Delta_X : in Relative_Coordinate;
        Delta_Y : in Relative_Coordinate)
        return Unit;

    --  Returns 0 for 'self'
    function Get_Relationship (
        Delta_X : in Relative_Coordinate;
        Delta_Y : in Relative_Coordinate)
        return Relationship;

    -- Returns the X and Y coordinates of the unit.
    function Get_X
        return Integer;
    function Get_Y
        return Integer;

    -- Get the health of the current unit.
    function Get_My_Health
        return Healthiness;

end MyBitMartyr;
