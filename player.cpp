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

}

/*
 * Destructor for the player.
 */
Player::~Player() {
    free(board);
}


/**
 * Sets the reference to the board in testminimax
 */
void Player::setBoard(Board *board_param)
{
    board = board_param;
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
    std::cerr << msLeft << "\n";
    // update the board with the opponent's move
    if (myside == BLACK)
        board->doMove(opponentsMove, WHITE);
    if (myside == WHITE)
        board->doMove(opponentsMove, BLACK);
    
    // find the best move
    Move *move;
    int score;
    std::tie(move, score) = explore_move(board, 3, 
        myside, -10000000, 10000000);
    ////move = nullptr;

    // make the move
    board->doMove(move, myside);
    return move;
}



/**
  * Calculate the token differential board score.
  */
int Player::board_score(Board *board, Side side)
{
    if (side == BLACK)
        return board->countBlack() - board->countWhite();
    
    return board->countWhite() - board->countBlack();
}


/**
  * Helper function to explore the subtree from this board.
  * @param board the board of the current tree position
  * @param depth the depth of the tree so far
  * @param side the current player's side
  * @returns a tuple of the chosen best move and score of the move
  */
std::tuple<Move*, int> Player::explore_move(Board *board, 
    int depth, Side side, int alpha, int beta)
{

    // leaf case end game
    if ( board->isDone() )
    {
        if (board_score(board, side) > 0)
            return std::make_tuple(nullptr, 10000);
        else
            return std::make_tuple(nullptr, -10000);
        return std::make_tuple(nullptr, 0);
    }


    // leaf case end of traversal
    if (depth == 0)
        return std::make_tuple(nullptr, 
            board->weighted_board_score(side));


    // transition cases
    int best_score = -100000;
    Move *best_move = nullptr;
    bool done = false;
    for (int x = 0; x < 8; x++)
    {
        if (done)
            break;

        for (int y = 0; y < 8; y++)
        {
            //if (beta <= alpha)
            //{
            //    done = true;
            //    break;
            //}

            Move *move = new Move(x, y);

            // check if move is legal
            if (board->checkMove(move, side))
            {
                Board *new_board = board->copy();
                new_board->doMove(move, side);
                
                // explore the subtree
                Side other_side = (side == BLACK) ? WHITE : BLACK;
                Move *chosen_move;
                int score;
                std::tie(chosen_move, score) = explore_move(new_board, 
                    depth - 1, other_side, -beta, -alpha);
                score = -score;
                //std::cerr << depth << "explored" << score << " \n";
                
                // if this is the best move we have seen, store it
                //if (score > alpha)
                if  (score > best_score)
                {
                    best_score = score;
                    best_move = move;
                    alpha = score;
                }
            }
        }
    }


    // pass case
    if (best_move == nullptr)
        return std::make_tuple(nullptr, board_score(board, side));

    return std::make_tuple(best_move, best_score);
}

