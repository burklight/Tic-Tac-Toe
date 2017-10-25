#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <algorithm>
#include <limits>
#include <vector>

#define MAX_DEPTH 3
#define POS_INFINITY std::numeric_limits<int>::max()
#define NEG_INFINITY std::numeric_limits<int>::min()

namespace TICTACTOE
{



class Player
{
public:
    ///perform a move
    ///\param pState the current state of the board
    ///\param pDue time before which we must have returned
    ///\return the next state the board is in after our move
    GameState play(const GameState &pState, const Deadline &pDue);
    int minimax(const GameState &pState, const uint8_t player, unsigned int depth);
    GameState findBestMove(const std::vector<GameState> lNextStates, uint8_t player,
      unsigned int depth);
    int evaluate(const GameState &pState, uint8_t player);
};

/*namespace TICTACTOE*/ }

#endif
