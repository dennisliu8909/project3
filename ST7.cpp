#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
#include <fstream>

#define ROW 5
#define COL 6

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
ofstream out("debug.data");

class min_Cell{
    private:
        int orbs_num;   // The number of the orbs on the cell 
        int capacity;   // The max number of the orbs of a cell
        char color;     // The color of the cell, 'w'(No one has placed the orbs), 'r'(Red Player) and 'b'(Blue player)
        bool explode;

    public:
        min_Cell() {
            orbs_num = 0, capacity = 8, color='w', explode = false;
        }
        // The basic functions of the Cell
        int get_orbs_num() {
            return this->orbs_num;
        }
        int get_capacity() {
            return this->capacity;
        }
        char get_color() {
            return this->color;
        }
        bool get_explode() {
            return this->explode;
        }

        void set_orbs_num(int orbs) {
            this->orbs_num = orbs;
        }
        void set_capacity(int cap) {
            this->capacity = cap;
        }
        void set_color(char col) {
            this->color = col;
        }
        void set_explode(bool tof) {
            this->explode = tof;
        }
};

class min_Board{
    private:
        min_Cell cells[ROW][COL];                       // The 5*6 board whose index (0,0) is start from the upper left corner
        void cell_reaction_marker() {                // After the explosion, mark all the cell that  will explode in next iteration
            // Mark the next cell whose number of orbs is equal to the capacity
            for(int i = 0; i < ROW; i++){
                    for(int j = 0; j < COL; j++){
                        cell_is_full(&cells[i][j]);
                    }
                }   
        }
        bool cell_is_full(min_Cell* cell) {             // Check whether the cell is full of orbs and set the explosion variable to be true
            if(cell->get_orbs_num() >= cell->get_capacity()){
                cell->set_explode(true);
                return true;
            }
            else return false;
        }
        void add_orb(int i, int j, char color) {     // Add orb into the cell (i, j)
            int orb_num = cells[i][j].get_orbs_num();
            orb_num ++;
            cells[i][j].set_orbs_num(orb_num);
            cells[i][j].set_color(color);
        }
        void cell_reset(int i, int j) {              // Reset the cell to the initial state (color = 'w', orb_num = 0)
            cells[i][j].set_orbs_num(0);
            cells[i][j].set_explode(false);
            cells[i][j].set_color('w');
        }
        void cell_explode(int i, int j) {            // The explosion of cell (i, j). It will add the orb into neighbor cells
            int orb_num;
            char color = cells[i][j].get_color();

            cell_reset(i, j);

            if( i + 1 < ROW){
                add_orb(i+1, j, color);
            }

            if( j + 1 < COL){
                add_orb(i, j+1, color);
            }

            if( i - 1 >= 0){
                add_orb(i-1, j, color);
            }

            if( j - 1 >= 0){
                add_orb(i, j-1, color);
            }
            if( i + 1 < ROW && j - 1 >= 0){
                add_orb(i+1, j-1, color);
            }
            if( i - 1 >= 0 && j - 1 >= 0){
                add_orb(i-1, j-1, color);
            }
            if( i + 1 < ROW && j + 1 < COL){
                add_orb(i+1, j+1, color);
            }
            if( i - 1 >= 0 && j + 1 < COL){
                add_orb(i-1, j+1, color);
            }
        }
        void cell_chain_reaction(char color) {    // If there is aa explosion, check wether the chain reaction is active
            bool chain_reac = true;

            while(chain_reac){

                chain_reac = false;

                for(int i = 0; i < ROW; i++){
                    for(int j = 0; j < COL; j++){
                        if(cells[i][j].get_explode()){
                            cell_explode(i ,j);
                            chain_reac = true;
                        }
                    }
                }

                if(win_the_game(color)){
                    return;
                }

                cell_reaction_marker();
            }
        }
    public:
        min_Board() {
            ////// Initialize the borad with correct capacity //////
            // The corners of the board
            cells[0][0].set_capacity(3), cells[0][5].set_capacity(3),
            cells[4][0].set_capacity(3), cells[4][5].set_capacity(3);

            // The edges of the board
            cells[0][1].set_capacity(5), cells[0][2].set_capacity(5), cells[0][3].set_capacity(5), cells[0][4].set_capacity(5),
            cells[1][0].set_capacity(5), cells[2][0].set_capacity(5), cells[3][0].set_capacity(5),
            cells[1][5].set_capacity(5), cells[2][5].set_capacity(5), cells[3][5].set_capacity(5),
            cells[4][1].set_capacity(5), cells[4][2].set_capacity(5), cells[4][3].set_capacity(5), cells[4][4].set_capacity(5);
        }
        
