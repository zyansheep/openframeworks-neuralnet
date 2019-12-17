#include "ofApp.h"

void ofApp::setup(){
  util.load();
  setupNet();
  
  mItems.push_back(new MenuItem("Import Training", &importTrainingTrigger));
  mItems.push_back(new MenuItem("Export Training", &exportTrainingTrigger));
  menu.setup(new MenuItem("Menu", mItems));
  //init training here (if needed)
}
void ofApp::update(){
  menu.update();
  if(importTrainingTrigger){
    importTrainingTrigger = false;
    ofFileDialogResult result = ofSystemLoadDialog("Select a Training File");
    
    //Check if the user opened a file
    if (result.bSuccess){
      cout << "Imported Training File From: "<<result.getPath() << endl;
      ofBuffer file = ofBufferFromFile(result.getPath());
      net.importData(file.getText());
    }else {
      cout << "User Canceled Import"<<endl;
    }
  }
  if(exportTrainingTrigger){ //auto save to home dir
    exportTrainingTrigger = false;
    string saveDir = ofFilePath::getUserHomeDir();
    saveDir += "/Training.txt";
    cout << "Saving Training File to: "<<saveDir<<endl;
    ofBuffer file;
    file.set(net.exportData());
    ofBufferToFile(saveDir, file);
  }
  
  if(isTraining){
    for(int i=0;i<100;i++){
      net.train();
      net.nudgeNet();
    }
    net.calcCost();
    net.render();
  }
}
void ofApp::draw(){
  ofSetColor(255);
  if(editTraining){
    ofSetColor(200);
  }
  net.rendering.draw(0,0);
  for(int i=0;i<net.neurons[net.neurons.size()-1].size();i++){
    auto n = net.neurons[net.neurons.size()-1][i];
    ofSetColor(ofMap(yToBe[i], 0,1, 0,255, true));
    ofDrawCircle(n->pos.x+n->cr*2, n->pos.y,n->cr);
  }
  ofSetColor(0);
  if(isTraining){
    util.drawStringAtPoint(util.font, "Is Training", ofGetWidth()/2, 50);
  }
  for(int i=0;i<net.trainX.size();i++){
    util.font.drawString(ofToString(net.trainX[i]), 5, 20*(i+1) );
    
    string yTrainStr = ofToString(net.trainY[i]);
    auto strWid = util.font.stringWidth(yTrainStr) + 5;
    util.font.drawString(yTrainStr, ofGetWidth()-strWid, 20*(i+1) );
  }
  ofSetColor(255);
  menu.draw();
}
void ofApp::setupNet(){
  net.setup(nstruct);
  
  /*//init train as zero in and zero out
  vector<double> szero(net.neurons[0].size(), 0.0);
  vector<double> ezero(net.neurons.back().size(), 0.0);
  net.trainX.push_back(szero);
  net.trainY.push_back(ezero);*/
  
  //get inputs and outputs
  //net.setInput(net.trainX[tIndex]);
  net.forward();
  net.render();
  yToBe = vector<double>( net.neurons.back().size(), 0.0 );
}
void ofApp::keyPressed(int key){
  if(!editTraining){
    if(isdigit(key)){
      int ti = key - '0';
      if(ti < net.trainX.size()){
        tIndex = ti;
        net.setInput(net.trainX[tIndex]);
        net.forward();
        yToBe = net.trainY[tIndex];
      }
    }
    switch (key) {
      case 't':
        net.train();
        break;
      case 'n':
        net.nudgeNet();
        net.forward();
        break;
      case 'r':
        cout << "Reloading Neural Net" << endl;
        setupNet();
        break;
      case 'c':
        cout << "Resetting Ai Training" << endl;
        net.setup(nstruct, false);
        break;
      case 's':
        isTraining = !isTraining;
        break;
      case 'p':
        cout << net.exportData() << endl;
        break;
      //Import and Export
      case '=':
        importTrainingTrigger = true;
        break;
      case '-':
        exportTrainingTrigger = true;
        break;
      default:
        break;
    }
    //set net.curInput and yToBe + forward current trainIndex
    net.forward();
  }else{
    if(key == 'a'){
      editTraining = false;
      //check if train input items to append
      //are already there
      cout << "To Add: " << ofToString(net.curInput) << " : "<< ofToString(yToBe)<<endl;
      for(auto &item : net.trainX){
        if(item == net.curInput){
          cout << "Failed to Add Training Set: Input Set already has outputs"<<endl;
          yToBe = net.trainY[tIndex];
          return;
        }
      }
      net.addDataSet(net.curInput, yToBe);
    }
  }
  net.calcCost();
  net.render();
}
void ofApp::mousePressed(int x, int y, int button){
  util.mousePressed(x,y,button);
  menu.mousePressed(x,y,button);
}
void ofApp::mouseReleased(int x, int y, int button){
  util.mouseReleased(x,y,button);
  //change inputs based on mouse clicking neuron
  bool didChangeInput = false;
  for(int i=0;i<net.neurons[0].size();i++){
    auto n = net.neurons[0][i];
    if(ofDist(x, y, n->pos.x, n->pos.y) <= n->cr){
      if(n->V == 1){
        n->V = 0;
      }else{n->V = 1;}
      net.forward();
      net.render();
      didChangeInput = true;
    }
    net.curInput[i] = n->V;
  }
  //look for matching outputs for input
  if(didChangeInput){
    yToBe.clear();
    yToBe.resize(nstruct.back().size());
    for(int ti=0;ti<net.trainX.size();ti++){
      if(net.curInput == net.trainX[ti]){
        yToBe = net.trainY[ti];
        //if found match cancel training edit
        editTraining = false;
        break;
      }
    }
  }
  
  for(int i=0;i<net.neurons[net.neurons.size()-1].size();i++){
    auto n = net.neurons[net.neurons.size()-1][i];
    if(ofDist(x, y, n->pos.x+2*n->cr, n->pos.y) <= n->cr){
      editTraining = true;
      if(yToBe[i]>0.5){
        yToBe[i] = 0;
      }else{
        yToBe[i] = 1;
      }
    }
  }
}
void ofApp::windowResized(int w, int h){
  util.windowResized(w,h);
  net.render();
}
