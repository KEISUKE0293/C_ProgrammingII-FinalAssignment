#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "consts.h"

void make_frame(Coordinate org, int width, int height) {
	width = 4 * width + 1;
	height = 2 * height + 1;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i % 2 == 0) {
				if (j % 4 != 0)
					mvaddch(org.y + i, org.x + j, '-');
			}
			else if (j % 4 == 0)
				mvaddch(org.y + i, org.x + j, '|');
		}
	}
}