/*
Bryan Litzsinger
*/

#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<fstream>
#include<sstream>

using namespace std;

#include"2c.h"
#include"2c.hpp"

int main(int argc, char* argv[]){
  string configfilename = "default.cfg";
  if(argc == 2)
    configfilename = argv[1];
  ifstream config;
  config.open ( configfilename.c_str ( ) );
  string in;
  getline(config, in);
  int width = atoi(in.c_str());
  getline(config, in);
  int height = atoi(in.c_str());
  getline(config, in);
  int density = atoi(in.c_str());
  getline(config, in);
  int r;
  if(in.compare("random") == 0){
    r = time(NULL);
    srand(r);
  }else{
    r=atoi(in.c_str());
    srand(r);
  }
  
  bool termcriteria;
  getline(config, in);
  if(in.compare("evaluations") == 0){
    termcriteria = false;
  }else{
    termcriteria = true;
  }
  
  getline(config, in);
  int runs = atoi(in.c_str());
  getline(config, in);
  int evals = atoi(in.c_str());
  getline(config, in);
  string log = in;
  getline(config, in);
  string highscore = in;
  getline(config, in);
  string pacsolution = in;
  getline(config, in);
  string ghostsolution = in;
  getline(config, in);
  int mu = atoi(in.c_str());
  getline(config, in);
  int lambda = atoi(in.c_str());
  getline(config, in);
  float parsimony = atof(in.c_str());
  getline(config, in);
  int gmu = atoi(in.c_str());
  getline(config, in);
  int glambda = atoi(in.c_str());
  getline(config, in);
  float gparsimony = atof(in.c_str());
  
  bool parentsel;
  getline(config, in);
  if(in.compare("FPS") == 0){
    parentsel = true;
  }else{
    parentsel = false;
  }
  bool survivalsel;
  int k;
  getline(config, in);
  if(in.compare("Truncation") == 0){
    survivalsel = true;
  }else{
    survivalsel = false;
    getline(config, in);
    k = atoi(in.c_str());
  }
  bool gparentsel;
  getline(config, in);
  if(in.compare("FPS") == 0){
    gparentsel = true;
  }else{
    gparentsel = false;
  }
  bool gsurvivalsel;
  int gk;
  getline(config, in);
  if(in.compare("Truncation") == 0){
    gsurvivalsel = true;
  }else{
    gsurvivalsel = false;
    getline(config, in);
    gk = atoi(in.c_str());
  }
  int maxdepth = INITIALDEPTH;
  config.close();
  
  int localbest, globalbest;
  float avgfit;
  vector<node*> ghostcontrollers(gmu, new node());
  
  vector<node*> childrenghostcontrollers(glambda, new node());
  vector<int> populationfitness(mu, 0);
  vector<int> ghostpopfit(gmu, 0);
  vector<int> childrenfitness(lambda, 0);
  vector<int> childghostfit(glambda, 0);
  vector<int> pastavgfit;
  int topscore = 0;
  globalbest=0;
  int currentevals=0;
  vector<string> linebyline;
  int parentpool[lambda];
  int gparentpool[glambda];
  
  ofstream logf;
  ofstream highscoref;
  ofstream solf;
  ifstream transfer;
  logf.open(log.c_str(),ios::out);
  highscoref.open(highscore.c_str(),ios::out);
  solf.open(pacsolution.c_str(),ios::out);
  logf.close();
  highscoref.close();
  solf.close();
  solf.open(ghostsolution.c_str(),ios::out);
  solf.close();
  logf.open(log.c_str());
  
  
  logf<<"Width: "<<width<<", Height: "<<height<<", Density: "<<density<<endl;
  logf<<"Seed: "<<r<<endl;
  logf<<"Runs: "<<runs;
  if(termcriteria == false)
    logf<<" Evals: "<<evals<<endl;
  else
    logf<<" Generations to convergence: "<<evals<<endl;
  logf<<"Ms Pacman controller: Mu: "<<mu<<" Lambda: "<<lambda<<" Parsimony: "<<parsimony<<endl;
  logf<<"Ghosts controller: Mu: "<<gmu<<" Lambda: "<<glambda<<" Parsimony: "<<gparsimony<<endl;
  logf<<"Ms Pacman Parent Selection: ";
  if(parentsel == false)
    logf<<" Overselection "<<endl;
  else
    logf<<" FPS "<<endl;
  logf<<"Ms Pacman Survival Selection: ";
  if(survivalsel == false)
    logf<<" K-Tournament without Replacement with k = "<<k<<endl;
  else
    logf<<" Truncation "<<endl;
  logf<<"Ghost Parent Selection: ";
  if(parentsel == false)
    logf<<" Overselection "<<endl;
  else
    logf<<" FPS "<<endl;
  logf<<"Ghost Survival Selection: ";
  if(survivalsel == false)
    logf<<" K-Tournament without Replacement with k = "<<gk<<endl;
  else
    logf<<" Truncation "<<endl;
  logf<<"Result Log"<<endl;
  
  gameboard game;
  vector<gameboard> population(mu, gameboard());
  vector<gameboard> children(lambda, gameboard());
  for(int x=0;x<runs;x++){
  logf<<endl<<"Run "<<x+1<<endl;
    avgfit = 0;
    game.gameinit(width, height, density, parsimony);
     
    //initialize population from equal base state and evaluate
    for(int initializer=0;initializer<mu;initializer++){//init mpac controllers
      population[initializer] = game;
      population[initializer].controllerinit(maxdepth);
    }
    for(int initializer=0;initializer<gmu;initializer++){//init ghost controllers
     
      if(rand() % 2){//full method
        gmaxcontrollerinit(maxdepth, 0, ghostcontrollers[initializer]);
      }else{//grow method
        ggrowcontrollerinit(maxdepth, 0, ghostcontrollers[initializer]);
      }
      
    }
    int counter=0;
    for(int initializer=0;initializer<mu;initializer++){
      populationfitness[initializer] = (population[initializer].playthrough(linebyline, ghostcontrollers[initializer % gmu]));
      if(initializer<gmu)
        ghostpopfit[initializer] = ((MAXFIT - population[initializer].score) - (gparsimony * depthfinder(ghostcontrollers[initializer])));
      
      //keep track of global best playthrough
      if(globalbest < population[initializer].score){
        globalbest = population[initializer].score;
        
        highscoref.open(highscore.c_str());
        highscoref<<width<<endl<<height<<endl;
        for(int z=0;z<linebyline.size();z++)
          highscoref<<linebyline[z]<<endl;
        highscoref.close();
        solf.open(pacsolution.c_str());
        solf<<"Solution File:"<<endl;
        solf<<"World Dimensions: Height = "<<height<<", Width = "<<width<<", Density = "<<density<<endl;
        solf<<"Ms Pacman controller:"<<endl;
        controllerprint(population[initializer].controller, solf);
        solf.close();
        solf.open(ghostsolution.c_str());
        solf<<"Solution File:"<<endl;
        solf<<"World Dimensions: Height = "<<height<<", Width = "<<width<<", Density = "<<density<<endl;
        solf<<"Ghost Controller:"<<endl;
        gcontrollerprint(ghostcontrollers[initializer % gmu], solf);
        solf.close();
      }
      linebyline.clear();
    }
    currentevals=mu;
    fitSort(mu, populationfitness, population);
    fitSortg(gmu, ghostpopfit, ghostcontrollers);
    
    //keep track of avg fitness and best local fitness
    localbest=0;
    avgfit=0;
    for(int y=0;y<mu;y++){
      avgfit=avgfit+populationfitness[y];
      if(localbest<populationfitness[y])
        localbest=populationfitness[y];
    }
    avgfit = avgfit/mu;
    pastavgfit.push_back(avgfit);
    logf<<mu<<"\t"<<avgfit<<"\t"<<localbest<<endl;
    
   
   while(continuerunning(termcriteria, evals, pastavgfit, currentevals)){
   
   
      //Ms Pacman child generation
      //parent selection: FPS or Overselection
      if(parentsel){//FPS
        FPS(populationfitness, lambda, mu, parentpool);
      }else{//Overselection
        oversel(populationfitness, lambda, mu, parentpool);
      }
   
      for(int i=0;i<lambda;i++){
      
        //recombination or mutation
        if(rand() % 100 <= 10){//provides a 10% chance for mutation to occur
          mutation(game, population[parentpool[i]], children[i]);//mutation
        }else{//for recombination make two children, so i needs to incremented one extra time. and double check you arent going over lambda
          
          int rightindex, leftindex;
          leftindex=i;
          if(i+1 == lambda)
            rightindex = 0;
          else
            rightindex = i+1;
            
            
          if(i+1 < lambda)
            recombination(game, population[parentpool[leftindex]], population[parentpool[rightindex]], children[i], children[i+1]);//recombination
          else
            recombination(game, population[parentpool[leftindex]], population[parentpool[rightindex]], children[i]);
          i++;
          
            
        }
      }
      
      //Ghost child generation
      if(gparentsel){//FPS
        FPS(ghostpopfit, glambda, gmu, gparentpool);
      }else{//Overselection
        oversel(ghostpopfit, glambda, gmu, gparentpool);
      }
      for(int i=0;i<glambda;i++){
        //recombination or mutation
        if(rand() % 100 <= 10){//provides a 10% chance for mutation to occur
          mutationg(ghostcontrollers[gparentpool[i]], childrenghostcontrollers[i]);//mutation
        }else{//for recombination make two children, so i needs to incremented one extra time. and double check you arent going over lambda
          
          int grightindex, gleftindex;
          gleftindex=i;
          if(i+1 == glambda)
            grightindex = 0;
          else
            grightindex = i+1;
            
          if(i+1 < glambda)
            recombinationg(ghostcontrollers[gparentpool[gleftindex]], ghostcontrollers[gparentpool[grightindex]], childrenghostcontrollers[i], childrenghostcontrollers[i+1]);//recombination
          else
            recombinationg(ghostcontrollers[gparentpool[grightindex]], ghostcontrollers[gparentpool[gleftindex]], childrenghostcontrollers[i]);
          i++;
          
            
        }
      }
      //evaluate the children
      for(int i=0;i<lambda;i++){
        childrenfitness[i]=(children[i].playthrough(linebyline, childrenghostcontrollers[i % glambda]));
        if(i<glambda)
          childghostfit[i]=((MAXFIT - children[i].score) - (gparsimony * depthfinder(childrenghostcontrollers[i])));
        
        //keep track of global best playthrough
        if(globalbest < children[i].score){
          globalbest = children[i].score;
          
          highscoref.open(highscore.c_str());
          highscoref<<width<<endl<<height<<endl;
          for(int z=0;z<linebyline.size();z++)
            highscoref<<linebyline[z]<<endl;
          highscoref.close();
          solf.open(pacsolution.c_str());
          solf<<"Solution File:"<<endl;
          solf<<"World Dimensions: Height = "<<height<<", Width = "<<width<<", Density = "<<density<<endl;
          solf<<"Ms Pacman Controller:"<<endl;          
          controllerprint(children[i].controller, solf);
          solf.close();
          solf.open(ghostsolution.c_str());
          solf<<"Solution File:"<<endl;
          solf<<"World Dimensions: Height = "<<height<<", Width = "<<width<<", Density = "<<density<<endl;
          solf<<"Ghost Controller:"<<endl;
          gcontrollerprint(childrenghostcontrollers[i % gmu], solf);
          solf.close();
        }
        linebyline.clear();
      }

      //survival selection
      if(survivalsel){//mpac survival
        trunc(population, children, mu, lambda, populationfitness, childrenfitness);
      }else{
        SurvivalKTourn(k, populationfitness, lambda, mu, population, children, childrenfitness);
      }
      fitSort(mu, populationfitness, population);
      
      if(gsurvivalsel){//ghost survival
        truncg(ghostcontrollers, childrenghostcontrollers, gmu, glambda, ghostpopfit, childghostfit);
      }else{
        SurvivalKTourng(gk, ghostpopfit, glambda, gmu, ghostcontrollers, childrenghostcontrollers, childghostfit);
      }
      fitSortg(gmu, ghostpopfit, ghostcontrollers);

      
      localbest=0;
      avgfit=0;
      for(int y=0;y<mu;y++){
      avgfit=avgfit+populationfitness[y];
      if(localbest<populationfitness[y])
        localbest=populationfitness[y];
      }
      avgfit = avgfit/mu;
      logf<<currentevals+lambda<<"\t"<<avgfit<<"\t"<<localbest<<endl;
      currentevals=currentevals+lambda;
      pastavgfit.push_back(avgfit);
    }//end of evals loop
  }//end of run loop
  
  logf.close();
  
  return 0;
}

