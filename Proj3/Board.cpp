
//  Board.cpp
//  Project3

//  Created by William  Escobar on 5/27/23.


#include <iostream>
#include "Board.h"
#include "Side.h"

//------------------------------------------------------------------------//
//                       CONSTRUCTOR
// Construct a Board with the indicated number of holes per side (not
// counting the pot) and initial number of beans per hole. If nHoles is
// not positive, act as if it were 1; if nInitialBeansPerHole is negative,
// act as if it were 0.
//------------------------------------------------------------------------//
Board::Board(int nHoles, int nInitialBeansPerHole) :m_nHoles(nHoles), m_beans(nInitialBeansPerHole)
{

    if(nHoles<0)
    {
        nHoles = 1;
        m_nHoles = nHoles;
    }
    if(nInitialBeansPerHole < 0)
    {
        nInitialBeansPerHole = 0;
        m_beans = nInitialBeansPerHole;
    }

    //set the pots to have a value of zero
    northSide.push_back(0);
    southSide.push_back(0);

    //now pouulate the holes on each side with nInitialBeansPerHole
    for(int i=1; i<=nHoles; i++)
    {
        northSide.push_back(nInitialBeansPerHole);
        southSide.push_back(nInitialBeansPerHole);
    }
}
//------------------------------------------------------------------------//
//                      Board::holes()
// Return the number of holes on a side (not counting the pot).
//------------------------------------------------------------------------//
int Board::holes() const
{
    return m_nHoles;
}
//------------------------------------------------------------------------//
//                      Board::beans(Side s, int hole)
// Return the number of beans in the indicated hole or pot, or −1 if the
// hole number is invalid.
//------------------------------------------------------------------------//
int Board::beans(Side s, int hole) const
{
    if(s == NORTH && (hole >= 0 && hole <= m_nHoles))
    {
        return northSide[hole];
    }

    if(s == SOUTH && (hole >= 0 && hole <= m_nHoles))
    {
        return southSide[hole];
    }
    return -1;
}
//------------------------------------------------------------------------//
//                      Board::beansInPlay(Side s)
// Return the total number of beans in all the holes on the indicated
// side, not counting the beans in the pot.
//------------------------------------------------------------------------//
int Board::beansInPlay(Side s) const
{
    if(s == NORTH)
    {
        int count = 0;
        for(int i=1;i<=m_nHoles; i++)//start at 1 since element 0 is the pot
        {
            count += northSide[i];
        }
        return count;
    }

    else//(s == SOUTH)
    {
        int count = 0;
        for(int i=1;i<= m_nHoles; i++)//start at 1 since element 0 is the pot
        {
            count += southSide[i];
        }
        return count;
    }
}
//------------------------------------------------------------------------//
//                      Board::totalBeans()
// Return the total number of beans in the game, including any in the pots.
//------------------------------------------------------------------------//
int Board::totalBeans() const
{
    //return m_beans * m_nHoles; //equal to the #holes * #beans per hole

    //alternate code since the setBeans() makes this difficult
        int count = 0;
        for(int i=0;i<=m_nHoles; i++)
        {
            count += northSide[i];
        }

        for(int i=0;i<=m_nHoles; i++)
        {
            count += southSide[i];
        }
        return count;
}
//------------------------------------------------------------------------//
//        Board::sow(Side s, int hole, Side& endSide, int& endHole)
// If the hole indicated by (s,hole) is empty or invalid or a pot, this
// function returns false without changing anything.
//
//Otherwise, it will return true after sowing the beans:
//  -the beans are removed from hole (s,hole) and sown counterclockwise,
//   including s's pot if encountered
//  -Skip s's opponent's pot.
//  -The function sets the parameters endSide and endHole to the side and
//   hole where the last bean was placed.
//
// (This function does not make captures or multiple turns; different Kalah
// variants have different rules about these issues, so dealing with them
// should not be the responsibility of the Board class.)
//------------------------------------------------------------------------//
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    //validate that (s, hole) is valid
    int temp = beans(s, hole);
    if(temp == -1 || temp == 0 || hole == 0)
        return false;

    //pick up seeds from hole
    (s == NORTH ? northSide[hole] = 0 : southSide[hole] = 0);

    //mark our end positions as our starting positions
    endHole = hole;
    endSide = s;

    //start on S side and go counterclockwise
    for(; temp!=0; temp--)
    {
        //we first need to advance to the next hole after picking up beans

        if(endHole == 0) //if we are currently in a pot, advance to the opponent's side
        {
            if(endSide == SOUTH)
            {
                endHole = m_nHoles;
            }
            else
            {
                endHole = 1;
            }
            endSide = opponent(endSide);//swap sides
        }

        else if(endHole == m_nHoles && endSide == SOUTH) //if we are at the end hole of the south, advance to south's pot
        {
            if(s == SOUTH)//south side's move
                endHole = 0;
            else//north side's move
            {
                endHole = m_nHoles; //if we originally are on the opposite side, skip the south's pot.
                endSide = opponent(endSide);//swap sides
            }

        }

        else if(endHole == 1 && endSide == NORTH) // if we are at the end hole of the north, advance to north's pot
        {
            if(s == NORTH) //north side's turn
                endHole = 0;
            else//south side's turn
            {
                endHole = 1; //if we originally are on the opposite side, skip the hole.
                endSide = opponent(endSide);//swap sides
            }
        }

        else
        {
            endHole += (endSide == NORTH)? -1:1; //in all other cases, decrement if it were north, or increment of on south
        }

        if(endSide == NORTH)
            northSide[endHole]++;
        else
            southSide[endHole]++;
    }
    return true;

}

//------------------------------------------------------------------------//
//            Board::moveToPot(Side s, int hole, Side potOwner)
// If the indicated hole is invalid or a pot, return false without
// changing anything. Otherwise, move all the beans in hole (s,hole) into
// the pot belonging to potOwner and return true.
//------------------------------------------------------------------------//
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole <= 0 || hole > m_nHoles) //either a pot or some invalid hole
        return false;
    if(s == NORTH)
    {
        int temp  = northSide[hole];
        northSide[hole] = 0; //empty the hole

        if(potOwner == NORTH)
        {
            northSide[0] += temp;
        }
        else
            southSide[0] += temp;
        return true;
    }

    if(s == SOUTH)
    {
        int temp = southSide[hole];
        southSide[hole] = 0;
        if(potOwner == NORTH)
        {
            northSide[0] += temp;
        }
        else
            southSide[0] += temp;
        return true;
    }

    return false; // if this executes, something else went wrong
}
//------------------------------------------------------------------------//
//            Board::setBeans(Side s, int hole, int beans)
// If the indicated hole is invalid or beans is negative, this function
// returns false without changing anything. Otherwise, it will return true
// after setting the number of beans in the indicated hole or pot to the
// value of the third parameter. (This could change what beansInPlay and
// totalBeans return if they are called later.) This function exists solely
// so that we and you can more easily test your program: None of your code
// that implements the member functions of any class is allowed to call this
// function directly or indirectly. (We'll show an example of its use below.)
//------------------------------------------------------------------------//
bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole > m_nHoles || hole <= 0 || beans < 0)
        return false;
    if(s == NORTH)
    {
        northSide[hole] = beans;
    }
    else //s == SOUTH
        southSide[hole] = beans;
    return true;
}

