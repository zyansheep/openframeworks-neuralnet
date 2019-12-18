//
//  NeuralNet.cpp
//  NeuralNetwork
//
//  Created by Zyan on 9/12/18.
//
//

#include "NeuralNet.hpp"
/*** Setup and Forwarding ***/
NeuralNet::NeuralNet(const NeuralNet& other){
  rendering = other.rendering;
  trainX = other.trainX;
  trainY = other.trainY;
  testX = other.testX;
  testY = other.testY;
  curInput = other.curInput;
  curOutput = other.curOutput;
  cost = other.cost;
  netCost = other.netCost;
  neurons = other.neurons;
  
  refreshNeurons(); //re-assign neuron inputs to the new adresses
}

void NeuralNet::setup(vector<int> neural_struct, bool totalReset){
  vector<vector<string>> output;
  output.reserve(neural_struct.size());
  for(int &num:neural_struct){
    vector<string> layer(num, "*");
    output.push_back(layer);
  }
  setup(output, totalReset);
}
void NeuralNet::setup(vector<vector<string>> neural_struct, bool totalReset){
  genNeurons(neural_struct);
  
  if(totalReset){
    clearData();
  }
}
void NeuralNet::setup(string filepath, bool totalReset){
  cout << "Loading Network from file"<<endl;
  importNetwork(filepath);
  
  if(totalReset){
    clearData();
  }
}
void NeuralNet::clearData(){
  trainX.clear();
  trainY.clear();
  testX.clear();
  testY.clear();
  //set network inputs to 0
  vector<double> izero( neurons[0].size(), 0.0 );
  setInput(izero);
}
void NeuralNet::genNeurons(vector<vector<string>> neural_struct){
  neurons.clear(); //clear everything for new structure
  neurons.reserve(neural_struct.size());
  for(int l=0;l<neural_struct.size();l++){
    vector<shared_ptr<neuron>> layer;
    for(int n=0;n<neural_struct[l].size();n++){
      //push_back default training
      //setup neuron
      layer.emplace_back(new neuron);
      if(l==0){ //setup input neurons
        layer[n]->setup(0.0);
      }else{
        layer[n]->setup(neural_struct[l][n], neurons[l-1]);
      }
    }
    if(layer.size() > biggestLayer){ biggestLayer = layer.size(); }
    //add layer to neurons
    neurons.push_back(layer);
  }
}
void NeuralNet::render(){
  rendering.allocate(ofGetWidth(), ofGetHeight());
  rendering.begin();
  ofClear(0);
  ofBackground(200);
  float horzInt = ofGetWidth()/( neurons.size()+1 );
  float size = util.smWindowDim/( biggestLayer+1 )/2;
  for(int l=0;l<neurons.size();l++){
    float vertInt = ofGetHeight()/( neurons[l].size()+1 );
    for(int n=0;n<neurons[l].size();n++){
      float xpos = horzInt*(l+1); //xpos is through layers
      float ypos = vertInt*(n+1); //ypos is neurons in layer
      shared_ptr<neuron> neur = neurons[l][n];
      neur->pos.x = xpos;
      neur->pos.y = ypos;
      neur->cr = size;
      neur->render();
    }
  }
  ofSetColor(0);
  string costStr = ofToString(cost);
  if(cost < 0.00001){ costStr = "Trained!"; }
  util.drawStringAtPoint(util.font, costStr, ofGetWidth()/2, 10);
  rendering.end();
}
void NeuralNet::setInput(vector<double> input){
  curInput = input;
  for(int i=0;i<neurons[0].size();i++){
    neurons[0][i]->V = input[i];
  }
}
void NeuralNet::forward(){
  curOutput.clear();
  curOutput.reserve(neurons.back().size());
  for(int l=1; l<neurons.size(); l++){
    for(int n=0; n<neurons[l].size(); n++){
      //calculate
      neurons[l][n]->calc();
      //if last layer
      if(l == neurons.size()-1){
        //record in output;
        curOutput.push_back( neurons[l][n]->V );
      }
    }
  }
}
vector<double> NeuralNet::getOutput(){
  return curOutput;
}
void NeuralNet::addDataSet(vector<double> input, vector<double> output, bool training){
  //if correct sizes
  if(input.size() == neurons[0].size() && output.size() == neurons[neurons.size()-1].size()){
    if(training){
      trainX.push_back(input);
      trainY.push_back(output);
    }else{
      testX.push_back(input);
      testY.push_back(output);
    }
  }
}
void NeuralNet::importData(string data, bool type){
  auto * inputX = &trainX;
  auto * inputY = &trainY;
  if(!type){
    cout << "[Import] Test Data"<<endl;
    inputX = &testX;
    inputY = &testY;
  }else{
    cout << "[Import] Train Data"<<endl;
  }
  
  //take input string and add training to network
  //There are no checksafes, so errors may occur if upload bad format
  vector<string> strVec = util.cutString(data, '\n');
  for(auto str: strVec){
    vector<string> tItem = util.cutString(str, ':');
    vector<double> tArr; //input array
    for(string tStr : util.cutString(tItem[0], ',')){
      tArr.push_back( stod(tStr) );
    }
    inputX->push_back(tArr);
    tArr.clear();
    for(string tStr : util.cutString(tItem[1], ',')){
      tArr.push_back( stod(tStr) );
    }
    inputY->push_back(tArr);
  }
}
string NeuralNet::exportData(bool type){
  auto * inputX = &trainX;
  auto * inputY = &trainY;
  if(!type){
    cout << "[Export] Test Data"<<endl;
    inputX = &testX;
    inputY = &testY;
  }else{
    cout << "[Export] Training Data"<<endl;
  }
  
  string output;
  for(int i=0;i<inputX->size();i++){
    for(double digit : (*inputX)[i]){
      output += ofToString(digit);
      output += ',';
    }
    output += ":";
    for(double digit: (*inputY)[i]){
      output += ofToString(digit);
      output += ',';
    }
    output += "\n";
  }
  return output;
}

