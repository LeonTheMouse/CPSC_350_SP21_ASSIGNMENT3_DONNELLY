/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#include "Cell.h"
#include <iostream>

using namespace std;

//cell defaults to -
Cell::Cell() {
  initializeGenNeighbors();
  m_status = '-';
}

Cell::Cell(char status) {
  initializeGenNeighbors();
  m_status = status;
}

Cell::~Cell() {
}

void Cell::initializeGenNeighbors() {
  for (int i = 0; i < 3; ++i) {
    m_genNeighbors[i] = 0;
  }
}

char Cell::getStatus() {
  return m_status;
}

void Cell::setStatus(char status) {
  m_status = status;
}

void Cell::inputNumNeighbors(int numNeighbors) {
  //store newstest gen at 0, replace old generations
  int temp1 = m_genNeighbors[0];
  int temp2 = m_genNeighbors[1];
  m_genNeighbors[0] = numNeighbors;
  m_genNeighbors[1] = temp1;
  m_genNeighbors[2] = temp2;
}

//overide to compare cell status when using ==
bool Cell::operator==(Cell& otherCell) {
  return (m_status == otherCell.getStatus());
}

float Cell::getAverageNeighbors(int genNum) {
  int sum = 0;
  for (int i = 0; i < 3; ++i) {
    sum += m_genNeighbors[i];
  }
  if (genNum == 0) {
    return (float(sum));
  }
  else if (genNum == 1) {
    return (float(sum) / 2);
  }
  else {
    return (float(sum) / 3);
  }
}
