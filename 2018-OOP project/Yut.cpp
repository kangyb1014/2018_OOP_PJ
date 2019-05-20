#include "Yut.h"

using namespace std;
using namespace sf;

struct {
	int x, y;
} pointLoc[30] = {
	{0,0},
	{797, 571},{797, 453},{798, 333},{798, 215},{798, 90},
	{676, 89},{558, 87},{440, 88},{322, 89},{206, 89},
	{206, 215},{207, 333},{206, 452},{207, 573},{206, 678},
	{323, 680},{441, 680},{558, 680},{677, 681},
	{693, 192},{607, 280},{311, 192},{396, 281},
	{501,386},
	{394, 491},	{303, 585},	{612, 492},	{705, 584},
	{797,680}
};

GameBoard::GameBoard() {
	for (int i = 0; i < 30; i++) {
		points[i] = Point(Neutral, i, 0, pointLoc[i].x, pointLoc[i].y, 40, 40, BlankPoint);
	}
}

int Point::canGo(int yutNum) {
	Pos tempPos = -1;
	/*여러 칸 갈 수 있는 것들*/
	/*center*/
	if (pos == 24) {
		tempPos = pos + yutNum + 2;
		if (tempPos > 30)	tempPos = 30;
	}
	/*first edge*/
	else if (pos == 5) {
		tempPos = pos + 14 + yutNum;
		if (tempPos > 21) tempPos += 2;
	}
	/*second edge*/
	else if (pos == 10) {
		tempPos = pos + 11 + yutNum;
		if (tempPos > 24)	tempPos += 2;
	}

	/*나머지 1칸만 갈수있는것중 오류생기는것들*/
	else if (pos >= 15 && pos <= 19) {
		tempPos = pos + yutNum;
		if (tempPos == 20)	tempPos = 29;
		else if (tempPos > 20)	tempPos = 30;
	}
	else if (pos == 20) {
		tempPos = pos + yutNum;
		if (tempPos == 21);
		else if (tempPos == 22)	tempPos = 24;
		else if (tempPos == 23 || tempPos == 24)	tempPos += 2;
		else /*tempPos == 25*/	tempPos = 15;
	}
	else if (pos == 21) {
		tempPos = pos + yutNum + 2;
		if (tempPos > 26) tempPos -= 12;
	}
	else if (pos == 22) {
		tempPos = pos + yutNum;
		if (tempPos > 24)	tempPos += 2;
	}
	else if (pos == 23) {
		tempPos = pos + yutNum;
		if (tempPos > 24)	tempPos += 2;
		if (tempPos > 30)	tempPos = 30;
	}
	else if (pos == 25) {
		tempPos = pos + yutNum;
		if (tempPos > 26)	tempPos -= 12;
	}
	else if (pos == 26){
		tempPos = pos + yutNum;
		tempPos -= 12;
	}
	else {
		tempPos = pos + yutNum;
		if (tempPos > 30)	tempPos = 30;
	}
	return tempPos;
}

/****************Game Class****************/

Game::Game() {
	srand(time(NULL));
	p[0] = new Player(0);
	p[1] = new Player(1);
	turn = Team1;
	phase = 1;
	thwCnt = 1;
	thwBtn = Object(1200, 625, 150, 150, Button);
	finishPoint = new Object(850, 750, 50, 50, FinishPoint);
}
void Game::throwYut() {
	yutNum = 0;
	for (int i = 0; i < 4; i++) {
		if (rand() % 100 > Yut::FRONTPROB)
			yutNum++;
	}

	if (yutNum == 0)	yutNum = 5;
}

Object* Game::ClickedObj(int x, int y) {
	if (p[0]->restPieces.inObject(x, y))
		return &p[0]->restPieces;
	else if (p[1]->restPieces.inObject(x, y))
		return &p[1]->restPieces;


	for (int i = 1; i < 30; i++) {
		if (gameBoard.points[i].inObject(x, y))
			return &gameBoard.points[i];

	}
	if (thwBtn.inObject(x, y))
		return &thwBtn;
	if (finishPoint->inObject(x, y))
		return finishPoint;
	return &NoneObj;
}

