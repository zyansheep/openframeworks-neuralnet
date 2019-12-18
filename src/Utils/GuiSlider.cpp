//
//  GuiSlider.cpp
//  
//
//  Created by Zyan on 4/23/18.
//
//

#include "GuiSlider.hpp"
void GuiSlider::setup(int xpos, int ypos, int len, float startPercent){
  x = xpos;
  y = ypos;
  length = len;
  percent = startPercent;
}
void GuiSlider::update(){
  if(util.mouseIsPressed && !selected){
    selected = isMouseInBox();
  }
  if(selected && util.mouseIsPressed){
    if(x < util.mouseX && util.mouseX < x+length){
      percent = (float)(util.mouseX-x)/(float)(length);
    }else{
      //prevent bar from begin dragged to far
      if(x > util.mouseX){ percent = 0; }
      if(util.mouseX > x+length){ percent = 1; }
    }
    
  }else {selected = false; }
}
void GuiSlider::draw(){
  ofSetColor(190,190,190);
  ofDrawRectangle(x,y, length, -thickness);
  ofSetColor(50,50,50);
  ofDrawRectangle(x,y, length*percent,-thickness);
  ofSetColor(255,255,255);
  ofDrawCircle(length*percent+x,y-(thickness/2),6);
}
bool GuiSlider::isMouseInBox(){
  if(util.mouseY < y+margin && util.mouseY > y-margin){
    if(x < util.mouseX && util.mouseX < x+length){
      return true;
    }
  }
  return false;
}
