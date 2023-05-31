//
//  main.cpp
//  Project3
//
//  Created by William  Escobar on 5/30/23.
//
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <cassert>
using namespace std;

void doBoardTests()
{
    Board b(3,2);
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
           b.beans(SOUTH, 0) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
           b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, 0) == 2  &&
           b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, 0) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    
    b.setBeans(NORTH, 0, 2);
    b.setBeans(NORTH, 1, 3);
    b.setBeans(NORTH, 2, 3);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 0, 1);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 3);
    b.setBeans(SOUTH, 3, 0);
    
    assert(!b.sow(SOUTH, 0, es, eh) && !b.sow(NORTH, 0, es, eh));//can't sow hole
    assert(!b.sow(SOUTH, 1, es, eh) && !b.sow(SOUTH, 3, es, eh) && !b.sow(NORTH, 3, es, eh));//can't sow empty hole
    
    assert(b.sow(SOUTH, 2, es, eh));
    assert(es == NORTH && eh == 3);
   

    
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3,2);
    b.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n ;//= hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    //assert(n == 1  ||  n == 3);
    //n = bp.chooseMove(b, SOUTH);
    //assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    
    b.setBeans(NORTH, 0, 2);
    b.setBeans(NORTH, 1, 3);
    b.setBeans(NORTH, 2, 3);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 0, 1);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 3);
    b.setBeans(SOUTH, 3, 0);
    
    n = sp.chooseMove(b, SOUTH);
    assert(n == 2); //can only move legally
    n = bp.chooseMove(b, SOUTH);
    assert(n == 2); //can only move legally
    
    b.setBeans(NORTH, 1, 0);
    b.setBeans(NORTH, 2, 0);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 0);
    b.setBeans(SOUTH, 3, 0);
    b.setBeans(NORTH, 0, 2);
    b.setBeans(SOUTH, 0, 2);
    
    n = sp.chooseMove(b, SOUTH);
    assert(n == -1); //while can't move, return -1
    n = bp.chooseMove(b, SOUTH);
    assert(n == -1); //while can't move, return -1

}



int main()
{
        
    
    doBoardTests();
    doPlayerTests();
    
    cout<< "Passed all tests";
}
