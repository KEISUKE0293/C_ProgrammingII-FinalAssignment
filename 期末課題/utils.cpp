#include <curses.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <Windows.h>
#include "consts.h"

using namespace std; //���O��Ԃ��w��

struct Result {
	string name;
	string difficulty;
	int score;
};

int vertical = 0; // �c
int horizontal = 0; // ��
int mine_cnt = 0; // �}�C����
int breaked_cnt = 0; //�j��

Coordinate current_location = { 0 }; // ���ڂɂ����錻�݂̍��W
Coordinate origin = { 0 }; // �t���[���̌��_

vector<vector<vector<int>>> mineArray; // �}�C���z�� �c�~���̔z����� (�}�C���̗L��, �j��ς݂��ۂ�)

void set_param(int ver, int hor, int cnt, Coordinate ori) {
	vertical = ver;
	horizontal = hor;
	mine_cnt = cnt;
	origin = ori;
	vector<vector<vector<int>>> m(vertical, vector<vector<int>>(horizontal, vector<int>(2, 0)));
	mineArray = m;
}

void make_frame() {
	int height = 2 * vertical + 1;
	int width = 4 * horizontal + 1;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i % 2 == 0) {
				if (j % 4 != 0)
					mvaddch(origin.y + i, origin.x + j, '-');
				else
					mvaddch(origin.y + i, origin.x + j, 'o');
			}
			else {
				if (j % 4 == 0)
					mvaddch(origin.y + i, origin.x + j, '|');
				else {
					attrset(COLOR_PAIR(1));
					mvaddch(origin.y + i, origin.x + j, ' ');
					attrset(COLOR_PAIR(0));
				}

			}
		}
	}
}

void make_point() {
	attrset(COLOR_PAIR(2));
	mvaddstr(origin.y + 1, origin.x + 1, "   ");
}

void move_point(int key) {
	if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
		if (mineArray[current_location.y][current_location.x][1] == 0) {
			attrset(COLOR_PAIR(1));
			mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, "   ");
		}
		else if (mineArray[current_location.y][current_location.x][1] == 1) {
			write_mineNum(current_location.y, current_location.x, 0);
		}
		else if (mineArray[current_location.y][current_location.x][1] == -1) {
			attrset(COLOR_PAIR(5));
			mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, " P ");
		}

		switch (key) {
		case KEY_UP:
			if (current_location.y > 0)
				current_location.y--;
			break;
		case KEY_DOWN:
			if (current_location.y < vertical - 1)
				current_location.y++;
			break;
		case KEY_LEFT:
			if (current_location.x > 0)
				current_location.x--;
			break;
		case KEY_RIGHT:
			if (current_location.x < horizontal - 1)
				current_location.x++;
			break;
		}

		if (mineArray[current_location.y][current_location.x][1] == 0) {
			attrset(COLOR_PAIR(2));
			mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, "   ");
		}
		else if (mineArray[current_location.y][current_location.x][1] == 1) {
			write_mineNum(current_location.y, current_location.x, 3);
		}
		else if (mineArray[current_location.y][current_location.x][1] == -1) {
			attrset(COLOR_PAIR(6));
			mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, " P ");
		}
		refresh();
	}
}

bool judge() {
	if (mineArray[current_location.y][current_location.x][0] == 1 || (vertical * horizontal - mine_cnt == breaked_cnt)) {
		return true;
	}
	else if (mineArray[current_location.y][current_location.x][1] != 1) {
		write_mineNum(current_location.y, current_location.x, 3);
		mineArray[current_location.y][current_location.x][1] = 1;
		breaked_cnt++;
	}
	return false;
}

void make_flag() {
	if (mineArray[current_location.y][current_location.x][1] != 1) {
		if (mineArray[current_location.y][current_location.x][1] == 0) {
			attrset(COLOR_PAIR(6));
			mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, " P ");
			mineArray[current_location.y][current_location.x][1] = -1;
		}
		else if (mineArray[current_location.y][current_location.x][1] == -1) {
			attrset(COLOR_PAIR(2));
			mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, "   ");
			mineArray[current_location.y][current_location.x][1] = 0;
		}
	}
}

void write_mineNum(int y, int x, int color) {
	int mine_num = 0;
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 1; j <= x + 1; j++) {
			if (i >= 0 && i < vertical && j >= 0 && j < horizontal) {
				if (mineArray[i][j][0] == 1)
					mine_num++;
			}
		}
	}
	attrset(COLOR_PAIR(color));
	mvprintw(y * 2 + origin.y + 1, x * 4 + origin.x + 1, " %d ", mine_num);
}

void write_score() {
	attrset(COLOR_PAIR(0));
	mvprintw(0, 0, "score: %2d ", breaked_cnt);
}

