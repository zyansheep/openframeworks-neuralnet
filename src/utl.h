//
//  utl.h
//  ZyanTerminal
//
//  Created by Zyan on 9/29/18.
//
//

#ifndef utl_h
#define utl_h

#include "Utils.hpp"


class Util : public Utils{
public:
  bool guiShown = true;
  ofTrueTypeFont font;
  ofImage logo;
  
  
  void load(){
    ofSetDataPathRoot("./data");
    font.load("Menlo.ttc",8);
    logo.load("Logo.png");
    cout << "Loaded Globals\n";
    
    windowResized(ofGetWidth(), ofGetHeight());
  }
};
extern Util util; //make util global!

#endif /* utl_h */
