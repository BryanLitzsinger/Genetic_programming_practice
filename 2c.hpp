

float node::eval(gameboard *game){
    if(operation == 0){//dist to nearest pill
      float shortestdist = 1000;
      for(int x = 0; x < game->pills.size(); x++){
        if((abs(game->mpac.x - game->pills[x].x) + abs(game->mpac.y - game->pills[x].y)) < shortestdist){
          shortestdist = (abs(game->mpac.x - game->pills[x].x) + abs(game->mpac.y - game->pills[x].y));
        }
      }
      return shortestdist;
    }
    if(operation == 1){//dist to nearest ghost
      float shortestdist = 1000;
      for(int x = 0; x < game->ghosts.size(); x++){
        if((abs(game->mpac.x - game->ghosts[x].x) + abs(game->mpac.y - game->ghosts[x].y)) < shortestdist){
          shortestdist = (abs(game->mpac.x - game->ghosts[x].x) + abs(game->mpac.y - game->ghosts[x].y));
        }
      }
      return shortestdist;
    }
    if(operation == 2){//addition
      return childleft->eval(game) + childright->eval(game);
    }
    if(operation == 3){//subtraction
      return childleft->eval(game) - childright->eval(game);
    }
    if(operation == 4){//multiplication
      return childleft->eval(game) * childright->eval(game);
    }
    if(operation == 5){//division
    float right = childright->eval(game);
    if(right == 0)
      right = 0.001;
      return childleft->eval(game) / right;
    }
    if(operation == 6){//random
      float mod;
      float a = childleft->eval(game);
      float b = childright->eval(game);
      if(b > a){
        float temp = a;
        a=b;
        b=temp;
      }
      if((int)((a-b) + b)==0)
        mod = 1;
      else  
        mod = (a-b) ;
        
      float re = (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / mod))) + b;
        return re;
    }
  }
  
  float node::evalg(gameboard *game, unit thing, int self){
    if(operation == 0){//dist to nearest ghost
      float shortestdist = 1000;
      for(int x = 0; x < game->ghosts.size(); x++){
        if((abs(thing.x - game->ghosts[x].x) + abs(thing.y - game->ghosts[x].y)) < shortestdist){ 
          if(x != self)
            shortestdist = (abs(thing.x - game->ghosts[x].x) + abs(thing.y - game->ghosts[x].y));
        }
      }
      if(shortestdist == 1000)
        shortestdist=0;
      return shortestdist;
    }
    if(operation == 1){//dist to ms pacman
      float shortestdist = 1000;
      
        
      shortestdist = (abs(game->mpac.x - thing.x) + abs(game->mpac.y - thing.y));
        
      
      return shortestdist;
    }
    
    if(operation == 2){//addition
      return childleft->evalg(game, thing, self) + childright->evalg(game, thing, self);
    }
    if(operation == 3){//subtraction
      return childleft->evalg(game, thing, self) - childright->evalg(game, thing, self);
    }
    if(operation == 4){//multiplication
      return childleft->evalg(game, thing, self) * childright->evalg(game, thing, self);
    }
    if(operation == 5){//division
    float right = childright->evalg(game, thing, self);
    if(right == 0)
      right = 0.001;
      return childleft->evalg(game, thing, self) / right;
    }
    if(operation == 6){//random
      float mod;
      float a = childleft->evalg(game, thing, self);
      float b = childright->evalg(game, thing, self);
      if(b > a){
        float temp = a;
        a=b;
        b=temp;
      }
      if((int)((a-b) + b)==0)
        mod = 1;
      else  
        mod = (a-b) ;
        
      float re = (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / mod))) + b;
        return re;
    }
  }
  
  void gameboard::controllerinit(int maxdepth){
    controller = new node();
    if(rand() % 2){//full method
      maxcontrollerinit(maxdepth, 0, controller);
    }else{//grow method
      growcontrollerinit(maxdepth, 0, controller);
    }
  }
  
  void gameboard::maxcontrollerinit(int maxdepth, int currdepth, node *controll){
    if(currdepth == maxdepth - 1){
      controll->operation = rand() % 2;
    } else{
      controll->operation = (rand() % 5) + 2;
      controll->childleft = new node();
      controll->childright = new node();
      maxcontrollerinit(maxdepth, currdepth+1, controll->childleft);
      maxcontrollerinit(maxdepth, currdepth+1, controll->childright);
    }
  }
  
  void gameboard::growcontrollerinit(int maxdepth, int currdepth, node *controll){
    if(currdepth == maxdepth - 1){
      controll->operation = rand() % 2;
    } else{
      controll->operation = (rand() % 7);
      if(controll->operation > 1){
        controll->childleft = new node();
        controll->childright = new node();
        growcontrollerinit(maxdepth, currdepth+1, controll->childleft);
        growcontrollerinit(maxdepth, currdepth+1, controll->childright);
      }
    }
  }
  
  void gcontrollerinit(int maxdepth, node* gcontr){
    gcontr = new node();
    if(rand() % 2){//full method
      gmaxcontrollerinit(maxdepth, 0, gcontr);
    }else{//grow method
      ggrowcontrollerinit(maxdepth, 0, gcontr);
    }
  }
  
  void gmaxcontrollerinit(int maxdepth, int currdepth, node* controll){
    if(currdepth == maxdepth - 1){
      controll->operation = rand() % 2;
    } else{
      controll->operation = (rand() % 5) + 2;
      controll->childleft = new node();
      controll->childright = new node();
      gmaxcontrollerinit(maxdepth, currdepth+1, controll->childleft);
      gmaxcontrollerinit(maxdepth, currdepth+1, controll->childright);
    }
  }
  
  void ggrowcontrollerinit(int maxdepth, int currdepth, node* controll){
    if(currdepth == maxdepth - 1){
      controll->operation = rand() % 2;
    } else{
      controll->operation = (rand() % 7);
      if(controll->operation > 1){
        controll->childleft = new node();
        controll->childright = new node();
        ggrowcontrollerinit(maxdepth, currdepth+1, controll->childleft);
        ggrowcontrollerinit(maxdepth, currdepth+1, controll->childright);
      }
    }
  }
  
  void gameboard::outputcontroller(){
    cout<<controller->operation<<endl;
    if(controller->operation > 1){
    outputcontrollerp2(controller->childleft);
    outputcontrollerp2(controller->childright);}
  }
  
  void gameboard::outputcontrollerp2(node *controll){
    cout<<controll->operation<<endl;
    if(controll->operation > 1){
    outputcontrollerp2(controll->childleft);
    outputcontrollerp2(controll->childright);}
  }
  
  void gameboard::gameinit(int w, int h, int d, float p){
    score = 0;
    height = h;
    width = w;
    time = h*w*2;
    density = d;
    pp = p;
    gameover = false;
    eatenpills=0;
    stringstream ss;

    board.resize(width);
    for(int x = 0; x < width; x++){
      for(int y = 0; y < height; y++){
        board[x].push_back('*');
      }
    }    
    mpac.x = 0;
    mpac.y = height - 1;
    

    
    
    board[0][height-1]='m';
    pillnum=0;
    if(density > 100)
      density = 100;
      
    pills.clear();
    while(pills.size() == 0){
    
      for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
          if((x != 0 || y != height-1) ){
            if(rand() % 100 <= density){
              unit temp(x,y);
              board[x][y] = 'p';
              pills.push_back(temp);
              pillnum++;
            }
          }
        }
      }
    }
    ghosts.clear();
    for(int x = 0; x < 3; x++){
      ghosts.emplace_back(unit{width-1, 0});
    }
    board[width-1][0] = 'g';
  }
  
  void gameboard::movempac(){
    int bestmove=0;
    float besteval=0;
    vector<unit> validMoves;
    if(mpac.y + 1 < height && board[mpac.x][mpac.y+1] != 'g')
      validMoves.push_back(unit(mpac.x, mpac.y+1));
    if(mpac.y - 1 > 0 && board[mpac.x][mpac.y-1] != 'g')
      validMoves.push_back(unit(mpac.x, mpac.y-1));
    if(mpac.x - 1 > 0 && board[mpac.x-1][mpac.y] != 'g')
      validMoves.push_back(unit(mpac.x-1, mpac.y));
    if(mpac.x + 1 < width && board[mpac.x+1][mpac.y] != 'g')
      validMoves.push_back(unit(mpac.x+1, mpac.y));
    validMoves.push_back(unit(mpac.x, mpac.y));
    
    gameboard test = *this;
    float moveeval=0;
    for(int x=0;x<validMoves.size();x++){
      test.mpac=validMoves[x];
      moveeval = controller->eval(&test);
      if(moveeval>besteval){
        besteval=moveeval;
        bestmove=x;
      }
    }
    
    mpac = validMoves[bestmove];
    
  }
  
  void gameboard::moveghost(unit& phantom, node* contr, int self){
    int bestmove=0;
    float besteval=0;
    vector<unit> validMoves;
    if(phantom.y + 1 < height)
      validMoves.push_back(unit(phantom.x, phantom.y+1));
    if(phantom.y - 1 > 0)
      validMoves.push_back(unit(phantom.x, phantom.y-1));
    if(phantom.x - 1 > 0)
      validMoves.push_back(unit(phantom.x-1, phantom.y));
    if(phantom.x + 1 < width)
      validMoves.push_back(unit(phantom.x+1, phantom.y));
      
    gameboard test = *this;
    float moveeval=0;
    for(int x=0;x<validMoves.size();x++){
      phantom=validMoves[x];
      moveeval = contr->evalg(&test, phantom, self);
      if(moveeval>besteval){
        besteval=moveeval;
        bestmove=x;
      }
    }
    phantom = validMoves[bestmove];
    
  }
  
  bool gameboard::isgameover(){
    time--;
    if(time == 0)
      return true;
    if(pills.size() == 0)
      return true;
    for(int x = 0; x < 3; x++){
      if(ghosts[x].x == mpac.x && ghosts[x].y == mpac.y)
        return true;
    }
    return false;
  }
  
  void gameboard::printboard(){
    for(int y = height-1; y >= 0; y--){
      for(int x = 0; x < width; x++){
        cout << board[x][y];
      }
      cout << endl;
    }
    cout<<"---"<<endl;
  }
  
  int gameboard::playthrough(vector<string>& log, node* contr){
  stringstream ss;
  //log<<"m "<<mpac.x<<" "<<mpac.y<<endl;
    ss<<"m "<<mpac.x<<" "<<mpac.y;
    log.push_back(ss.str());
    //log<<"1 "<<ghosts[1].x<<" "<<ghosts[1].y<<endl;
    ss.str("");
    ss<<"1 "<<ghosts[0].x<<" "<<ghosts[0].y;
    log.push_back(ss.str());
    //log<<"2 "<<ghosts[2].x<<" "<<ghosts[2].y<<endl;
    ss.str("");
    ss<<"2 "<<ghosts[1].x<<" "<<ghosts[1].y;
    log.push_back(ss.str());
    //log<<"3 "<<ghosts[3].x<<" "<<ghosts[3].y<<endl;
    ss.str("");
    ss<<"3 "<<ghosts[2].x<<" "<<ghosts[2].y;
    log.push_back(ss.str());
    for(int x=0; x<pills.size();x++){
      //log<<"p "<<pills[x].x<<" "<<pills[x].y<<endl;
      ss.str("");
      ss<<"p "<<pills[x].x<<" "<<pills[x].y;
      log.push_back(ss.str());
    }
    //log<<"t "<<time<<" "<<score<<endl;
    ss.str("");
    ss<<"t "<<time<<" "<<score;
    log.push_back(ss.str());
    while(gameover == false){
      board[mpac.x][mpac.y] = '*';  
      movempac();
      board[mpac.x][mpac.y] = 'm';
      for(int x=0; x<3; x++){
        board[ghosts[x].x][ghosts[x].y] = '*';
        moveghost(ghosts[x], contr, x);
        board[ghosts[x].x][ghosts[x].y] = 'g';
      }
      
      
      for(int x=0; x<pills.size(); x++){
        if(pills[x].x == mpac.x && pills[x].y == mpac.y){
          eatenpills++;
          score = ((float)eatenpills/pillnum)*100;
          pills.erase(pills.begin()+x);
          x--;
        }
      }
      
      gameover = isgameover();
      if(gameover == false){
      //  log<<"m "<<mpac.x<<" "<<mpac.y<<endl;
      ss.str("");
      ss<<"m "<<mpac.x<<" "<<mpac.y;
      log.push_back(ss.str());
      //  log<<"1 "<<ghosts[1].x<<" "<<ghosts[1].y<<endl;
      ss.str("");
      ss<<"1 "<<ghosts[0].x<<" "<<ghosts[0].y;
      log.push_back(ss.str());
      //  log<<"2 "<<ghosts[2].x<<" "<<ghosts[2].y<<endl;
      ss.str("");
      ss<<"2 "<<ghosts[1].x<<" "<<ghosts[1].y;
      log.push_back(ss.str());
      //  log<<"3 "<<ghosts[3].x<<" "<<ghosts[3].y<<endl;
      ss.str("");
      ss<<"3 "<<ghosts[2].x<<" "<<ghosts[2].y;
      log.push_back(ss.str());
      //  log<<"t "<<time<<" "<<score<<endl;
      ss.str("");
      ss<<"t "<<time<<" "<<score;
      log.push_back(ss.str());
      }
    }
    //gameover score updates(fully eaten pills, and parsimony pressure)
    if(pills.size() == 0)
      score = score + (((float)time/(height*width*2))*100);
    //score = score - (pp * depthfinder(controller));
      
   
    //log<<"m "<<mpac.x<<" "<<mpac.y<<endl;
    ss.str("");
    ss<<"m "<<mpac.x<<" "<<mpac.y;
    log.push_back(ss.str());
    //log<<"1 "<<ghosts[1].x<<" "<<ghosts[1].y<<endl;
    ss.str("");
    ss<<"1 "<<ghosts[0].x<<" "<<ghosts[0].y;
    log.push_back(ss.str());
    //log<<"2 "<<ghosts[2].x<<" "<<ghosts[2].y<<endl;
    ss.str("");
    ss<<"2 "<<ghosts[1].x<<" "<<ghosts[1].y;
    log.push_back(ss.str());
    //log<<"3 "<<ghosts[3].x<<" "<<ghosts[3].y<<endl;
    ss.str("");
    ss<<"3 "<<ghosts[2].x<<" "<<ghosts[2].y;
    log.push_back(ss.str());
    //log<<"t "<<time<<" "<<score<<endl;
    ss.str("");
    ss<<"t "<<time<<" "<<score;
    log.push_back(ss.str());
    return (score - (pp * depthfinder(controller)));
  }
  
  
  
  
  
  
  bool continuerunning(bool termcriteria, int condition, vector<int> pastavgfit, int currentevals){
  if(termcriteria == false){
    if(currentevals>=condition)
      return false;    
  }
  else{
    if(pastavgfit.size() < condition)
      return true;
    for(int x=0;x<condition;x++){
      if(pastavgfit[pastavgfit.size()-(x+1)] != pastavgfit[pastavgfit.size()-1])
        return true;
    }
    return false;
  }
  return true;
}

