#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

#include <SFML/Graphics.hpp>

class Cell
{
  public:
    int x, y;
    bool alive;

    sf::RectangleShape cell;

    void setColour(sf::Color colour);
    void setPosition(int new_x, int new_y);
    void setInternalPosition(int new_x, int new_y);

    Cell(int new_x, int new_y, bool isAlive, sf::RectangleShape shape);
    Cell();
};

void Cell::setColour(sf::Color colour)
{
    cell.setFillColor(colour);
}

void Cell::setPosition(int new_x, int new_y)
{
    x = new_x;
    y = new_y;
}

void Cell::setInternalPosition(int new_x, int new_y)
{
    cell.setPosition(new_x, new_y);
}

Cell::Cell(int new_x, int new_y, bool isAlive, sf::RectangleShape shape)
{
    alive = isAlive;
    cell = shape;
    x = new_x;
    y = new_y;
}

Cell::Cell()
{

}

#endif // CELL_H_INCLUDED
