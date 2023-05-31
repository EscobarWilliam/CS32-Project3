//
//  Payer.cpp
//  Project3
//
//  Created by William  Escobar on 5/27/23.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include "Player.h"
#include "Board.h"
#include "Side.h"

//------------------------------------------------------------//
// Class for a timer to be use with SmartPlayer Implementation
//____________________________________________________________//
#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

Timer timer;
//------------------------------------------------------------------------//
//         int BadPlayer::chooseMove(const Board& b, Side s) const
//
// Chooses and arbitrary valid move and returns that choice. Meaning we
// return the # of the pot that the bad player chooses
//------------------------------------------------------------------------//
int BadPlayer::chooseMove(const Board& b, Side s) const
{
    //valid means that the choice we make is within the [1,m_nHoles] range
    //of holes on the board. Remmeber that '0' is the pot on each side
    
    //cases:
    //1. BadPlayers' side is empty, so no move can be made. Return -1
    if(b.beansInPlay(s) == 0)
        return -1;
    
    //2. BadPlayer chooses a hole randomly. Valid choice for anything not empty
    int n;
    do
    {
        n = (rand() % b.holes()) + 1;
    }
    while (n<=0 || n>b.holes() || b.beans(s, n) == 0);;
    
    return n;
}

//------------------------------------------------------------------------//
//         int HumanPlayer::chooseMove(const Board& b, Side s) const
//
// Promts the HumanPlayer to choose a move (repromting until a valid choice
// made) and returns that choice.
//------------------------------------------------------------------------//
int HumanPlayer::chooseMove(const Board &b, Side s) const
{
    //Check if HumanPlayers' side is empty
    if(b.beansInPlay(s) == 0)
        return -1;
    
    int n;
    do {
        std::cout << "Select a hole from 1 - " << b.holes() << ": ";
        std::cin >> n;
    } while (n<=0 || n>b.holes() || b.beans(s, n) == 0);
    //continue to ask until a valid non-empty hole is chosen
    
    return n;
}

//------------------------------------------------------------------------//
//         int SmartPlayer::chooseMove(const Board& b, Side s) const
//
// A SmartPlayer chooses a valid move and returns it. For any game played
// on a board of up to six holes per side, with up to four initial beans
// per hole, SmartPlayer::chooseMove must return its choice in no more
// than five seconds
//------------------------------------------------------------------------//
int SmartPlayer::chooseMove(const Board &b, Side s) const
{
    int bestHole, value;
        
        if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
            return -1; //if beans in both sides' holes are empty, return -1
        
       do
        {
            bestHole = (rand() % b.holes()) + 1;
        }while(b.beans(s, bestHole) == 0);

        //assign a random valid hole. If the hole is empty, assign again.
        int searchDepth = 10;

        Timer timer;
        timer.start();
        minmax(b, s, bestHole, value, searchDepth);
        
        return bestHole;
    ///***********IF I CAN"T GET SMART PLAYWER ALG TO WORK, USE THE CODE BELOW**********///
    ///
//    //valid means that the choice we make is within the [1,m_nHoles] range
//    //of holes on the board. Remmeber that '0' is the pot on each side
//
//    //cases:
//    //1. BadPlayers' side is empty, so no move can be made. Return -1
//    if(b.beansInPlay(s) == 0)
//        return -1;
//
//    //2. BadPlayer chooses a hole randomly. Valid choice for anything not empty
//    int n;
//    do
//    {
//        n = (rand() % b.holes()) + 1;
//    }
//    while (n<=0 || n>b.holes() || b.beans(s, n) == 0);;
//
//    return n;
}


