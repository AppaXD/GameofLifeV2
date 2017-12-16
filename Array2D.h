#ifndef ARRAY2D_H_INCLUDED
#define ARRAY2D_H_INCLUDED

#include "Cell.h"

class Array2D
{
  public:
    std::vector<Cell> cells;

    int w, h;

    Array2D(int width, int height);
    Cell get(int width, int height);
    Cell get1D(int index);
    void set(Cell value);

    void reset1D(int, Cell);
    void reset(int, int, Cell);
};

Array2D::Array2D(int width, int height)
{
    w = width; h = height;
}

Cell Array2D::get(int x, int y)
{
    return cells[(y*h)+x];
}

Cell Array2D::get1D(int index)
{
    return cells[index];
}

void Array2D::set(Cell value)
{
    cells.push_back(value);
}

void Array2D::reset1D(int index, Cell value)
{

}

void Array2D::reset(int x, int y, Cell value)
{

}

#endif // ARRAY2D_H_INCLUDED
