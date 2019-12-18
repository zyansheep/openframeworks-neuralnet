//
//  Animation.hpp
//  Games
//
//  Created by Zyan on 10/1/18.
//
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include "ofMain.h"

class Animation{
public:
  void setup(ofPoint start_, ofPoint end_, int frames_);
  void update();
  
  int counter = 1;
  int frames; //number of frames
  ofPoint start;
  ofPoint end;
  
  ofPoint pos;
  int x,y;
  
  bool isDone = false;
  bool isRunning = false;
};

#endif /* Animation_hpp */