        // The basic functions of the Board
        int get_orbs_num(int i, int j) {
            return cells[i][j].get_orbs_num();
        }
        int get_capacity(int i, int j) {
            return cells[i][j].get_capacity();
        }
        char get_cell_color(int i, int j) {
            return cells[i][j].get_color();
        }

        void place_orb(int i, int j, char color) {     // Use this function to place a orb into a cell
            if(i >= 0 && i <= 4 && j >= 0 && j <= 5) {
                int temp = cells[i][j].get_orbs_num();
                temp += 1;
                cells[i][j].set_orbs_num(temp);
                cells[i][j].set_color(color);
            }
            /*else{
                player->set_illegal();
                return false;
            }*/

            if(cell_is_full(&cells[i][j])){
                cell_explode(i, j);
                cell_reaction_marker();
                cell_chain_reaction(color);
            }

            //return true;
        }
        void print_current_board(int i, int j, int round) { // Print out the current state of the hole board
            int orb_num;
            char symbol;

            ////// Print out the current state of the board //////
            out << "Round: " << round << endl;
            out << "Place orb on (" << i << ", " << j << ")" << endl;
            out << "=============================================================" << endl;
            for(int i = 0; i < ROW; i++){
                for(int j = 0; j < COL; j++){

                    symbol = cells[i][j].get_color();
                    switch(symbol){
                        case 'r':
                            symbol = 'O';
                            break;
                        case 'b':
                            symbol = 'X';
                            break;
                        default:
                            break;
                    }

                    orb_num = cells[i][j].get_orbs_num();
                    switch(orb_num){
                        case 0:
                            out << "|       | ";
                            break;
                        case 1:
                            out << "|" << symbol << "      | ";
                            break;
                        case 2: 
                            out << "|" << symbol << symbol << "     | ";
                            break;
                        case 3:
                            out << "|" << symbol << symbol << symbol << "    | ";
                            break;
                        case 4:
                            out << "|" << symbol << symbol << symbol << symbol << "   | ";
                            break;
                        case 5:
                            out << "|" << symbol << symbol << symbol << symbol << symbol << "  | ";
                            break;
                        case 6:
                            out << "|" << symbol << symbol << symbol << symbol << symbol << symbol << " | ";
                            break;
                        default:
                            out << "|" << symbol << symbol << symbol << symbol << symbol << symbol << symbol << "| ";
                            break;

                    }
                }
                out << endl;
            }
            out << "=============================================================" << endl << endl;
        }

        bool win_the_game(char player_color) {                  // The function that is used to check wether the player wins the game after his/her placemnet operation
            bool win = true;

            for(int i = 0; i < ROW; i++){
                for(int j = 0; j < COL; j++){
                    if(cells[i][j].get_color() == player_color || cells[i][j].get_color() == 'w') continue;
                    else{
                        win = false;
                        break;
                    }
                }
                if(!win) break;
            }
            return win;
        }
        void set_information(int row, int col, int orb, char color) {
            cells[row][col].set_color(color);
            cells[row][col].set_orbs_num(orb);
        }
        void copy(min_Board &board2) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 6; j++) {
                    board2.set_information(i, j, this->get_orbs_num(i, j), this->get_cell_color(i, j));
                }
            }
        }
        void copy(Board &board) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 6; j++) {
                    this->set_information(i, j, board.get_orbs_num(i, j), board.get_cell_color(i, j));
                }
            }
        }
};

