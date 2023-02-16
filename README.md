# Simple C++ Minesweeper
[![](https://img.shields.io/badge/Author-Romain%20Besson-brightgreen)](https://github.com/R-Besson) ![](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white) ![](https://img.shields.io/badge/Using-SFML-green)

> **Minesweeper** features a grid of clickable squares, with hidden "mines" scattered throughout the board. The objective is to clear the board (find all the mines by placing a flag) without detonating any mines, with help from clues about the number of neighboring mines in each square. [Wikipedia](https://en.wikipedia.org/wiki/Minesweeper_(video_game)) 

<img src="game.png" width="400">

## Requirements
You will need :<br>
- c++ compiler (example: [g++](https://gcc.gnu.org/))
- [GNU make](https://www.gnu.org/software/make/) for building ([documentation](https://www.gnu.org/software/make/manual/make.html)),<br>
- [SFML](https://www.sfml-dev.org/index.php) for graphics (for example, you can install SFML with [MSYS2](https://www.msys2.org/) using `pacman -S mingw-w64-x86_64-sfml` from MSYS2 terminal in windows)

## Building
```
cd minesweeper
make
```
This will create a **build** directory with the executable and the assets (images, fonts, ...)<br>
Alternatively look in the **Makefile** file for compilation & linking instructions without using make

## Running
```
cd build
```
To show usage instructions run the executable using `./minesweeper` (linux) or `minesweeper.exe` (win) in the terminal