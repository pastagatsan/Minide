#include <ncurses.h>
#include <string.h> 

const char * words[] = {
	"This", "is", "a", "list", "of", "words!"
};

const char * modes[] = {
	"[0] Editor", "[1] Keypress"
};

const char BKSPACE  = 127;
const int UP_KEY    = 259,
		  DOWN_KEY  = 258,
		  LEFT_KEY  = 260,
		  RIGHT_KEY = 261;
const int CTRL_X    = 24;

int mode = 1;
const int MODE_COUNT = 2;
const int MODE_EDITOR = 0, MODE_KEYPRESS = 1;

unsigned int editor_pos = 11;
char editor_string[256] = "Hello world!";

/* All this function does is clear and show the screen again. */
void show_screen(int);

/* Fills the line with spaces. */
void fill(int);

/* Keypress mode code. /b/ is the key pressed. */
void mode_keypress(int b);

/* Editor mode code. /b/ is the key pressed. */
void mode_editor(int b);

/* Finds the amount of newline characters within /string/. */
int lines(char* string);

/* Get the current line a character at index /i/ is in. */
int get_line(char* s, int i);

/* Gets the position of an index (epos) in a line.*/
int get_line_pos(char* s, int epos);

int main()
{
	initscr();
	keypad(stdscr, TRUE);
	if (has_colors() == FALSE)
	{
		mvprintw(0, 0, "Your terminal doesn't support color. Oh well.\n");
		mvprintw(1, 0, "~ Press any key to quit");
		getch();
		endwin();
		return 1;
	}
	start_color();
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	
	while (true)
	{
		int b = getch();
		
		show_screen(b);
	}
	endwin();
}

void show_screen(int b)
{
	erase();
	if (b == CTRL_X)
	{
		mode++;
		if (mode == MODE_COUNT) mode = 0;
	}
	
	attron(COLOR_PAIR(2));
		fill(0);
		mvprintw(0, 0, "~~ Mode: %s", modes[mode]);
	attroff(COLOR_PAIR(2));
	
	if (mode == MODE_KEYPRESS)
	{
		mode_keypress(b);
	}
	else if (mode == MODE_EDITOR)
	{
		mode_editor(b);
	}
	refresh();
}

void fill(int ln)
{
	int wd = 0, wh = 0;
	getmaxyx(stdscr, wh, wd);
	for (int i = 0; i < wd; i++)
	{
		mvprintw(ln, i, " ");
	}
}

void mode_keypress(int b)
{
	attron(COLOR_PAIR(1));
		mvprintw(1, 0, "Character: %c | Decimal: %d", b, (int)b);
	attroff(COLOR_PAIR(1));
}

void mode_editor(int b)
{
	int length = strlen(editor_string);
	if (b == CTRL_X)
	{
		return;
	}
	if (b == BKSPACE)
	{
		if (length > 0)
		{
			if (editor_pos < length)
			{
				for (int i = length; i > editor_pos; i--)
				{
					editor_string[i-1] = editor_string[i];
				}
				editor_pos--;
			} else {
				editor_string[length - 1] = '\0';
				editor_pos--;
			}
			if (editor_pos < 0) editor_pos = 0;
		}
	}
	else if (b == LEFT_KEY) // ?
	{
		if (editor_pos > 0) editor_pos--;
		
		move(get_line(editor_string, editor_pos),
			get_line_pos(editor_string, editor_pos));
	}
	else if (b == RIGHT_KEY)
	{
		if (editor_pos < strlen(editor_string)) {
			editor_pos++;
			move(get_line(editor_string, editor_pos),
				get_line_pos(editor_string, editor_pos));
		}
	}
	else
	{
		if (editor_pos == length)
		{
			editor_string[editor_pos] = b;
			editor_string[editor_pos + 1] = '\0';
		} else {
			for (int i = strlen(editor_string); i > editor_pos - 1; i--)
			{
				editor_string[i + 1] = editor_string[i];
			}
			editor_string[editor_pos] = b;
			
		}
		editor_pos++;
	}
	
	int str_len        = strlen(editor_string);
	int current_line   = get_line(editor_string, 0);
	int line_pos       = get_line_pos(editor_string, 0);
	int line_edit_pos  = get_line_pos(editor_string, editor_pos);
	
	attron(COLOR_PAIR(2));
		fill(15);
		mvprintw(15, 0, "Buffer Name: test.c | Buffer Position: %d | Buffer Length: %d | Current Line: %d", editor_pos, str_len, current_line);
	attroff(COLOR_PAIR(2));
	
	attron(COLOR_PAIR(3));
		mvprintw(1 + current_line, line_pos, "%s", editor_string);
		move(1 + current_line, line_edit_pos);
	attroff(COLOR_PAIR(3));
	
}

int lines(char * string)
{
	int i = 0, j = 0;
	while (true)
	{
		if (string[j] == '\n') i++;
		if (string[j] == '\0') return i;
		j++;
	}
}

int last_newline(char * string)
{
	int i = 0, j = 0;
	while (true)
	{
		if (string[j] == '\n') i = j;
		if (string[j] == '\0') return i;
		j++;
	}
}

int get_line(char * string, int epos)
{
	int i = 0, j = 0;
	while (true)
	{
		if (string[j] == '\n') i++;
		if (j == epos) return i;
		j++;
	}
}

int get_line_pos(char * string, int epos)
{
	int str_pos = 0, x_pos = 0;
	while (true)
	{
		if (string[str_pos] == '\n') x_pos = 0;
		if (x_pos == epos) return x_pos;
		x_pos++;
	}
}