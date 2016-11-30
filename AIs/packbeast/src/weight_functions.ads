with BitMartyr;
with Weight_Utils;

package Weight_Functions is
    -- Return 1 if a unit at the coordinate has the provided health and team
    generic 
        Target_Health : in BitMartyr.Healthiness;
        Target_Team : in BitMartyr.Allegiance;
    function Count_Of_Team_Units (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight;

    --  Return 1 if a unit at the coordinate has the provided health
    generic 
        Target_Health : in BitMartyr.Healthiness;
    function Count_Of_Units (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight;

    function Weight_Presence (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight;

    function Weight_Relationship (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight;
end Weight_Functions;
