/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#include "Game.h"
#include "FileProcessor.h"
#include "Generation.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
  string configType = "";
  string boundaryType = "";
  string outputType = "";
  string outputFileName = "";
  while (configType != "random" && configType != "fromfile") {
    cout << "Enter a config type (options: random and fromfile)[q to quit]: " << endl;//keeps prompting until input is valid
    cin >> configType;
    if (configType == "q")
      return 0;
  }
  while (boundaryType != "normal" && boundaryType != "donut" && boundaryType != "mirror") {
    cout << "Enter a boundary mode (options: normal, donut, or mirror)[q to quit]: " << endl;
    cin >> boundaryType;
    if (boundaryType == "q")
      return 0;
  }
  while (outputType != "pause" && outputType != "enter" && outputType != "tofile") {
    cout << "Enter an output mode (options: pause, enter, or tofile)[q to quit]: " << endl;
    cin >> outputType;
    if (outputType == "q")
      return 0;
    if (outputType == "tofile") {
      FileProcessor FP;
      cout << "Enter name of output file: " << endl;
      cin >> outputFileName;
      while (!FP.canOpen(outputFileName)) {
        cout << "Couldn't open " << outputFileName << ", please give a valid file name [q to quit]: " << endl;//makes sure file is valid
        cin >> outputFileName;
        if (outputFileName == "q") {
          return 0;
        }
      }
    }
  }

  if (configType == "random") {
    int firstDim = -1;
    int secondDim = -1;
    float proportionOfFilled = -1;
    while (firstDim <= 0) {
      cout << "Enter the first dimension of your grid (positive)[-1 to quit]: " << endl;
      cin >> firstDim;
      if (firstDim == -1)
        return 0;
    }
    while (secondDim <= 0) {
      cout << "Enter the second dimension of your grid (positive)[-1 to quit]: " << endl;
      cin >> secondDim;
      if (secondDim == -1)
        return 0;
    }
    while (proportionOfFilled > 1 || proportionOfFilled < 0) {
      cout << "Enter the proportion of cells to fill [-1 to quit]" << endl;
      cin >> proportionOfFilled;
      if (proportionOfFilled == -1)
        return 0;
    }
    if (outputType == "tofile") {
      //random game with tofile output overloaded constructor
      Game game(configType, firstDim, secondDim, proportionOfFilled, boundaryType, outputType, outputFileName);
      game.startGame();
    }
    else {
      //random game normal output constructor
      Game game(configType, firstDim, secondDim, proportionOfFilled, boundaryType, outputType);
      game.startGame();
    }
  }
  else {
    //get file names
    FileProcessor fileProcessor;
    string inputFileName = "";
    cout << "Enter name of input file: " << endl;
    cin >> inputFileName;
    while (!fileProcessor.canReadConfigFromFile(inputFileName)) {
      cout << "Please enter another file name or enter q to quit: " << endl;
      cin >> inputFileName;
      if (inputFileName == "q")
        return 0;
    }
    if (outputType == "tofile") {
      //output to file mode constructor
      Game game(configType, inputFileName, boundaryType, outputType, outputFileName);
      game.startGame();
    }
    else {
      Game game(configType, inputFileName, boundaryType, outputType);
      game.startGame();
    }
  }
  return 0;
}
