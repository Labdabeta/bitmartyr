with BitMartyr; use BitMartyr;

procedure Scared is 
    function AI return Action is
        Danger : array (Direction) of Integer := (others => 0);
        Check : Unit;

        Best : Integer := 10000;
        Best_Directions : array (1 .. 4) of Direction;
        Num_Best : Integer := 0;
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

        for d in Direction'Range loop
            if Danger (d) < Best then
                Best := Danger (d);
                Best_Directions (1) := d;
                Num_Best := 1;
            elsif Danger (d) = Best then
                Num_Best := Num_Best + 1;
                Best_Directions (Num_Best) := d;
            end if;
        end loop;

        return Best_Directions (Uniform_Random (1, Num_Best));
    end AI;

    procedure Run_AI is new Run (AI => AI);
begin
    Run_AI;
end Scared;
