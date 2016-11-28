package BitMartyr is

    type Allegiance is (NONE, FRIEND, ENEMY, SELF);

    type Direction is (UP, RIGHT, DOWN, LEFT);
    for Direction use (UP => 1, RIGHT => 2, DOWN => 3, LEFT => 4);

    type Healthiness is range 0 .. 5;
    type Relative_Coordinate is range -2 .. 2;

    --  These are the raw integers given to the program, negatives and all
    type Relationship is new Integer range -5 .. 5;

    type Unit is
        record
            Health : Healthiness;
            Team : Allegiance;
        end record;

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

    procedure Load_Data;

    procedure Set_Return_Value (
        Movement_Direction : in Direction);
end BitMartyr;
