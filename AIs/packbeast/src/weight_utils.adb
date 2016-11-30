with BitMartyr; use BitMartyr;

package body Weight_Utils is

    function Random_Direction return Direction is begin
        return Direction'Val (Uniform_Random (
            Minimum => Direction'Pos (Direction'First),
            Maximum => Direction'Pos (Direction'Last)));
    end Random_Direction;

    function Is_Any_Weight_Non_Zero (
        Weights : in Direction_Weights)
        return Boolean is begin
        return 
            Weights (UP) /= 0 or 
            Weights (RIGHT) /= 0 or 
            Weights (DOWN) /= 0 or
            Weights (LEFT) /= 0;
    end Is_Any_Weight_Non_Zero;

    function Weight_Of_Each_Direction (
        Weighter : in Weight_Function)
        return Direction_Weights is
        Weights : Direction_Weights := (others => 0);
    begin
        for Delta_A in Relative_Coordinate range 1 .. 2 loop
            for Delta_B in Relative_Coordinate'Range loop
                Weights (UP) := Weights (UP) +
                    Weighter.all (Delta_B, -Delta_A);
                Weights (RIGHT) := Weights (RIGHT) +
                    Weighter.all (Delta_A, Delta_B);
                Weights (DOWN) := Weights (DOWN) +
                    Weighter.all (Delta_B, Delta_A);
                Weights (LEFT) := Weights (LEFT) +
                    Weighter.all (-Delta_A, Delta_B);
            end loop;
        end loop;
        
        return Weights;
    end Weight_Of_Each_Direction;

    function Weight_Of_Step_Direction (
        Weighter : in Weight_Function)
        return Direction_Weights is begin
        return ( 
            UP => Weighter.all (0, -1),
            RIGHT => Weighter.all (1, 0),
            DOWN => Weighter.all (0, 1),
            LEFT => Weighter.all (-1, 0));
    end Weight_Of_Step_Direction;

    function Lowest_Weight (
        Weights : in Direction_Weights)
        return Directions_List is
        Best : Weight := 10000;
        Best_Directions : array (Integer range 1 .. 4) of Direction;
        Number_Of_Equal_Bests : Integer := 1;
    begin
        for Current_Direction in Direction'Range loop
            if Weights (Current_Direction) < Best then
                Best := Weights (Current_Direction);
                Best_Directions (1) := Current_Direction;
                Number_Of_Equal_Bests := 1;
            elsif Weights (Current_Direction) = Best then
                Number_Of_Equal_Bests := Number_Of_Equal_Bests + 1;
                Best_Directions (Number_Of_Equal_Bests) := Current_Direction;
            end if;
        end loop;

        declare
            Result_Directions : 
                Directions_List (1 ..  Number_Of_Equal_Bests);
        begin
            for Index in 1 .. Number_Of_Equal_Bests loop
                Result_Directions (Index) := Best_Directions (Index);
            end loop;
            return Result_Directions;
        end;
    end Lowest_Weight;

    function Safest_Direction (
        Scariness : in Direction_Weights)
        return Direction is
        Best : Weight := 10000;
        Best_Directions : array (Integer range 1 .. 4) of Direction;
        Number_Of_Equal_Bests : Integer := 0;
    begin
        for Current_Direction in Direction'Range loop
            if Scariness (Current_Direction) < Best then
                Best := Scariness (Current_Direction);
                Best_Directions (1) := Current_Direction;
                Number_Of_Equal_Bests := 1;
            elsif Scariness (Current_Direction) = Best then
                Number_Of_Equal_Bests := Number_Of_Equal_Bests + 1;
                Best_Directions (Number_Of_Equal_Bests) := Current_Direction;
            end if;
        end loop;

        --  If three equal bests, move to the middle (opposite missing)
        if Number_Of_Equal_Bests = 3 then
            if
                Best_Directions (1) /= UP and
                Best_Directions (2) /= UP and
                Best_Directions (3) /= UP
            then
                return DOWN;
            elsif
                Best_Directions (1) /= RIGHT and
                Best_Directions (2) /= RIGHT and
                Best_Directions (3) /= RIGHT
            then
                return LEFT;
            elsif
                Best_Directions (1) /= DOWN and
                Best_Directions (2) /= DOWN and
                Best_Directions (3) /= DOWN
            then
                return UP;
            else
                return RIGHT;
            end if;
        end if;

        --  For 1, 2 and 4 equal bests, any best will do
        return Best_Directions (Uniform_Random (1, Number_Of_Equal_Bests));
    end Safest_Direction;

    function Highest_Weight_Under (
        Threshold : in Weight;
        Threat : in Direction_Weights)
        return Action is

        Best : Weight := Weight'First;
        Best_Directions : array (Integer range 1 .. 4) of Direction;
        Number_Of_Equal_Bests : Integer := 0;
    begin
        for Current_Direction in Direction'Range loop
            if 
                Best < Threat (Current_Direction) and 
                Threat (Current_Direction) < Threshold
            then
                Best := Threat (Current_Direction);
                Number_Of_Equal_Bests := 1;
                Best_Directions (1) := Current_Direction;
            elsif Best = Threat (Current_Direction) then
                Number_Of_Equal_Bests := Number_Of_Equal_Bests + 1;
                Best_Directions (Number_Of_Equal_Bests) := Current_Direction;
            end if;
        end loop;

        if Number_Of_Equal_Bests = 0 then
            return STAY;
        else
            return Best_Directions (Uniform_Random (
                Minimum => 1,
                Maximum => Number_Of_Equal_Bests));
        end if;
    end Highest_Weight_Under;
end Weight_Utils;
