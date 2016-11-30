-- My PackBeast BitMartyr AI:

-- This is my first AI for the game. Also my first program in Ada and
-- between the two packbeast ended up being much different than I first
-- intended.

with BitMartyr; use BitMartyr;
with Weight_Utils; use Weight_Utils;
with Hunters; use Hunters;
with Pubs; use Pubs;
with Mothers; use Mothers;

procedure PackBeast is
    function Decide_Unit return Action is begin
        case Get_Unit (0, 0).Health is
            when 5 => return Decide_Mother;
            when 1 => return Decide_Pub;
            when others => return Decide_Hunter;
        end case;
    end Decide_Unit;

    procedure Run_AI is new Run (AI => Decide_Unit);
begin
    Run_AI;
end PackBeast;
