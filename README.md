# Reversi Game

A [Reversi](https://en.wikipedia.org/wiki/Reversi) game built with C++

## Installation/Play

```bash
cd reversi
make
./test-reversi [Chess Board Size]
```

## Rules

### Commands

* p: play by specifying the coordinates of the location to place the piece. The coordinate system will use lower-case letters a, b, ... for rows and numbers 1, 2, ... for columns. Use whitespace to seperate p and the coordinates. Example input: `p d2`, `p c1`

* c: checkpoint the current game state by saving the current board and whose turn it is. You can have multiple checkpoints

* u: undo the current game state by restoring the game state to the last saved checkpoint

* any other characters: quit the game and display results

For general Reversi rules, see [here](https://en.wikipedia.org/wiki/Reversi#Rules) on Wikipedia or a simplified version [here](https://cardgames.io/reversi/#rules)

In this version of the game, Black always goes first. The game quits automatically when the current player has no legal location or when the board is full.

## Extra info

Pulled this from a school homework, it's a very simple implementation of the game with no GUI. If you want to play a more polished version of the game, you can play it [here](https://cardgames.io/reversi)