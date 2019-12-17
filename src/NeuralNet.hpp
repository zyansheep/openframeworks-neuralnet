//
//  NeuralNet.hpp
//  NeuralNetwork
//
//  Created by Zyan on 9/12/18.
//
//

#ifndef NeuralNet_hpp
#define NeuralNet_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofThread.h"
#include "utl.h"
using namespace std;

class NeuralNet{
public:
  NeuralNet(const NeuralNet& other);
  NeuralNet(){};
  
  void setup(vector<int> neural_struct, bool totalReset = true);
  void setup(vector<vector<string>> neural_struct, bool totalReset=true);
  void setup(string filepath, bool totalReset=true);
  void genNeurons(vector<vector<string>> neural_struct);
  void render();
  ofFbo rendering;
  int biggestLayer;
  
  //add data to built-in data sets (default training)
  void addDataSet(vector<double> input, vector<double> output, bool training = true);
  void clearData();
  
  //used to train network
  vector<vector<double>> trainX; //training inputs
  vector<vector<double>> trainY; //training outputs
  
  //used to see how good network is for unknown inputs
  vector<vector<double>> testX; //testing inputs
  vector<vector<double>> testY; //testing output
  
  //import and export training to string and training/testing
  //default type = training (true) or testing (false)
  void importData(string data, bool type = true);
  string exportData(bool type = true);
  //import and export network to .bin file
  bool importNetwork(string filepath);
  void exportNetwork(string filepath);
  void refreshNeurons();
  
  vector<double> curInput;
  vector<double> curOutput;
  
  void setInput(vector<double>);
  void forward();
  vector<double> getOutput();
  
  //runs training or testing data sets (built-in)
  void train();
  void test();
  //run data set (inputs and outputs) through network with option to train network
  void runDataSet(vector<vector<double>> &inputX, vector<vector<double>> &inputY, bool doBackprop=false);
  double cost = 0;
  double netCost = 0;
  //calculate cost for testing data or training data
  double calcNetCost() { netCost = calcCost(testX, testY); return netCost; }
  double calcCost(){ cost = calcCost(trainX,trainY); return cost; }
  //calculates cost for data set (default is built-in training dataset)
  double calcCost(vector<vector<double>> &inputX, vector<vector<double>> &inputY);
  void writeNudges(unsigned long layer, vector<double> Nnudges);
  void backprop(vector<double> y);
  void nudgeNet();
  
  static vector<double> randArr(unsigned long len, double mini, double maxi);
  static vector<double> sumArr(const vector<double> &rr1, const vector<double> &arr2);
  static vector<double> minusArr(const vector<double> &arr1, const vector<double> &arr2);
  static vector<double> divArr(const vector<double> & arr, double num);
  static vector<double> multArr(const vector<double> & arr, double num);
  
  static double roundPlace(double num, double placeval);
  static double squish(double x);
  static double squishprime(double x);
    
    
    
  class neuron{
  public:
    vector<double> A; //activations in prev layer
    vector<double> W; //weights applied to input activations
    double B=0; //bias
    double Z=0; //activations*weights + bias
    
    double V=0; //value of neuron
    
    //-1 is input neuron
    //0 is hidden neuron
    //1 is output neuron
    int type = 0;
    
    string inputString;
    vector<shared_ptr<neuron>> inputs; //input layer
    //nudge to val of neuron
    //used to calculate nudge to weights
    double Vnudge = 0;
    
    //used to calculate nudges
    vector<vector<double>> Wnudges;
    vector<double> Bnudges;
    
    void setup(double input){
      //just recieve inputs to neuron
      V = input;
      type = -1;
    }

    void setup(string _inputString, vector<shared_ptr<neuron>> inputLayer){
      //reg neuron recieves access to neurons from prev layer
      inputString = _inputString;
      //set weights randomly from -0.5 to 0.5
      genInputs(inputLayer);
      W = randArr(inputs.size(), -0.5,0.5);
      //A.reserve(W.size());
    }
    void genInputs(vector<shared_ptr<neuron>> inputLayer){
      if(type == -1){return; cout<<"InputNeuron\n"; }
      if(inputString.empty()){ return; cout<<"No String\n"; }
      if(inputLayer.size() == 0){ return; cout<<"no input\n"; }
      
      if(inputString == "*"){
        inputs = inputLayer;
      }else{
        inputs.clear();
        vector<string> strVec = util.cutString(inputString, ',');
        inputs.reserve(strVec.size());
        for(string &s : strVec){
          //add index to inputs
          int npos = stoi(s);
          if(0 <= npos && npos < inputLayer.size()){
            inputs.push_back(inputLayer[npos]);
          }else{
            cout<<"String out of bounds\n";
          }
          
        }
      }
    }
    
