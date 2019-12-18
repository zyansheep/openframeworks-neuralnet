//
//  Animation.cpp
//  Games
//
//  Created by Zyan on 10/1/18.
//
//

#include "Animation.hpp"
void Animation::setup(ofPoint start_, ofPoint end_, int frames_){
  isRunning = false;
  isDone = false;
  counter = 1;
  frames = frames_;
  start = start_;
  end = end_;
  
  pos = start;
  x = pos.x;
  y = pos.y;
}
void Animation::update(){
  if(isDone){ return; }
  if(!isRunning){isRunning = true;}
  
  //add segment of difference each frame
  pos += (end-start) * ((float)1 / frames);
  x = pos.x;
  y = pos.y;
  
  counter++;
  
  if(counter > frames){
    isDone = true;
    isRunning = false;
  }
}
