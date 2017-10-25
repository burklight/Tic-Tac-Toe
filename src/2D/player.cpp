#include "player.hpp"
#include <cstdlib>

namespace TICTACTOE
{

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
  //std::cerr << "Processing " << pState.toMessage() << std::endl;

  std::vector<GameState> lNextStates;
  pState.findPossibleMoves(lNextStates);

  if(lNextStates.size() == 0) return GameState(pState, Move());

  if(lNextStates.size() == 1) return lNextStates[0];

  return findBestMove(lNextStates, pState.getNextPlayer(), MAX_DEPTH);
}

GameState Player::findBestMove(const std::vector<GameState> lNextStates, uint8_t player,
  unsigned int depth)
{
  if(player == CELL_O)
  {
    //maximize
    GameState bestState;
    int bestPossible = NEG_INFINITY;
    for(unsigned int i = 0; i < lNextStates.size(); i++)
    {
      int v  = minimax(lNextStates[i], CELL_X, depth - 1);
      if(v >= bestPossible)
      {
        bestPossible = v;
        bestState = lNextStates[i];
      }
    }
    return bestState;
  }
  else
  {
    //minimize
    GameState bestState;
    int bestPossible = POS_INFINITY;
    for(unsigned int i = 0; i < lNextStates.size(); i++)
    {
      int v  = minimax(lNextStates[i], CELL_O, depth - 1);
      if(v <= bestPossible)
      {
        bestPossible = v;
        bestState = lNextStates[i];
      }
    }
    return bestState;
  }
}

int Player::minimax(const GameState &pState, const uint8_t player, unsigned int depth)
{
  //if a terminal state has been reached, evaluate it
  if(pState.isEOG())
    return evaluate(pState, player);

  std::vector<GameState> nextStates;
  pState.findPossibleMoves(nextStates);

  if(player == CELL_O)
  {
    int bestPossible = NEG_INFINITY;
    for(unsigned int i = 0; i < nextStates.size(); i++)
      bestPossible = std::max(bestPossible, minimax(nextStates[i], CELL_X, depth - 1));
    return bestPossible;
  }
  else //if player == CELL_X
  {
    int bestPossible = POS_INFINITY;
    for(unsigned int i = 0; i < nextStates.size(); i++)
      bestPossible = std::min(bestPossible, minimax(nextStates[i], CELL_O, depth - 1));
    return bestPossible;
  }
}

int Player::evaluate(const GameState &pState, uint8_t player)
{
  if(pState.isOWin())
    return 1;
  else if(pState.isXWin())
    return -1;
  else
    return 0;
}


/*namespace TICTACTOE*/ }
