//
//  Side.h
//  Project3
//
//  Created by William  Escobar on 5/27/23.
//

#ifndef Side_h
#define Side_h
//#include <iostream>
//class Player;
//class Board;

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    Side temp = static_cast<Side>(NSIDES - 1 - s);
    return temp;
}

#endif /* Side_h */
