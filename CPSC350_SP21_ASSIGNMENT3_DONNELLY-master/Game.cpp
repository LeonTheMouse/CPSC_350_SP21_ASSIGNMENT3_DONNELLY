/*
Scott Foreman
2324296
Jack Donnelly
2342203
CPSC 350-02
Assignment 3
*/

#include "Game.h"
#include "Generation.h"
#include "Cell.h"
#include "FileProcessor.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <unistd.h>
using namespace std;

Game::Game() {
  m_configType = "random";
  m_firstDim = 1;
  m_secondDim = 1;
  m_boundaryMode = "normal";
  m_outputMode = "normal";
}

// random config
Game::Game(string configType, int firstDim, int secondDim, float proportionOfFilled, string boundaryMode, string outputMode) {
  m_configType = configType;
  m_firstDim = firstDim;
  m_secondDim = secondDim;
  m_boundaryMode = boundaryMode;
  m_outputMode = outputMode;
  initializeGenerationArray(firstDim, secondDim);
  if (boundaryMode == "donut" || boundaryMode == "mirror") {
    initializeEmbeddedGenArray(firstDim + 2, secondDim + 2);
  }
  generateRandomly(proportionOfFilled);
}

// random config with output file
Game::Game(string configType, int firstDim, int secondDim, float proportionOfFilled, string boundaryMode, string outputMode, string outputFile) {
  m_configType = configType;
  m_firstDim = firstDim;
  m_secondDim = secondDim;
  m_boundaryMode = boundaryMode;
  m_outputMode = outputMode;
  m_outputFile = outputFile;
  FProcessor.clearFile(m_outputFile);
  initializeGenerationArray(firstDim, secondDim);
  if (boundaryMode == "donut" || boundaryMode == "mirror") {
    initializeEmbeddedGenArray(firstDim + 2, secondDim + 2);
  }
  generateRandomly(proportionOfFilled);
}

// flatfile config
Game::Game(string configType, string fileName, string boundaryMode, string outputMode) {
  m_configType = configType;
  m_boundaryMode = boundaryMode;
  m_outputMode = outputMode;
  generateFromFile(fileName);
}

// flatfile config with output file
Game::Game(string configType, string fileName, string boundaryMode, string outputMode, string outputFile) {
  m_configType = configType;
  m_boundaryMode = boundaryMode;
  m_outputMode = outputMode;
  m_outputFile = outputFile;
  FProcessor.clearFile(m_outputFile);
  generateFromFile(fileName);
}

Game::~Game() {
}

void Game::initializeGenerationArray(int firstDim, int secondDim) {
  setFirstDim(firstDim);
  setSecondDim(secondDim);
  Generation generation(firstDim, secondDim);
  for (int i = 0; i < 3; ++i) {
    m_generations[i] = generation;
  }
}

// creation of embedded generation array with dimensions firstDim + 2, secondDim + 2
void Game::initializeEmbeddedGenArray(int firstDim, int secondDim) {
  Generation generation(firstDim, secondDim);
  m_embeddedGen = generation;
}

// will input gen in argument at front of generation array, shifting previous ones forward
void Game::inputCurrentGen(Generation currentGen) {
  Generation temp1;
  Generation temp2;
  temp1 = m_generations[0];
  temp2 = m_generations[1];
  m_generations[0] = currentGen;
  m_generations[1] = temp1;
  m_generations[2] = temp2;
  // preparing the mirror embedded generation corresponding to currentGen
  if (m_boundaryMode == "mirror") {
    SetEmbeddedGenMirror();
  }
  // preparing the donut embedded generation corresponding to currentGen
  else if (m_boundaryMode == "donut") {
    SetEmbeddedGenDonut();
  }
  ++m_genCount;
  // dealing with output mode
  if (m_outputMode == "enter") {
    pressEnterForNext();
  }
  else if (m_outputMode == "pause") {
    // only pause if past 0
    if (m_genCount > 0) {
      if (!m_areGensOscillating) {
        sleep(2);
      }
      else {
        cin.get();
        pressEnterForNext();
      }
    }
  }
  else {
    // if there are more than 5000 gens outputted assume there is an oscillation of length greater than 2.
    if (m_genCount > 5000) {
      cout << "Max size of 5000 gens reached. Oscillation is expected. File output terminated." << endl;
      m_areGensOscillating = true;
    }
    if (m_areGensOscillating != true)
      FProcessor.outputGen(m_outputFile, m_genCount, m_generations[0]);
  }
  if (m_outputMode != "tofile") {
    cout << "\n";
    if (!m_areGensOscillating) {
      cout << m_genCount << endl;
      m_generations[0].printGrid();
    }
  }
}

