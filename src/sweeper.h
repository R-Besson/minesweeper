#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#ifndef SWEEPER_H
#define SWEEPER_H

enum GameState {
    Defined,
    Initiated,
    Won,
    Lost
};

class Cell {
public:
    int x = 0;
    int y = 0;
    int num = 0;
    bool isRevealed = false;
    bool isFlagged = false;
    bool isMine = false;
};

typedef std::vector<Cell*> CellList;
typedef std::vector<CellList*> GameGrid;

class Game {
    public:
        int GRID_WIDTH;
        int GRID_HEIGHT;
        int NB_MINES;
        int FLAGS_LEFT;

        GameState state = GameState::Defined;
        GameGrid* grid = new GameGrid;
    
    void initialize(int, int, int);
    CellList* getAdjacentCells(Cell*&);
    void populateGrid(Cell*&);
    void flagCell(Cell*&);
    void revealCell(Cell*&);
    void revealAll();
    bool checkWin();
    void printGrid();
};

#endif