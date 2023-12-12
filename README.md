# Pong Game

This is a simple implementation of the classic Pong game in C, using the Ncurses library for terminal-based graphics. It features in-game speed adjustment and the usual mechanics of Pong.

## How to Play

- Player 1 (Left Side): 'a' - Move Up, 'z' - Move Down
- Player 2 (Right Side): 'k' - Move Up, 'm' - Move Down
- Increase Speed: '+' Key
- Decrease Speed: '-' Key
- Quit Game: 'q' Key
- The game ends once one of the players gets 21 points.

## Building and Running

To build and play the game, make sure you have `gcc` and the `ncurses` library installed. Use the following command in the terminal:

``` bash
make start
```

## Additional Commands

- `make clean`: Removes the compiled files and the executable.
- `make rebuild`: Cleans and rebuilds the project.
- `make cpp`: Runs static code analysis using cppcheck.
- `make clang`: Formats the code using clang-format.


