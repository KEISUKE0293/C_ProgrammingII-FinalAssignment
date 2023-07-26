#define BUFFSIZE 1024
#define CHARBUFF 124
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

struct Coordinate {
	int x;
	int y;
};

// ������������֐�
void set_param(int ver, int hor, int cnt, Coordinate ori);

// �g��`�悷��֐�
void make_frame();

// �ŏ��Ƀ|�C���g����֐�
void make_point();

// 1��ڂ̔j��Ɠ����Ƀ}�C����ݒu����֐�
void install_mines();

// �|�C���g�𓮂����֐�
void move_point(int key);

// �~�X���ǂ����𔻒肵�Ă���Ɋւ��鏈�����s���֐�
bool judge();

// ���𗧂Ă�֐�
void make_flag();

// �Ώۂ̃s�N�Z���̎��͂ɉ��̃}�C�������݂��邩��`�悷��֐�
void write_mineNum(int y, int x, int color);

// �X�R�A��`�悷��֐�
void write_score();

// �Q�[�����I������֐�
void finish_game();

// ���ʂ��o�͂���֐�
void make_resultFile(char name[CHARBUFF], char mode[CHARBUFF]);