int SmartPlayer::eval(const Board& b, Side s) const
{
    //code for when we hit a leaf, i.e. no more moves can be simulated
    if(b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0) // if game is over
       {
           if(b.beansInPlay(s) + b.beans(s, 0) >
              b.beansInPlay(opponent(s)) + b.beans(opponent(s), 0))//s has more beans including final sweep
           {
               return 10000;
           }
           else if (b.beansInPlay(s) + b.beans(s, 0) <
                    b.beansInPlay(opponent(s)) + b.beans(opponent(s), 0))//opponent has more beans
           {
               return -10000;
           }
           else//tie
           {
               return 0;
           }
       }
    
    //Code that measures how good the board layout is for player turn 's'
    //and returs that value
    if(b.beans(s, 0) > b.totalBeans()/2) //south has more than half of benas in game
        return 1000000000;
    else if(b.beans(s, 0) < b.totalBeans()/2)//north has more than half of benas in game
        return -1000000000;
    
    //otherwise return the difference betwen the pots
    else
        return b.beans(s,0) - b.beans(opponent(s), 0);

}
//------------------------------------------------------------------------//
//              void SmartPlayer::minmax()
//  This function helps determine which is the best move out of the
//  simulated moves. This function recursively calls iteself until no
// more moves can be made.(Either exceeding the time limit or search depth.
//------------------------------------------------------------------------//
void SmartPlayer::minmax(const Board& b, Side s, int& bestHole, int& value, int searchDepth) const
{
    value = (s == NORTH)? 1000000000:-1000000000;//if the side is south, set value to negative infinity, while set value to positive infinity if the side is south
    
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
    {
        //if no move for player exists
        bestHole = -1;
        if(b.beans(SOUTH, 0) > b.beans(NORTH, 0))
            value = 1000000000; //set to positive inifinity if south wins
        if(b.beans(SOUTH, 0) > b.beans(NORTH, 0))
            value = -1000000000; //set to negative infinity if north win
        else
            value = 0; //set value to 0 if there's a tie
        return;
    }
    
    
    if(timer.elapsed() > 4900)
    {
        //cerr << "time limit reached" << endl;
        value = eval(b, s);
        return;
    }
    
    
    if(searchDepth <= 0) //criterion
    {
        //cerr << "searchDepth reached" << endl;
        bestHole = -1;
        value = eval(b, s);
        return;
    }
    
    Side endSide;
    
    
    
    for(int hole = 1; hole <= b.holes(); hole++)
    {
        int endHole;
        
        Board tempB(b);// a temp Board: without moving for real
        if(!tempB.sow(s, hole, endSide, endHole))//if it's sowable, make the move
            continue;
        
        
        if(endHole != 0 && endSide == s && tempB.beans(s, endHole) - 1 == 0 && tempB.beans(opponent(s), endHole) != 0)
        {
            //If it was placed in one of the player's own holes that was empty just a moment before, and if the opponent's hole directly opposite from that hole is not empty, capture the beans.
            tempB.moveToPot(s, endHole, s);
            tempB.moveToPot(opponent(s), endHole, s);
            value+=2;
        }
        
        while(endHole == 0 && s == endSide)
        {
            //if the bean ends at the the player's pot, it gets another turn
            int n;
            do
            {
                n = (rand() % b.holes()) + 1;
            }while(tempB.beans(s, n) == 0 && tempB.beansInPlay(s) != 0 && tempB.beansInPlay(opponent(s)) != 0);
            if(!tempB.sow(s, n, endSide, endHole))
                break;
            value++;
        }
        
        
        if(tempB.beansInPlay(SOUTH) == 0 || tempB.beansInPlay(NORTH) == 0)
        {
            for(int i = 1; i <= b.holes(); i++)
            {
                tempB.moveToPot(s, i, s);
                tempB.moveToPot(opponent(s), i, opponent(s));
            }//move the remaining beans to the pots when the game is over
        }
        
        //Test the opponents coutner move
        int opponentHole, opponentValue;
        
        minmax(tempB, opponent(s), opponentHole, opponentValue, searchDepth - 1); //call oponent's minimax to know its next move
        
        
        
        if(s == SOUTH && value <= opponentValue)//if v2 is better for the player than best seen so far. South will choose the move with large value
        {
            bestHole = hole;
            value = opponentValue;
        }
        
        if(s == NORTH && value >= opponentValue)//if v2 is better for the player than best seen so far. North will choose the move with small value
        {
            bestHole = hole;
            value = opponentValue;
        }
        
    }
    return;
}
