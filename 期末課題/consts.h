#pragma once

#define BUFFSIZE 1024
#define CHARBUFF 124
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

struct Coordinate {
	int x;
	int y;
};

struct Pixel {
	bool isMined;
	int mine_cnt;
	Pixel* Up;
	Pixel* Down;
	Pixel* Left;
	Pixel* Right;
};

void make_frame(Coordinate org, int width, int height);