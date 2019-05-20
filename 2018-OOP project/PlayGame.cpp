#include "Yut.h"

using namespace std;
using namespace sf;

void Game::playGame() {
	yutNum = 0;
	initGUI();
	while (app->isOpen()) {
		Event e;
		while (app->pollEvent(e))
		{
			Object* obj;
			if (e.type == Event::Closed)
				app->close();

			/*게임이 종료되면 더이상 진행하지 않음*/
			if (p[0]->iswin() || p[1]->iswin())	continue;

			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left) {
					Vector2i pos = Mouse::getPosition(*app);
					cout << pos.x << " " << pos.y << endl;
					obj = ClickedObj(pos.x, pos.y);

					/*throw */
					if (obj->kind == Button && phase == 1) {
						throwYut();
						thwCnt--;
						/*윷 또는 모가 나왔을 때*/
						if (getYutNum() > 3)
							thwCnt++;
						cout << "윷을 던져서 " << getYutNum() << "이 나왔다" << endl;
						/*graphic*/
						phase = 2;
					}

					/*게임판에 새로 놓을 말을 선택할 때*/
					else if (obj->kind == restPiece && phase == 2) {
						selectedPoint = (Point*)obj;
						/*자기의 말을 선택해야만 하고, 남은 말이 1개 이상이어야 함*/
						if (selectedPoint->team == turn && selectedPoint->count > 0) {
							cout << "플레이어 " << turn << "이 말을 올립니다" << endl;
							phase = 3;
						}
					}

					/*이미 게임판 위에 있는 말을 선택할 때*/
					else if (obj->kind == Piece && phase == 2) {
						selectedPoint = (Point*)obj;
						/*자기의 말을 선택해야만 함*/
						if (selectedPoint->team == turn) {
							cout << "플레이어 " << turn << "이 움직일 말을 선택합니다" << endl;
							phase = 3;
						}
					}

					/*빈 공간으로 이동할 때*/
					else if (obj->kind == BlankPoint && phase == 3) {
						nextPoint = (Point*)obj;
						if (nextPoint->pos == selectedPoint->canGo(getYutNum())) {
							moveGamePiece(selectedPoint, nextPoint);
							cout << "움직임 성공 " << endl;
							yutNum = 0;
							phase = 1;
							if (thwCnt < 1) {
								changeTurn();
								thwCnt = 1;
							}
						}
					}

					/*완주할 때*/
					else if (obj->kind == FinishPoint && phase == 3) {
						if (30 == selectedPoint->canGo(getYutNum())) {
							finishGamePiece(selectedPoint);
							cout << "완주 성공 " << endl;
							yutNum = 0;
							phase = 1;
							if (thwCnt < 1) {
								changeTurn();
								thwCnt = 1;
							}
						}
					}

					/*다른 말이 있는 곳으로 이동할 때*/
					else if (obj->kind == Piece && phase == 3) {
						nextPoint = (Point*)obj;
						if (nextPoint->team == turn && selectedPoint->canGo(yutNum) == nextPoint->pos) {
							/*업기*/
							moveGamePiece(selectedPoint, nextPoint);
							cout << "업기 성공 " << endl;
							yutNum = 0;
							phase = 1;
							if (thwCnt < 1) {
								changeTurn();
								thwCnt = 1;
							}
						}
						else if(nextPoint->team != turn && selectedPoint->canGo(yutNum) == nextPoint->pos){
							/*잡기*/
							getEnemy()->catched(nextPoint->count);
							nextPoint->team = Neutral;
							nextPoint->count = 0;
							nextPoint->kind = BlankPoint;
							moveGamePiece(selectedPoint, nextPoint);
							thwCnt++;
							cout << "잡기 성공 " << endl;
							yutNum = 0;
							phase = 1;
						}
					}

					/*디버깅용*/
					if (obj->kind == BlankPoint) {
						Point* p = (Point*)obj;
						cout << p->pos << "포인트 클릭됨" << endl;
					}
					else if (obj->kind == FinishPoint) {
						cout << "완주점 클릭됨" << endl;
					}

					switch (phase) {
					case 1:	cout << "플레이어 " << turn << "이 윷을 던질 차례입니다." << endl;	break;
					case 2:	cout << "플레이어 " << turn << "이 옮길 말을 선택할 차례입니다" << endl;	break;
					case 3:	cout << "플레이어 " << turn << "이 말을 옮길 위치를 선택할 차례입니다." << endl;	break;
					default:
						break;
					}


				}
				if (p[0]->iswin()) {
					cout << "1P win!" << endl;
					winF = true;
				}
				else if (p[1]->iswin()) {
					cout << "2P win!" << endl;
					winF = true;
				}
		}
		show();
		
	}
}
