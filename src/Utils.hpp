//
//  Utils.hpp
//  
//
//  Created by Zyan on 4/24/18.
//
//

#pragma once

#include <stdio.h>
#include <ofMain.h>

using namespace std;

class Utils {
public:
  //Global constant objects
  stringstream dss; //debug stringstream to display
  void drawDebug(); //func for drawing debug
  
  //Openframework Events
  void mousePressed(int x, int y, int button);
  void mouseMoved(int x, int y);
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button);
  void windowResized(int width, int height);
  void keyPressed(int key);
  void keyReleased(int key);
  void drawStringAtPoint(ofTrueTypeFont& font,string str, float x, float y, int xformat = 0, int yformat = 0);
  ofRectangle makeSquare(int x, int y, int radius);
  bool inRect(ofRectangle rect, int x, int y);
  bool mouseInRect(int x, int y, int width, int height);
  static vector<string> cutString(const string& str, const char& ch);
  unsigned int factorial(unsigned int n);
  bool collideCircleRect(ofPoint pos, unsigned int radius, ofRectangle rect);
  
  template<class asset>
  class GameAsset{
  public:
    string name;
    
    vector<asset> assets;
    
    vector<string> names;
    unsigned long size;
    
    string typeAsset;
    
    void load(string pathToFolder){
      //find out wich type
      vector<string> allowArray;
      if(is_same<asset,ofImage>::value){
        allowArray = {"png","jpg"};
        typeAsset = "image";
      }else if(is_same<asset,ofSoundPlayer>::value){
        allowArray = {"wav","mp3","ogg"};
        typeAsset = "sound";
      }else{
        cout << "GameAsset can only get sounds and images through the load function Use the loadFonts function for fonts\n";
        return;
      }
      
      ofDirectory dir(pathToFolder);
      for(auto item:allowArray){dir.allowExt(item);}
      dir.listDir();
      //imgOutput.reserve(imgDir.size());
      for(unsigned int imgInt=0;imgInt<dir.size();imgInt++){
        try {
          asset curAsset;
          curAsset.load(dir.getPath(imgInt));
          assets.push_back(curAsset);
          names.push_back(cutString(dir.getName(imgInt),'.')[0]);
        } catch (int e) {
          cout << "Error loading " <<dir.getPath(imgInt) << " Code: "<<e;
        }
        //cout << typeAsset << ": "<<names[imgInt]<<endl;
      }
      
      size = names.size();
      name = cutString(pathToFolder,'/').back();
    }
    void loadFonts(string pathToFolder){
      ofDirectory dir(pathToFolder);
      typeAsset = "font";
      dir.allowExt("ttf");
      dir.allowExt("ttc");
      dir.listDir();
      for(unsigned int imgInt=0;imgInt<dir.size();imgInt++){
        try {
          asset curAsset;
          curAsset.load(dir.getPath(imgInt), 13);
          assets.push_back(curAsset);
          names.push_back(cutString(dir.getName(imgInt),'.')[0]);
        } catch (int e) {
          cout << "Error loading "<<dir.getPath(imgInt) << " Code: "<<e;
        }
        //cout << typeAsset << ": "<< names[imgInt]<<endl;
      }
      
      size = names.size();
      name = cutString(pathToFolder,'/').back();
    }
    asset & getAsset(string assetName){
      auto pos = find(names.begin(), names.end(), assetName) - names.begin();
      if(pos < names.size()) {
        return assets[pos];
      }
      return assets[0]; //If cant find return #1
    }
  };
  
  template<typename Unknown>
  Unknown & findAsset(vector<Unknown> & assetAr, string name){
    for(auto & item: assetAr){
      if(item.name==name){return item;}}
    return assetAr[0];
  }
  
  template<typename Unknown>
  void range(vector<Unknown> & vec, unsigned long & inval){
    //if out of range loop around with modulus
    if(inval >= vec.size()){ inval = inval%vec.size(); }
  }
  
  template<typename Unknown>
  bool isIn(Unknown input, vector<Unknown> inAr){if(std::find(inAr.begin(), inAr.end(), input) != inAr.end()) {return true;}else{return false;}}
  template<typename Unknown>
  vector<Unknown> getArraySquished(vector<vector<Unknown>> &vec){
    vector<Unknown> output;
    output.reserve(vec.size() * vec[0].size());
    for(auto &line : vec){
      for(auto &item : line){
        output.push_back(item);
      }
    }
    return output;
  }
  
  template<typename Unknown>
  void drawMiddle(Unknown image, int x,int y,int width=-1,int height=-1){
    int ox,oy;
    if(width==-1 | height==-1){
      ox = x-image.getWidth();
      oy = y-image.getHeight();
      image.draw(ox, oy);
    }else{
      ox = x-width/2;
      oy = y-height/2;
      image.draw(ox, oy, width,height);
    }
    
  }
  
  template<typename Unknown>
  unsigned long findBiggest(vector<Unknown> &arr){
    Unknown num = 0;
    unsigned long index;
    for(unsigned long i=0;i<arr.size();i++){
      if(num < arr[i]){ num = arr[i]; index = i; }
    }
    return index;
  }
  template<typename Unknown>
  unsigned long findSmallest(vector<Unknown> &arr){
    Unknown num = 100000000;
    unsigned long index;
    for(unsigned long i=0;i<arr.size();i++){
      if(num > arr[i]){ num = arr[i]; index = i; }
    }
    return index;
  }
  //Openframework Constants
  int mouseX; //Mouse X and Y setter in ofApp.cpp
  int mouseY;
  int windowWidth; // window Width and Height
  int windowHeight;
  int smWindowDim;
  int curKey;
  bool keys[100000];
  
  unsigned long updateCounter; //setter in ofApp
  
  //Custom Constants
  bool mouseIsDragging;
  bool mouseIsPressed;
  bool keyIsPressed;
};
