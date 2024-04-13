# 3D Labirynth

Program to create a 3D visualization of a labyrinth.

It draws a maze and fills it with treasures based on .txt file where:
- first line contains size of the maze,
- X indicates wall,
- o indicates random treasure.

### Sample file:
`13,15`  
`XXXXXX XXXXXX
X    X      X
X XX X  XXX X
X oX   oX X X
XXXX  XXX X X
X     X   X X
X XXXXX X X X
X  o    X XoX
X XXXXXXX XXX
X   X   X   X
XXX X XXXX  X
X  oX       X
X XXXXX XXX X
X o   X X   X
XXXXXXX XXXXX`

### Result
 ![lab](https://github.com/Agnram/3D-Labyrinth/blob/main/images/lab13x15.png)

## Features
- Rotating labirynth,
- Zooming and moving the labyrinth,
- Controlling the position of the light source.

### Controls
- labirynth:
  - rotate: `mouse left-click`
  - zoom: `+` and `-`
  - move: `A` `W` `S` `D`
- light source:
  - move horizontally: `4` `8` `2` `6`
  - move vertically: `9` `3`