//Sorts solutions by fitness
void fitSort(int mu, vector<int> &fitness, vector< gameboard > &solutions){
  gameboard tempSol;
  int temp;
 
  
  for(int i=0;i<mu;i++){
    for(int j=0;j<mu-1;j++){
      if(fitness[j]<fitness[j+1]){
        temp=fitness[j];
        tempSol=solutions[j];
        fitness[j]=fitness[j+1];
        solutions[j]=solutions[j+1];
        fitness[j+1]=temp;
        solutions[j+1]=tempSol;
      }
    }
  }
  return;
}
void fitSortg(int mu, vector<int> &fitness, vector< node* > &solutions){
  node* tempSol;
  int temp;
 
  
  for(int i=0;i<mu;i++){
    for(int j=0;j<mu-1;j++){
      if(fitness[j]<fitness[j+1]){
        temp=fitness[j];
        tempSol=solutions[j];
        fitness[j]=fitness[j+1];
        solutions[j]=solutions[j+1];
        fitness[j+1]=temp;
        solutions[j+1]=tempSol;
      }
    }
  }
  return;
}

//Chooses which stratagies survive each round by using truncation
//Changes v_val to contain the mu best solutions from a combined pool of previous solutions and the new children
void trunc(vector< gameboard > &v_val, vector< gameboard > children, int mu, int lambda, vector<int> &fitLog, vector<int> childfit/*, bool pc*/){
  int groupsize;
  int offset = 0;
  //if(pc){
    groupsize = mu + lambda;
    offset = mu;
  //}else{
  //  groupsize = lambda;
  //}
      
  vector< gameboard > grouptotal ((groupsize), gameboard());
  vector<int> groupfit((groupsize), 0);
      
  //if(pc){
    for(int x=0; x<mu; x++){
      grouptotal[x] = v_val[x];
      groupfit[x] = fitLog[x];
    }
  //}
      
  for(int x=0; x<lambda; x++){
    grouptotal[x+offset] = children[x];
    groupfit[x+offset] = childfit[x];
  }  
      
  fitSort((groupsize), groupfit, grouptotal);
  
      
  for(int x=0; x<mu; x++){
  
    v_val[x] = grouptotal[x];
    fitLog[x] = groupfit[x];
  }
      
  return;
}

