with BitMartyr; use BitMartyr;

procedure Hunter is
    --  Like a short-ranged scared
    function Timid_AI (Turn_Number : in Integer) return Action is
        Danger : array (Direction) of Integer := (others => 0);

        Best : Integer := 10000;
        Best_Directions : array (1 .. 4) of Direction;
        Num_Best : Integer := 0;
    begin
        for d in Direction'Range loop
            Danger (d) := Integer (Get_Unit (0, 0).Neighbors (d).Health);
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

        if Num_Best > 0 then
            return Best_Directions (Uniform_Random (1, Num_Best));
        else 
            return UP;
        end if;
    end Timid_AI;

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

    --  Ripped directly from scared
    function Scared_AI (Turn_Number : in Integer) return Action is
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
    end Scared_AI;

    function AI (Turn_Number : in Integer) return Action is
        Best_Target_Health : Integer := -1;
        Best_Target_Direction : Direction;

        Me : Unit := Get_Unit (0, 0);
    begin
        --  If we can kill somebody, kill the best somebody
        for d in Direction'Range loop
            if 
                Me.Neighbors (d).Health <= Me.Health and
                Integer (Me.Neighbors (d).Health) > Best_Target_Health and
                Me.Neighbors (d).Team = ENEMY
            then
                Best_Target_Health := Integer (Me.Neighbors (d).Health);
                Best_Target_Direction := d;
            end if;
        end loop;

        if Best_Target_Health > 0 then
            return Best_Target_Direction;
        end if;

        case Turn_Number mod 3 is
            when 0 => return Scared_AI (Turn_Number);
            when 1 => return Paranoid_AI (Turn_Number);
            when others => return Timid_AI (Turn_Number);
        end case;
    end AI;
    procedure Run_AI is new Run (AI => AI);
begin
    Run_AI;
end Hunter;
