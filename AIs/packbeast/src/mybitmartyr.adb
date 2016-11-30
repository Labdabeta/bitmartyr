with Ada.Numerics.Float_Random;
with Ada.Text_IO;
with Ada.Integer_Text_IO;
with Ada.Command_Line;

package body MyBitMartyr is

    package Health_Text_IO is new Ada.Text_IO.Integer_IO (Healthiness);
    package Relationship_Text_IO is new Ada.Text_IO.Integer_IO (Relationship);

    Units : array (Relative_Coordinate, Relative_Coordinate) of Unit;

    Seed : Ada.Numerics.Float_Random.Generator;

    Our_X_Coordinate : Integer;
    Our_Y_Coordinate : Integer;

    Turn_Number : Integer := 0;

    Is_A_Unit_Loaded : Boolean := False;

    function Get_Relationship (
        Delta_X : in Relative_Coordinate;
        Delta_Y : in Relative_Coordinate)
        return Relationship is
        The_Unit : Unit := Units (Delta_X, Delta_Y);
    begin
        if
            The_Unit.Team = FRIEND or
            The_Unit.Team = SELF
        then
            return Relationship (The_Unit.Health);
        elsif
            The_Unit.Team = ENEMY
        then
            return Relationship (The_Unit.Health) * (-1);
        else
            return 0;
        end if;
    end Get_Relationship;

    function Get_Unit (
        Delta_X : in Relative_Coordinate;
        Delta_Y : in Relative_Coordinate)
        return Unit is
    begin
        return Units (Delta_X, Delta_Y);
    end Get_Unit;

    procedure Load_Data is
        A_Relationship : Relationship;
        Random_Value : Integer;
    begin
        Health_Text_IO.Get (Units (0, 0).Health);
        Ada.Integer_Text_IO.Get (Our_X_Coordinate);
        Ada.Integer_Text_IO.Get (Our_Y_Coordinate);
        Units (0, 0).Team := SELF;

        for Delta_Y in Units'Range (1) loop
            for Delta_X in Units'Range (2) loop
                if Delta_X /= 0 or Delta_Y /= 0 then
                    Relationship_Text_IO.Get (A_Relationship);

                    if A_Relationship > 0 then
                        Units (Delta_X, Delta_Y).Health :=
                            Healthiness (A_Relationship);
                        Units (Delta_X, Delta_Y).Team := ENEMY;
                    elsif A_Relationship < 0 then
                        Units (Delta_X, Delta_Y).Health :=
                            Healthiness (A_Relationship * (-1));
                        Units (Delta_X, Delta_Y).Team := FRIEND;
                    else
                        Units (Delta_X, Delta_Y).Health := 0;
                        Units (Delta_X, Delta_Y).Team := NONE;
                    end if;
                end if;
            end loop;
        end loop;

        Ada.Integer_Text_IO.Get (Random_Value);
        Ada.Numerics.Float_Random.Reset (Seed, Random_Value);
    end Load_Data;

    procedure Set_Unit_Action (
        Unit_Action : in Action) is
    begin
        --  This is a bit of a hack using the order of the enumerated type
        --  Correct strategy is to do an unchecked conversion
        Ada.Command_Line.Set_Exit_Status (
            Ada.Command_Line.Exit_Status (
                Action'Pos (Unit_Action)));
    end Set_Unit_Action;

    -- Standard (Puedo-Basic) Format:
    procedure Load_Next_Unit is
        A_Relationship : Relationship;
    begin
        -- TODO: Some times I get an error on this...
        if Relationship_Text_IO.End_Of_File then
            Is_A_Unit_Loaded := false;
            return;
        end if;

        for Delta_Y in Units'Range (1) loop
            for Delta_X in Units'Range (2) loop
                if Delta_X /= 0 or Delta_Y /= 0 then
                    Relationship_Text_IO.Get (A_Relationship);

                    if A_Relationship > 0 then
                        Units (Delta_X, Delta_Y).Health :=
                            Healthiness (A_Relationship);
                        Units (Delta_X, Delta_Y).Team := ENEMY;
                    elsif A_Relationship < 0 then
                        Units (Delta_X, Delta_Y).Health :=
                            Healthiness (A_Relationship * (-1));
                        Units (Delta_X, Delta_Y).Team := FRIEND;
                    else
                        Units (Delta_X, Delta_Y).Health := 0;
                        Units (Delta_X, Delta_Y).Team := NONE;
                    end if;
                end if;
            end loop;
        end loop;
        Is_A_Unit_Loaded := true;
    end Load_Next_Unit;

    procedure Begin_Turn is
        New_Integer : Integer;
        New_Relationship : Relationship;
    begin
        Ada.Integer_Text_IO.Get (New_Integer);
        Ada.Numerics.Float_Random.Reset (Seed, New_Integer);
        Ada.Integer_Text_IO.Get (New_Integer);
        Ada.Integer_Text_IO.Get (Turn_Number);

        Load_Next_Unit;
    end Begin_Turn;

    function Is_There_A_Current_Unit
        return Boolean is
    begin
        return Is_A_Unit_Loaded;
    end Is_There_A_Current_Unit;    

    procedure Set_Current_Unit_Action (
        Unit_Action : in Action) is
    begin
        -- TODO: How to print the action?
    end Set_Current_Unit_Action;

    procedure Set_Current_Unit_Action_And_Load_Next_Unit (
        Unit_Action : in Action) is
    begin
        if not Is_Unit_Loaded then
            raise No_Current_Unit;
        end if;

        Set_Current_Unit_Action (Unit_Action);
        Load_Next_Unit;
    end Set_Current_Unit_Action_And_Load_Next_Unit;

    function Uniform_Random (
        Minimum : in Integer;
        Maximum : in Integer)
        return Integer is

        Random_Ratio : Ada.Numerics.Float_Random.Uniformly_Distributed;

        --  Out of Range Error Warning
        The_Range : Integer := Maximum - Minimum;
    begin
        Random_Ratio := Ada.Numerics.Float_Random.Random (Seed);

        return Integer (Float (Minimum) + (Random_Ratio * Float (The_Range)));
    end Uniform_Random;

    function Get_X
        return Integer is
    begin
        return Our_X_Coordinate;
    end Get_X;

    function Get_Y
        return Integer is
    begin
        return Our_Y_Coordinate;
    end Get_Y;

    function Get_My_Health
        return Healthiness is
    begin
        return Units (0, 0).Health;
    end Get_My_Health;

end MyBitMartyr;
