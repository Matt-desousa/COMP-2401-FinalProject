COMP2401           FINAL PROJECT
--------------------------------
           *AUTHORS* 
Fatima Atweri      ID: 101259778
Matthew De Sousa   ID: 101266991
--------------------------------

*PURPOSE OF THIS PROJECT*
The purpose of this project is to demonstrate a majority of the topics covered in this course which include:
- Dynamically and Statically allocated memory
- Linked Lists
- Static Arrays of Pointers
- Multi-threaded programming
- Makefiles

*FILES IN THIS PROJECT*
README.txt   -> the file that you are currently reading
Makefile     -> for compiling and linking all files in this project
defs.h       -> contains all constant definitons, structure definitions, and function protoypes 
utils.c      -> contains all helper functions 
house.c      -> contains all functions for initializing a house, populating a house with rooms, and cleaning up the allocated memory associated with the house
room.c       -> contains all functions for intialiing a room/room list, cleaning up the allocated memory for rooms, and other functions for managing rooms 
evidence.c   -> contains all functions for initializing evidence/evidence lists, cleaning up the allocated room for evidence, and other functions for managing evidence
ghost.c      -> contains all functions for initializing a ghost, cleaning up the allocated memory for the ghost, and other functions for managing ghost actions
hunter.c     -> contains all functions for intializing hunters and managing hunter actions
logger.c     -> contains all functions for logging ghost and hunter intializations/activity
main.c       -> initializes a house, populates rooms, initializes a ghost and hunters, manages the threading and randomized activity of the ghost and hunters, and cleans up the house and ghost
map.txt      -> text file that contains all room connections (to be read in house.c)
game_log.txt -> text file where all the ghost and hunter actions are logged/appended (logger.c functions append the logs)

*COMPILING INSTRUCTIONS*
to clean, type in command:
make clean

to compile, type in command:
make

*RUNNING INSTRUCTIONS*
to run, type in command:
./final_project

*HOW TO USE (OR HOW THE PROGRAM RUNS)*
once the program is running, a ghost will be initialized and you will be asked to type in the names of four hunters.
once you've entered their names, their activity, along with the ghost's activity, will be logged.
the ghost can either move around, leave evidence or do nothing.
the hunters can either move around, collect evidence, or review evidence.
if the ghost reaches a certain level of boredom, they will exit.
if a hunter reaches a certain level of fear or boredom, or they've reviewed and found a sufficient amount of evidence, they will exit.
once the ghost and all hunters exit, the program ends.