void truncg(vector< node* > v_val, vector< node* > children, int mu, int lambda, vector<int> &fitLog, vector<int> childfit/*, bool pc*/){
  int groupsize;
  int offset = 0;
  //if(pc){
    groupsize = mu + lambda;
    offset = mu;
  //}else{
  //  groupsize = lambda;
  //}
      
  vector< node* > grouptotal ((groupsize), new node());
  vector<int> groupfit((groupsize), 0);
      
  //if(pc){
    for(int x=0; x<mu; x++){
      grouptotal[x] = v_val[x];
      groupfit[x] = fitLog[x];
    }
  //}
      
  for(int x=0; x<lambda; x++){
    grouptotal[x+offset] = children[x];
    groupfit[x+offset] = childfit[x];
  }  
      
  fitSortg((groupsize), groupfit, grouptotal);
  
      
  for(int x=0; x<mu; x++){
  
    v_val[x] = grouptotal[x];
    fitLog[x] = groupfit[x];
  }
      
  return;
}

void recombination(gameboard basegame, const gameboard parenta, const gameboard parentb, gameboard &child){//returns only one child
  child = basegame;
  node* tmp1;
  node* tmp2;
  tmp1 = clone(parenta.controller);
  tmp2 = clone(parentb.controller);
  node* pointholderas1 = findrandchild(tmp1);
  node* pointholderbs1 = findrandchild(tmp2);
  node* pointholderas2 = clone(pointholderas1);
  node* pointholderbs2 = clone(pointholderbs1);
  pointholderas1 = pointholderbs2;
  child.controller = tmp1;
  
}

