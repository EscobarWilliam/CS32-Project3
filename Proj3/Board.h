//
//  Board.h
//  Project3
//
//  Created by William  Escobar on 5/27/23.
//

#ifndef Board_h
#define Board_h
#include "Side.h"
#include <vector>

class Board
{
    public:
        Board(int nHoles, int nInitialBeansPerHole);
        int holes() const;
        int beans(Side s, int hole) const;
        int beansInPlay(Side s) const;
        int totalBeans() const;
        bool sow(Side s, int hole, Side& endSide, int& endHole);
        bool moveToPot(Side s, int hole, Side potOwner);
        bool setBeans(Side s, int hole, int beans);

    private:
        int m_nHoles;//holds the # of holes per side of the board
        int m_beans;// holds the # of beans each hole will begin with
    
    //these vectors will be of the length of m_nHoles and each element holds the #
    //of beans per hole. The vector element 0 is the pot for that side
    std::vector<int> northSide;
    std::vector<int> southSide;
};

#endif /* Board_h */
