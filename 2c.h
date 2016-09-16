#ifndef TWOC_H
#define TWOC_H

//Defines the initial max depth of the behavior trees
const int INITIALDEPTH = 5;

//Defines the maximum possible fitness value
const int MAXFIT = 200;


struct gameboard;

//A struct to easily store the location of different units
struct unit{
  int x;
  int y;
  unit(){
  }
  unit(int xcoor, int ycoor){
    x=xcoor;
    y=ycoor;
  }
};

//Struct that contains the information for individual nodes of the behavior trees
struct node{
  int operation;
  node *childleft;
  node *childright;
  
  //Purpose: Use on a variety of positions to find which is the best for ms. pacman
  //Input: Pointer to a gameboard that represents a current gamestate
  //Output: A float representing how fit a current position on the map is for ms. pacman
  float eval(gameboard *game);
  
  //Purpose: Use on a variety of positions to find which is the best for a ghost
  //Input: Pointer to a gameboard that represents a current gamestate
  //Output: A float representing how fit a current position on the map is for a ghost
  float evalg(gameboard *game, unit thing, int self);
};

  //Purpose: To find the depth of a tree
  //Input: A pointer to a node in a behavior tree
  //Output: The number of levels in a tree below the input node
  int depthfinder(node* root);



struct gameboard{
  
  int height, width, time, score;
  vector< vector< char > > board;
  int density, pillnum, eatenpills;
  vector< unit > pills;
  vector< unit > ghosts;
  unit mpac;
  bool gameover;
  node* controller;
  float pp;
  
  //Purpose: To create an initial genetic tree that will control Ms. Pacman's actions
  //Input: The max size the controlling tree can be
  //Output: Changes the controller variable of the gameboard struct to be the initialized tree
  void controllerinit(int maxdepth);
  
  //Purpose: Creates a full genetic tree that will control Ms. Pacman
  //Input: The max size the controlling tree can be, how deep in the tree the node being worked on is, pointer to the node that is being assigned a value
  //Output: Assigns the controll variable an operation
  void maxcontrollerinit(int maxdepth, int currdepth, node *controll);
  
  //Purpose: Creates a partially built genetic tree that will control Ms. Pacman
  //Input: The max size the controlling tree can be, how deep in the tree the node being worked on is, pointer to the node that is being assigned a value
  //Output: Assigns the controll variable an operation
  void growcontrollerinit(int maxdepth, int currdepth, node *controll);
  
  //Purpose: Prints to the screen the genetic tree that controls Ms. Pacman
  //Input: N/A
  //Output: Prints to the screen the genetic tree that controls Ms. Pacman
  void outputcontroller();
  
  //Purpose: A recursive function to aid in outputting the controll tree
  //Input: Pointer to the node that will be printed
  //Output: Prints to the screen a value representing what operation the node performs
  void outputcontrollerp2(node *controll);
  
  //Purpose: Initializes the gameboard.
  //Input: Width of the gameboard, height of the gameboard, density of the pills, parsimony pressure
  //Output: N/A 
  void gameinit(int w, int h, int d, float p);
  
  //Purpose: Moves a unit one spot up on the board.
  //Input: The unit to move up.
  //Output: N/A
  void moveup(unit& object){object.y++;}
  
  //Purpose: Moves a unit one spot down on the board.
  //Input: The unit to move down.
  //Output: N/A 
  void movedown(unit& object){object.y--;}
  
  //Purpose: Moves a unit one spot left on the board.
  //Input: The unit to move left.
  //Output: N/A 
  void moveleft(unit& object){object.x--;}
  
  //Purpose: Moves a unit one spot right on the board.
  //Input: The unit to move right.
  //Output: N/A
  void moveright(unit& object){object.x++;}
  
  //Purpose: Chooses which direction Ms. pacman should move and performs the move.
  //Input: N/A
  //Output: N/A
  void movempac();
  
  //Purpose: Chooses which direction the ghost should move and performs the move.
  //Input: The ghost to move, the root node of the ghost control tree, an int that represents which ghost is moving
  //Output: N/A
  void moveghost(unit& phantom, node* contr, int self);
  
  //Purpose: Determines if the game is over.
  //Input: N/A
  //Output: True if the game is over, false otherwise
  bool isgameover();
  
  //Purpose: Prints the current gamestate to the screen for debugging purposes.
  //Input: N/A
  //Output: N/A
  void printboard();
  
