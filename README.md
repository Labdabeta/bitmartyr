# BitMartyr

## Table of Contents

 - [Rules](#rules)
 - [Standard AI Format](#standard-ai-format)
 - [Output Formats](#output-formats)
 - [Submission Format](#submission-format)
 - [Execution Parameters](#execution-parameters)
 - [Utilities](#utilities)
 - [Running a Game](#running-a-game)

## Rules

BitMartyr is a game played by a number of AIs placed in a grid.

Initially each team begins with some number of units spread out around the
playing field, all at full health. Units begin at 5 health and die when they
have 0 health. 

Each frame, each of your units must issue a command. When a unit moves it leaves
behind a clone with 1 less health than itself. When units end a turn on the same
pixel, the one with more health survives and the one with less health dies. If
they have the same health, they will both die. 

At the end of the game, whichever team has more individual units alive wins. 

The board wraps around such that leaving from the right causes you to appear on
the left, etc.

## Standard AI Format

There are two types of AI formatting that are allowed. 

### Basic Format

The first is "basic" formatting. In basic formatting the AI will receive on
standard input 25 integers representing relative nearby healths (-ve indicates
enemies, +ve indicates allies, the 13'th integer is your own health). For
example:

```
0 0 1 0 0 
0 0 2 0 0
0 -4 3 0 0
0 -3 0 0 0
0 0 0 0 0
```

Indicates two allies in position UP from you, one with health of 2, one with
health of 1. One enemy directly LEFT from you, with 4 health. An enemy LEFT and
DOWN from you with 3 health. Yourself, in the center, with 3 health.

Before the relative surroundings have been transmitted, you will receive three
more integers. The first is a random number, the second is the maximum value
said random number could take, the third is the current turn number.

After the fact your executable may indicate what it wants the unit to do in two
ways. It may either produce the desired direction on standard output using any
of the [output formats](#output-formats). Otherwise it may choose to return a system status code as
its action in integer format.

This format is primarily for illustrative and code-golf purposes. It is not
intended to be used in actual gameplay. Note that a standard format executable
is still a valid basic format executable!

### Standard Format (recommended)

The standard format is recommended since it is far more efficient. In standard
format your AI will first receive the three common integers (random value,
maximum random value, turn number) on standard input. After that it will
continuously receive sets of 25 numbers indicating a unit as in the basic
format. This list's end will be indicated by the end of standard input.

As output, an executable in standard format shall produce a series of outputs in
any output format each corresponding to the provided units in order.

This format is recommended, especially for higher-level languages, as it is much
faster to run, requiring only a single initialization per turn.

## Output Formats

There are 3 equivalent output formats: symbol, character, and integer. They are 
encoded as follows:

```
DIE = '*' = 'x' = 'X' = 5
STAY = '.' = 's' = 'S' = 0
UP = '^' = 'u' = 'U' = 1
RIGHT = '>' = 'r' = 'R' = 2
DOWN = 'v' = 'd' = 'D' = 3
LEFT = '<' = 'l' = 'L' = 4
```

In standard form the outputs may or may not be separated by whitespace as per
programmer convenience.

## Submission Format

See AIs/sample for an example of how an AI can be submitted. In general an AI
*must* provide a makefile which will have the following properties:

 - `make` will generate required files to run, after which
 - `make name` will print the executable's name on standard output, and
 - `./bin/$(make name)` will run the AI **in standard format**.

Additionally AIs *should* provide a README.md which will have the same format as
the sample AI. The makefile and README should be the only top level files in an
AI's directory. All source files shall be placed within the src directory, while
binaries shall be placed within the bin directory. 

## Execution Parameters

There are a number of execution parameters. They are:

 - Width: The width of the playing field (-w, --width)
 - Height: The height of the playing field
 - Scale: The scaling factor to use when rendering
 - Initial Population: How many units to spawn on setup
 - Duration: How many turns to run

They are provided on the command line. After that the directory names for the
AI's will be specified in order. The colours given to AIs are set in the
following order:

 - Red: #FF0000, #CC0000, #990000, #660000, #330000
 - Green: #00FF00, #00CC00, #009900, #006600, #003300
 - Blue: #0000FF, #0000CC, #000099, #000066, #000033
 - Grey: #FFFFFF, #CCCCCC, #999999, #666666, #333333
 - Cyan: #00FFFF, #00CCCC, #009999, #006666, #003333
 - Magenta: #FF00FF, #CC00CC, #990099, #660066, #330033
 - Yellow: #FFFF00, #CCCC00, #999900, #666600, #333300

For example the command:

```
./bitmartyr.sh -w 50 -h 100 -s 5 -p 3 -d 1000 a b c d
```

will run the AIs in directories a (red), b (green), c (blue), and d (grey) on a
50 by 100 map, with 3 equally spaced initial spawn points for each team and a
1000 turn game length. When rendering it will use 25 real pixels per unit (in a
5x5 square).

Optionally, you may also provide a filename, in which case each frame will be
saved to files starting with that name, with the frame number appended to them.
The filename option switch is '-f'.

If an AI name is prefixed with a hyphen, it indicates that the AI will be using
basic format instead of standard format. Otherwise everything will be the same.

## Utilities

There are a number of helper utilities for writing BitMartyr AIs. They are in
folders named after languages in the util directory. They are left as source to
enable modification to specific needs and ease of compilation.

Meanwhile the running main executable for the game is created by the makefile
using the sources in the src directory. On linux just run `make`, on windows
you'll have to modify the makefile to point to your sdl2 installation then run
`make windows`.

## Running a Game

To run a game, run the bitmartyr.sh file with arguments as above.

On windows, if you don't have access to bash, you will have to call the
bitmartyr.exe executable manually after compiling the desired AIs. As arguments
it takes, in order, with no gaps:

```
Width, Height, Scale, Initial Population, Duration, program names, optional
-- output filename.
```

For example, the above shell command would run (assuming make name values):

```
bitmartyr.exe 50 100 5 3 1000 a/bin/a.exe b/bin/b.exe c/bin/c.exe d/bin/d.exe
```

When you run a game a window will appear displaying the game live. It will
respond to WM events and thus can be quit as normal. 

Additionally a log file named after the current time will be generated to store
partial updates. It will be flushed at the end of each turn and thus can be
viewed by a pager like less.

Finally, after the game is over the winner will be announced on standard output
along with how many units each team had alive.
