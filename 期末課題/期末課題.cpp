#include <curses.h>
#include <vector>
#include <Windows.h>
#include "consts.h"


int main(int argc, char* argv[])
{
	char currentDirectory[CHARBUFF];
	GetCurrentDirectory(CHARBUFF, currentDirectory);

	char mode[3][CHARBUFF] = { "Easy", "Normal", "Difficult" };
	char section[3][CHARBUFF], keyWord[3][CHARBUFF]{}, settingFile[CHARBUFF];
	sprintf_s(section[0], mode[0]);
	sprintf_s(section[1], mode[1]);
	sprintf_s(section[2], mode[2]);
	sprintf_s(keyWord[0], "vertical");
	sprintf_s(keyWord[1], "horizontal");
	sprintf_s(keyWord[2], "mine_cnt");
	sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);

	char setting[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			char num[CHARBUFF];
			if (GetPrivateProfileString(section[i], keyWord[j], "none", num, CHARBUFF, settingFile) != 0)
				setting[i][j] = atoi(num);
		}
	}

	char name[CHARBUFF] = {};
	GetPrivateProfileString("Name", "name", "none", name, CHARBUFF, settingFile);

	if (initscr() == NULL)
		return 1;
	noecho();
	cbreak();
	timeout(0);
	keypad(stdscr, TRUE);

	start_color();
	init_pair(0, COLOR_BLACK, COLOR_WHITE); // 基本の色，マス破壊後の色
	init_pair(1, COLOR_WHITE, COLOR_BLACK); // マスの初期色，スコア等の表示色
	init_pair(2, COLOR_GREEN, COLOR_GREEN); // 選択時の色
	init_pair(3, COLOR_GREEN, COLOR_WHITE); // 破壊済マスの選択時の色
	init_pair(4, COLOR_WHITE, COLOR_RED); // マインマスの色
	init_pair(5, COLOR_WHITE, COLOR_BLUE); // フラッグマスの色
	init_pair(6, COLOR_WHITE, COLOR_CYAN); // フラッグマスの選択時の色


	while (true) {
		erase();

		int mode_num = 0;
		bool confirm = true;
		mvprintw(0, 0, "mode -> %s", mode[mode_num]);
		while (confirm) {
			int key = getch();
			if (key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT) {
				erase();
				switch (key) {
				case KEY_UP:
					confirm = false;
					break;
				case KEY_LEFT:
					if (mode_num == 0)
						mode_num = 3;
					mvprintw(0, 0, "mode -> %s", mode[--mode_num]);
					break;
				case KEY_RIGHT:
					if (mode_num == 2)
						mode_num = -1;
					mvprintw(0, 0, "mode -> %s", mode[++mode_num]);
					break;
				}
				refresh();
			}

		}
		Coordinate origin = { 0 };
		origin.y = 1;
		erase();

		set_param(setting[mode_num][0], setting[mode_num][1], setting[mode_num][2], origin); // 縦・横・マイン数

		make_frame();
		make_point();
		write_score();
		refresh();

		bool first_break = true;

		while (true) {
			int key = getch();
			move_point(key);
			if (key == KEY_DC) {
				if (first_break) {
					install_mines();
					first_break = false;
				}
				else {
					if (judge())
						break;
				}
				write_score();
			}
			if (key == KEY_HOME)
				make_flag();
		}
		finish_game();
		refresh();

		make_resultFile(name, mode[mode_num]);

		while (true) {

		}
	}
}