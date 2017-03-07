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
    
    // update the board with the opponent's move
    if (myside == BLACK)
        board->doMove(opponentsMove, WHITE);
    if (myside == WHITE)
        board->doMove(opponentsMove, BLACK);
    
    // find the best move
    Move *move;
    int score;
    //std::cerr << "yo\n";
    std::tie(move, score) = explore_move(board, 0, myside);

    // make the move
    //std::cerr << move << "\n";
    board->doMove(move, myside);
    return move;
}

/**
  * Helper function to explore the subtree from this board.
  * @param board the board of the current tree position
  * @param depth the depth of the tree so far
  * @param side the current player's side
  * @returns a tuple of the chosen best move and score of the move
  */
std::tuple<Move*, int> Player::explore_move(Board *board, 
    int depth, Side side)
{

    // leaf case
    if ( board->isDone() || 
        (testingMinimax == true && depth == 2) ||
        (testingMinimax == false && depth == 3) )
    {
        // use coin differential
        // note this is the player's side, not the exploring side
        if (myside == BLACK)
        {
            int score = board->countBlack() - board->countWhite();
            return std::make_tuple(nullptr, score);
        }
        else
        {
            int score = board->countWhite() - board->countBlack();
            return std::make_tuple(nullptr, score);
        }
    }

    // for intermediate cases, loop all move possibilities
    Move *best_move = nullptr;
    int best_score = (depth % 2 == 0) ? -100 : 100;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Move *move = new Move(x, y);

            // check if move is legal
            if (board->checkMove(move, side))
            {
                Board *new_board = board->copy();
                new_board->doMove(move, side);
                
                Side next_side = (side == BLACK) ? WHITE : BLACK;
                Move *chosen_move;
                int score;
                std::tie(chosen_move, score) = explore_move(new_board, 
                    depth + 1, next_side);
                //std::cerr << depth << " " << move->x << " " << move->y << " " << score << "\n";

                // get the max score if player is proponent
                if (side == myside && score > best_score)
                {
                    best_move = move;
                    best_score = score;
                }
                // get the min score if player is opponent
                else if (side != myside && score < best_score)
                {
                    best_move = move;
                    best_score = score;
                }
                free(new_board);

            }
        }
    }

    // explore the pass case if no move is found
    if (best_move == nullptr)
    {
        Board *new_board = board->copy();
        Side next_side = (side == BLACK) ? WHITE : BLACK;
        Move *chosen_move;
        int score;
        std::tie(chosen_move, score) = explore_move(new_board, 
            depth + 1, next_side);

        // get the max score if player is proponent
        if (side == myside && score > best_score)
        {
            best_move = nullptr;
            best_score = score;
        }
        // get the min score if player is opponent
        else if (side != myside && score < best_score)
        {
            best_move = nullptr;
            best_score = score;
        }
        free(new_board);
    }

    // return the pair of move and score for that move
    return std::make_tuple(best_move, best_score);
    
}

