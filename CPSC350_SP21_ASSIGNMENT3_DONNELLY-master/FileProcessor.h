/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H
#include <iostream>
#include <fstream>
#include "Generation.h"

using namespace std;

class FileProcessor {
public:
  //constructor
  FileProcessor();

  //destructor
  ~FileProcessor();

  //checks file format is correct
  bool canReadConfigFromFile(string inputFile);

  bool canOpen(string fileName);

  //empty generation to put file contents into
  Generation readGridFromFile(string inputFile, int firstDim, int secondDim);

  //reads specified dimensions from first to lines of file
  void readDimFromFile(int &firstDim, int &secondDim, string inputFile);

  void outputGen(string outputFile, int genNum, Generation gen);

  //reset contents of file
  void clearFile(string fileToClear);
private:
  //checks file formatting
  bool areValidCharacters(string line);

  void removeWhiteSpace(string& line);
};

#endif
