with Ada.Numerics.Float_Random;
with Ada.Text_IO;
with Ada.Integer_Text_IO;
with Ada.Command_Line;

package body BitMartyr is

    package Health_Text_IO is new Ada.Text_IO.Integer_IO (Healthiness);
    package Relationship_Text_IO is new Ada.Text_IO.Integer_IO (Relationship);

    Units : array (Relative_Coordinate, Relative_Coordinate) of aliased Unit;

    Seed : Ada.Numerics.Float_Random.Generator;

    function Get_Unit (
        Delta_X : in Relative_Coordinate;
        Delta_Y : in Relative_Coordinate)
        return Unit is
    begin
        return Units (Delta_X, Delta_Y);
    end Get_Unit;

    procedure Run is
        Turn_Number : Integer;
        Random_Value : Integer;
        Relation : Relationship;
    begin
        --  Create links between units.
        for dy in Relative_Coordinate'Range loop
            for dx in Relative_Coordinate'Range loop
                if dy > -2 then
                    Units (dx, dy).Neighbors (UP) := Units (dx, dy - 1)'Access;
                else
                    Units (dx, dy).Neighbors (UP) := null;
                end if;

                if dy < 2 then
                    Units (dx, dy).Neighbors (DOWN) := Units (dx, dy + 1)'Access;
                else
                    Units (dx, dy).Neighbors (DOWN) := null;
                end if;

                if dx > -2 then
                    Units (dx, dy).Neighbors (LEFT) := Units (dx - 1, dy)'Access;
                else
                    Units (dx, dy).Neighbors (LEFT) := null;
                end if;

                if dx < 2 then
                    Units (dx, dy).Neighbors (RIGHT) := Units (dx + 1, dy)'Access;
                else
                    Units (dx, dy).Neighbors (RIGHT) := null;
                end if;
            end loop;
        end loop;

        Ada.Integer_Text_IO.Get (Random_Value);
        Ada.Numerics.Float_Random.Reset (Seed, Random_Value);
        Ada.Integer_Text_IO.Get (Random_Value);
        Ada.Integer_Text_IO.Get (Turn_Number);

        loop
            for dy in Relative_Coordinate'Range loop
                for dx in Relative_Coordinate'Range loop
                    Relationship_Text_IO.Get (Relation);

                    Units (dx, dy).Relation := Relation;
                    Units (dx, dy).Health := Healthiness (abs Relation);
                    if Relation > 0 then
                        if dy = 0 and dx = 0 then
                            Units (dx, dy).Team := SELF;
                        else
                            Units (dx, dy).Team := FRIEND;
                        end if;
                    elsif Relation < 0 then
                        Units (dx, dy).Team := ENEMY;
                    else
                        Units (dx, dy).Team := NONE;
                    end if;

                    if Relation /= 0 then
                        Units (dx, dy).Previous_Health := Units (dx, dy).Health;
                    end if;
                end loop;
            end loop;

            case AI (Turn_Number) is
                when STAY => Ada.Text_IO.Put (".");
                when UP => Ada.Text_IO.Put ("^");
                when DOWN => Ada.Text_IO.Put ("v");
                when LEFT => Ada.Text_IO.Put ("<");
                when RIGHT => Ada.Text_IO.Put (">");
                when DIE => Ada.Text_IO.Put ("*");
            end case;

        end loop;
    exception
        when Ada.Text_IO.End_Error => return;
    end Run;

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

end BitMartyr;
