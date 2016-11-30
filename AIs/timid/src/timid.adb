with BitMartyr; use BitMartyr;

procedure Timid is
    --  Like a short-ranged scared
    function Timid_AI return Action is
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

    procedure Run_AI is new Run (AI => Timid_AI);
begin
    Run_AI;
end Timid;
