with BitMartyr; use BitMartyr;

procedure Petrified is

    function AI return Action is
        function Is_Safe (Target : in Unit) return Boolean is
        begin
            return Target.Health <= Get_Unit (0, 0).Health;
        end Is_Safe;

        function Is_Guaranteed (Target : in Unit) return Boolean is
            Our_Health : Healthiness;
        begin
            Our_Health := Get_Unit (0, 0).Health;
            if not Is_Safe (Target) then return False; end if;

            for d in Direction'Range loop
                if Target.Neighbors (d) /= null then
                    if 
                        Target.Neighbors (d).Health >= Our_Health and
                        Target.Neighbors (d).Team /= SELF
                    then
                        return False;
                    end if;
                end if;
            end loop;

            return True;
        end Is_Guaranteed;

        Shuffled_Directions : array (Direction) of Direction := (
            UP => UP, DOWN => DOWN, RIGHT => RIGHT, LEFT => LEFT);

        procedure Shuffle_Directions is
            Swap_Index, Swap_Value : Direction;
        begin
            for I in reverse Shuffled_Directions'Range loop
                Swap_Index := Direction'Val (Uniform_Random (
                    Minimum => Direction'Pos (Direction'First),
                    Maximum => Direction'Pos (Direction'Last)));
                Swap_Value := Shuffled_Directions (I);
                Shuffled_Directions (I) := Shuffled_Directions (Swap_Index);
                Shuffled_Directions (Swap_Index) := Swap_Value;
            end loop;
        end Shuffle_Directions;

        Me : Unit;
    begin
        Shuffle_Directions;

        Me := Get_Unit (0, 0);

        --  Try to find a guaranteed spot
        for I in Shuffled_Directions'Range loop
            if Is_Guaranteed (Me.Neighbors (Shuffled_Directions (i)).all) then
                return Shuffled_Directions (i);
            end if;
        end loop;

        --  Try to find a safe spot
        for I in Shuffled_Directions'Range loop
            if Is_Safe (Me.Neighbors (Shuffled_Directions (i)).all) then
                return Shuffled_Directions (i);
            end if;
        end loop;

        --  Stand still and hope nobody notices
        return STAY;
    end AI;

    procedure Run_AI is new Run (AI => AI);
begin
    Run_AI;
end Petrified;