void recombination(gameboard basegame, const gameboard parenta, const gameboard parentb, gameboard &childa, gameboard &childb){//returns 2 children
  childa = basegame;
  childb = basegame;
  node* tmp1; 
  node* tmp2;
  tmp1 = clone(parenta.controller);
  tmp2 = clone(parentb.controller);
  node* pointholderas1 = findrandchild(tmp1);
  node* pointholderbs1 = findrandchild(tmp2);
  node* pointholderas2 = clone(pointholderas1);
  node* pointholderbs2 = clone(pointholderbs1);
  pointholderas1 = pointholderbs2;
  pointholderbs1 = pointholderas2;
  childa.controller = tmp1;
  childb.controller = tmp2;
  
  
}

void recombinationg(node* parenta, node* parentb, node* child){//returns only one child
  node* tmp1;
  node* tmp2;
  tmp1 = clone(parenta);
  tmp2 = clone(parentb);
  node* pointholderas1 = findrandchild(tmp1);
  node* pointholderbs1 = findrandchild(tmp2);
  node* pointholderas2 = clone(pointholderas1);
  node* pointholderbs2 = clone(pointholderbs1);
  pointholderas1 = pointholderbs2;
  child = tmp1;
  
}

void recombinationg(node* parenta, node* parentb, node* childa, node* childb){//returns 2 children
  node* tmp1; 
  node* tmp2;
  tmp1 = clone(parenta);
  tmp2 = clone(parentb);
  node* pointholderas1 = findrandchild(tmp1);
  node* pointholderbs1 = findrandchild(tmp2);
  node* pointholderas2 = clone(pointholderas1);
  node* pointholderbs2 = clone(pointholderbs1);
  pointholderas1 = pointholderbs2;
  pointholderbs1 = pointholderas2;
  childa = tmp1;
  childb = tmp2;
  
  
}