bool NeuralNet::importNetwork(string filepath){
  //returns true if successful at importing
  ifstream file;
  file.open(filepath, ios::binary | ios::in);
  
  unsigned long pos = 0;
  //read len bytes from file and return char array + increment reader
  auto getRead = [&](auto &x){
    file.read(reinterpret_cast<char*>(&x), sizeof(x));
  };
  
  int layer = 0;
  int index = 0;
  
  vector<shared_ptr<neuron>> curLayer;
  while (!file.eof()) {
    //read from file
    int l; getRead(l);
    int n; getRead(n);
    
    //get input string
    vector<char> cvec;
    char curChar; getRead(curChar);
    while(curChar != ';'){
      cvec.push_back(curChar);
      getRead(curChar);
    }
    string str(cvec.begin(), cvec.end());
    
    unsigned long nlen; getRead(nlen);
    
    vector<double> weights(nlen);
    for(int i=0;i<weights.size();i++){
      getRead(weights[i]);
    }
    double bias; getRead(bias);
    
    //set neuron
    index = n;
    //if first time setup input layer using
    if(layer == 0 && index==0){
      vector<shared_ptr<neuron>> inputLayer;
      inputLayer.reserve(weights.size());
      for(int i=0;i<weights.size();i++){
        inputLayer.emplace_back(new neuron);
        inputLayer[i]->setup(0.0);
      }
      neurons.push_back(inputLayer);
      layer = l;
      //cout << "Loaded Input Neurons" << endl;;
    }
    
    //if new layer, add current layer
    if(l > layer){
      neurons.push_back(curLayer);
      layer = l;
      //cout << "Loaded Layer with size: "<<curLayer.size()<<endl;
      curLayer.clear();
    }
    if(file.eof()){
      break;
    }
    
    //setup this neuron
    curLayer.emplace_back(new neuron());
    curLayer[index]->setup(str, neurons[layer-1]);
    
    //setup weights and biases
    curLayer[index]->W = weights;
    curLayer[index]->B = bias;
  }
  //if last layer - just push back
  neurons.push_back(curLayer);
  cout << "Loaded Last Layer with size: "<<curLayer.size()<<endl;
}
void NeuralNet::exportNetwork(string filepath){
  ofstream f;
  f.open(filepath, ios::binary | ios::trunc);
  f.clear();
  
  //write neurons to file
  for(int l=1;l<neurons.size();l++){
    for(int n=0;n<neurons[l].size();n++){
      //write binary position of neuron
      //add string ending
      string str = neurons[l][n]->inputString + ';';
      unsigned long len = neurons[l][n]->W.size();
      
      //write neuron position
      f.write((char*)&l, sizeof(l));
      f.write((char*)&n, sizeof(n));
      f << str.c_str();
      f.write((char*)&len, sizeof(len));
      //write binary inputString and weight size
      
      for(double weight : neurons[l][n]->W){
        f.write((char*)&weight, sizeof(weight));
      }
      double bias = neurons[l][n]->B;
      f.write((char*)&bias, sizeof(bias));
    }
  }
  f.close();
  cout << "Successfully exported network" <<endl;
}
void NeuralNet::refreshNeurons(){
  //This function copys the objects pointed to by the neuron vector
  //and makes a new vector pointing to different objects
  //Copy vector
  vector<vector<shared_ptr<neuron>>> prevN = neurons;
  //clear old
  neurons.clear();
  for(auto l:prevN){
    vector<shared_ptr<neuron>> layer;
    for(int n=0;n<l.size();n++){
      layer.emplace_back( new neuron(*l[n]) );
      //if not input neuron
      if(layer.back()->type != -1){
        //set inputs to prev layer
        layer.back()->genInputs(neurons.back());
      }
    }
    neurons.push_back(layer);
  }
}

