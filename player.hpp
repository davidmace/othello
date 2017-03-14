#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <tuple>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    void setBoard(Board *board_param);
    std::tuple<Move*, int> explore_move(Board *board, 
        int depth, Side side, int alpha, int beta);
    Move *doMove(Move *opponentsMove, int msLeft);
    int board_score(Board *board, Side side);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side myside;
    Board *board;

};

#endif