void mutation(gameboard basegame, const gameboard parent, gameboard &child){
  child = basegame;
  child.controller = clone(parent.controller);
  child.growcontrollerinit(INITIALDEPTH, 0 , findrandchild(child.controller));
}
void mutationg(node* parent, node* child){
  child = clone(parent);
  ggrowcontrollerinit(INITIALDEPTH, 0 , findrandchild(child));
}

node* clone(node *root){
  node *temp = new node();
  temp->operation = root->operation;
  if(root->operation > 1){
    temp->childleft = clone(root->childleft);
    temp->childright = clone(root->childright);
  }
  return temp;
}

node* findrandchild(node* root){  
  return findrandchildp2(root, depthfinder(root));
}

node* findrandchildp2(node* root, int depth){
  if(root->operation < 2)
    return (root);
  if(rand() % depth == 0)
    return (root);
  if(rand() % 2 == 0)
    return findrandchildp2(root->childleft, depth);
  else
    return findrandchildp2(root->childright, depth);
}


//Chooses set of parents by using the fitness proportional selection
void FPS( vector<int> fitLog, int lambda, int mu, int parentpool[]){
  double totalfit;
  double value;
  int offset = 0;
  if(fitLog[mu-1]<0)
    offset = abs(fitLog[mu-1]);
  for(int x=0; x<lambda; x++){
    totalfit = 0;
    for(int i=0; i<mu; i++) {
      totalfit += (fitLog[i] + offset);
    }
    value = rand() % 100;
    value = value / 100.0;
    value = value * totalfit;

   
    for(int i=0; i<mu && value>=0; i++) {		
      value = value - (fitLog[i] + offset);	
      if(value <= 0){
        parentpool[x] = i;
      }
    }
  }

	return;
}

