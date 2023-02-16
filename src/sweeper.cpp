#include "sweeper.h"

const int xDir[8] {0,  1,  0, -1,  1,  1, -1, -1};
const int yDir[8] {1,  0, -1,  0,  1, -1,  1, -1};

// Initializes/Resets the Grid
void Game::initialize(int GRID_WIDTH, int GRID_HEIGHT, int NB_MINES) {
    grid->clear();

    this->GRID_WIDTH = GRID_WIDTH;
    this->GRID_HEIGHT = GRID_HEIGHT;
    this->NB_MINES = NB_MINES;
    
    for (int x = 0; x < GRID_WIDTH; x++)
    {
        grid->push_back(new CellList);

        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            Cell* cell = new Cell;

            cell->x = x;
            cell->y = y;

            grid->at(x)->push_back(cell);
        }
    }

    state = GameState::Initiated;
}

// Add Mines + Calulcate Numbers adjacent to Mines, to the Grid
void Game::populateGrid(Cell* &exception)
{
	// Generate mines
	int generated = 0;
	while (generated != NB_MINES)
	{
		int x = rand() % GRID_WIDTH;
		int y = rand() % GRID_HEIGHT;

        Cell* cell = grid->at(x)->at(y);

		if (cell->isMine) continue;

        if (
            abs(exception->x - cell->x) <= 1 &&
            abs(exception->y - cell->y) <= 1
        ) continue;

		cell->isMine = true;
		generated++;
	}
    FLAGS_LEFT = generated;

	// Generate adjacent numbered cells
	// NSWE + Diagonals
	for (int x = 0; x < GRID_WIDTH; x++)
	{
		for (int y = 0; y < GRID_HEIGHT; y++)
		{
            Cell* cell = grid->at(x)->at(y);

			if (!(cell->isMine)) continue;

            CellList* adjs = getAdjacentCells(cell);

            for(auto it = adjs->begin(); it != adjs->end(); it++)
                if (!(*it)->isMine) (*it)->num++;
		}
	}
}

// Gets 8 cells surrounding the root cell
CellList* Game::getAdjacentCells(Cell* &root)
{
    CellList* adjs = new CellList;

    // Up, Down, Left, Right, + Diagonals -> 8 Squares total
    for (int d = 0; d < 8; d++)
    {
        int nX = root->x + xDir[d];
        int nY = root->y + yDir[d];

        if (
            nX >= GRID_WIDTH ||
            nX < 0 ||
            nY >= GRID_HEIGHT ||
            nY < 0
        ) continue;

        adjs->push_back(grid->at(nX)->at(nY));
    }

    return adjs;
}

// Flags a cell (False flag is cell isnt a Mine, else is it Valid (Not known to user))
void Game::flagCell(Cell* &cell)
{
    if (cell->isRevealed) return;

    cell->isFlagged = !(cell->isFlagged);
    
    if (cell->isFlagged) FLAGS_LEFT--;
    else FLAGS_LEFT++;
}

// Recursively reveals an area that is purely mine-free
void Game::revealCell(Cell* &cell)
{
    if (!cell->isRevealed && !cell->isFlagged && state == GameState::Initiated) {
        cell->isRevealed = true;

        // End the game if the Cell is a mine
        if (cell->isMine) {
            state = GameState::Lost;
        } else if (cell->num == 0) {
            // If the cell has 0 adjacent mines, we need to recurse to clear out all adjacent 0 cells
            CellList* adjs = getAdjacentCells(cell);

            for(auto it = adjs->begin(); it != adjs->end(); it++)
                if (!(*it)->isMine) revealCell(*it);
        }
    }
}

// Reveals all cells (End game)
void Game::revealAll()
{
    for (int x = 0; x < GRID_WIDTH; x++)
	{
		for (int y = 0; y < GRID_HEIGHT; y++)
		{
            grid->at(x)->at(y)->isRevealed = true;
        }
    }
}

// Checks if user has successfully guessed all Mines
bool Game::checkWin()
{
    if (FLAGS_LEFT != 0) return false;

    for (int x = 0; x < GRID_WIDTH; x++)
	{
		for (int y = 0; y < GRID_HEIGHT; y++)
		{
            Cell* cell = grid->at(x)->at(y);

            if (cell->isMine && !cell->isFlagged) return false;
            if (!cell->isRevealed && !cell->isMine) return false;
        }
    }
    return true;
}

// ...
void Game::printGrid()
{
    for (int y = 0; y < GRID_WIDTH; y++)
	{
        std::cout << "\n";
		for (int x = 0; x < GRID_HEIGHT; x++)
		{
            Cell* cell = grid->at(x)->at(y);

            if (cell->isMine) std::cout << "X ";
            else std::cout << cell->num << " ";
        }
    }
}