# Spaceboopers
A simple hot-seat multiplayer game written in C++ and SDL2

## Objective
Hunt down the other player.

## How it's played?/Rules
There are two players, each controlling the other ship.
```
red = Player 1
blue = Player 2
```
As a ship commander you can shoot projectiles at the other player.

You have a limited firing rate, so it's best aim before you shoot :)

Each kill gets you a point. Whoever gets 10 points first wins!

The points are shown as lines on the side of the player who got them.

There's also a so-called "bullethell" mode (activated by passing `bullethell`
 into the commadline args of the game), where the firing happens automatically
and the one who has better bullet dodging skills wins!

## Controls

### Keyboard

Player 1:
```
WASD - for movement
L-Ctrl - for firing
```

Player 2:
```
Arrow keys - for movement
R-Ctrl - for firing
```

### Controller

The controller number corresponds to the player which it will be controlling.

```
Left-Analog stick or D-Pad (depending on device) - for movement
Any button - for firing
```

## Building
You need: `cmake`, `a decent C++ compiler`, `SDL2`, `commandline knowelegde`

To build:
```
cd Spaceboopers
mkdir build
cmake ..
make
```

