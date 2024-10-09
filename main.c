#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

time_t start_time;
unsigned long inputted = 0;
unsigned long mistakes = 0;

void end_prac(void) {
    time_t end_time = time(NULL);
    time_t diff = end_time - start_time;
    int minutes = diff / 60;
    int seconds = diff - minutes * 60;
    float accuracy = (float) (inputted - mistakes) / inputted * 100;
    addch('\n');
    printw("耗时%d分%02d秒，输入了%lu个字符，准确率%.1f%%\n按任意键以退出", minutes, seconds, inputted, accuracy);
    getch();
    endwin();
    exit(0);
}

bool print_line(char* str, size_t pos) {
    for (int i = 0; i < COLS; i++) {
        if (str[pos + i] == '\0')
            return false;
        addch(str[pos + i]);
    }
    refresh();
    return true;
}

void check_input(char* str, size_t pos) {
    for (int i = 0; i < COLS; i++) {
        char current_ch = str[pos + i];
        if (current_ch == '\0' || current_ch == '\n')
            break;
        int ch = getch();
        if (ch == '\n') end_prac();
        if (ch == (int)current_ch) {
            addch(ch | COLOR_PAIR(1));
        } else {
            addch(ch | COLOR_PAIR(2));
            ++mistakes;
        }
        ++inputted;
    }
}

int main(void) {
    FILE* file = fopen("text.txt", "r");
    char str[1024];
    fgets(str, 1024, file);

    initscr();
    noecho();
    cbreak();

    addstr("按Enter可退出练习\n");

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_RED);

    size_t pos = 0;
    bool not_ended = true;

    start_time = time(NULL);

    while (not_ended) {
        not_ended = print_line(str, pos);
        check_input(str, pos);
        pos += COLS;
        int y = getcury(stdscr);
        if (y + 2 >= LINES) clear();
    }

    end_prac();

    return 0;
}

