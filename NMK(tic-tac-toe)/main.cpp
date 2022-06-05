#include <iostream>
#include <cstring>
#include "SingleLinkedList.h"

#define MAX_COMMAND 32
#define INF 1000    //values for minimax
#define MININF -1000    //-||-
#define VICTORYPLAYER1 1
#define VICTORYPLAYER2 -1
#define TIE 0
#define GAMENOTOVER 3
#define NEITHERWON 2
#define PLAYER1 '1'
#define PLAYER2 '2'
#define EMPTY   '0'
#define SPACE ' '

void printBoard(char **board, int number_of_rows, int number_of_columns) {  //function for printing the whole representation of the board
    for (int i = 0; i < number_of_rows; i++) {
        for (int j = 0; j < number_of_columns; j++) {
            std::cout << board[i][j] << SPACE;
        }
        std::cout << std::endl;
    }
}

bool checkInRightDirection(char **board, int number_of_columns, int value_to_cross, char player, int x, int y) {
    // function checks whether the player has crossed the correct value in east direction
    int count = 0;
    for (int i = 0; y + i < number_of_columns; i++) {
        if (board[x][y + i] != player)
            return false;

        count++;
        if (count == value_to_cross)
            return true;
    }
    return false;
}

bool checkInRightDownDirection(char **board, int number_of_rows, int number_of_columns, int value_to_cross, char player,
                               int x, int y) {
    // function checks whether the player has crossed the correct value in the south-east direction
    int count = 0;
    for (int i = 0; x + i < number_of_rows && y + i < number_of_columns; i++) {
        if (board[x + i][y + i] != player)
            return false;

        count++;
        if (count == value_to_cross)
            return true;
    }
    return false;
}

bool checkInDownDirection(char **board, int number_of_rows, int value_to_cross, char player, int x, int y) {
    // function checks whether the player has crossed the correct value in south direction
    int count = 0;
    for (int i = 0; x + i < number_of_rows; i++) {
        if (board[x + i][y] != player)
            return false;

        count++;
        if (count == value_to_cross)
            return true;
    }
    return false;
}

bool checkInLeftDownDirection(char **board, int number_of_rows, int value_to_cross, char player, int x, int y) {
    // function checks whether the player has crossed the correct value in west direction
    int count = 0;
    for (int i = 0; x + i < number_of_rows && y - i >= 0; i++) {
        if (board[x + i][y - i] != player)
            return false;

        count++;
        if (count == value_to_cross)
            return true;
    }
    return false;
}

bool checkAllDirections(char **board, int number_of_rows, int number_of_columns, int value_to_cross, char player,
                        int i, int j) {  //function checks all the necessary directions to win
    if (checkInRightDirection(board, number_of_columns, value_to_cross, player, i, j))
        return true;
    if (checkInRightDownDirection(board, number_of_rows, number_of_columns, value_to_cross, player, i, j))
        return true;
    if (checkInDownDirection(board, number_of_rows, value_to_cross, player, i, j))
        return true;
    if (checkInLeftDownDirection(board, number_of_rows, value_to_cross, player, i, j))
        return true;
    return false;
}

bool checkIfWon(char **board, int number_of_rows, int number_of_columns, int value_to_cross, char player) {
    //function checks for each cell whether the provided player won
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] == player)
                if(checkAllDirections(board, number_of_rows, number_of_columns, value_to_cross, player, i, j))
                    return true;
    return false;
}

int whoWon(char **board, int number_of_rows, int number_of_columns, int value_to_cross) {
    //similar function to check if won, but this one returns the value similar to minimax
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] != EMPTY) {
                if (board[i][j] == PLAYER1)
                    if (checkAllDirections(board, number_of_rows, number_of_columns, value_to_cross, PLAYER1, i, j))
                        return VICTORYPLAYER1;
                if (checkAllDirections(board, number_of_rows, number_of_columns, value_to_cross, PLAYER2, i, j))
                    return VICTORYPLAYER2;
            }
    return NEITHERWON;
}

