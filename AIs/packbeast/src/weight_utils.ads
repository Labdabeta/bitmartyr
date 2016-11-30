with BitMartyr; 

package Weight_Utils is
    type Weight is new Integer;
    type Direction_Weights is array (BitMartyr.Direction) of Weight;
    type Weight_Function is access function (
        Delta_X : BitMartyr.Relative_Coordinate;
        Delta_Y : BitMartyr.Relative_Coordinate)
        return Weight;
    type Directions_List is array (Positive range <>) of BitMartyr.Direction;

    function Random_Direction 
        return BitMartyr.Direction;

    --  Returns true if any of the provided weights are zero
    function Is_Any_Weight_Non_Zero (Weights : in Direction_Weights) 
        return Boolean;

    --  Calculate the weight of each direction
    function Weight_Of_Each_Direction (
        Weighter : in Weight_Function)
        return Direction_Weights;

    --  Calculate the weight of each 1-step direction
    function Weight_Of_Step_Direction (
        Weighter : in Weight_Function)
        return Direction_Weights;

    --  Returns all lowest weighted directions.
    function Lowest_Weight (
        Weights : in Direction_Weights)
        return Directions_List;

    --  Returns the "safest" direction. IE: the lowest one
    --  Replaces the Run_From function
    function Safest_Direction (
        Scariness : in Direction_Weights)
        return BitMartyr.Direction;

    --  Returns the highest direction with a weight unter the threshold.
    --  If none are available, returns STAY
    function Highest_Weight_Under (
        Threshold : in Weight;
        Threat : in Direction_Weights)
        return BitMartyr.Action;
end Weight_Utils;
