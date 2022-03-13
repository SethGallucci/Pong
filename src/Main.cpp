/*
 To-Do List
    + Implement proper memory mangement.
    + Implement basic, predictive AI for the opponent.
    + Resolve reset-rally bug.
    + Resolve hang-on-exit bug.
    + Separate this program into different files.
    + Add "plink" noise for collisions.
    + Change the behaviour of when the ball hits the opponent paddle to include angle-changes.
    + Change Game_State_Flags to an enum system.
    + Refactor the project and make file so that the structure is logical and coherent.
    + Set up VSCode to build and debug the program.
    + Refactor Score_Display, decoupling it from the font, so the font can be loaded with other SDL assets.
    - Check for memory leaks.
    
*/

#include "Pong.hpp"

int main(int argc, char* argv[]){
    
    Pong pong;
    pong.play();
    
    return 0;
}