/*** Training ***/
void NeuralNet::train(){
  runDataSet(trainX,trainY, true);
}
void NeuralNet::test(){
  runDataSet(testX, testY);
}
void NeuralNet::runDataSet(vector<vector<double>> &inputX, vector<vector<double>> &inputY, bool doBackprop){
  if(inputX.size()==0){ return; }
  
  for(int i=0;i<inputX.size();i++){
    setInput(inputX[i]);
    forward();
    if(doBackprop){
      backprop(inputY[i]);
    }
  }
}
double NeuralNet::calcCost(vector<vector<double>> &inputX, vector<vector<double>> &inputY){
  if(inputX.size() == 0){ return -1; }
  
  double output = 0;
  //for input size
  for(int ti=0; ti<inputX.size(); ti++){
    //get neural output for inputX
    setInput(inputX[ti]);
    forward();
    for(int n=0; n<inputY[ti].size(); n++){
      //in perfect network nthOut is 0 for each output
      //cost function is (yhat - y)^2
      double nthOut = pow(curOutput[n] - inputY[ti][n], 2);
      output += nthOut;
    }
  }
  return output;
}
void NeuralNet::nudgeNet(){
  //apply training to Weights & Biases of network
  
  for(auto layer:neurons){
    for(auto n:layer){
      n->nudge();
    }
  }
}
void NeuralNet::backprop(vector<double> y){
  //This function calculates nudges to the value of each neuron in network
  //for one training set by comparing the 
  
  //for each layer (before last layer)
  for(int l=neurons.size()-1; l>=1; l-=1){//loop backwards through layers
    //if last layer
    if(l == neurons.size()-1){
      //get nudges to neurons for last layer
      //this is derivative cost function derivative
      //yhat is activations of last layer
      //(yhat-y)^2 -> 2(yhat-h)
      for(int i=0;i<neurons[l].size();i++){
        double n = 2 * (neurons[l][i]->V - y[i]);
        neurons[l][i]->Vnudge = n;
      }
    }
    for(int i=0;i<neurons[l].size(); i++){
      //run backprop func which calcs nudge to input
      //neurons to the neuron it is called from
      neurons[l][i]->backprop();
    }
  }
}

/*** Array Manipulation ***/
vector<double> NeuralNet::randArr(unsigned long len, double mini, double maxi){
  vector<double> output;
  output.reserve(len);
  for(int a=0;a<len;a++){
    //gen random number from mini to maxi
    float r = mini + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxi-mini)));
    output.push_back(r);
  }
  return output;
}
vector<double> NeuralNet::sumArr(const vector<double> &arr1, const vector<double> &arr2){
  unsigned long len = (arr1.size() <= arr2.size())? arr2.size() : arr1.size();
  vector<double> output;
  output.reserve(len);
  for(unsigned long a=0; a<len; a++){
    output.emplace_back(arr1[a] + arr2[a]);
  }
  return output;
}
vector<double> NeuralNet::minusArr(const vector<double> &arr1, const vector<double> &arr2){
  unsigned long len = (arr1.size() <= arr2.size())? arr2.size() : arr1.size();
  vector<double> output;
  output.reserve(len);
  for(unsigned long a=0; a<len; a++){
    output.push_back(arr1[a] - arr2[a]);
  }
  return output;
}
vector<double> NeuralNet::divArr(const vector<double> &arr, double num){
  vector<double> output;
  output.reserve(arr.size());
  for(int a=0; a<arr.size(); a++){
    output.push_back(arr[a] / num);
  }
  return output;
}
vector<double> NeuralNet::multArr(const vector<double> &arr, double num){
  vector<double> output;
  output.reserve(arr.size());
  for(int a=0; a<arr.size(); a++){
    output.emplace_back(arr[a] * num);
  }
  return output;
}

/*** Maths ***/
double NeuralNet::roundPlace(double num, double placeval){
  return round(num*placeval)/placeval;
}
double NeuralNet::squish(double x){
  //return (x>0)?x:0; //ReLU
  return 1 / (1+exp(-x)); //sigmoid
}
double NeuralNet::squishprime(double x){
  //return (x>0)?1:0; //ReLU
  
  //sigmoid
  double expnum = exp(-x);
  return expnum/pow(1+expnum, 2);
}
