#  Game Of Life NCurses

Realization of the "Game Of Life" using the ncurses library.

![Game Demo](https://raw.githubusercontent.com/kirpichik/GameOfLife-Curses/master/image.png "Game Demo" )

## About Game Of Life

[Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

## Control

### Mouse

You can click the mouse on the field, thereby placing the cells.

### Keyboard

**Q** - quit game

**N** - next step

**B** - Cancel last step (you can cancel only one step).

**R** - Clear field and reset steps counter

**C** - Enable command mode

### Avaliable commands in command mode:

`<required>` - Required argument.

`[optional]` - Optional arargument.

- **reset [width] [height]**

Clears the field and resets the steps counter.
If you pass width and height arguments, it creates a field with given width and height.

- **set <position X> <position Y>**

Sets or removes life in a cell.

- **step [steps count or '-']**

Performs the specified number of steps. If there is no argument, it performs 1 step.
If the argument is '-', performs an infinite number of steps, until the key 'I' is pressed.

- **back**

Cancels the last step.
You can not undo more than one step.

- **save [filename]**

Saves field to file.
If no filename is specified, will be used: "game_of_life.fld"

- **load [filename]**

Loads field from file.
If no filename is specified, will be used: "game_of_life.fld"

## Install libncurses

### Linux

**In most distrs, the library will already be preinstalled.**

Use the package manager of your distr. Example for Debian:

`sudo apt-get update`

`sudo apt-get install libncurses5-dev`

### Mac OS X

Use the [Homebrew](https://brew.sh) package manager:

`brew install ncurses`

### Windows

**Without mouse support.**

- [PDCurses](https://pdcurses.sourceforge.io)

- ~~Install to Windows 10 tools from Ubuntu?~~

## Build

**CMake is required for build!**

Getting the source files:

`git clone https://github.com/kirpichik/GameOfLife-Curses.git`

Go to the dirrectory:

`cd GameOfLife-Curses`

Building cmake:

`mkdir cmake-build && cd cmake-build && cmake ..`

Building game executable:

`make`

Running tests:

`./GameOfLifeTests`

Launch the Game!

`./GameOfLife`