//must have performed fitsort prior to using this function
void oversel(vector<int> fitLog, int lambda, int mu, int parentpool[]){//with the frequently chosen group being the top 40% of the population
  int top40 = (mu * .3);
  int breakpoint = lambda * .8;
  int offset=0;
  double totalfit;
  double value;
  if(fitLog[mu-1]<0)
    offset = abs(fitLog[mu-1]);
  for(int x=0; x<breakpoint; x++){    
    totalfit = 0;
    for(int i=0; i<top40; i++) {
      totalfit += (fitLog[i] + offset);
    }
    value = rand() % 100;
    value = value / 100.0;
    value = value * totalfit;
    for(int i=0; i<top40 && value>=0; i++) {		
      value = value - (fitLog[i] + offset);	
      if(value <= 0){
        parentpool[x] = i;
      }
    }
    
  }  
  for(int x=breakpoint; x<lambda; x++){
    totalfit = 0;
    for(int i=top40; i<mu; i++){
      totalfit += (fitLog[i] + offset);
    }
    value = rand() % 100;
    value = value / 100.0;
    value = value * totalfit;
    for(int i=top40; i<mu && value>=0; i++) {		
      value = value - (fitLog[i] + offset);	
      if(value <= 0){
        parentpool[x] = i;
      }
    }
  }
	return;
}

