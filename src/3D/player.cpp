#include "player.hpp"
#include <cstdlib>

namespace TICTACTOE3D
{

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if(lNextStates.size() == 0) return GameState(pState, Move());

    if(lNextStates.size() == 1) return lNextStates[0];

    return findBestMove(lNextStates, pState.getNextPlayer(), MAX_DEPTH);
}

void Player::orderStates(std::vector<GameState> &lNextStates,
  uint8_t player)
{
  unsigned int nStates = lNextStates.size();
  if(nStates == 1 || nStates == 0) return;
  std::vector<int> lNextStatesScores(nStates);
  for(unsigned int i = 0; i < nStates - 1; i++)
    lNextStatesScores[i] = evaluate(lNextStates[i], player);
  quickSortStates(lNextStates, lNextStatesScores, 0, nStates - 1);
}

void Player::quickSortStates(std::vector<GameState> &lNextStates,
  std::vector<int> &lNextStatesScores, unsigned int low, unsigned int high)
{
  if(lNextStates.size() == 1 || lNextStates.size() == 0) return;
  if(low < high)
  {
    unsigned int p = partitionStates(lNextStates, lNextStatesScores, low, high);
    quickSortStates(lNextStates, lNextStatesScores, low, p);
    quickSortStates(lNextStates, lNextStatesScores, p + 1, high);
  }
}

unsigned int Player::partitionStates(std::vector<GameState> &lNextStates,
  std::vector<int> &lNextStatesScores, unsigned int low, unsigned int high)
{
  int pivot = lNextStatesScores[low];
  unsigned int i = low;
  for(unsigned int j = low + 1; j < high; j++)
  {
    if(lNextStatesScores[j] <= pivot)
    {
      i++;
      int tmpScore = lNextStatesScores[i];
      lNextStatesScores[i] = lNextStatesScores[j];
      lNextStatesScores[j] = tmpScore;
      GameState tmpState = lNextStates[i];
      lNextStates[i] = lNextStates[j];
      lNextStates[j] = tmpState;
    }
  }
  int tmpScore = lNextStatesScores[i];
  lNextStatesScores[i] = lNextStatesScores[low];
  lNextStatesScores[low] = tmpScore;
  GameState tmpState = lNextStates[i];
  lNextStates[i] = lNextStates[low];
  lNextStates[low] = tmpState;

  return i;
}

GameState Player::findBestMove(std::vector<GameState> lNextStates, uint8_t player,
  unsigned int depth)
{
  int alpha = NEG_INFINITY;
  int beta = POS_INFINITY;

  // We order the states in increasing order according to the heuristic
  //orderStates(lNextStates, player);

  if(player == CELL_O)
  {
    //maximize
    GameState bestState;
    int bestPossible = NEG_INFINITY;
    // We go backwards from the states to get the most important first
    for(int i = lNextStates.size() - 1 ; i >= 0; i--)
    {
      //std::cerr << "Doing O alpha beta " << i << " \n";
      int v  = alphaBeta(lNextStates[i], CELL_X, alpha, beta, depth - 1);
      if(v >= bestPossible)
      {
        bestPossible = v;
        bestState = lNextStates[i];
      }
      alpha = std::max(alpha, v);
    }
    return bestState;
  }
  else
  {
    //minimize
    GameState bestState;
    int bestPossible = POS_INFINITY;
    // We go forwards from the states to get the most importante (less heuristic)
    // first
    for(unsigned int i = 0; i < lNextStates.size(); i++)
    {
      //std::cerr << "Doing X alpha beta " << i << " \n";
      int v  = alphaBeta(lNextStates[i], CELL_O, alpha, beta, depth - 1);
      if(v <= bestPossible)
      {
        bestPossible = v;
        bestState = lNextStates[i];
      }
      beta = std::min(beta, v);
    }
    return bestState;
  }
}

int Player::alphaBeta(const GameState &pState, const uint8_t player, int alpha,
  int beta, unsigned int depth)
{
  //if a terminal state has been reached, evaluate it
  if(pState.isEOG() || depth == 0)
    return evaluate(pState, player);

  std::vector<GameState> nextStates;
  pState.findPossibleMoves(nextStates);
  //std::cerr << "Hello \n";

  // We order the states in increasing order according to the heuristic
  //orderStates(nextStates, player);

  if(player == CELL_O)
  {
    int bestPossible = NEG_INFINITY;
    // We go backwards from the states to get the most important first
    for(int i = nextStates.size() - 1 ; i >= 0; --i)
    {
      //std::cerr << "Doing O alpha beta " << i << " \n";
      bestPossible = std::max(bestPossible, alphaBeta(nextStates[i], CELL_X,
        alpha, beta,  depth - 1));
      alpha = std::max(alpha, bestPossible);
      if(beta <= alpha)
        return bestPossible;
    }
    return bestPossible;
  }
  else //if player == CELL_X
  {
    int bestPossible = POS_INFINITY;
    // We go forwards from the states to get the most importante (less heuristic)
    // first
    for(unsigned int i = 0; i < nextStates.size(); i++)
    {
      //std::cerr << "Doing X alpha beta " << i << " \n";
      bestPossible = std::min(bestPossible, alphaBeta(nextStates[i], CELL_O,
        alpha, beta, depth - 1));
      beta = std::min(beta, bestPossible);
      if(beta <= alpha)
        return bestPossible;
    }
    return bestPossible;
  }
}

int Player::evaluate(const GameState &pState, uint8_t player)
{
  if(pState.isOWin())
    return POS_INFINITY;
  else if(pState.isXWin())
    return NEG_INFINITY;
  else if(pState.isDraw())
    return 0;
  else
  {
    int score = 0;
    for(unsigned int i = 0; i < WAYS_TO_WIN; i++)
    {
      unsigned int nOPieces = 0;
      unsigned int nXPieces = 0;

      for(unsigned int j = 0; j < SIZE; j++)
      {
        int piece = pState.at(victories[i][j]);
        if(piece == CELL_O)
          nOPieces++;
        else if(piece == CELL_X)
          nXPieces++;
      }

      score += heuristic[nOPieces][nXPieces];
    }

    return score;
  }
}


} /*namespace TICTACTOE3D*/