int countPossibleMoves(char **board, int number_of_rows, int number_of_columns) {
    //function returns number of empty fields (possiblemoves)
    int count = 0;
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] == EMPTY)
                count++;
    return count;
}

int stateOfGame(char **board, int number_of_rows, int number_of_columns, int value_to_cross, int count) {
    // function returns the correct state of game, for minimax and SOLVE_GAME_STATE
    int winCondition = whoWon(board, number_of_rows, number_of_columns, value_to_cross);
    if (count != 0 && winCondition != VICTORYPLAYER1 && winCondition != VICTORYPLAYER2)
        return GAMENOTOVER;
    if (winCondition == VICTORYPLAYER1)
        return VICTORYPLAYER1;
    if (winCondition == VICTORYPLAYER2)
        return VICTORYPLAYER2;
    return TIE;
}

void announceEndOfGame(int state) { //function for announcing the ending state of the game
    if (state == VICTORYPLAYER1) {
        std::cout << "FIRST_PLAYER_WINS\n";
        return;
    }

    if (state == VICTORYPLAYER2) {
        std::cout << "SECOND_PLAYER_WINS\n";
        return;
    }

    std::cout << "BOTH_PLAYERS_TIE\n";
}

SingleLinkedList *generatePossibleMoves(char **board, int number_of_rows, int number_of_columns, int value_to_cross,
                                        int depth, bool isMaximizing) {
    //function returns a list with nodes that contain the coordinates of empty fields,
    // if it finds a winning move it returns just a list with this single node
    auto *possibleMoves = new SingleLinkedList();
    int winCondition;
    isMaximizing ? winCondition = VICTORYPLAYER1 : winCondition = VICTORYPLAYER2;
    char active_player;
    isMaximizing ? active_player = PLAYER1 : active_player = PLAYER2;
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] == EMPTY) {
                board[i][j] = active_player;
                if (stateOfGame(board, number_of_rows, number_of_columns, value_to_cross, depth) == winCondition) {
                    auto *winningMove = new SingleLinkedList();
                    Node *node = new Node(i, j);
                    winningMove->insertNodeAfter(node);
                    delete possibleMoves;
                    return winningMove;
                }
                board[i][j] = EMPTY;
                Node *node = new Node(i, j);
                possibleMoves->insertNodeAfter(node);
            }
    return possibleMoves;
}

int minimax(char **&board, int number_of_rows, int number_of_columns, int value_to_cross, int alpha,
            int beta, int depth, bool isMaximizing) {   //function alternated for the case of nmk
    int score = stateOfGame(board, number_of_rows, number_of_columns, value_to_cross, depth), best, value;
    if (score != GAMENOTOVER)
        return score;
    SingleLinkedList *possibleMoves = generatePossibleMoves(board, number_of_rows, number_of_columns, value_to_cross,
                                                            depth, isMaximizing);
    Node *temp = possibleMoves->head;
    if (isMaximizing) {
        best = MININF;
        for (int i = 0; i < possibleMoves->length; i++) {
            int x = temp->x, y = temp->y;
            temp = temp->next;
            board[x][y] = PLAYER1;
            value = minimax(board, number_of_rows, number_of_columns, value_to_cross, alpha, beta,
                            depth - 1,false);
            board[x][y] = EMPTY;
            if (value == VICTORYPLAYER1) {
                delete possibleMoves;
                return VICTORYPLAYER1;
            }
            if (value > best)
                best = value;
            if (value > alpha)
                alpha = value;
            if (beta <= alpha)
                break;
        }
        delete possibleMoves;
        return best;
    }
    best = INF;
    for (int i = 0; i < possibleMoves->length; i++) {
        int x = temp->x, y = temp->y;
        temp = temp->next;
        board[x][y] = PLAYER2;
        value = minimax(board, number_of_rows, number_of_columns, value_to_cross, alpha, beta,
                        depth - 1, true);
        board[x][y] = EMPTY;
        if (value == VICTORYPLAYER2) {
            delete possibleMoves;
            return VICTORYPLAYER2;
        }
        if (value < best)
            best = value;
        if (value < beta)
            beta = value;
        if (beta <= alpha)
            break;
    }
    delete possibleMoves;
    return best;
}

