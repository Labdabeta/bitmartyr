with BitMartyr; use BitMartyr;

procedure Scared is 
    function AI (Turn_Number : in Integer) return Action is
        Danger : array (Direction) of Integer := (others => 0);
        Check : Unit;

        Best : Integer;
        Best_Directions : array (1 .. 4) of Direction;
        Num_Best : Positive;
    begin
        for dy in Relative_Coordinate'Range loop
            for dx in Relative_Coordinate'Range loop
                Check := Get_Unit (Delta_X => dx, Delta_Y => dy);

                if dy > 0 then 
                    Danger (DOWN) := Danger (DOWN) + Integer (Check.Health);
                end if;
                if dy < 0 then
                    Danger (UP) := Danger (UP) + Integer (Check.Health);
                end if;
                if dx > 0 then
                    Danger (RIGHT) := Danger (RIGHT) + Integer (Check.Health);
                end if;
                if dx < 0 then
                    Danger (LEFT) := Danger (LEFT) + Integer (Check.Health);
                end if;
            end loop;
        end loop;

        Best := Danger (UP);
        Best_Directions (1) := UP;
        Num_Best := 1;

        if Danger (DOWN) < Best then
            Best := Danger (DOWN);
            Best_Directions (1) := DOWN;
            Num_Best := 1;
        elsif Danger (DOWN) = Best then
            Num_Best := Num_Best + 1;
            Best_Directions (Num_Best) := DOWN;
        end if;

        if Danger (LEFT) < Best then
            Best := Danger (LEFT);
            Best_Directions (1) := LEFT;
            Num_Best := 1;
        elsif Danger (LEFT) = Best then
            Num_Best := Num_Best + 1;
            Best_Directions (Num_Best) := LEFT;
        end if;

        if Danger (RIGHT) < Best then
            Best := Danger (RIGHT);
            Best_Directions (1) := RIGHT;
            Num_Best := 1;
        elsif Danger (RIGHT) = Best then
            Num_Best := Num_Best + 1;
            Best_Directions (Num_Best) := RIGHT;
        end if;

        return Best_Directions (Uniform_Random (1, Num_Best));
    end AI;

    procedure Run_AI is new Run (AI => AI);
begin
    Run_AI;
end Scared;