int min(min_Board board_min, int first_depth, int depth, bool maxplayer, char color, char another_color, int a, int b, int method);
bool endgame01(min_Board board_min);
bool inside_range01(int row, int col);
int score01(min_Board board_min, const char &color, const char &another_color);
int score02(min_Board board_min, const char &color, const char &another_color);
int score03(min_Board board_min, const char &color, const char &another_color);
int score04(min_Board board_min, const char &color, const char &another_color);

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////

    int alpha = -99999999, beta = 99999999;
    char color = player.get_color();
    char another_color;
    int orb_num = 0;
    int position = 0;
    min_Board board_min;

    board_min.copy(board);

    // define another_color
    if (color == 'r')
        another_color = 'b';
    else 
        another_color = 'r';

    // get numer of orbs on the board
    for (int i = 0; i < 5; i++) 
        for (int j = 0; j < 6; j++)
            if (board.get_cell_color(i, j) != 'w')
                orb_num += board.get_orbs_num(i, j);
    cout << orb_num << endl;
    if (orb_num <= 20)
        position = min(board_min, 4, 4, true, color, another_color, alpha, beta, 1);
    else if (orb_num <= 80)
        position = min(board_min, 4, 4, true, color, another_color, alpha, beta, 3);
    else 
        position = min(board_min, 5, 5, true, color, another_color, alpha, beta, 4);
    index[0] = position / 10;
    index[1] = position % 10;
    
}
    
int min(min_Board board_min, int first_depth, int depth, bool maxplayer, char color, char another_color, int alpha, int beta, int method) {
    int value_each_time = 0;
    int best_situation;
    int node_origin_value = 120;   // 3 * 5 * 8

    if (depth == 0 || endgame01(board_min)) {
        if (method == 1)
            return score01(board_min, color, another_color);
        else if (method == 2)
            return score02(board_min, color, another_color);
        else if (method == 3)
            return score03(board_min, color, another_color);
        else
            return score03(board_min, color, another_color);
    }
    
    min_Board board2;
    if (maxplayer) {
        board_min.copy(board2);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (board_min.get_cell_color(i, j) != another_color) {
                    board_min.place_orb(i, j, color);
                    value_each_time = min(board_min, first_depth, depth - 1, false, color, another_color, alpha, beta, method);
                    if (alpha < value_each_time) {  // alpha is max in this depth
                        alpha = value_each_time;
                        best_situation = i * 10 + j;
                    }
                    board2.copy(board_min);
                    if (beta <= alpha)
                        return alpha;
                }
                //board_min.print_current_board(i, j, depth * 100 + 10 * i + j);
            }
        }
        if (first_depth == depth) 
            return best_situation;
        else 
            return alpha;
    } else {
        board_min.copy(board2);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (board_min.get_cell_color(i, j) != color) {
                    board_min.place_orb(i, j, another_color);
                    value_each_time =  min(board_min, first_depth, depth - 1, true, color, another_color, alpha, beta, method);
                    if (beta > value_each_time) {   // beta is min in this depth
                        beta = value_each_time;
                        best_situation = i * 10 + j;
                    }
                    board2.copy(board_min);
                    if (beta <= alpha) 
                        return beta;
                }
                //board_min.print_current_board(i, j, depth * 100 + 10 * i + j);
            }
        }
        if (first_depth == depth) 
            return best_situation;
        else 
            return beta;
    }
}

bool endgame01(min_Board board_min) {
    int red_num = 0, blue_num = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (board_min.get_cell_color(i, j) == 'r')
                red_num++;
            else if (board_min.get_cell_color(i, j) == 'b')
                blue_num++;
        }
    }

    if (blue_num + red_num >= 2 && (blue_num == 0 || red_num == 0))
        return true;
    else 
        return false;
}

int score01(min_Board board_min, const char &color, const char &another_color) {
    int node_origin_value = 120;
    int sum = 0;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            if (board_min.get_cell_color(row, col) == color) {
                sum += (board_min.get_orbs_num(row, col) + 3) * (120 / board_min.get_capacity(row, col));
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_range01(row + i, col + j)) {
                            if (board_min.get_cell_color(row + i, col + j) == 'w')
                                sum += 40 / board_min.get_capacity(row + i, col + j);
                            else if (board_min.get_cell_color(row + i, col + j) == color)
                                sum += 15 / board_min.get_capacity(row + i, col + j);
                        }
                    }
                }
            } else if (board_min.get_cell_color(row, col) == another_color) {
                sum -= (board_min.get_orbs_num(row, col) + 3) * (120 / board_min.get_capacity(row, col));
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_range01(row + i, col + j)) {
                            if (board_min.get_cell_color(row + i, col + j) == 'w')
                                sum -= 40 / board_min.get_capacity(row + i, col + j);
                            else if (board_min.get_cell_color(row + i, col + j) == another_color)
                                sum -= 15 / board_min.get_capacity(row + i, col + j);
                        }
                    }
                }
            }
        }
    }
    

    return sum;
}

