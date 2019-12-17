#pragma once

#include "ofMain.h"
#include "NeuralNet.hpp"
#include "utl.h"
#include "Menu.hpp"

class ofApp : public ofBaseApp{
public:
  /*vector<vector<string>> nstruct = {
    {"*","*","*","*"},
    {"0","0","1","1","2","2","3","3"},
    {"*"},
    {"*","*","*","*","*","*","*","*"},
    {"0,1","2,3","4,5","6,7"}
  };*/
  /*vector<vector<string>> nstruct = {
    {"*","*","*","*","*"},
    {"*","*","*","*","*"}
  };*/
  vector<vector<string>> nstruct = {
    {"*","*","*","*"},
    {"*","*","*"}
  };
  NeuralNet net;
  
  vector<MenuItem*> mItems;
  Menu menu;
  
  bool importTrainingTrigger;
  bool exportTrainingTrigger;
  
  bool editTraining = false;
  bool isTraining = false;
  
  int tIndex=0;
  vector<double> yToBe;
  
  void setup();
  void update();
  void draw();
  void setupNet();
  
  void keyPressed(int key);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
};
