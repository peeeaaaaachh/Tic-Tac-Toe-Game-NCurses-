#include <stdio.h> 
#include <ncurses.h>
#include <locale.h>
#define SIZE 60
#define CELLS 3
#define WIDTH 1
#define HEIGHT 1

void draw_grid(char board[CELLS][CELLS],int sel_y, int sel_x);
void ncurses(); 
void game_loop(int total_height, int total_width);
int draw_symbols(int x, int y, int sel_x, int sel_y); 
int win (char board[CELLS][CELLS], int sel_x, int sel_y, int total_height, int total_width);


int main()
{
    int total_width = CELLS * WIDTH + CELLS + 1;
    int total_height = CELLS * HEIGHT + CELLS + 1;
    ncurses(); 
    game_loop(total_height, total_width);
    endwin (); 
    return 0; 
}

void ncurses (){
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color(); 
    use_default_colors();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
}

void draw_grid(char board[CELLS][CELLS], int sel_x, int sel_y) {
    int total_width = CELLS * WIDTH + CELLS + 1;
    int total_height = CELLS * HEIGHT + CELLS + 1;

    for (int y = 0; y < total_height; y++) {
        for (int x = 0; x < total_width; x++) {
            int cell_x = x / (WIDTH + 1);
            int cell_y = y / (HEIGHT + 1);
            int highlight = (cell_x == sel_x && cell_y == sel_y);

            if (y % (HEIGHT + 1) == 0) {
                if (x % (WIDTH + 1) == 0) {
                    if (y == 0 && x == 0) printw("┌");
                    else if (y == 0 && x == total_width - 1) printw("┐");
                    else if (y == total_height - 1 && x == 0) printw("└");
                    else if (y == total_height - 1 && x == total_width - 1) printw("┘");
                    else if (y == 0) printw("┬");
                    else if (y == total_height - 1) printw("┴");
                    else if (x == 0) printw("├");
                    else if (x == total_width - 1) printw("┤");
                    else printw("┼");
                } else {
                    printw("─");
                }
            } else if (x % (WIDTH + 1) == 0) {
                printw("│");
            } 
            else {
                if (highlight) attron(COLOR_PAIR(1));
                printw(" ");
                if (highlight) attroff(COLOR_PAIR(1));
            }
        }
        printw("\n");
    }
    for (int y = 0; y < CELLS; y++) {
        for (int x = 0; x < CELLS; x++) {
            if (board[y][x] != ' ') {
                int draw_y = y * (HEIGHT + 1) + HEIGHT / 2 + 1;
                int draw_x = x * (WIDTH + 1) + WIDTH / 2 + 1;
                attron(COLOR_PAIR(2));
                mvprintw(draw_y, draw_x, "%c", board[y][x]); 
                attroff(COLOR_PAIR(2));
            }
        }
    }

    refresh();
}


void game_loop(int total_height, int total_width) {
    char board[CELLS][CELLS];
    for (int y = 0; y < CELLS; y++)
        for (int x = 0; x < CELLS; x++)
            board[y][x] = ' ';

    int sel_x = 0, sel_y = 0;
    int ch;
    char current_player = 'X';
    int game_over = 0; 
    while (!game_over) {
        clear();
        draw_grid(board, sel_x, sel_y);
        ch = getch();

        if (ch == 'q') break;

        else if (ch == KEY_UP && sel_y > 0) sel_y--;
        else if (ch == KEY_DOWN && sel_y < CELLS - 1) sel_y++;
        else if (ch == KEY_LEFT && sel_x > 0) sel_x--;
        else if (ch == KEY_RIGHT && sel_x < CELLS - 1) sel_x++;
        else if (ch == '\n' || ch == KEY_ENTER) {
            if (board[sel_y][sel_x] == ' ') {
                board[sel_y][sel_x] = current_player;
                current_player = (current_player == 'X') ? 'O' : 'X';
                game_over = win(board, sel_x, sel_y, total_height, total_width);
            }
        }
    }
    if (game_over) {
    while(getch() != 'q'); 
}

}
int win (char board[CELLS][CELLS], int sel_x, int sel_y, int total_height, int total_width){
    int count = 0; 
    clear(); 
    draw_grid(board, sel_x, sel_y);
 for (int y = 0; y < CELLS; y++) {
        count = 0;
        for (int x = 0; x < CELLS; x++) {
            if (board[y][x] == 'X') count++;
            else if (board[y][x] == 'O') count--;
        }
        if (count == 3) {
            mvprintw(total_height, 0, "X — is winner!\nPress q to exit");
            refresh();
            return 1;
        }
        if (count == -3) {
            mvprintw(total_height, 0, "O — is winner!\nPress q to exit");
            refresh();
            return 1;
        }
    }

    for (int x = 0; x < CELLS; x++) {
        count = 0;
        for (int y = 0; y < CELLS; y++) {
            if (board[y][x] == 'X') count++;
            else if (board[y][x] == 'O') count--;
        }
        if (count == 3) {
            mvprintw(total_height, 0, "X — is winner!\nPress q to exit");
            refresh();
            return 1;
        }
        if (count == -3) {
            mvprintw(total_height, 0, "O — is winner!\nPress q to exit");
            refresh();
            return 1;
        }
    }

    count = 0;
    for (int i = 0; i < CELLS; i++) {
        if (board[i][i] == 'X') count++;
        else if (board[i][i] == 'O') count--;
    }
    if (count == 3) {
        mvprintw(total_height, 0, "X — is winner!\nPress q to exit");
        refresh();
        return 1;
    }
    if (count == -3) {
        mvprintw(total_height, 0, "O — is winner!\nPress q to exit");
        refresh();
        return 1;
    }

    count = 0;
    for (int i = 0; i < CELLS; i++) {
        if (board[i][CELLS - 1 - i] == 'X') count++;
        else if (board[i][CELLS - 1 - i] == 'O') count--;
    }
    if (count == 3) {
        mvprintw(total_height, 0, "X — is winner!\nPress q to exit");
        refresh();
        return 1;
    }
    if (count == -3) {
        mvprintw(total_height, 0, "O — is winner!\nPress q to exit");
        refresh();
        return 1;
    }

    return 0; 
}
        