string Game::getConfigType() {
  return m_configType;
}

void Game::setConfigType(string configInput) {
  m_configType = configInput;
}

string Game::getBoundaryType() {
  return m_boundaryMode;
}

void Game::setBoundaryType(string boundaryType) {
  m_boundaryMode = boundaryType;
}

string Game::getOutputType() {
  return m_outputMode;
}

void Game::setOutputType(string outputType) {
  m_outputMode = outputType;
}

int Game::getFirstDim() {
  return m_firstDim;
}

void Game::setFirstDim(int firstDim) {
  m_firstDim = firstDim;
}

void Game::setSecondDim(int secondDim) {
  m_secondDim = secondDim;
}

int Game::getSecondDim() {
  return m_secondDim;
}

void Game::setOutputFile(string outputFile) {
  m_outputFile = outputFile;
}

string Game::getOutputFile() {
  return m_outputFile;
}

void Game::generateRandomly(float proportionOfFilled) {
  Generation generation(m_firstDim, m_secondDim);
  srand(time(NULL));
  int count = 0;
  int firstCoordinate;
  int secondCoordinate;
  float cellComposition = proportionOfFilled * (m_firstDim * m_secondDim);
  //number of cells to fill
  int cellsToFill = round(cellComposition);
  while (cellsToFill > count){
    firstCoordinate = rand() % m_firstDim;
	  secondCoordinate = rand() % m_secondDim;
  	if (generation.getCellStatusWCoords(firstCoordinate, secondCoordinate) == '-'){
  		generation.setCellStatusWCoords(firstCoordinate, secondCoordinate, 'X');
  		count++;
  	}
    // from 2-23 lecture
    else {
      // speeds up process by checking next coordinates modulo the dimensions
      firstCoordinate = (++firstCoordinate) % m_firstDim;
      secondCoordinate = (++secondCoordinate) % m_secondDim;
      if (generation.getCellWCoords(firstCoordinate, secondCoordinate).getStatus() == '-') {
        continue;
      }
    }
  }
  inputCurrentGen(generation);
}

void Game::generateFromFile(string fileName) {
  FileProcessor fp;
  // sets m_firstDim and m_secondDim to correct values from file
  fp.readDimFromFile(m_firstDim, m_secondDim, fileName);
  initializeGenerationArray(m_firstDim, m_secondDim);
  if (m_boundaryMode == "donut" || m_boundaryMode == "mirror") {
    initializeEmbeddedGenArray(m_firstDim + 2, m_secondDim + 2);
  }
  // grabs grid from file and inputs it onto generation array
  inputCurrentGen(fp.readGridFromFile(fileName, m_firstDim, m_secondDim));
}

void Game::startGame() {
  // checks for oscillation or if the output mode is tofile and there are less than 5000 gens
  while (!m_areGensOscillating) {
    Generation currentGen(m_firstDim, m_secondDim);
    if (m_boundaryMode == "donut" || m_boundaryMode == "mirror") {
      //uses embedded gen to simulate mirror or donut mode leveraging normal mode to get neighbors
      findAllNeighborsCurrentGen(m_embeddedGen);
    }
    else {
      //just normal mode
      findAllNeighborsCurrentGen(m_generations[0]);
    }
    // copy of most recent gen so most recent isn't modified
    currentGen = m_generations[0];
    // sets each cell status according to their average neighbors
    for (int i = 0; i < m_firstDim; ++i) {
      for (int j = 0; j < m_secondDim; ++j) {
        float averageOfCell = currentGen.getCellWCoords(i, j).getAverageNeighbors(m_genCount);
        if (averageOfCell < 1.5) {
          currentGen.setCellStatusWCoords(i, j, '-');
        }
        else if (averageOfCell < 2.5) {
          continue;
        }
        else if (averageOfCell < 3.5) {
          currentGen.setCellStatusWCoords(i, j, 'X');
        }
        else {
          currentGen.setCellStatusWCoords(i, j, '-');
        }
      }
    }
    //storing currentGen and iterating to the next generation
    m_areGensOscillating = isOscillating();
    inputCurrentGen(currentGen);
  }
}

