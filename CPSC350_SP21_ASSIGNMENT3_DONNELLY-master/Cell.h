/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#ifndef CELL_H
#define CELL_H

class Cell {
public:
  // constructors, destructor
  Cell();
  Cell(char c);
  ~Cell();

  // getters, setters
  char getStatus();
  void setStatus(char status);

  // adding next number of neighbors to array
  void inputNumNeighbors(int numNeighbors);

  //operator overload to compare cells
  bool operator==(Cell& otherCell);

  //check all cells around cell
  float getAverageNeighbors(int genNum);

  // array of numbers of last three generations of neighbors
  int m_genNeighbors[3];

private:
  // 'X' for occupied / '-' for unoccupied
  char m_status;
  void initializeGenNeighbors();
};

#endif
