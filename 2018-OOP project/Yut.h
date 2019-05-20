#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/*���߿� �ٽ� ����*/
#define Pos int
#define OFFSET 25

enum objKind {
	None, BlankPoint, Piece, restPiece, FinishPoint, Button
};
enum Team {
	Neutral, Team1, Team2
};

class Object {
protected:
	int x, y;
	int width, height;
public:
	objKind kind;
	Object(int x = 0, int y = 0, int width = 0, int height = 0,objKind kind = None) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->kind = kind;
	}
	int getX() { return x; }
	int getY() { return y; }
	bool inObject(int x, int y) {
		if (x < this->x - width / 2)	return false;
		if (x > this->x + width / 2)	return false;
		if (y < this->y - height / 2)	return false;
		if (y > this->y + height / 2)	return false;
		return true;
	}
};

/*GamePiece�� �����Ѵ�*/
class Point : public Object{
public:
	int pos;
	Team team;
	int count;	/*�ش� ĭ�� �� �� ���� �ֳ�*/
	Point(Team team = Neutral, int pos = 0, int count = 0, int x = 0, int y = 0, int width = 0, int height = 0, objKind kind = None) : Object(x,y,width,height,kind) {
		/*�׳� ���� ���� ���ǵ��� ����*/
		this->team = team;
		this->pos = pos;
		this->count = count;
	}
	int canGo(int yutnum);
};

class GameBoard {
public:
	Point points[30];
	GameBoard();
};

class Player {
private:
	int team;
	int finished = 0;
public:
	Point restPieces;
	Player(int team) {
		this->team = team;
		if (team == 0)
			restPieces = Point(Team1, 0, 4, 75, 230, 50, 50, restPiece);
		else
			restPieces = Point(Team2, 0, 4, 950, 230, 50, 50, restPiece);
	}
	/*GamePieceCnt��ŭ ���� �ִ� ���� ������.*/
	void catched(int GamePieceCnt) {
		restPieces.count += GamePieceCnt;
	}
	/*�����ǿ��� ���� Cnt�� ���ֽ�Ų��*/
	void finish(int GamePieceCnt) {
		finished += GamePieceCnt;
	}
	/*�����ǿ� ���� 1�� ���´�.*/
	void lay() {
		restPieces.count--;
	}
	bool iswin() {
		if (finished == 4)	return true;
		return false;
	}

};

class Yut {
public:
	/*probability that a yut is front*/
	static const int FRONTPROB = 55;
};

class Game {
private:
	Yut yuts[4];
	Object thwBtn;
	GameBoard gameBoard;
	int yutNum;
	RenderWindow *app;
	Texture BackGroundT, ButtonT, PieceTexture[2], highlightTexture, YutTexture, TextTexture;
	Sprite *background, *throwBtn, *pieceSprite[2], *highlightSprite, *YutSprite, *TextSprite;
	Object* finishPoint;
	Point* selectedPoint = NULL, *nextPoint = NULL;
	bool winF = false;
public:
	Object NoneObj;
	Player* p[2];
	int phase;
	int thwCnt;
	Game();
	Team turn;
	int getYutNum() { return yutNum; }
	void throwYut();
	Object* ClickedObj(int, int);
	void changeTurn() {
		if (p[0]->iswin() || p[1]->iswin())	return;
		if (turn == Team1)	turn = Team2;
		else turn = Team1;
	}
	Player* getEnemy() {
		if (turn == Team1)
			return p[1];
		else return p[0];
	}
	Player* getPlayerOfPoint(Team team) {
		return p[team - 1];
	}
	void finishGamePiece(Point* before);
	void moveGamePiece(Point*, Point*);
	void initGUI();
	void playGame();
	void show();
};

static bool isIn(int pos, vector<int> list);