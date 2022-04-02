#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int SIZE;

int** initialize_board(int board_size);
void update_board(int marker, int** board, int col, int row);
int validate_move(int col, int row, int** board);
int check_winner(int** board);
void print_state(int** board);
int check_vertical(int** board);
int check_horizontal(int** board);
int check_diagonal(int** board);
int check_vertical_for_col(int** board, int col, int player);
int check_horizontal_for_row(int** board, int row, int player);
int check_diagonal_for_player(int** board, int player);
int check_moveable(int** board);
char* get_moves(const char* filename, int board_size);
void free_board(int** board);

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        int board_size = atoi(argv[2]);
        SIZE = board_size;
        int** board = initialize_board(board_size);
        char* moves = get_moves(argv[1], board_size);
        int player = 1;
        size_t index = 0;
        
        print_state(board);
        while (!check_winner(board) && check_moveable(board))
        {
            printf("%s player moves to %c, %c\n", (tolower(moves[index + 2]) == 'x' ? "First" : "Second"), moves[index], moves[index + 1]);

            int move_row = (moves[index] - 48);
            int move_col = (moves[index + 1] - 48);
            index += 4;

            if (validate_move(move_col, move_row, board))
                update_board(tolower(moves[(index - 4) + 2]) == 'x' ? 1 : 2, board, move_col, move_row);
            
            else
            {
                printf("%s player made an invalid move!\n", (tolower(moves[(index - 4) + 2]) == 'x' ? "First" : "Second"));
                return 1;
            }

            print_state(board);
        }

        if (check_winner(board))
            printf("The winner is the %s player!\n", (tolower(moves[(index - 4) + 2]) == 'x' ? "First" : "Second"));
        
        else
            printf("It's a tie!\n");

        free_board(board);
        free(moves);
        return 0;
    }

    else
    {
        printf("Missing information in the parameter!\nThus the program is now being terminated!\n");
        return 1;
    }
}

int** initialize_board(int board_size)
{
    int** board = calloc(board_size, sizeof(int*));

    for (size_t i = 0; i < board_size + 1; i++)
        board[i] = calloc(board_size, sizeof(int));

    return board;
}

void update_board(int marker, int** board, int col, int row)
{
    board[row][col] = marker;
}

int validate_move(int col, int row, int** board)
{
    return board[row][col] == 0;
}

void print_state(int** board)
{
    for (size_t i = 0; i < SIZE; i++)
    {    
        for (size_t j = 0; j < SIZE; j++)
            printf("%c ", (board[i][j] == 1 ? 'X' : (board[i][j] == 2 ? 'O' : '_')));

        printf("\n");
    }

    printf("\n");
}

int check_winner(int** board)
{
    return (check_vertical(board) || check_horizontal(board) || check_diagonal(board));
}

int check_vertical(int** board)
{
    for (size_t i = 0; i < SIZE; i++)
        if (check_vertical_for_col(board, i, 1))
            return 1;

    for (size_t i = 0; i < SIZE; i++)
        if (check_vertical_for_col(board, i, 2))
            return 1;

    return 0;
}

int check_horizontal(int** board)
{
    for (size_t i = 0; i < SIZE; i++)
        if (check_horizontal_for_row(board, i, 1))
            return 1;

    for (size_t i = 0; i < SIZE; i++)
        if (check_horizontal_for_row(board, i, 2))
            return 1;

    return 0;
}

int check_diagonal(int** board)
{
    return check_diagonal_for_player(board, 1) || check_diagonal_for_player(board, 2);
}

int check_vertical_for_col(int** board, int col, int player)
{
    for (size_t i = 0; i < SIZE; i++)
        if (board[i][col] != player)
            return 0;

    return 1;
}

int check_horizontal_for_row(int** board, int row, int player)
{
    for (size_t i = 0; i < SIZE; i++)
        if (board[row][i] != player)
            return 0;

    return 1;
}

int check_diagonal_for_player(int** board, int player)
{
    int winner = 1;

    for (size_t i = 0; i < SIZE; i++)
        if (board[i][i] != player)
        {
            winner = 0;
            break;
        }

    if (!winner)
    {    
        winner = 1;

        for (size_t i = SIZE - 1, j = 0; i >= 0 && j < SIZE; i--, j++)
            if (board[i][j] != player)
            {
                winner = 0;
                break;
            }
    }

    return winner;
}

int check_moveable(int** board)
{
    for (size_t i = 0; i < SIZE; i++)
        for (size_t j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
                return 1;

    return 0;
}

char* get_moves(const char* filename, int board_size)
{
    FILE* file = fopen(filename, "r");
    char* moves;
    char c;
    size_t i = 0;

    if (file)
    {
        moves = calloc((board_size * board_size * 3) + 1, sizeof(char));

        while ((c = fgetc(file)) != EOF)
            if (isalnum(c) || c == '\n')
                moves[i++] = c;
        
        moves[i] = 0;
        fclose(file);
        
        return moves;
    }

    else
    {
        printf("File '%s' couldn't be found!\nThus the program is now being terminated!\n", filename);
        exit(1);
    }
}

void free_board(int** board)
{
    for (size_t i = 0; i < SIZE; i++)
        free(board[i]);
    
    free(board);
}