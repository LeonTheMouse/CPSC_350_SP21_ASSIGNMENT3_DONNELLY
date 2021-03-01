/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#include "FileProcessor.h"

FileProcessor::FileProcessor() {}

FileProcessor::~FileProcessor() {}

bool FileProcessor::canReadConfigFromFile(string inputFile) {
  ifstream iFS;
  string line;
  unsigned int lineNumber = 1;//counter
  unsigned int height;
  unsigned int width;

  iFS.open(inputFile);
  if (!iFS.is_open()) {
    cout << "Couldn't open input file " << inputFile << endl;
    return false;
  }
  while (getline(iFS, line)) { //while theres a line to read
    if (lineNumber == 1){
      height = atoi(line.c_str());
    }
    if (lineNumber == 2) {
      width = atoi(line.c_str());
    }
    if (lineNumber > 2) {
        if (lineNumber - 2 > height) {
            cout << "Height of grid exceeds given height of " << height << endl;
            return false;
        }
        removeWhiteSpace(line);
        if (width == line.size()) {
            if (!areValidCharacters(line)) {
                cout <<"On line " << lineNumber << ", your file must use 'X' or '-' in the grid" << endl;
                return false;
            }
        }
        else {
            cout << "Row length of line " << lineNumber << " doesn't match width provided of " << width << endl;
            return false;
        }
    }
    lineNumber++;
  }
  if ((lineNumber - 3) < height) {
      cout << "Specified height of " << height << " exceeds grid height" << endl;
      return false;
  }
  if (iFS.fail()) {
    if (!iFS.eof()) {
      cout << "Error during file input" << endl;
      return false;
    }
  }
  iFS.close();
  return true;
}

void FileProcessor::readDimFromFile(int &firstDim, int &secondDim, string inputFile) {
  ifstream iFS;
  string line;
  iFS.open(inputFile);
  getline(iFS, line);//get height
  firstDim = atoi(line.c_str());
  getline(iFS, line);//get width
  secondDim = atoi(line.c_str());
}

Generation FileProcessor::readGridFromFile(string fileName, int firstDim, int secondDim) {
  ifstream iFS;
  string line;
  unsigned int lineNumber = 1;
  int height = firstDim;
  int width = secondDim;
  iFS.open(fileName);
  Generation generation(height, width);
  // moving past dimensions
  getline(iFS, line);
  getline(iFS, line);
  for (int i = 3; i < height + 3; ++i) {
    getline(iFS, line);
    for (int j = 0; j < width; ++j) {
      generation.setCellStatusWCoords(i - 3, j, line.at(j));//sets cell from file to array coordinates
    }
  }
  iFS.close();
  return generation;
}

void FileProcessor::removeWhiteSpace(string& line) {
    while (line.find(' ') != string::npos) {
        line.erase(line.find(' '), 1);
    }
    for (unsigned int i = 0; i < line.size(); ++i) {
        if (isspace(line.at(i))) {
            line.erase(i, 1);
        }
    }
}

bool FileProcessor::areValidCharacters(string line) {
    for (unsigned int i = 0; i < line.size(); ++i) {
        if (line.at(i) != 'X' && line.at(i) != '-') {
            return false;
        }
    }
    return true;
}

bool FileProcessor::canOpen(string fileName) {
  ifstream iFS;
  iFS.open(fileName);
  if (iFS.is_open()) {
    return true;
  }
  else {
    return false;
  }
}


void FileProcessor::outputGen(string outputFile, int genNum, Generation gen) {
  string genNumStr = to_string(genNum);
  ofstream OFS;
  // append mode
  OFS.open(outputFile, ios_base::app);
  OFS << genNumStr << endl;
  for (int i = 0; i < gen.m_firstDim; ++i) {
    string outputLine = "";
    for (int j = 0; j < gen.m_secondDim; ++j) {
      outputLine += gen.getCellStatusWCoords(i, j);
    }
    OFS << outputLine;
    OFS << '\n';
  }
  OFS.close();
}

void FileProcessor::clearFile(string fileToClear) {
  ofstream OFS;
  OFS.open(fileToClear);
  OFS.close();
}
