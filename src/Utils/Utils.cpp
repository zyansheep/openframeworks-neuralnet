//
//  Utils.cpp
//  
//
//  Created by Zyan on 4/24/18.
//
//

#include "Utils.hpp"
//Misc Functions
void Utils::drawStringAtPoint(ofTrueTypeFont& font, string str,float x, float y, int xformat, int yformat){
  float strX;
  switch (xformat) {
    case -1: strX = x; break;
    case 0: strX = x-font.stringWidth(str)/2.0f; break;
    case 1: strX = x-font.stringWidth(str); break;
    default: strX = x-font.stringWidth(str)/2.0f; break;
  }
  float strY;
  switch (yformat) {
    case -1: strY = y; break;
    case 0: strY = y-font.stringHeight(str)/2.0f; break;
    case 1: strY = y-font.stringHeight(str); break;
    default: strY = y-font.stringHeight(str)/2.0f; break;
  }
  
  font.drawString(str, strX, strY);
}
ofRectangle Utils::makeSquare(int x, int y, int radius){
  auto rect = ofRectangle(x-radius,y-radius,radius*2, radius*2);
  return rect;
}
bool Utils::inRect(ofRectangle rect, int x, int y){
  if(rect.x < x && x < rect.x+rect.width && rect.y < y && y < rect.y+rect.height){
    return true;
  }else{
    return false;
  }
}
bool Utils::mouseInRect(int x, int y, int width, int height){
  auto rect = ofRectangle(x,y,width,height);
  return inRect(rect,mouseX, mouseY);
}
vector<string> Utils::cutString(const string& str, const char& ch) {
  string next;
  vector<string> result;
  bool skp = false;
  
  // For each character in the string
  for (string::const_iterator it = str.begin(); it != str.end(); it++) {
    // If we've hit the terminal character
    if (*it == ch && !skp) {
      // If we have some characters accumulated
      if (!next.empty()) {
        // Add them to the result vector
        result.push_back(next);
        next.clear();
      }
    } else {
      // Accumulate the next character into the sequence
      if(*it == '\\' && *(it+1) == ch){
        skp = true;

      }else{
        skp = false;
        next += *it;
      }
    }
  }
  if (!next.empty())
    result.push_back(next);
  return result;
}
unsigned int Utils::factorial(unsigned int n){
  if (n == 0)
    return 1;
  return n * factorial(n - 1);
}
bool Utils::collideCircleRect(ofPoint cPos, unsigned int radius, ofRectangle rect){
  // Find the closest point to the circle within the rectangle
  float closestX = glm::clamp(cPos.x, rect.x, rect.x+rect.width);
  float closestY = glm::clamp(cPos.y, rect.y, rect.y+rect.height);
  
  // Calculate the distance between the circle's center and this closest point
  float distanceX = cPos.x - closestX;
  float distanceY = cPos.y - closestY;
  
  // If the distance is less than the circle's radius, an intersection occurs
  float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
  return distanceSquared < (radius*radius);
  
  /*ofPoint circleDistance;
  circleDistance.x = abs(cPos.x - rect.x);
  circleDistance.y = abs(cPos.y - rect.y);
  
  if (circleDistance.x > (rect.width/2 + radius)) { return false; }
  if (circleDistance.y > (rect.height/2 + radius)) { return false; }
  
  if (circleDistance.x <= (rect.width/2)) { return true; }
  if (circleDistance.y <= (rect.height/2)) { return true; }
  
  float cornerDistance_sq = pow(circleDistance.x - rect.width/2, 2) + pow(circleDistance.y - rect.height/2, 2);
  
  return (cornerDistance_sq <= pow(radius,2));*/
}

void Utils::drawDebug(){
  int pos;
  for(string line;getline(dss,line);){
    //cout << line<<endl;
    pos+=20;
    ofDrawBitmapString(line,20,pos);
  }
}
//OPENFRAMEWORK EVENTS CALLED FROM ofapp.cpp
void Utils::mousePressed(int x, int y, int button){
  mouseIsPressed = true;
}
void Utils::mouseReleased(int x, int y, int button){
  mouseIsPressed = false;
  mouseIsDragging = false;
}
void Utils::mouseDragged(int x, int y, int button){
  mouseIsDragging = true;
}
void Utils::mouseMoved(int x, int y){
  mouseX = x;
  mouseY = y;
}
void Utils::windowResized(int width, int height){
  smWindowDim = min(width,height);
  windowWidth = width;
  windowHeight = height;
}
void Utils::keyPressed(int key){
  curKey = key;
  keys[key] = true;
}
void Utils::keyReleased(int key){
  keys[key] = false;
}
