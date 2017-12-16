#include <SFML/Graphics.hpp>
#include <iostream>

#include "Cell.h"
#include "Array2D.h"

float cooldown = 0.1;
float thickness = 0.0;

int cell_size = 15;

sf::Color dead(0, 0, 0);
sf::Color alive(255, 255, 255);

std::vector<int> alives = {};
std::vector<int> deaths = {};

int round(int n, int multiple)
{
    return ((n + multiple/2) / multiple) * multiple;
}

int roundUp(int n, int factor)
{
    return n+factor-1-(n-1)%factor;
}

int main()
{
    srand(time(NULL));

    sf::VideoMode vm = sf::VideoMode::getDesktopMode();
    unsigned int width = round(vm.width/2.25, cell_size);
    unsigned int height = round(vm.height/2.25, cell_size);

    Array2D cells(width/cell_size, height/cell_size);

    sf::RectangleShape defaultCellShape;
    defaultCellShape.setSize(sf::Vector2f(cell_size, cell_size));
    defaultCellShape.setOutlineColor(sf::Color::White);
    defaultCellShape.setFillColor(dead);
    defaultCellShape.setOutlineThickness(thickness);

    Cell defaultCell(0, 0, false, defaultCellShape);

    int fits_width = width/cell_size;
    int fits_height = height/cell_size;

    for(int i = 0; i < width/cell_size; ++i)
    {
        for(int j = 0; j < height/cell_size; ++j)
        {
            bool isAlive = rand()%10+1 < 4;

            if(isAlive)
            {
                defaultCell.cell.setFillColor(alive);
            }
            else
            {
                defaultCell.cell.setFillColor(dead);
            }

            defaultCell.setPosition(i, j);
            defaultCell.setInternalPosition((i*cell_size)+thickness, (j*cell_size)+thickness);
            cells.set(defaultCell);
        }
    }

    sf::Clock clock;
    float elapsedTime = 0.0;

    bool lmb_down = false;
    bool rmb_down = false;

    sf::RenderWindow window(sf::VideoMode(width+thickness, height+thickness), "Conway's Game of Life");

    while(window.isOpen())
    {
        float DeltaTime = clock.restart().asSeconds();
        elapsedTime += DeltaTime;

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                int mb = event.mouseButton.button;

                if(mb == sf::Mouse::Left)
                {
                    lmb_down = true;

                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    int x = round(pos.x-(cell_size/2), cell_size)/cell_size;
                    int y = round(pos.y-(cell_size/2), cell_size)/cell_size;

                    Cell c = cells.cells[x*fits_height+y];
                    c.alive = true;
                    c.setColour(sf::Color(alive));

                    cells.cells[x*fits_height+y] = c;
                }
                else if(mb == sf::Mouse::Right)
                {
                    rmb_down = true;

                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    int x = round(pos.x-(cell_size/2), cell_size)/cell_size;
                    int y = round(pos.y-(cell_size/2), cell_size)/cell_size;

                    Cell c = cells.cells[x*fits_height+y];
                    c.alive = false;
                    c.setColour(sf::Color(dead));

                    cells.cells[x*fits_height+y] = c;
                }
            }
            else if(event.type == sf::Event::MouseButtonReleased)
            {
                int mb = event.mouseButton.button;

                if(mb == sf::Mouse::Left)
                    lmb_down = false;
                else if(mb == sf::Mouse::Right)
                    rmb_down = false;
            }
            else if(event.type == sf::Event::MouseMoved)
            {
                if(lmb_down)
                {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    int x = round(pos.x-(cell_size/2), cell_size)/cell_size;
                    int y = round(pos.y-(cell_size/2), cell_size)/cell_size;

                    Cell c = cells.cells[x*fits_height+y];c.alive = true;
                    c.setColour(sf::Color(alive));

                    cells.cells[x*fits_height+y] = c;
                }
                else if(rmb_down)
                {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    int x = round(pos.x-(cell_size/2), cell_size)/cell_size;
                    int y = round(pos.y-(cell_size/2), cell_size)/cell_size;

                    Cell c = cells.cells[x*fits_height+y];
                    c.alive = false;
                    c.setColour(sf::Color(dead));

                    cells.cells[x*fits_height+y] = c;
                }
            }
        }

        if(elapsedTime >= cooldown)
        {
            elapsedTime = 0.0;
            for(int i = 0; i < cells.cells.size(); ++i)
            {
                Cell cell = cells.get1D(i);

                int x = cell.x;
                int y = cell.y;

                int neighbours = 0;

                if(x != 0) neighbours += cells.get(x-1, y).alive ? 1 : 0;
                if(y != 0) neighbours += cells.get(x, y-1).alive ? 1 : 0;

                if(x != fits_width) neighbours += cells.get(x+1, y).alive ? 1 : 0;
                if(y != fits_height) neighbours += cells.get(x, y+1).alive ? 1 : 0;
                if(x != 0 && y != 0) neighbours += cells.get(x-1, y-1).alive ? 1 : 0;
                if(x != fits_width && y != 0) neighbours += cells.get(x+1, y-1).alive ? 1 : 0;
                if(x != 0 && y != fits_height) neighbours += cells.get(x-1, y+1).alive ? 1 : 0;
                if(x != fits_width && y != fits_height) neighbours += cells.get(x+1, y+1).alive ? 1 : 0;

                if(cell.alive)
                {
                    if(neighbours < 2 || neighbours > 3)
                    {
                        deaths.push_back(i);
                    }
                }
                else
                {
                    if(neighbours == 3)
                    {
                        alives.push_back(i);
                    }
                }
            }
        }

        for(int i = 0; i < deaths.size(); ++i)
        {
            Cell cell = cells.cells[deaths[i]];
            cell.alive = false;
            cell.setColour(dead);
            cells.cells[deaths[i]] = cell;
        }

        for(int i = 0; i < alives.size(); ++i)
        {
            Cell cell = cells.cells[deaths[i]];
            cell.alive = true;
            cell.setColour(alive);
            cells.cells[alives[i]] = cell;
        }

        alives.clear(); deaths.clear();

        window.clear();
        for(int i = 0; i < cells.cells.size(); ++i)
        {
            Cell cell = cells.get1D(i);
            window.draw(cell.cell);
        }
        window.display();
    }

    return 0;
}
