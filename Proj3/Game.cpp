//
//  Game.cpp
//  Project3
//
//  Created by William  Escobar on 5/27/23.
//

#include <stdio.h>
#include "Game.h"
#include "Player.h"
#include <iomanip>

//------------------------------------------------------------------------//
//                       CONSTRUCTOR
// Construct a Game to be played with the indicated players on a copy of
// the board b. The player on the south side always moves first.
//------------------------------------------------------------------------//
Game::Game(const Board& b, Player* south, Player* north) : m_board(b)
{
    m_south = south;
    m_north = north;
    m_turn = SOUTH; //south always goes first
}

//------------------------------------------------------------------------//
//                      Game::display() const
// Display the game's board in a manner of your choosing, provided you
// show the names of the players and a reasonable representation of the
// state of the board.
//------------------------------------------------------------------------//
void Game::display()const
{
    //************NORTH SIDE STUFF***************//
    //distplay North Side name
    std::cout << std::setw(m_north->name().length() + 15.0 + m_board.holes() ) << m_north->name() << std::endl;
    std::cout << std::endl;
    
    //print North side hole numbers
    std::cout << std::setw(m_north->name().length() + 12.0) << "";
    for(int i = 1; i <= m_board.holes(); i++)
    {
        std::cout << i << "  ";
    }
    std::cout << std::endl;
    
    //print separation line
    std::cout << std::setw(m_north->name().length() + 12.0) << "";
    for(int i = 1; i<= m_board.holes()-1; i++)
    {
        std::cout << "---";
    }
    std::cout<< "-" << std::endl;
    
    //print out the # of beans in each hole on norh side
    std::cout << std::setw(m_north->name().length() + 12.0) << "";
    for(int i=1; i<= m_board.holes(); i++)
    {
        std::cout << m_board.beans(NORTH, i) << "  ";
    }
    std::cout << std::endl;
    
    //************PRINT POTS OF BOTH SIDES***************//
    //print name of North side pot and # of beans in pot
    std::cout << m_north->name() << "'s pot  " << std::setw(2) << std::right << m_board.beans(NORTH, POT) << "  ";
    std::cout << std::setw(m_board.holes() + m_board.holes()*2) << "  ";
    
    std::cout << std::setw(2) << std::left << m_board.beans(SOUTH, POT) << "  " << m_south->name() << "'s pot" << std::endl;
    
    //************SOUTH SIDE STUFF***************//
    //pritn South side holes and # of beans
    std::cout << std::setw(m_north->name().length() + 12.0) << "";
    for(int i=1; i<= m_board.holes(); i++)
    {
        std::cout << m_board.beans(SOUTH, i) << "  ";
    }
    std::cout << std::endl;
    
    //print separation line
    std::cout << std::setw(m_north->name().length() + 12.0) << "";
    for(int i = 1; i<= m_board.holes()-1; i++)
    {
        std::cout << "---";
    }
    std::cout<< "-" << std::endl;
    
    //print South side hole numbers
    std::cout << std::setw(m_north->name().length() + 12.0) << "";
    for(int i = 1; i <= m_board.holes(); i++)
    {
        std::cout << i << "  ";
    }
    std::cout << std::endl;
    
    //distplay South Side name
    std::cout << std::endl;
    std::cout << std::setw(m_north->name().length() + 15.0 + m_board.holes() )<< std::right << m_south->name() << std::endl <<std::endl;
    
    
}

//------------------------------------------------------------------------//
//         Game::status(bool& over, bool& hasWinner, Side& winner)
// If the game is over (i.e., the move member function has been called
// and returned false), set 'over' to true; otherwise, set over to false
// and do not change anything else.

