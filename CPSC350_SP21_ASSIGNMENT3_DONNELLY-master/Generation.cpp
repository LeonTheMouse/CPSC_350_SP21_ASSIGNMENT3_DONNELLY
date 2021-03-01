/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#include "Generation.h"
#include <iostream>
using namespace std;

Generation::Generation() {
  array = new Cell[10];
  Cell cell;
  for (int i = 0; i < 10; ++i) {
    array[i] = cell;
  }
}

Generation::Generation(int firstDim, int secondDim) {
  array = new Cell[firstDim * secondDim];
  Cell cell;//empty cell to fill base gen with
  for (int i = 0; i < firstDim; ++i) {
    for (int j = 0; j < secondDim; ++j) {
      // mapping of dimensions onto a 1d array
      array[i + j * firstDim] = cell;
    }
  }
  m_firstDim = firstDim;
  m_secondDim = secondDim;
}

Generation::Generation(const Generation& genToCopy) {
  this->m_firstDim = genToCopy.m_firstDim;
  this->m_secondDim = genToCopy.m_secondDim;
  array = new Cell[m_firstDim * m_secondDim];
  for (int i = 0; i < m_firstDim * m_secondDim; ++i) {
    array[i] = genToCopy.array[i];
  }
}

Generation::~Generation() {
  if (array != nullptr) {
    delete []array;
  }
}

Generation& Generation::operator=(Generation& genToCopy) {
  if (&genToCopy != this) {
    this->m_firstDim = genToCopy.m_firstDim;
    this->m_secondDim = genToCopy.m_secondDim;
    // match up dimensions
    Cell* temp = new Cell[m_firstDim * m_secondDim];
    // populate temp
    for (int i = 0; i < (m_firstDim * m_secondDim); ++i) {
      temp[i] = genToCopy.array[i];
    }
    // interchange arrays
    swap(this->array, temp);
    // deallocate previous this->array
    delete []temp;
  }
  return *this;
}

bool Generation::operator==(Generation &genToCompare) {
  for (int i = 0; i < m_firstDim * m_secondDim; ++i) {
    if (!(this->array[i] == genToCompare.array[i])) {//check every cell to compare arrays
      return false;
    }
  }
  return true;
}

Cell Generation::getCellWCoords(int X, int Y) {
  return array[X + Y * m_firstDim];
}

char Generation::getCellStatusWCoords(int X, int Y) {
  return array[X + Y * m_firstDim].getStatus();
}

void Generation::setCellWCoords(int X, int Y, Cell cellToInput) {
    array[X + Y * m_firstDim] = cellToInput;
}

void Generation::setCellStatusWCoords(int X, int Y, char charToInput) {
  array[X + Y * m_firstDim].setStatus(charToInput);
}


void Generation::setNumNeighborsAtCoord(int X, int Y, int numNeighbors) {
  array[X + Y * m_firstDim].inputNumNeighbors(numNeighbors);
}

void Generation::printGrid() {
  for (int i = 0; i < m_firstDim; ++i) {
    for (int j = 0; j < m_secondDim; ++j) {
      cout << getCellStatusWCoords(i, j);
    }
    cout << '\n';
  }
}
