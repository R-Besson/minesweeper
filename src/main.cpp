#include <SFML/Graphics.hpp>
#include "sweeper.h"

#define NODE_SIZE       30
#define NODE_MARGIN_Y   40
#define NODE_MARGIN_X   20
#define NODE_OFFSET     4
#define INF             100000

sf::RenderWindow* window;
Game* game = new Game;

sf::Font bit8;

sf::Texture flagIcon;
sf::Texture mineIcon;
sf::Texture crossIcon;

sf::Color numColors[9] = {
    sf::Color(200, 200, 200), // Background (0) - No Number
    sf::Color(0, 0, 255), // 1
    sf::Color(0, 130, 0), // 2
    sf::Color(245, 0, 0), // 3
    sf::Color(0, 0, 130), // 4
    sf::Color(130, 0, 0), // 5
    sf::Color(0, 130, 130), // 6
    sf::Color(0, 0, 0), // 7
    sf::Color(130, 130, 130) // 8
};


sf::Vector2f centerText(int x, int y, sf::Text txt)
{
    sf::FloatRect bounds = txt.getLocalBounds();

    float bX = bounds.width;
    float bY = bounds.height;

    return sf::Vector2f (
        x - bX/2,
        y - bY/2
    );
}
sf::Text getText(int size, std::string str, sf::Color col, sf::Text::Style* style)
{
    sf::Text txt;

    txt.setFont(bit8);
    txt.setCharacterSize(size);
    txt.setString(str);
    txt.setFillColor(col);
    if (style) txt.setStyle(*style);

    return txt;
}
sf::RectangleShape getRect(sf::Vector2f size, sf::Vector2f pos, sf::Color col, int ot, sf::Color otcol)
{
    sf::RectangleShape rect;

    rect.setSize(size);
    rect.setPosition(pos);
    rect.setFillColor(col);

    rect.setOutlineThickness(ot);
    rect.setOutlineColor(otcol);

    return rect;
}
sf::Sprite getSprite(sf::Texture* img, sf::Vector2f pos)
{
    sf::Sprite sprite;

    sprite.setTexture(*img);
    sprite.setPosition(pos);

    return sprite;
}


int closestFloor(int n, int m)
{
    return floor((float) n/m)*m;
}

Cell* toGrid(int x, int y)
{
    int relx = x - NODE_MARGIN_X;
    int rely = y - NODE_MARGIN_Y;

    relx = closestFloor(relx, NODE_SIZE+NODE_OFFSET)/(NODE_SIZE+NODE_OFFSET);
    rely = closestFloor(rely, NODE_SIZE+NODE_OFFSET)/(NODE_SIZE+NODE_OFFSET);

    if (relx >= 0 && relx < game->GRID_WIDTH && rely >= 0 && rely < game->GRID_HEIGHT)
    {
        return game->grid->at(relx)->at(rely);
    }
    return NULL;
}

bool started = false;
void revealAtMouse()
{
    if (game->state == GameState::Won || game->state == GameState::Lost) return;

    sf::Vector2i pos = sf::Mouse::getPosition(*window);
    Cell* clicked = toGrid(pos.x, pos.y);

    if (!clicked || clicked->isFlagged) return;

    if (!started)
    {
        started = true;
        game->populateGrid(clicked);
    }

    if (clicked->isRevealed && clicked->num > 0 && !clicked->isMine)
    {
        CellList* adjs = game->getAdjacentCells(clicked);

        int f = 0;
        for(auto it = adjs->begin(); it != adjs->end(); it++)
            if ((*it)->isFlagged) f++;

        if (f == clicked->num) // If we flagged the correct number of mines, reveal the adjacent squares
        {
            for(auto it = adjs->begin(); it != adjs->end(); it++)
                game->revealCell(*it);
        }
    }

    game->revealCell(clicked);
}

void flagAtMouse()
{
    if (game->state == GameState::Won || game->state == GameState::Lost) return;

    sf::Vector2i pos = sf::Mouse::getPosition(*window);
    Cell* clicked = toGrid(pos.x, pos.y);

    if (!clicked) return;

    game->flagCell(clicked);
}