    /***Rendering***/
    ofPoint pos;
    int cr = 50; //radius of neuron
    void render(){
      //draw neuron with value and weights - colorcoded
      //draw weight lines
      for(int n=0;n<inputs.size();n++){
        //set alpha to weights
        //red is negative, blue is positive
        int wAlpha = ofMap(W[n], -2,2, -200,200, true);
        if(wAlpha > 0){
          ofSetColor(0,0,255,abs(wAlpha));
        }else{
          ofSetColor(255,0,0,abs(wAlpha));
        }
        double lWidth = min(W[n]/2, 10.0);
        
        glLineWidth(lWidth);
        
        ofPoint& ip = inputs[n]->pos;
        ofPolyline line;
        line.addVertex(pos.x,pos.y);
        line.addVertex(ip.x,ip.y);
        line.draw();
        
        //get rotate angle
        float angle = atan2(pos.y-ip.y, pos.x-ip.x);
        //pop matrix and transform + rotate
        ofPushMatrix();
        ofTranslate(line.getPointAtIndexInterpolated(0.5));
        ofRotateZRad(angle);
        //draw centered string
        util.drawStringAtPoint(util.font, ofToString(W[n]), 0,-lWidth+1);
        ofPopMatrix();
      }
      
      glLineWidth(1);
      //draw circle
      ofFill();
      ofSetColor(ofMap(V, 0,1, 0,255));
      ofDrawCircle(pos.x,pos.y,cr);
      ofNoFill();
      ofSetColor(0);
      ofDrawCircle(pos.x,pos.y, cr);
      
      ofSetColor(255);
      if(type != -1){
        //draw value
        util.drawStringAtPoint(util.font, ofToString(V), pos.x,pos.y);
        //draw bias with same color scheme as weights
        int bAlpha = ofMap(B, -2,2, -200,200, true);
        if(bAlpha > 0){
          ofSetColor(0,0,255,abs(bAlpha));
        }else{
          ofSetColor(255,0,0,abs(bAlpha));
        }
        util.drawStringAtPoint(util.font, ofToString(B), pos.x,pos.y+20);
      }
    }
    
    /***Training***/
    //takes nudge to neuron and applies its nudges
    //opinion to input neurons of previous layer
    void backprop(){
      writeNudge(Vnudge);
      //use Vnudge to calculate nudges to input neurons
      //see 3Blue1Brown for equation
      vector<double> nudgeArr = multArr(W, squishprime(Z) * Vnudge);
      for(int n=0;n<inputs.size();n++){
        inputs[n]->Vnudge += nudgeArr[n];
      }
      Vnudge = 0; //erase Vnudge for next training cycle
    }
    void writeNudge(double nudge){
      if(type == -1){ return; }
      //use nudge calculated by next layer's neurons or by cost func
      //derivative
      //to calculate nudge to weights and biases
      vector<double> Wnudge = multArr(A, squishprime(Z) * nudge);
      double Bnudge = squishprime(Z) * nudge;
      Wnudges.push_back(Wnudge);
      Bnudges.push_back(Bnudge);
    }
    void nudge(){
      if(Bnudges.size() == 0){return;}
      //Collapse Wnudges and Bnudges arrays - then apply nudges
      double Bnudge = 0;
      for(int i=0;i<Bnudges.size();i++){
        Bnudge += Bnudges[i];
      }
      Bnudge /= Bnudges.size();
      
      vector<double> Wnudge;
      Wnudge.reserve(W.size());
      for(int n=0;n<W.size();n++){
        double curWsum = 0;
        for(int ti=0;ti<Wnudges.size();ti++){
          curWsum += Wnudges[ti][n];
        }
        curWsum /= Wnudges.size();
        Wnudge.push_back(curWsum);
      }
      
      //how much to apply the gradient before calculating new gradient
      int learningRate = 1;
      Wnudge = multArr(Wnudge, learningRate);
      Bnudge *= learningRate;
      
      //apply nudge
      W = minusArr(W, Wnudge);
      B = B - Bnudge;
      Bnudges.clear();
      Wnudges.clear();
    }
    
    double calc(){
      //set A vector to activations of previous layer
      A.clear();
      std::vector<double>(inputs.size()).swap(A);
      for(int i=0;i<inputs.size();i++){ //get activations
        A[i] = inputs[i]->V;
      }
      
      Z=0;
      for(int i=0; i<W.size(); i++){
        //multiply weights by input activations
        //add to sum
        Z += (A[i] * W[i]);
      }
      //apply bias of neuron
      Z += B;
      //apply squishification function
      V = squish(Z);
      //return neuron value
      return V;
    }
    
  };
  
  //maybe just turn this into
  //neuron* neurons
  vector<vector<shared_ptr<neuron>>> neurons;
};

class TrainingThread : public ofThread {
public:
  //train on network copy
  ofThreadChannel<NeuralNet> input;
  ofThreadChannel<NeuralNet> output;
  
  TrainingThread(){
    startThread();
  }
  ~TrainingThread(){
    input.close();
    output.close();
    waitForThread(true);
  }
  
  bool isReady = true; //is thread ready for another training cycle
  bool getNet(NeuralNet &net){
    if( output.tryReceive(net) ){
      //cout << "Found network"<<endl;
      isReady = true;
      return true;
    }
    return false;
  }
  
  void threadedFunction(){
    NeuralNet net;
    while (input.receive(net)){
      isReady = false;
      net.train();
      net.nudgeNet();
      net.calcCost();
      output.send( move(net) );
    }
  }
};

#endif /* NeuralNet_hpp */
