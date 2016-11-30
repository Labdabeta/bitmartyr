with BitMartyr; use BitMartyr;
with Weight_Utils; use Weight_Utils;
with Weight_Functions; use Weight_Functions;

package body Mothers is

    function Check_For_Fives is new Count_Of_Units (Target_Health => 5);
    function Check_For_Fours is new Count_Of_Team_Units (
        Target_Health => 4, Target_Team => BitMartyr.FRIEND);
    function Decide_Mother return Action is
        Count_Fours, Count_Fives : Direction_Weights;
    begin
        Count_Fives := Weight_Of_Each_Direction (Check_For_Fives'Access);

        if Is_Any_Weight_Non_Zero (Count_Fives) then
            return Safest_Direction (Count_Fives);
        end if;

        Count_Fours := Weight_Of_Step_Direction (Check_For_Fours'Access);

        if Is_Any_Weight_Non_Zero (Count_Fours) then
            return Safest_Direction (Count_Fours);
        end if;
        
        return Direction'Val (Uniform_Random (0, 3));
    end Decide_Mother;
end Mothers;
