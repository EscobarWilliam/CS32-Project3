//
//  Player.h
//  Project3
//
//  Created by William  Escobar on 5/27/23.
//

#ifndef Player_h
#define Player_h
#include "Side.h"

#include <iostream>
class Board;


class Player
{

    public:
        Player(std::string name):m_pName(name){}
        std::string name() const {return m_pName;}
        virtual bool isInteractive() const {return false;}
        virtual int chooseMove(const Board& b, Side s) const = 0;
        virtual ~Player(){}

    private:
        std::string m_pName;
};

class BadPlayer : public Player
{
    public:
        BadPlayer(std::string name):Player(name){}
        virtual bool isInteractive(){return false;}
        virtual int chooseMove(const Board& b, Side s) const;
        
        virtual ~BadPlayer(){}
};

class HumanPlayer : public Player
{
    public:
        HumanPlayer(std::string name):Player(name){}
        virtual bool isInteractive() const {return true;}
        virtual int chooseMove(const Board&b, Side s) const;
        
        virtual ~HumanPlayer(){}
};

class SmartPlayer : public Player
{
    public:
        SmartPlayer(std::string name):Player(name){}
        virtual bool isInteractive() const {return false;}
        virtual int chooseMove(const Board& b, Side s) const;
        
        virtual ~SmartPlayer(){}
    
        int eval(const Board& b, Side s) const;
        void minmax(const Board& b, Side s, int& bestHole, int& value, int searchDepth) const;
        bool simulateSow(Board& copy_b, int hole, Side s) const;
};
    
    


#endif /* Player_h */