void GEN_ALL_POS_MOV(char **board, int number_of_rows, int number_of_columns, int value_to_cross, char active_player,
                     char inactive_player) {    //function for the first part of the project
    int feasible_moves = 0;
    if (checkIfWon(board, number_of_rows, number_of_columns, value_to_cross, inactive_player)) {
        std::cout << EMPTY << std::endl;
        return;
    }
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] == EMPTY)
                feasible_moves++;
    if (feasible_moves == 0) {
        std::cout << feasible_moves << std::endl;
        return;
    }
    std::cout << feasible_moves << std::endl;
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] == EMPTY) {
                board[i][j] = active_player;
                printBoard(board, number_of_rows, number_of_columns);
                board[i][j] = EMPTY;
            }
}

void GEN_ALL_POS_MOV_CUT_IF_GAME_OVER(char **board, int number_of_rows, int number_of_columns, int value_to_cross,
                                      char active_player, char inactive_player) {   //function for the second part of the project
    if (checkIfWon(board, number_of_rows, number_of_columns, value_to_cross, inactive_player)) {
        std::cout << EMPTY << std::endl;
        return;
    }
    for (int i = 0; i < number_of_rows; i++)
        for (int j = 0; j < number_of_columns; j++)
            if (board[i][j] == EMPTY) {
                board[i][j] = active_player;
                if (checkIfWon(board, number_of_rows, number_of_columns, value_to_cross, active_player)) {
                    std::cout << PLAYER1 << std::endl;
                    printBoard(board, number_of_rows, number_of_columns);
                    return;
                }
                board[i][j] = EMPTY;
            }
    GEN_ALL_POS_MOV(board, number_of_rows, number_of_columns, value_to_cross, active_player, inactive_player);
}

void SOLVE_GAME_STATE(char **board, int number_of_rows, int number_of_columns, int value_to_cross, char active_player) {
    //function for the third part of the project
    int depth = countPossibleMoves(board, number_of_rows, number_of_columns);
    int state = stateOfGame(board, number_of_rows, number_of_columns, value_to_cross, depth);
    if (state != GAMENOTOVER) {
        announceEndOfGame(state);
        return;
    }
    if (active_player == PLAYER1)
        state = minimax(board, number_of_rows, number_of_columns, value_to_cross, MININF, INF, depth, true);
    else
        state = minimax(board, number_of_rows, number_of_columns, value_to_cross, MININF, INF, depth, false);
    announceEndOfGame(state);
}

int main() {
    char command[MAX_COMMAND];
    for (int i = 0; i < MAX_COMMAND; i++)
        command[i] = EMPTY;
    int number_of_rows, number_of_columns, value_to_cross, player; //N,M,K, whose turn it is

    while (!std::cin.eof()) {
        std::cin >> command >> number_of_rows >> number_of_columns >> value_to_cross >> player;
        char **board = new char *[number_of_rows];
        for (int i = 0; i < number_of_rows; i++) {
            board[i] = new char[number_of_columns];
            for (int j = 0; j < number_of_columns; j++)
                std::cin >> board[i][j];
        }

        char active_player, inactive_player;
        player == 1 ? inactive_player = PLAYER2 : inactive_player = PLAYER1;
        player == 1 ? active_player = PLAYER1 : active_player = PLAYER2;

        if (!strcmp(command, "GEN_ALL_POS_MOV"))
            GEN_ALL_POS_MOV(board, number_of_rows, number_of_columns, value_to_cross, active_player, inactive_player);

        else if (!strcmp(command, "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER"))
            GEN_ALL_POS_MOV_CUT_IF_GAME_OVER(board, number_of_rows, number_of_columns, value_to_cross, active_player,
                                             inactive_player);

        else if (!strcmp(command, "SOLVE_GAME_STATE"))
            SOLVE_GAME_STATE(board, number_of_rows, number_of_columns, value_to_cross, active_player);

        for (int i = 0; i < number_of_rows; i++)
            delete[] board[i];

        delete[] board;
    }
    return 0;
}