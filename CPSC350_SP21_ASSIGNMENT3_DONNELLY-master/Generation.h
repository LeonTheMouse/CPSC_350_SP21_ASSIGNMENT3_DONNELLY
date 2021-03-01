/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#ifndef GENERATION_H
#define GENERATION_H

#include "Cell.h"

class Generation {
  public:
    Cell *array;//1d array
    int control = 0;
    int m_firstDim;
    int m_secondDim;

    //defualt constructor
    Generation();

    //overloaded constructor
    Generation(int firstDim, int secondDim);

    //copy constructor
    Generation(const Generation& genToCopy);

    //destructor
    ~Generation(); // destruct array

    //operater overload so generations can be set using gen1 = gen2
    Generation& operator=(Generation& genToCopy);

    //operater overload to compare generations using gen1 == gen2
    bool operator==(Generation& genToCompare);

    //getters and setters that use coordinates
    Cell getCellWCoords(int X, int Y);
    char getCellStatusWCoords(int X, int Y);
    void setCellWCoords(int X, int Y, Cell cellToInput);
    void setCellStatusWCoords(int X, int Y, char charToInput);
    void setNumNeighborsAtCoord(int X, int Y, int numNeighbors);

    //function to print grid to console
    void printGrid();
};

#endif
