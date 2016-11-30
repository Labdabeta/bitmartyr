with BitMartyr; use BitMartyr;
with Weight_Utils; use Weight_Utils;
with Weight_Functions; use Weight_Functions;

package body Pubs is
    function Decide_Pub return Action is begin
        return Safest_Direction (
            Weight_Of_Each_Direction (Weight_Presence'Access));
    end Decide_Pub;
end Pubs;