//checks for oscillation based on generation
bool Game::isOscillating() {
  if (m_genCount == 0) {
    return false;
  }
  else if (m_genCount == 1) {
    if (m_generations[0] == m_generations[1]) {
      ++m_oscillationCountOne;
      return false;
    }
    else {
      return false;
    }
  }
  // if m_genCount >= 2 then start checking for both types
  else {
    // length-one
    if (m_generations[0] == m_generations[1]) {
      ++m_oscillationCountOne;
    }
    else {
      m_oscillationCountOne = 0;
    }
    // length-two
    if (m_generations[0] == m_generations[2]) {
      ++m_oscillationCountTwo;
    }
    else {
      m_oscillationCountTwo = 0;
    }

    if (m_oscillationCountOne == 3) {
      return true;
    }
    else if (m_oscillationCountTwo == 4) {
      return true;
    }
    else {
      return false;
    }
  }
}

// substitute m_gen[0] for gen for normal mode/ embeddedGen for mirror or donut
void Game::findAllNeighborsCurrentGen(Generation& gen) {
  // looping through cells in array
  for (int yCoord = 0; yCoord < gen.m_firstDim; ++yCoord) {
    for (int xCoord = 0; xCoord < gen.m_secondDim; ++xCoord) {
      // grabbing first coordinate
      int yCoordNeighbor = yCoord - 1;
      int numNeighbors = 0;
      // checking neighbors and itself (will run 9 times per cell)
      for (int i = 0; i < 3; ++i) {
        for (int xCoordNeighbor = xCoord - 1; xCoordNeighbor < xCoord + 2; ++xCoordNeighbor) {
          // indexes are incorrect, continue
          if (yCoordNeighbor < 0 || yCoordNeighbor >= gen.m_firstDim || xCoordNeighbor < 0 || xCoordNeighbor >= gen.m_secondDim) {
            continue;
          }
          else {
            // checks if the neighbor coords are not the coords of the cell being calculated and if the neighbor is full
            if ((yCoordNeighbor != yCoord || xCoordNeighbor != xCoord) && gen.getCellStatusWCoords(yCoordNeighbor, xCoordNeighbor) == 'X') {
              ++numNeighbors;
            }
          }
        }
        // increment yCoordNeighbor/ will happen 3 times checking 9 cells
        ++yCoordNeighbor;
      }
      gen.setNumNeighborsAtCoord(yCoord, xCoord, numNeighbors);
    }
  }
  if (m_boundaryMode != "normal") {
    // clone center of embedded generation into the most recent gen on m_generations array,
    // transferring neighbor data
    GetCenter();
  }
}

// waits until user presses next
void Game::pressEnterForNext() {
  if (!m_areGensOscillating && m_genCount != 0) {
    cout << "Press enter for next gen";
  }
  if (m_areGensOscillating) {
    cout << "The generations are oscillating. Press enter to exit program" << endl;
  }
  cin.clear();
  cin.get();
  if (!m_areGensOscillating) {
    cout << '\n';
  }
}

//these methods create a border around the current generation to simulate mirror behavior
void Game::SetEmbeddedGenMirror() {
  GetTopStripMirror();
  GetBottomStripMirror();
  GetLeftStripMirror();
  GetRightStripMirror();
  GetTopLeftCornerMirror();
  GetTopRightCornerMirror();
  GetBottomLeftCornerMirror();
  GetBottomRightCornerMirror();
  EmbedGen();
}

void Game::GetTopStripMirror() {
  for (int i = 1; i < m_secondDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(0, i, m_generations[0].getCellWCoords(0, i - 1));
  }
}

void Game::GetRightStripMirror() {
  for (int i = 1; i < m_firstDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(i, m_embeddedGen.m_secondDim - 1, m_generations[0].getCellWCoords(i - 1, m_secondDim - 1));
  }
}

void Game::GetLeftStripMirror() {
  for (int i = 1; i < m_firstDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(i, 0, m_generations[0].getCellWCoords(i - 1, 0));
  }
}

