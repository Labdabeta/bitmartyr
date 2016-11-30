with BitMartyr; use BitMartyr;
with Weight_Utils; use Weight_Utils;
with Weight_Functions; use Weight_Functions;

package body Hunters is
    function Decide_Hunter return Action is
        Destinations : Direction_Weights;
    begin
        Destinations := Weight_Of_Step_Direction (Weight_Relationship'Access);

        return Highest_Weight_Under (
            Threshold => Weight (Get_Unit (0, 0).Health),
            Threat => Destinations);
    end Decide_Hunter;
end Hunters;