static bool isIn(int pos, vector<int> list) {
	for (auto it = list.begin(); it != list.end(); it++) {
		if (*it == pos)	return true;
	}
	return false;
}

void Game::moveGamePiece(Point *before, Point *after) {
	if (before->kind == restPiece) {
		before->count--;
		after->count++;
		after->kind = Piece;
		after->team = before->team;
		return;
	}
	else {
		after->count += before->count;
		after->kind = Piece;
		after->team = before->team;

		before->count = 0;
		before->team = Neutral;
		before->kind = BlankPoint;
		return;
	}
}

void Game::finishGamePiece(Point* before) {
	Player *p = getPlayerOfPoint(before->team);
	p->finish(before->count);
	before->team = Neutral;
	before->count = 0;
	before->kind = BlankPoint;
}

void Game::initGUI(){
	app = new RenderWindow(VideoMode(1366, 768), "Game!");
	app->setFramerateLimit(60);
	BackGroundT.loadFromFile("images/background.png");
	ButtonT.loadFromFile("images/throw.png");
	PieceTexture[0].loadFromFile("images/Piece1.png");
	PieceTexture[1].loadFromFile("images/Piece2.png");
	highlightTexture.loadFromFile("images/highlight.png");
	YutTexture.loadFromFile("images/Yuts.png");
	TextTexture.loadFromFile("images/Text.png");

	YutSprite = new Sprite(YutTexture);
	YutSprite->setPosition(1050, 150);

	background = new Sprite(BackGroundT);

	throwBtn = new Sprite(ButtonT);
	throwBtn->setPosition(1130, 550);

	pieceSprite[0] = new Sprite(PieceTexture[0]);
	pieceSprite[1] = new Sprite(PieceTexture[1]);
	highlightSprite = new Sprite(highlightTexture);

	TextSprite = new Sprite(TextTexture);
}

void Game::show() {
	app->draw(*background);
	app->draw(*throwBtn);
	
	if (phase == 3) {
		int go = selectedPoint->canGo(getYutNum());
		if(go >= 0) {
			if (go == 30)
				highlightSprite->setPosition(finishPoint->getX() - 40, finishPoint->getY() - 40);
			else
				highlightSprite->setPosition(gameBoard.points[go].getX() - 40, gameBoard.points[go].getY() - 40);
			app->draw(*highlightSprite);
		}
	}
	
	for (int i = 1; i < 30; i++) {
		if (gameBoard.points[i].kind == Piece) {
			int t = gameBoard.points[i].team - 1;
			pieceSprite[t]->setTextureRect(IntRect((gameBoard.points[i].count - 1) * 50, 0, 50, 50));
			pieceSprite[t]->setPosition(gameBoard.points[i].getX() - OFFSET, gameBoard.points[i].getY() - OFFSET);
			app->draw(*pieceSprite[t]);
		}
	}

	for (int i = 0; i < 2; i++) {
		int temp = p[i]->restPieces.count;
		if (temp > 0) {
			pieceSprite[i]->setTextureRect(IntRect((temp - 1) * 50, 0, 50, 50));
			pieceSprite[i]->setPosition(p[i]->restPieces.getX() - OFFSET, p[i]->restPieces.getY() - OFFSET);
			app->draw(*pieceSprite[i]);
		}
	}

	YutSprite->setTextureRect(IntRect(getYutNum() * 280, 0, 280, 350));
	app->draw(*YutSprite);
	
	
	if (p[0]->iswin() || turn == Team1) TextSprite->setTextureRect(IntRect(0, 0, 140, 35));
	if (p[1]->iswin() || turn == Team2) TextSprite->setTextureRect(IntRect(0, 35, 140, 35));
	TextSprite->setPosition(1050, 10);
	app->draw(*TextSprite);

	if (p[0]->iswin() || p[1]->iswin())		TextSprite->setTextureRect(IntRect(0, 105, 140, 35));
	else	TextSprite->setTextureRect(IntRect(0, 70, 140, 35));
	TextSprite->setPosition(1200, 10);
	app->draw(*TextSprite);
	
	app->display();
}