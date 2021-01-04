#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

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

char left_color(Board board, int row, int col);
char right_color(Board board, int row, int col);
char up_color(Board board, int row, int col);
char down_color(Board board, int row, int col);
char left_up_color(Board board, int row, int col);
char left_down_color(Board board, int row, int col);
char right_up_color(Board board, int row, int col);
char right_down_color(Board board, int row, int col);
bool inside_the_range(int row, int col);
int check_enermy_arround(Board board, int another_color, int row, int col);

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////

    int row, col;
    char color = player.get_color();
    char another_color;
    int orb_num = 0;

    // define another_color
    if (color == 'r')
        another_color = 'b';
    else 
        another_color = 'r';

    // get numer of orbs on the board
    for (int i = 0; i < 5; i++) 
        for (int j = 0; j < 6; j++)
            if (board.get_cell_color(i, j) != 'w')
                orb_num++;

    if (orb_num <= 16) {
        // pick corner first
        if (board.get_cell_color(0, 0) == 'w' || board.get_cell_color(0, 5) == 'w' || board.get_cell_color(4, 0) == 'w' || board.get_cell_color(4, 5) == 'w') {
            if (board.get_cell_color(0, 0) == 'w') {
                index[0] = 0;
                index[1] = 0;
            } else if (board.get_cell_color(0, 5) == 'w') {
                index[0] = 0;
                index[1] = 5;
            } else if (board.get_cell_color(4, 0) == 'w') {
                index[0] = 4;
                index[1] = 0;
            } else if (board.get_cell_color(4, 5) == 'w') {
                index[0] = 4;
                index[1] = 5;
            }
            return;
        }

        // make one edge controlled by you
        if (board.get_cell_color(0, 2) == 'w' && left_color(board, 0, 2) == 'w' && right_color(board, 0, 2) == 'w'
            && down_color(board, 0, 2) == 'w') {
            index[0] = 0; index[1] = 2;
            return;
        } else if (board.get_cell_color(2, 0) == 'w' && right_color(board, 2, 0) == 'w'
            && up_color(board, 2, 0) == 'w' && down_color(board, 2, 0) == 'w') {
            index[0] = 2; index[1] = 0;
            return;
        } else if (board.get_cell_color(2, 5) == 'w' && left_color(board, 2, 5) == 'w'
            && up_color(board, 2, 5) == 'w' && down_color(board, 2, 5) == 'w') {
            index[0] = 2; index[1] = 5;
            return;
        }else if (board.get_cell_color(4, 3) == 'w' && left_color(board, 4, 3) == 'w' && right_color(board, 4, 3) == 'w'
            && up_color(board, 4, 3) == 'w') {
            index[0] = 4; index[1] = 3;
            return;
        }
    }

    srand(time(NULL)*time(NULL));
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }

    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 6; c++) {
            int situation = check_enermy_arround(board, another_color, r, c) / 100;
            int enermy_row = 0, enermy_col = 0;
            int difference = 0;
            int min = 8;

            if (board.get_cell_color(r, c) == color && situation == 2) {
                index[0] = r; index[1] = c;
                return;
            } else if (board.get_cell_color(r, c) == color && situation == 1) {
                enermy_row = (check_enermy_arround(board, another_color, r, c) / 10) % 10;
                enermy_col = check_enermy_arround(board, another_color, r, c) % 10;
                row = r; col = c;

                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (inside_the_range(r + i, c + j) && i != 0 && j != 0 
                            && board.get_cell_color(r + i, c + j) != another_color) {
                            difference = board.get_capacity(r + i, c + j) - board.get_orbs_num(r + i, c + j);
                            if (difference < min) {
                                min = difference;
                                row = r + i;
                                col = c + j;
                            }
                        }
                    }
                }
                board.print_current_board(row, col, 6666);
            }
        }
    }
    index[0] = row;
    index[1] = col;
}
    
            

int check_enermy_arround(Board board, int another_color, int row, int col) {
    int difference = board.get_capacity(row, col) - board.get_orbs_num(row, col);
    int enermy_difference;
    int situation = 0;

    for (int i= -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (inside_the_range(row + i, col + j) && board.get_cell_color(row + i, col + j) == another_color && i != 0 && j != 0) {
                enermy_difference = board.get_capacity(row + i, col + j) - board.get_orbs_num(row + i, col + j);
                if (difference > enermy_difference)
                    return 300;
                else if (difference == enermy_difference)
                    situation =  200;
                else if (situation < 200 && difference < enermy_difference)
                    situation = 1 * 100 + (row + i) * 10 + col + j;
            }
        }
    }
    return situation;
}
    /*if (left_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row, col - 1) - board.get_orbs_num(row, col - 1);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (right_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row, col + 1) - board.get_orbs_num(row, col + 1);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (up_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row - 1, col) - board.get_orbs_num(row - 1, col);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (down_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row + 1, col) - board.get_orbs_num(row + 1, col);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (left_up_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row - 1, col - 1) - board.get_orbs_num(row - 1, col - 1);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (left_down_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row + 1, col - 1) - board.get_orbs_num(row + 1, col - 1);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (right_up_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row - 1, col + 1) - board.get_orbs_num(row - 1, col + 1);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    if (right_down_color(board, row, col) == another_color) {
        enermy_difference = board.get_capacity(row + 1, col + 1) - board.get_orbs_num(row + 1, col + 1);
        if (difference == enermy_difference)
            return 'A';
        else if (difference < enermy_difference)
            return 'B';
    }
    return 'F';*/

bool inside_the_range(int row, int col) {
    if (row < 0 || row > 4 || col < 0 || col > 5)
        return false;
    else 
        return true;
}

/******************************************************/

char left_color(Board board, int row, int col) {
    if (col == 0)
        return 'x';
    else       
        return board.get_cell_color(row, col - 1);
}

char right_color(Board board, int row, int col) {
    if (col == 5)
        return 'x';
    else       
        return board.get_cell_color(row, col + 1);
}

char up_color(Board board, int row, int col) {
    if (row == 0)
        return 'x';
    else       
        return board.get_cell_color(row - 1, col);
}

char down_color(Board board, int row, int col) {
    if (row == 4)
        return 'x';
    else       
        return board.get_cell_color(row + 1, col);
}

char left_up_color(Board board, int row, int col) {
    if (row == 0 || col == 0) 
        return 'x';
    else 
        return board.get_cell_color(row - 1, col - 1);
}

char right_up_color(Board board, int row, int col) {
    if (row == 0 || col == 5) 
        return 'x';
    else 
        return board.get_cell_color(row - 1, col + 1);
}

char left_down_color(Board board, int row, int col) {
    if (row == 4 || col == 0) 
        return 'x';
    else 
        return board.get_cell_color(row + 1, col - 1);
}

char right_down_color(Board board, int row, int col) {
    if (row == 4 || col == 5) 
        return 'x';
    else 
        return board.get_cell_color(row + 1, col + 1);
}