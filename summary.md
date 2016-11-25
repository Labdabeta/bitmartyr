3 AIs compete on a bitmap to have their colour win!

At the start of the game each team's units are randomly placed in the playing
field with full health (15). Units will not spawn on top of each other.

Each frame each of your units must move. When a unit moves it leaves behind a
clone with 1 less health than itself. When units end a turn on the same pixel,
the one with more health survives and the one with less health dies. If they
have the same health they will both die. Each frame the team with the most of
their colour showing wins. Whichever team has the most points at the end of the
game wins. If two teams have the same amount of their colour showing, they each
get a point.

The board wraps around such that leaving from the right causes you to appear on
the left, etc.

You must write a command-line executable which will be your units AI. 

This executable will read integers from standard input and eventually produce a
return value indicating what it wishes to do. The return codes are:

 1: move up
 2: move right
 3: move down
 4: move left

Anything else will kill your unit on the spot!

The first integer on standard input is the current unit's health. The next is
its x-coordinate and the next is its y-coordinate. Following that it will
receive 24 integers representing its surroundings. They are ordered as:

12345
6789A
BC DE
FGHIJ
KLMNO

relative to your position in the center. If they are positive they are allies,
otherwise they are enemies. You will then be provided with a random integer
followed by its maximum value. Finally, you will be given the current turn
number.

