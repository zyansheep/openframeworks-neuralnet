//
//  Menu.cpp
//  ZyanTerminal
//
//  Created by Zyan on 4/29/18.
//
//
//THERE IS A MEMORY BUG HERE (pointers in ofApp may be culprit)
#include "Menu.hpp"
MenuItem::MenuItem(string itemName, vector<MenuItem*> newItems){
  items = newItems;
  name = itemName;
  typeList = true;
  typeInteract == 0;
}
MenuItem::MenuItem(string itemName, bool * callbackBool){
  name = itemName;
  callback = callbackBool;
  
  typeInteract = 1;
}
MenuItem::MenuItem(string itemName, int minInt, int* startInt, int maxInt){
  name = itemName;
  min = minInt;
  num = startInt;
  max = maxInt;
  
  typeInteract = 2;
}

void MenuItem::renderGraphic(){
  //get max width of menu
  int width = 0;
  int curWidth;
  for(int i=0;i<items.size();i++){
    curWidth = util.font.stringWidth(items[i]->name);
    if(width < curWidth){
      width = curWidth;
    }
    if(items[i]->typeList){ width+=10; }
  }
  width+=30;
  int height = util.font.stringHeight(items[0]->name)*2;
  menuGraphic.allocate(width, height*(int)items.size(), GL_RGBA);
  menuGraphic.begin();
  for(int n=0;n<items.size();n++){
    //This is drawing on the graphic, not in relation to the screen
    ofSetColor(255);
    if(items[n]->highlighted){ ofSetColor(0,0,200); }
    ofDrawRectangle(0,n*height,width,height);
    ofSetColor(0);
    util.font.drawString(items[n]->name, 10, (float)(n*height + (height/2)+5));
    if(items[n]->typeList){
      float diff = height/4;
      ofDrawTriangle(width-10,height*n+2*diff, width-20,height*n+3*diff, width-20,height*n+diff);
    }
    
    //cout << *items[n]->callback << endl;
    if(!items[n]->typeList){
      if(items[n]->typeInteract == 1){
        if((*items[n]->callback)){
          ofSetColor(0,255,0);
        }else{
          ofSetColor(255,0,0);
        }
        ofDrawCircle(width-10, height*n+height/2, 5);
      }
      if(items[n]->typeInteract == 2){
        util.font.drawString(ofToString(*items[n]->num), width-20, height*n +(height/2+5));
      }
      
    }
    
    
    //Graphic positioning for next menu lists
    if(items[n]->typeList){
      items[n]->x = x+width;
      items[n]->y = y+height*n;
    }
  }
  menuGraphic.end();
  
  //render others systematically
  for(int i=0;i<items.size();i++){
    if(items[i]->typeList){
      items[i]->renderGraphic();
    }
  }
}
void MenuItem::drawGraphic(){ //using menuGraphic and internal x & y vals
  menuGraphic.draw(x,y);
  
  for(auto item: items){ //for item in menu item list
    if(item->menuOpen && item->typeList){ //if menu is open
      item->drawGraphic(); //draw graphic
    }
  }
}

void MenuItem::detectMouse(){
  bool doRender = false;
  
  int itemH = menuGraphic.getHeight()/items.size();
  for(int i=0;i<items.size();i++){
    auto item = items[i];
    //If mouse is inside current items[i] menu item
    if(util.mouseInRect(x, y+itemH*i, menuGraphic.getWidth(), itemH)){
      if(!item->highlighted){ doRender = true; }
      
      item->mouseOver = true;
      item->highlighted = true;
      for(int j=0;j<items.size();j++){
        if(i != j){
          items[j]->closeMenu();
        }
      }
    }else{
      item->mouseOver = false;
      if(!item->menuOpen){
        if(item->highlighted){ doRender = true; }
        item->highlighted = false;
      }
    }
    
    //if highlighted and has a menu then open menu
    if(item->highlighted && item->typeList){
      item->menuOpen = true;
    }
    
    if(doRender){ renderGraphic(); }
    if(item->typeList && item->menuOpen){
      item->detectMouse();
    }
  }
}
bool MenuItem::mouseIn(){
  if(mouseOver){
    return true;
  }else if(typeList){
    for(auto mitem : items){
      if(mitem->mouseIn()){
        return true;
      }
    }
  }
  return false;
}
void MenuItem::closeMenu(){
  menuOpen = false;
  highlighted = false;
  for(auto item : items){
    item->closeMenu();
  }
}
void MenuItem::itemClicked(){
  if(mouseOver && !typeList){
    if(typeInteract == 1){
      *callback = !(*callback); //init callback function
    }
    if(typeInteract == 2){
      if(*num < max){
        (*num)++;
      }else{
        (*num) = min;
      }
    }
  }else if(typeList && highlighted){ //else check for open menu
    for(auto item: items){
      item->itemClicked();
    }
  }
}

void Menu::setup(MenuItem* menuItems){
  list = menuItems;
  list->x = x;
  list->y = y;
  list->renderGraphic();
}
void Menu::update(){
  if(!util.guiShown){return;}
  list->detectMouse();
}
void Menu::draw(){
  if(drawMenu){
    ofSetColor(255);
    list->drawGraphic();
  }
}
void Menu::mousePressed(int x, int y,int button){
  if(!util.guiShown){return;}
  
  if(util.mouseInRect(0,0,50,50)){
    drawMenu = !drawMenu;
    cout << "[Menu] Toggled : " <<drawMenu <<endl;
    list->closeMenu(); //close menus
  }else if(!list->mouseIn()){
    drawMenu = false;
    list->closeMenu(); //close menus
  }
  
  //if(!drawMenu){return;}
  for(auto item : list->items){
    item->itemClicked();
    list->renderGraphic();
  }
}
