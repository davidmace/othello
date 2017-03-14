#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

    int position_values[8][8] = 
    {
        { 99,  -8,  8,  6,  6,  8,  -8, 99},
        { -8, -24, -4, -3, -3, -4, -24, -8},
        {  8,  -4,  7,  4,  4,  7,  -4,  8},
        {  6,  -3,  4,  0,  0,  4,  -3,  6},
        {  6,  -3,  4,  0,  0,  4,  -3,  6},
        {  8,  -4,  7,  4,  4,  7,  -4,  8},
        { -8, -24, -4, -3, -3, -4, -24, -8},
        { 99,  -8,  8,  6,  6,  8,  -8, 99}
    };

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    int weighted_board_score(Side side);

    void setBoard(char data[]);
};

#endif
