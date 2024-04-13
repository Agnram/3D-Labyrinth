# 3D Labirynth

Program to create a 3D visualization of a labyrinth.

It draws a maze and fills it with treasures based on .txt file where:
- first line contains size of the maze,
- X indicates wall,
- o indicates random treasure.

### Sample file:
`22,22`  
`XXXXXXXXXXXXXXXXXXXXXX
X  o    X X o      X X
XXXXXXX X X XXXXXX X X
X o  X    X    X     X
X XXXX XX X XXXX XXX X
X X     X XXX  X X X X
X  oXXX X Xo  XX X X X
XXXXX   X XXX X X     
  X X XXX   X   X XX X
X X X   XXX XXX XoXX X
X o X X X X X   X    X
X X XoX   X X X XXXX X
X X XXXXX X X X    X X
X         X X XXX XX X
X XXXXXXXXX X   Xo X X
X  o    X   XXX XX X X
XXX X XXXXX   X  X X X
X X X Xo  XXX XX X X X
X   X XXX      X X XoX
XXXXX   X XXXX XXX X X
X  o  X X    X     X X
XXXXXXXXXXXXXXXXXXXXXX`

### Result
 ![lab](images/lab%2022X22.png)

## Features
- Rotating labirynth  
![lab](images/rotate.gif)

- Zooming and moving the labyrinth  
![lab](images/zoom.gif)

- Controlling the position of the light source  
![lab](images/light.gif)

### Controls
- labirynth:
  - rotate: `mouse left-click`
  - zoom: `+` and `-`
  - move: `A` `W` `S` `D`
- light source:
  - move horizontally: `4` `8` `2` `6`
  - move vertically: `9` `3`


