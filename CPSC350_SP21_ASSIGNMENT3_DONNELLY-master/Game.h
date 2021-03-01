/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include "Generation.h"
#include "FileProcessor.h"
using namespace std;

class Game {
public:
  // constructors and destructor
  Game();
  // random config with output file constructor
  Game(string configType, int firstDim, int secondDim, float proportionOfFilled, string boundaryMode, string outputMode, string outputFile);
  // random config constructor
  Game(string configType, int firstDim, int secondDim, float proportionOfFilled, string boundaryMode, string outputMode);
  // fromfile config with output file constructor
  Game(string configType, string fileName, string boundaryMode, string outputMode, string outputFile);
  // fromfile config constructor
  Game(string configType, string fileName, string boundaryMode, string outputMode);
  ~Game();

  // getters and setters
  string getConfigType();
  void setConfigType(string configInput);
  int getFirstDim();
  void setFirstDim(int firstDim);
  int getSecondDim();
  void setSecondDim(int secondDim);
  string getBoundaryType();
  void setBoundaryType(string boundaryType);
  string getOutputType();
  void setOutputType(string outputType);
  string getOutputFile();
  void setOutputFile(string outputFile);

  // generate first generation
  void generateRandomly(float proportionOfFilled);
  void generateFromFile(string fileName);

  // initializing arrays
  void initializeGenerationArray(int firstDim, int secondDim);
  void initializeEmbeddedGenArray(int firstDim, int secondDim);

  // store current gen on generation array
  void inputCurrentGen(Generation currentGen);

  // normal boundary mode
  void startGame();

  // checks if oscillating (max length 2)
  bool isOscillating();

  // find all neighbors for each cell in generation
  void findAllNeighborsCurrentGen(Generation& gen);

  // Mirror mode
  // set corresponding mirror grid of (firstDim + 2, secondDim + 2)
  void SetEmbeddedGenMirror();
  void GetTopStripMirror();
  void GetRightStripMirror();
  void GetBottomStripMirror();
  void GetLeftStripMirror();
  void GetTopLeftCornerMirror();
  void GetTopRightCornerMirror();
  void GetBottomLeftCornerMirror();
  void GetBottomRightCornerMirror();

  // Donut mode
  // set corresponding donut grid of (firstDim + 2, secondDim + 2)
  void SetEmbeddedGenDonut();
  void GetTopStripDonut();
  void GetRightStripDonut();
  void GetBottomStripDonut();
  void GetLeftStripDonut();
  void GetTopLeftCornerDonut();
  void GetTopRightCornerDonut();
  void GetBottomLeftCornerDonut();
  void GetBottomRightCornerDonut();

  // pulls center out of embedded generation into first index of generation array
  void GetCenter();

  // embeds entire generation array in first index of generation array into bigger array
  void EmbedGen();

  void pressEnterForNext();

private:
  // keeps track of the generation number.
  int m_genCount = -1;
  string m_configType;
  int m_firstDim;
  int m_secondDim;
  string m_boundaryMode;
  string m_outputMode;
  string m_outputFile;
  // for mirror and donut mode
  Generation m_embeddedGen;
  // array of 3 most recent generations
  Generation m_generations[3];
  bool m_areGensOscillating = false;
  // counts consecutive length-one oscillations
  int m_oscillationCountOne = 0;
  // counts consecutive length-two oscillations
  int m_oscillationCountTwo = 0;
  FileProcessor FProcessor;
};
#endif
