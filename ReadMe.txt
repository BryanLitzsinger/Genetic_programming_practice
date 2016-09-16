This program uses the Genetic Programming aspect of Evolutionary Algorithms and a competitive environment to attempt to create optimal  movement
algorithms, in a simplified version of the game Ms. Pacman called GPac, for Ms. Pacman and the Ghosts.

To compile run "g++ -std=c++11 2b.cpp"

To execute the program with default settings run "a.out"

To execute the program with a custom config file run "a.out <filename>

Config files must be set up as shown below:
<world width>
<world height>
<pill density>
<"random" to use a random seed in the number generator, otherwise it will use the value entered here>
<"evaluations" to use evals as the termination criteria otherwise it will be generations to avg fitness convergence>
<# of runs>
<# of evals or # of generations to convergence>
<log filename>
<highscore filename>
<ms pacman solution filename>
<ghost solution filename>
<mu>
<lambda>
<parsimony pressure>
<ghost mu>
<ghost lambda>
<ghost parsimony>
<"FPS" to use FPS as the parent selection method for the pacman controller, otherwise its overselection>
<"Truncation" to use Truncation as the survival selection method for the pacman controller, otherwise its K-tournament without replacement>
<if k tournament is chosen it will be the k value otherwise leave this out>
<"FPS" to use FPS as the parent selection method for the ghost controller, otherwise its overselection>
<"Truncation" to use Truncation as the survival selection method for the ghost controller, otherwise its K-tournament without replacement>
<if k tournament is chosen it will be the k value otherwise leave this out>

GPac Rules:

GPac
In GPac, the world is a two-dimensional grid, the only walls are the edges of the world, and there is no world
wrap.  There are two types of units:  Ms.  Pac-Man and the Ghosts.  Ms.  Pac-Man always starts at the top
left cell and all three the ghosts always start at the bottom right cell.  These units are guided by controllers,
which is what your GP will evolve.  Units move in cardinal directions (up, down, left, right); Ms.  Pac-Man
can choose to hold position, but the Ghosts cannot.  They move from one grid cell to another in a discrete
fashion (i.e., they move a whole cell at a time).  Units cannot move o  the edges of the map.  Ms.  Pac-Man
cannot move onto a grid cell currently occupied by a ghost.  Ghosts can occupy the same grid cell as other
ghosts.  If Ms.  Pac-Man and a ghost occupy the same cell, the game is over.  Before the game begins, cells
are chosen at random according to a preset density parameter to contain "pills".  The density parameter
speci es the percentage chance for any given cell to contain a pill, subject to the constraints (a) at least one
cell needs to contain a pill, and (b) Ms.  Pac-Man's starting cell cannot contain a pill.  Thus: E[number of cells
containing a pill] = MAX (1 , Density * (total number of cells - 1))

If  Ms.   Pac-Man  occupies  a  cell  that  contains  a  pill,  the  pill  is  removed,  and  Ms.   Pac-Man's  score  is
increased.  When all pills have been removed from the world, the game is over.

Time
Each GPac game starts with time equal to twice the number of grid cells in the world.  Each turn is one
time step.  When the time limit reaches zero, the game is over.  This prevents games from getting stuck in
infnite loops.  It also promotes efficient controller evolution.

Game Play
Each turn, the game gives each of the unit's controllers the current game state.  This state includes at least:
where all of the units are currently located and where all of the pills are located.  Each controller will then
choose what move to make (up, down, left, right for all controllers, also hold just for Ms.  Pac-Man).  Once
all of the units have determined their next move, the game state will update everyone's position and decrease
the time remaining by one.  Once everyone has moved, the game will check if:
1.  Ms.  Pac-Man and any of the ghosts are in the same cell, causing game-over.
2.  Ms.  Pac-Man is in a cell with a pill, causing the pill to be removed, and the score to be adjusted.
3.  All the pills are removed, causing game-over.
4.  Time remaining is equal to zero, causing game-over.

Score
Ms.  Pac-Man's score is equal to the percentage of the total number of pills she has consumed truncated to
an integer.  If the game ends because there are no more pills on the board, Ms.  Pac-Man's score is increased
by the percentage of time remaining truncated to an integer.  This score can be used directly for the  tness
of the Ms.  Pac-Man controller.  Ghost  tness should be inversely proportional to Ms.  Pac-Man's fitness (for
example, negate her fitness).