void install_mines() {
	vector<vector<vector<int>>> mines(vertical, vector<vector<int>>(horizontal, vector<int>(2, 0)));
	int pixel_num = vertical * horizontal;
	int* randArray = new int[pixel_num];
	int cnt = 0;
	int ini_in_flame = (current_location.y + 1) * (horizontal + 2) + current_location.x + 1; //�����j��ʒu�̉��z�t���[����ł̈ʒu

	srand(time(NULL));
	while (cnt < mine_cnt) {
		int random = rand();
		int mine_location = random % pixel_num; // 0�`�Z���̌�
		int mine_in_flame = mine_location + (mine_location / horizontal) * 2 + (horizontal + 2) + 1; //�����_���}�C���̉��z�t���[���ł̈ʒu

		// �����_���}�C���������j��ʒu�̎���9�}�X�ɗ����ꍇ��continue����
		if (mine_in_flame == ini_in_flame - (horizontal + 2) - 1
			|| mine_in_flame == ini_in_flame - (horizontal + 2)
			|| mine_in_flame == ini_in_flame - (horizontal + 2) + 1
			|| mine_in_flame == ini_in_flame - 1
			|| mine_in_flame == ini_in_flame
			|| mine_in_flame == ini_in_flame + 1
			|| mine_in_flame == ini_in_flame + (horizontal + 2) - 1
			|| mine_in_flame == ini_in_flame + (horizontal + 2)
			|| mine_in_flame == ini_in_flame + (horizontal + 2) + 1
			)
			continue;

		int mine_x = mine_location % horizontal;
		int mine_y = mine_location / horizontal;

		// ���Ƀ}�C�������݂��Ă��ꍇ��continue����
		if (mines[mine_y][mine_x][0] != 0)
			continue;

		mines[mine_y][mine_x][0] = 1; // �Y���ӏ��Ƀ}�C����ݒu
		cnt++; // �}�C���J�E���g��+1

	}
	mineArray = mines;

	// �����j��ʒu�𒆐S��3*3�͈̔͂�j��ς݂ɕύX
	for (int i = current_location.y - 1; i <= current_location.y + 1; i++) {
		for (int j = current_location.x - 1; j <= current_location.x + 1; j++) {
			if (i >= 0 && i < vertical && j >= 0 && j < horizontal) {
				write_mineNum(i, j, 0);
				mines[i][j][1] = 1;
				breaked_cnt++;
			}
		}
	}
	write_mineNum(current_location.y, current_location.x, 3);
	mineArray = mines;
}

void finish_game() {
	attrset(COLOR_PAIR(4));
	mvaddstr(current_location.y * 2 + origin.y + 1, current_location.x * 4 + origin.x + 1, "���");
	mineArray[current_location.y][current_location.x][1] = 1;
	for (int i = 0; i < vertical; i++) {
		for (int j = 0; j < horizontal; j++) {
			if (mineArray[i][j][1] != 1) {
				if (mineArray[i][j][0] == 1) {
					attrset(COLOR_PAIR(4));
					mvaddstr(i * 2 + origin.y + 1, j * 4 + origin.x + 1, "   ");
				}
				else {
					write_mineNum(i, j, 0);
				}
			}
		}
	}
	if (vertical * horizontal - mine_cnt == breaked_cnt) {
		attrset(COLOR_PAIR(5));
		mvaddstr(0, 13, "Clear!!!");
	}
	else {
		attrset(COLOR_PAIR(4));
		mvaddstr(0, 13, "Failure...");
	}
}

void make_resultFile(char name[CHARBUFF], char mode[CHARBUFF]) {
	char s[BUFFSIZE], * p, delim[] = ",", * ctx;
	vector<Result> result_array;
	FILE* fp;
	errno_t error = fopen_s(&fp, "result.csv", "r");
	if (error != 0)
		fprintf_s(stderr, "failed to open");
	else {
		int i = 0;
		while (fgets(s, BUFFSIZE, fp) != NULL) {
			if (i != 0) {
				p = strtok_s(s, delim, &ctx);
				Result r;
				for (int j = 0; j < 3; j++) {
					switch (j) {
					case 0:
						r.name = p;
						break;
					case 1:
						r.difficulty = p;
						break;
					case 2:
						r.score = atoi(p);
						break;
					}
					p = strtok_s(NULL, delim, &ctx);
				}
				result_array.push_back(r);
			}
			i++;
		}
		fclose(fp);
	}
	error = fopen_s(&fp, "result.csv", "w");
	if (error != 0) {
		fprintf_s(stderr, "failed to open");
	}
	else {
		char str[512];
		fputs("�v���C���[��,��Փx,�X�R�A\n", fp);
		for (int i = 0; i < result_array.size(); i++) {
			sprintf_s(str, "%s,%s,%d\n", result_array[i].name.c_str(), result_array[i].difficulty.c_str(), result_array[i].score);
			fputs(str, fp);
		}
		sprintf_s(str, "%s,%s,%d\n", name, mode, breaked_cnt);
		fputs(str, fp);
		fprintf_s(stdout, "The csv file was generated successfully.");
	}
}