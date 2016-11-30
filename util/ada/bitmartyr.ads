package BitMartyr is

    type Allegiance is (NONE, FRIEND, ENEMY, SELF);

    type Action is (STAY, UP, LEFT, DOWN, RIGHT, DIE);
    subtype Direction is Action range UP .. RIGHT;

    type Healthiness is range 0 .. 5;
    type Relative_Coordinate is range -2 .. 2;

    --  These are the raw integers given to the program, negatives and all
    type Relationship is new Integer range -5 .. 5;

    type Unit;
    type Neighborhood is array (Direction) of access Unit;

    type Unit is
        record
            Health : Healthiness;
            Relation : Relationship;
            Team : Allegiance;
            
            Neighbors : Neighborhood;
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

    type AI is access function (Turn_Number : in Integer) return Action;

    procedure Run (My_AI : in AI);
end BitMartyr;