bool inside_range01(int row, int col) {
    if (row < 0 || row > 4 || col < 0 || col > 5)
        return false;
    else 
        return true;
}

int score02(min_Board board_minimax, const char &color, const char &another_color) {
    int node_origin_value = 120;
    int sum = 0, difference = 8;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            if (board_minimax.get_cell_color(row, col) == color) {
                sum += (board_minimax.get_orbs_num(row, col) + 3) * (120 / board_minimax.get_capacity(row, col));
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_range01(row + i, col + j)) {
                            difference = board_minimax.get_capacity(row + i, col + j) - board_minimax.get_orbs_num(row + i, col + j);
                            if (board_minimax.get_cell_color(row + i, col + j) == color)
                                sum += (13 - difference) * 2 / board_minimax.get_capacity(row + i, col + j);
                            else if (board_minimax.get_cell_color(row + i, col + j) == another_color)
                                sum -= (13 - difference) * 2 / board_minimax.get_capacity(row + i, col + j);
                            difference = 8;
                        }
                    }
                }
            } else if (board_minimax.get_cell_color(row, col) == another_color) {
                sum -= (board_minimax.get_orbs_num(row, col) + 3) * (120 / board_minimax.get_capacity(row, col));
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_range01(row + i, col + j)) {
                            difference = board_minimax.get_capacity(row + i, col + j) - board_minimax.get_orbs_num(row + i, col + j);
                            if (board_minimax.get_cell_color(row + i, col + j) == color)
                                sum += (13 - difference) * 2 / board_minimax.get_capacity(row + i, col + j);
                            else if (board_minimax.get_cell_color(row + i, col + j) == another_color)
                                sum -= (13 - difference) * 2 / board_minimax.get_capacity(row + i, col + j);
                            difference = 8;
                        }
                    }
                }
            }
        }   
    }

    return sum;
}

int score03(min_Board board_min, const char &color, const char &another_color) {
    int node_origin_value = 120;
    int sum = 0, difference = 8;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            if (board_min.get_cell_color(row, col) == color) {
                sum += (board_min.get_orbs_num(row, col) + 3) * (120 / board_min.get_capacity(row, col));
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_range01(row + i, col + j)) {
                            difference = board_min.get_capacity(row + i, col + j) - board_min.get_orbs_num(row + i, col + j);
                            if (difference == 1 && board_min.get_cell_color(row + i, col + j) == color)
                                sum += 60 / board_min.get_capacity(row + i, col + j);
                            else if (difference == 1 && board_min.get_cell_color(row + i, col + j) == another_color)
                                sum -= 60 / board_min.get_capacity(row + i, col + j);
                            difference = 8;
                        }
                    }
                }
            } else if (board_min.get_cell_color(row, col) == another_color) {
                sum -= (board_min.get_orbs_num(row, col) + 3) * (120 / board_min.get_capacity(row, col));
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_range01(row + i, col + j)) {
                            difference = board_min.get_capacity(row + i, col + j) - board_min.get_orbs_num(row + i, col + j);
                            if (difference == 1 && board_min.get_cell_color(row + i, col + j) == color)
                                sum += 60 / board_min.get_capacity(row + i, col + j);
                            else if (difference == 1 && board_min.get_cell_color(row + i, col + j) == another_color)
                                sum -= 60 / board_min.get_capacity(row + i, col + j);
                            difference = 8;
                        }
                    }
                }
            }
        }   
    }

    return sum;
}

int score04(min_Board board_minimax, const char &color, const char &another_color) {
    int node_origin_value = 120;
    int sum = 0;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            if (board_minimax.get_cell_color(row, col) == color) {
                sum += (board_minimax.get_orbs_num(row, col) + 3) * (120 / board_minimax.get_capacity(row, col));
            } else if (board_minimax.get_cell_color(row, col) == another_color) {
                sum -= (board_minimax.get_orbs_num(row, col) + 3) * (120 / board_minimax.get_capacity(row, col));
            } 
        }
    }
    

    return sum;
}