// If the game is over, set 'hasWinner' to true if the game has a winner,
// or false if it resulted in a tie. If hasWinner is set to false, leave
// winner unchanged; otherwise, set it to the winning side.
//------------------------------------------------------------------------//
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    //first check if there are still beans in play
    if(m_board.beansInPlay(m_turn) != 0)
    {
        over = false; //a possible move exists
        return;
    }

    over = true;
    
    //determin if we have a winner
    if(m_board.beans(NORTH, POT) == m_board.beans(SOUTH, POT))
        hasWinner = false;//a tie
    else
    {
        //find which side's has more beans in its pot
        (m_board.beans(NORTH, POT) > m_board.beans(SOUTH, POT) ? winner = NORTH : winner = SOUTH);
        hasWinner = true;
    }
}
//------------------------------------------------------------------------//
//                      bool Game::move(Side s)
// Attempt to make a complete move for the player playing side s.

// "Complete" means that the player sows the seeds from a hole and takes
// any additional turns required or completes a capture.
//
// Whenever the player gets an additional turn, you should display the
// board so someone looking at the screen can follow what's happening. If
// the move can be completed, return true; if not, because the move is not
// yet completed but side s has no holes with beans to pick up and sow,
// sweep any beans in s's opponent's holes into that opponent's pot and
// return false.
//------------------------------------------------------------------------//
bool Game::move(Side s)
{
    //we use these variables make a capture or if we need to take another
    //turn
    Side endSide;
    int endHole;
    
    //check if a move is possible
    if(s == NORTH)
    {
        do
        {
            int choice = m_north->chooseMove(m_board, s);
            if(choice == -1)//no move can be made
            {
                //move all the remaing beans on opposite side to that sides pot
                for(int i=1; i<=m_board.holes(); i++)
                {
                    m_board.moveToPot(SOUTH, i, SOUTH);
                }
                return false;
            }
            else//insert code to make the move
            {
                m_board.sow(s, choice, endSide, endHole);
            }
            
        }while (endSide == s && endHole == 0);//code to handle if another turn is taken
        
    }
    
    else //(s == SOUTH)
    {
        do
        {
            int choice = m_south->chooseMove(m_board, s);
            if(choice == -1)//no move can be made
            {
                //move all the remaing beans on opposite side to that sides pot
                for(int i=1; i<=m_board.holes(); i++)
                {
                    m_board.moveToPot(NORTH, i, NORTH);
                }
                return false;
            }
            else//insert code to make the move
            {
                m_board.sow(s, choice, endSide, endHole);
            }
        
        }while (endSide == s && endHole == 0);//code to handle if another turn is taken
        
    }
    
    //insert code here to handle the capture
    if(endSide == s && m_board.beans(s,endHole) == 1 && m_board.beans(opponent(s), endHole) > 0)
    {
        m_board.moveToPot(s, endHole, s); //add last bean to pot
        m_board.moveToPot(opponent(s), endHole, s); //add beans opposite on opponent side to pot
        std::cout << "A capture was made by " << (s==NORTH ? m_north->name() : m_south->name()) << std::endl;
    }
    display();
    
    return true;
    
}

//------------------------------------------------------------------------//
//                          void Game::play()
// Play the game. Display the progress of the game, provided that someone
// looking at the screen can follow what's happening. If neither player is
// interactive, then to keep the display from quickly scrolling through
// the whole game, it would be reasonable periodically to prompt the viewer
// to press ENTER to continue and not proceed until ENTER is pressed.
//      (The ignore function for input streams is useful here.)
//
// Announce the winner at the end of the game. You can apportion to your
// liking the responsibility for displaying the board between this function
// and the move function.
//------------------------------------------------------------------------//
void Game::play()
{
    //the play() function will keep track of whether the game is over
    //and if there is a winner. These variable are used:
    bool hasWinner = false, over = false;
    Side winner;
    
    //diplay the board and take moves
    display();
    do
    {
        
        if(!m_north->isInteractive() && !m_south->isInteractive())
        {
            
            std::cout << "Press ENTER to continue: ";
            std::cin.ignore();
        }
      
        move(m_turn);
        status(over, hasWinner, winner);
        m_turn = opponent(m_turn);//alternate turns
    } while (!over);
    
    if(hasWinner)
    {
        Player* temp;
        temp  = (winner == NORTH ? m_north : m_south);
        
        std::cout << "The winner is " << temp->name() << "!";
    }
    else
        std::cout << "The game is a tie.";
}
