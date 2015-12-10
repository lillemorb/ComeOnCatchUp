# ComeOnCatchUp v1.0

What is this?
-------------
A simple 2D platform game with puzzle elements made. Is was made
Sep-Dec 2015 for an assignment in the course TDDI02 - Programmeringsprojekt
at the Institute of Technology, Linköping University, Sweden. 

Who made this?
-------------
Lillemor Blom, Anna Jangius, Daniel Petersson, Rasmus Vesik.

Who made the graphics?
-------------
Rasmus Vesik.

Who made the sounds and music?
-------------
The sounds were made with a sounds generator at the website
http://www.bfxr.net/

The music is a song called "Jumpshot" by Eric Skiff, released under
a Creative Commons Attribution License.
Website: http://ericskiff.com/music/

How do you control the game?
-------------
Left	-	A/Left
Right	-	D/Right
Jump	-	Up/W/Space
Reset level	R
Paus/Resume	P
Menu	-	M
Quit	-	Esc
Make choice	Return (ie Enter)

For which operating systems does it work?
-------------
Tried on Linux Mint and Ubunto. Will probably work on Windows, but it
has not been tested.

What external libs do you need to run the game?
-------------
SFML 2.1 (or possibly newer versions). Website: http://www.sfml-dev.org/

Do we want comments on the code?
-------------
Depends. If the comment is "this code is messy" - then no thanks, we know.
If you have other comments, go ahead :)

Why is the user manual in Swedish?
-------------
Because we are from Sweden. It is basically a short summary of the game,
a list of the controls plus pretty pictures to illustrate what happens
in the game.

If you want to make your own levels
-------------
Edit or add levels in Level.txt. The codes for the different elements are:

Code 	Type    Collision borders:
00	- (empty)
01	Player
02	Door
03	Block
		
10	Ground	left, top, right, bottom
11	Ground	left, bottom
12	Ground	bottom
13	Ground	right, bottom
14	Ground	left
15	Ground	none
16	Ground	right
17	Ground	left, top
18	Ground	top
19	Ground	top, right
20	Ground	left, top, bottom
21	Ground	top, right, bottom
22	Ground	left, top, right
23	Ground	left, right, bottom
24	Ground	left, right
25	Ground	top, bottom

30	Background    Start
31	Background    Goal
32	Background    Tomato2
33	Background    Sign
34	Background    KeyboardMove
35	Background    KeyboardJump
36	Trigger	      Trigger (triggers animation on first and last level)