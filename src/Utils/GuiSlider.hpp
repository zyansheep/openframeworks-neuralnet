//
//  GuiSlider.hpp
//  
//
//  Created by Zyan on 4/23/18.
//
//

#ifndef GuiSlider_hpp
#define GuiSlider_hpp
#include "ofMain.h"
#include <stdio.h>
#include "utl.h"

class GuiSlider{
  public:
    void setup(int xpos, int ypos, int len, float startPercent);
    void update();
    void draw();
    float percent;
    bool selected;
  
  
  bool isMouseInBox();
  int x;
  int y;
  int length;
  
  int thickness = 5;
  int margin = 25; //margin around slider for mouse detection
};
#endif /* GuiSlider_hpp */