void printUsage()
{
    std::cout << "Usage: <exe> <width> <height> <nb_mines>" << '\n';
    std::cout << "Example:" << '\n';
    std::cout << "    ./minesweeper 10 15 20    -> 10x10 grid with 20 hidden mines" << "\n\n";
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printUsage();
        return EXIT_FAILURE;
    }

	srand(time(NULL));

    int WIDTH = atoi(argv[1]);
    int HEIGHT = atoi(argv[2]);
    int MINES = atoi(argv[3]);

    if (MINES > WIDTH*HEIGHT-9)
    {
        std::cout << "You put too many mines ! Don't be too confident ;)" << '\n';
        std::cout << "(Max is " << (WIDTH*HEIGHT-9) << " mines for a " << WIDTH << 'x' << HEIGHT << " grid)" << '\n';
        return EXIT_FAILURE;
    }

    int windowWidth = WIDTH*(NODE_SIZE+NODE_OFFSET)+NODE_MARGIN_X+20;
    int windowHeight = HEIGHT*(NODE_SIZE+NODE_OFFSET)+NODE_MARGIN_Y+20;

    window = new sf::RenderWindow (sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);

    // Load files
    bit8.loadFromFile("./assets/font.ttf");
    flagIcon.loadFromFile("./assets/flag.png");
    mineIcon.loadFromFile("./assets/mine.png");
    crossIcon.loadFromFile("./assets/minecross.png");

    game->initialize(WIDTH, HEIGHT, MINES);

    // UI
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed : {
                    window->close();
                    break;
                }

                case sf::Event::MouseButtonPressed:{
                    if (event.mouseButton.button == sf::Mouse::Left)
                        revealAtMouse();
                    else if (event.mouseButton.button == sf::Mouse::Right)
                        flagAtMouse();
                }
                
                default: 
                    break;
            }
        }

        window->clear(sf::Color(100, 100, 100));

        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                Cell* cell = game->grid->at(x)->at(y);
                sf::Vector2f rectSize(
                    NODE_SIZE,
                    NODE_SIZE
                );
                sf::Vector2f rectPos(
                    x*(NODE_SIZE+NODE_OFFSET) + NODE_MARGIN_X,
                    y*(NODE_SIZE+NODE_OFFSET) + NODE_MARGIN_Y
                );

                if (cell->isRevealed)
                {
                    int n = cell->num;
                    std::string num = std::to_string(n);

                    // Text
                    if (cell->isMine)
                    {
                        if (!cell->isFlagged)
                        {
                            window->draw(getRect(rectSize, rectPos, sf::Color(255, 0, 0), 2, sf::Color(100, 100, 100)));
                            window->draw(getSprite(&mineIcon, rectPos));
                        } else {
                            window->draw(getRect(rectSize, rectPos, sf::Color(200, 200, 200), 2, sf::Color(100, 100, 100)));
                            window->draw(getSprite(&mineIcon, rectPos));
                        }
                    } else if (cell->isFlagged) {
                        window->draw(getRect(rectSize, rectPos, sf::Color(200, 200, 200), 2, sf::Color(100, 100, 100)));
                        window->draw(getSprite(&crossIcon, rectPos));
                    } else {
                        window->draw(getRect(rectSize, rectPos, sf::Color(200, 200, 200), 2, sf::Color(100, 100, 100)));

                        auto style = sf::Text::Bold;
                        sf::Text txt = getText(
                            NODE_SIZE - 8,
                            num,
                            numColors[n],
                            &style
                        );

                        txt.setPosition(centerText(
                            rectPos.x + NODE_SIZE/2,
                            rectPos.y + NODE_SIZE/2,
                            txt
                        ));

                        window->draw(txt);
                    }
                } else {
                    // Cover
                    window->draw(getRect(rectSize, rectPos, sf::Color(200, 200, 200), 2, sf::Color(255, 255, 255)));

                    if (cell->isFlagged)
                    {
                        window->draw(getSprite(&flagIcon, rectPos));
                    }
                }
            }
        } 

        // HUD
        sf::Text flagsleft = getText(14, "Flags left: " + std::to_string(game->FLAGS_LEFT), sf::Color(255, 255, 255), NULL);

        flagsleft.setPosition(sf::Vector2f(20, 13));

        window->draw(flagsleft);
        //

        // End of game
		if (game->checkWin()) game->state = GameState::Won;

		if (game->state == GameState::Won || game->state == GameState::Lost)
        {
            sf::Text txt = getText(
                14,
                (game->state == GameState::Won) ? "You Win!" : "You Lose :(",
                (game->state == GameState::Won) ? sf::Color(255, 220, 0) : sf::Color(255, 0, 0),
                NULL
            );

            txt.setPosition(centerText(windowWidth/2, 20, txt));

            window->draw(txt);

            game->revealAll();
        }

        window->display();
    }

    return EXIT_SUCCESS;
}