void Game::GetBottomStripMirror() {
  for (int i = 1; i < m_secondDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(m_embeddedGen.m_firstDim - 1, i, m_generations[0].getCellWCoords(m_firstDim - 1, i - 1));
  }
}

void Game::GetTopLeftCornerMirror() {
  if (m_firstDim == 1 || m_secondDim == 1) {
    m_embeddedGen.setCellWCoords(0, 0, m_generations[0].getCellWCoords(0, 0));
  }
  else {
    m_embeddedGen.setCellWCoords(0, 0, m_generations[0].getCellWCoords(1, 1));
  }
}

void Game::GetTopRightCornerMirror() {
  m_embeddedGen.setCellWCoords(0, m_embeddedGen.m_secondDim - 1, m_generations[0].getCellWCoords(0, m_secondDim - 1));
}

void Game::GetBottomLeftCornerMirror() {
  m_embeddedGen.setCellWCoords(m_embeddedGen.m_firstDim - 1, 0, m_generations[0].getCellWCoords(m_firstDim - 1, 0));
}

void Game::GetBottomRightCornerMirror() {
  m_embeddedGen.setCellWCoords(m_embeddedGen.m_firstDim - 1, m_embeddedGen.m_secondDim - 1, m_generations[0].getCellStatusWCoords(m_firstDim - 1, m_secondDim - 1));
}

//these methods create a border around the current generation to simulate donut behavior
void Game::SetEmbeddedGenDonut() {
  GetTopStripDonut();
  GetRightStripDonut();
  GetBottomStripDonut();
  GetLeftStripDonut();
  GetTopLeftCornerDonut();
  GetTopRightCornerDonut();
  GetBottomLeftCornerDonut();
  GetBottomRightCornerDonut();
  EmbedGen();
}

void Game::GetTopStripDonut() {
  for (int i = 1; i < m_secondDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(0, i, m_generations[0].getCellWCoords(m_firstDim - 1, i - 1));
  }
}

void Game::GetRightStripDonut() {
  for (int i = 1; i < m_firstDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(i, m_embeddedGen.m_secondDim - 1, m_generations[0].getCellWCoords(i - 1, 0));
  }
}

void Game::GetLeftStripDonut() {
  for (int i = 1; i < m_firstDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(i, 0, m_generations[0].getCellWCoords(i - 1, m_secondDim - 1));
  }
}

void Game::GetBottomStripDonut() {
  for (int i = 1; i < m_secondDim + 1; ++i) {
    m_embeddedGen.setCellWCoords(m_embeddedGen.m_firstDim - 1, i, m_generations[0].getCellWCoords(0, i - 1));
  }
}

void Game::GetTopLeftCornerDonut() {
  m_embeddedGen.setCellWCoords(0, 0, m_generations[0].getCellWCoords(m_firstDim - 1, m_secondDim - 1));
}

void Game::GetTopRightCornerDonut() {
  m_embeddedGen.setCellWCoords(0, m_embeddedGen.m_secondDim - 1, m_generations[0].getCellWCoords(m_firstDim - 1, 0));
}

void Game::GetBottomLeftCornerDonut() {
  m_embeddedGen.setCellWCoords(m_embeddedGen.m_firstDim - 1, 0, m_generations[0].getCellWCoords(0, m_secondDim - 1));
}

void Game::GetBottomRightCornerDonut() {
  m_embeddedGen.setCellWCoords(m_embeddedGen.m_firstDim - 1, m_embeddedGen.m_secondDim - 1, m_generations[0].getCellWCoords(0, 0));
}

// transferring center of embedded to most recent gen
void Game::GetCenter() {
  for (int i = 1; i < m_firstDim + 1; ++i) {
    for (int j = 1; j < m_secondDim + 1; ++j) {
      m_generations[0].setCellWCoords(i - 1, j - 1, m_embeddedGen.getCellWCoords(i, j));
    }
  }
}

// embedding most recent gen into center of embedded gen
void Game::EmbedGen() {
  for (int i = 1; i < m_firstDim + 1; ++i) {
    for (int j = 1; j < m_secondDim + 1; ++j) {
      m_embeddedGen.setCellWCoords(i, j, m_generations[0].getCellWCoords(i - 1, j - 1));
    }
  }
}
