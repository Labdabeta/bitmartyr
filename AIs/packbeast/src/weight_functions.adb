with BitMartyr; use BitMartyr;
with Weight_Utils; use Weight_Utils;

package body Weight_Functions is

    function Count_Of_Team_Units (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight is
        Check : Unit;
    begin
        Check := Get_Unit (Delta_X, Delta_Y);
        if Check.Team = Target_Team and Check.Health = Target_Health then
            return 1;
        end if;
        return 0;
    end Count_Of_Team_Units;

    function Count_Of_Units (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight is
    begin
        if Get_Unit (Delta_X, Delta_Y).Health = Target_Health then
            return 1;
        end if;
        return 0;
    end Count_Of_Units;

    function Weight_Presence (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight is
        Allegiance_Boost : Weight := 0;
        Check : Unit;
    begin
        Check := Get_Unit (Delta_X, Delta_Y);

        case Check.Team is
            when FRIEND => Allegiance_Boost := 1;
            when ENEMY => Allegiance_Boost := 2;
            when others => Allegiance_Boost := 0;
        end case;

        return Allegiance_Boost + Weight (Check.Health);
    end Weight_Presence;

    function Weight_Relationship (
        Delta_X : in BitMartyr.Relative_Coordinate;
        Delta_Y : in BitMartyr.Relative_Coordinate) 
        return Weight_Utils.Weight is begin
        return Weight (Get_Unit (Delta_X, Delta_Y).Relation);
    end Weight_Relationship;

end Weight_Functions;
