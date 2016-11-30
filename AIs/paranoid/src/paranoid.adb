with BitMartyr; use BitMartyr;

procedure Paranoid is
    --  So scared that empty tiles freak them out
    function Paranoid_AI (Turn_Number : in Integer) return Action is
        Danger : array (Direction) of Integer := (others => 0);
        Check : Unit;

        Best : Integer := 1000000000;
        Best_Directions : array (1 .. 4) of Direction;
        Num_Best : Integer := 0;

        function Value_Of (u : in Unit) return Integer is
        begin
            if u.team = NONE then
                return Integer (u.Previous_Health);
            else
                return Integer (u.Health);
            end if;
        end Value_Of;
    begin
        for dy in Relative_Coordinate'Range loop
            for dx in Relative_Coordinate'Range loop
                Check := Get_Unit (Delta_X => dx, Delta_Y => dy);

                if dy > 0 then 
                    Danger (DOWN) := Danger (DOWN) + Value_Of (Check);
                end if;
                if dy < 0 then
                    Danger (UP) := Danger (UP) + Value_Of (Check);
                end if;
                if dx > 0 then
                    Danger (RIGHT) := Danger (RIGHT) + Value_Of (Check);
                end if;
                if dx < 0 then
                    Danger (LEFT) := Danger (LEFT) + Value_Of (Check);
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
    end Paranoid_AI;
    procedure Run_AI is new Run (AI => Paranoid_AI);
begin
    Run_AI;
end Paranoid;
