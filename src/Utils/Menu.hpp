//
//  Menu.hpp
//  ZyanTerminal
//
//  Created by Zyan on 4/29/18.
//
//

#ifndef Menu_hpp
#define Menu_hpp

#include <ofMain.h>
#include <stdio.h>
#include "utl.h"

class MenuItem{
public:
  bool typeList = false;
  string name;
  bool mouseOver;
  bool highlighted; //only turns off
  
  int typeInteract; //0 for list, 1 for bool, 2 for int
  
  //Containing Menu Item
  MenuItem(string itemName, vector<MenuItem*> newItems);
  vector<MenuItem*> items;
  ofFbo menuGraphic;
  int x,y;
  bool menuOpen = false;
  
  //Executive Menu Item
  MenuItem(string itemName, bool * callbackBool);
  bool * callback;
  
  MenuItem(string itemName, int minInt, int* startInt, int maxInt);
  int min;
  int* num;
  int max;
  
  //Only call these functions on lists
  void renderGraphic();
  void drawGraphic();
  void detectMouse();
  bool mouseIn();
  void closeMenu();
  void itemClicked();
};

class Menu{
public:
  void setup(MenuItem* menuItems);
  void update();
  void draw();
  void mousePressed(int x,int y,int button);
  bool drawMenu;
  int numLists;
  int x = 0; int y = 50;
  MenuItem* list;
};
#endif /* Menu_hpp */
