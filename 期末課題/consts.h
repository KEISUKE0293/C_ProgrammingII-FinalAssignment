#define BUFFSIZE 1024
#define CHARBUFF 124
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

struct Coordinate {
	int x;
	int y;
};

// 初期化をする関数
void set_param(int ver, int hor, int cnt, Coordinate ori);

// 枠を描画する関数
void make_frame();

// 最初にポイントする関数
void make_point();

// 1回目の破壊と同時にマインを設置する関数
void install_mines();

// ポイントを動かす関数
void move_point(int key);

// ミスかどうかを判定してそれに関する処理を行う関数
bool judge();

// 旗を立てる関数
void make_flag();

// 対象のピクセルの周囲に何個のマインが存在するかを描画する関数
void write_mineNum(int y, int x, int color);

// スコアを描画する関数
void write_score();

// ゲームを終了する関数
void finish_game();

// 結果を出力する関数
void make_resultFile(char name[CHARBUFF], char mode[CHARBUFF]);