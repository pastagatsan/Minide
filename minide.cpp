#include <ncurses.h>

const char * words[] = {
	"This", "is", "a", "list", "of", "words!"
};

/* All this function does is clear and show the screen again. */
void show_screen(char b);

int main()
{
	initscr();
	if (has_colors() == FALSE)
	{
		mvwprintw(stdscr, 0, 0, "Your terminal doesn't support color. Oh well.\n");
		mvwprintw(stdscr, 0, 0, "~ Press any key to quit");
		getch();
		endwin();
		return 1;
	}
	start_color();
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	
	
	
	while (true)
	{
		char b = getch();
		show_screen(b);
	}
	endwin();
}

void show_screen(char b)
{
	erase();
	attron(COLOR_PAIR(1));
	mvwprintw(stdscr, 0, 0, "Character: %c | Decimal: %d", b, (int)b);
	attroff(COLOR_PAIR(1));
	refresh();
}