//Chooses which stratagies survive each round using the k-tournament selection
//without replacement
void SurvivalKTourn(int k, vector<int> &fitLog, int lambda, int mu, vector<gameboard> &v_val, vector<gameboard> children, vector<int> childfit/*, bool pc*/){
  int groupsize;
  int offset = 0;
  //if(pc){
    groupsize = mu + lambda;
    offset = mu;
  //}else{
  //  groupsize = lambda;
  //}
  vector< gameboard > grouptotal((groupsize), gameboard());
  int groupfit[(groupsize)];
  
  //if(pc){
    for(int x=0; x<mu; x++){
      grouptotal[x] = v_val[x];
      groupfit[x] = fitLog[x];
      
    }
  //}
  for(int x=0; x<lambda; x++){
    grouptotal[x+offset] = children[x];
    groupfit[x+offset] = childfit[x];
    
  }  
  
  int selection[k];
  vector<int> used;
  bool found=false;
  int currbest;
  int temp;
  for(int currentMember = 0; currentMember < mu; currentMember++){ 
    used.clear();
    for(int a = 0; a < k; a++){
      found = false;
      temp = rand() % (groupsize);
      for(int b = 0; b<used.size(); b++){
        if(used[b] == temp){
          found = true;          
        }
      }
      if(found == false){
        selection[a] = temp;
        used.push_back(temp);
      }
      else{
        a--;
      }
    }
    
    currbest=selection[0];
    for(int a = 0; a < k; a++){
      if(selection[a] < groupfit[currbest]){
        currbest = selection[a];
      }
    }
    v_val[currentMember] = grouptotal[currbest];
    fitLog[currentMember] = groupfit[currbest];
    
  }  
  return;
}

void SurvivalKTourng(int k, vector<int> &fitLog, int lambda, int mu, vector<node*> v_val, vector<node*> children, vector<int> childfit/*, bool pc*/){
  int groupsize;
  int offset = 0;
  //if(pc){
    groupsize = mu + lambda;
    offset = mu;
  //}else{
  //  groupsize = lambda;
  //}
  vector< node* > grouptotal((groupsize), new node());
  int groupfit[(groupsize)];
  
  //if(pc){
    for(int x=0; x<mu; x++){
      grouptotal[x] = v_val[x];
      groupfit[x] = fitLog[x];
      
    }
  //}
  for(int x=0; x<lambda; x++){
    grouptotal[x+offset] = children[x];
    groupfit[x+offset] = childfit[x];
    
  }  
  
  int selection[k];
  vector<int> used;
  bool found=false;
  int currbest;
  int temp;
  for(int currentMember = 0; currentMember < mu; currentMember++){ 
    used.clear();
    for(int a = 0; a < k; a++){
      found = false;
      temp = rand() % (groupsize);
      for(int b = 0; b<used.size(); b++){
        if(used[b] == temp){
          found = true;          
        }
      }
      if(found == false){
        selection[a] = temp;
        used.push_back(temp);
      }
      else{
        a--;
      }
    }
    
    currbest=selection[0];
    for(int a = 0; a < k; a++){
      if(selection[a] < groupfit[currbest]){
        currbest = selection[a];
      }
    }
    v_val[currentMember] = grouptotal[currbest];
    fitLog[currentMember] = groupfit[currbest];
    
  }  
  return;
}

int depthfinder(node* root){
  int check1=0;
  int check2=0;
  if(root->operation > 1){
    check1 = check1 + depthfinder(root->childleft);
    check2 = check2 + depthfinder(root->childright);
  }
  if(check1 > check2)
    return 1 + check1;
  else 
    return 1 + check2;
}

void controllerprint(node* root, ofstream& outp){
  switch (root->operation)
  {
  case 0: outp << "nearest pill dist";
    break;
  case 1: outp << "nearest ghost dist";
    break;
  case 2: outp << "+";
    break;
  case 3: outp << "-";
    break;
  case 4: outp << "*";
    break;
  case 5: outp << "/";
    break;
  case 6: outp << "rand";
    break;
  default: cout << "Error Printing controller";
    break;
  }
  
  
  if(root->operation>1){
  outp<<"( ";
  controllerprint(root->childleft, outp);
  outp<<", ";
  controllerprint(root->childright, outp);
  outp<<")";
  }
}

void gcontrollerprint(node* root, ofstream& outp){
  switch (root->operation)
  {
  case 0: outp << "nearest pill dist";
    break;
  case 1: outp << "nearest ghost dist";
    break;
  case 2: outp << "+";
    break;
  case 3: outp << "-";
    break;
  case 4: outp << "*";
    break;
  case 5: outp << "/";
    break;
  case 6: outp << "rand";
    break;
  default: cout << "Error Printing controller";
    break;
  }
  
  
  if(root->operation>1){
  outp<<"( ";
  controllerprint(root->childleft, outp);
  outp<<", ";
  controllerprint(root->childright, outp);
  outp<<")";
  }
}