  //Purpose: Plays through the game of Ms. Pacman controlling Ms. Pacman and the ghosts with their respective controllers.
  //Input: The address of a vector of strings to pass a record of what events occured during the game out of the function, root node to the ghost control tree.
  //Output: The fitness of the Ms. Pacman controller.
  int playthrough(vector<string>& log, node* contr);
};

  //Purpose: To create an initial genetic tree that will control ghosts' actions
  //Input: The max size the controlling tree can be, the root node of the control tree
  //Output: Changes gcontr to be the initialized tree
  void gcontrollerinit(int maxdepth, node* gcontr);

  //Purpose: Creates a full genetic tree that will control ghosts
  //Input: The max size the controlling tree can be, how deep in the tree the node being worked on is, pointer to the node that is being assigned a value
  //Output: Assigns the controll variable an operation
  void gmaxcontrollerinit(int maxdepth, int currdepth, node *controll);

  //Purpose: Creates a partially built genetic tree that will control ghosts
  //Input: The max size the controlling tree can be, how deep in the tree the node being worked on is, pointer to the node that is being assigned a value
  //Output: Assigns the controll variable an operation
  void ggrowcontrollerinit(int maxdepth, int currdepth, node *controll);

  //Purpose: Used to determine whether the current run is finished.
  //Input: True to end after the average fitness has converged for a certain number of generations or false to end after a specific number of evaluations, 
  //the number of evaluations per run or the number of generations of convergence, the average fitness of previous round of evaluations, the current number of evaluation
  //Output: True to continue, false to end
  bool continuerunning(bool termcriteria, int evals, vector<int> pastavgfit, int currentevals);

  //Purpose: Sorts gameboards by the fitness of their Ms. Pacman controllers
  //Input: The number of gameboards to be sorted, a vector of the fitnesses, a vector of the gameboards
  //Output: Reorders fitness and solutions from highest fitness to lowest fitness
  void fitSort(int mu, vector<int> &fitness, vector< gameboard > &solutions);

  //Purpose: Sorts ghost control trees by thier fitness
  //Input: The number of trees to be sorted, a vector of the fitnesses, a vector of the trees
  //Output: Reorders fitness and solutions from highest fitness to lowest fitness
  void fitSortg(int mu, vector<int> &fitness, vector< node* > &solutions);

  //Purpose: Combines the main population and child population and then replaces the main population with the most fit individuals
  //Input: the main population, the child population, size of the main population, size of the child population, fitnesses of the main population,
  //fitnesses of the child population.
  //Output: Replaces v_val with the 'mu' most fit individuals from the main and child populations
  void trunc(vector< gameboard > &v_val, vector< gameboard > children, int mu, int lambda, vector<int> &fitLog, vector<int> childfit/*, bool pc*/);

  //Purpose: To combine parts from two Ms. Pacman controllers to create a new controller.
  //Input: An unplayed game, the first parent to take parts from, the second parent to take parts from, The child game that will pass back the new child.
  //Output: Changes child to contain basegame board setup with the combined controller from parenta and parentb.
  void recombination(gameboard basegame, gameboard parenta, gameboard parentb, gameboard &child);

  //Purpose: To combine parts from two Ms. Pacman controllers to create two new controllers.
  //Input: An unplayed game, the first parent to take parts from, the second parent to take parts from, The first child game that will pass back the new child,
  //the second child game that will pass back the second child.
  //Output: Changes childa and childb to contain basegame board setup with the combined controller from parenta and parentb (a different controller for each child).
  void recombination(gameboard basegame, const gameboard parenta, const gameboard parentb, gameboard &childa, gameboard &childb);

  //Purpose: To combine parts from two ghost controllers to create a new controller.
  //Input: The first parent to take parts from, the second parent to take parts from, The child that will pass back the new child.
  //Output: Changes child to contain the combined controller from parenta and parentb. 
  void recombinationg(node* parenta, node* parentb, node* child);

  //Purpose: To combine parts from two ghost controllers to create two new controllers.
  //Input: The first parent to take parts from, the second parent to take parts from, The first child that will pass back the new child,
  //the second child that will pass back the second child.
  //Output: Changes childa and childb to contain the combined controller from parenta and parentb (a different controller for each child).
  void recombinationg(node* parenta, node* parentb, node* childa, node* childb);

  //Purpose: Mutates a Ms. Pacman controller to create a new controller.
  //Input: An unplayed game, the parent to mutate, The child game that will pass back the new child.
  //Output: Changes child to contain basegame board setup with the mutated controller from parent.
  void mutation(gameboard basegame, gameboard parent, gameboard &child);

  //Purpose: Mutates a ghost controller to create a new controller.
  //Input: The parent to mutate, The child game that will pass back the new child.
  //Output: Changes child to contain the mutated controller from parent.
  void mutationg(node* parent, node* child);

  //Purpose: Creates a copy of a tree from the passed root node down.
  //Input: The node of a tree you want to copy down from.
  //Output: Returns the root node of the copy tree.
  node* clone(node* root);

  //Purpose: Picks a random node in the tree from root or below
  //Input: A node in a tree.
  //Output: The pointer to the randomly chosen node.
  node* findrandchild(node* root);

  //Purpose: Recursive function that aids in picking a random node.
  //Input: A node in a tree , how deep the tree is from root.
  //Output: The pointer to a randomly chosen node.
  node* findrandchildp2(node* root, int depth);

  //Purpose: Uses fitness proportional selection to choose which individuals will be parents.
  //Input: A vector containing the fitnesses, the number of parents being chosen, the number of individuals to chose from, array to store the indexes of the parents.
  //Output: Changes parentpool to contain the current indeces of the chosen parents.
  void FPS( vector<int> fitLog, int lambda, int mu, int parentpool[]);

  //Purpose: Uses overselection to choose which individuals will be parents.
  //Input: number of parents being chosen, number of parents to chose from, array to store the indexes of the parents.
  //Output: Changes parentpool to contain the current indeces of the chosen parents.
  void oversel(int lambda, int mu, int parentpool[]);

  //Purpose: 
  //Input: 
  //Output: 
  void SurvivalKTourn(int k, vector<int> &fitLog, int lambda, int mu, vector<gameboard> &v_val, vector<gameboard> children, vector<int> childfit/*, bool pc*/);

  //Purpose: To print out a representation of the Ms. Pacman control tree.
  //Input: The root node of the control tree, the output stream to print to.
  //Output: Prints the tree to the output specified by outp.
  void controllerprint(node* root, ofstream& outp);
  
  //Purpose: To print out a representation of the ghost control tree.
  //Input: The root node of the control tree, the output stream to print to.
  //Output:Prints the tree to the output specified by outp.
  void gcontrollerprint(node* root, ofstream& outp);


#endif