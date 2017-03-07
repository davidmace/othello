#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    std::cerr << "init\n";
    board = new Board();
    myside = side;
    //board = new int[8][8];
    //for(int x = 0; x < 8; x++)
    //    for(int y = 0; y < 8; y++)
    //        board[x][y] = 0;
    //board[3][3] = 1;
    //board[4][4] = 1;
    //board[3][4] = 2;
    //board[4][3] = 2;

}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    if (myside == BLACK)
        board->doMove(opponentsMove, WHITE);
    if (myside == WHITE)
        board->doMove(opponentsMove, BLACK);
    
    bool found = false;
    Move *best_move;
    int best_score = -100;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Move *move = new Move(x, y);
            if (board->checkMove(move, myside))
            {
                Board *new_board = board->copy();
                new_board->doMove(move, myside);
                int score = board->count(myside);
                
                //done = true;
                if (score > best_score)
                {
                    best_move = move;
                    best_score = score;
                    std::cerr << x << " " << y << " " << score << "\n";
                    found = true;
                }
            }
            //free(move);
        }
    }

    if (found == false)
        return nullptr;

    board->doMove(best_move, myside);
    return best_move;
}
