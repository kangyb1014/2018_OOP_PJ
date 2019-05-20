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

			/*������ ����Ǹ� ���̻� �������� ����*/
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
						/*�� �Ǵ� �� ������ ��*/
						if (getYutNum() > 3)
							thwCnt++;
						cout << "���� ������ " << getYutNum() << "�� ���Դ�" << endl;
						/*graphic*/
						phase = 2;
					}

					/*�����ǿ� ���� ���� ���� ������ ��*/
					else if (obj->kind == restPiece && phase == 2) {
						selectedPoint = (Point*)obj;
						/*�ڱ��� ���� �����ؾ߸� �ϰ�, ���� ���� 1�� �̻��̾�� ��*/
						if (selectedPoint->team == turn && selectedPoint->count > 0) {
							cout << "�÷��̾� " << turn << "�� ���� �ø��ϴ�" << endl;
							phase = 3;
						}
					}

					/*�̹� ������ ���� �ִ� ���� ������ ��*/
					else if (obj->kind == Piece && phase == 2) {
						selectedPoint = (Point*)obj;
						/*�ڱ��� ���� �����ؾ߸� ��*/
						if (selectedPoint->team == turn) {
							cout << "�÷��̾� " << turn << "�� ������ ���� �����մϴ�" << endl;
							phase = 3;
						}
					}

					/*�� �������� �̵��� ��*/
					else if (obj->kind == BlankPoint && phase == 3) {
						nextPoint = (Point*)obj;
						if (nextPoint->pos == selectedPoint->canGo(getYutNum())) {
							moveGamePiece(selectedPoint, nextPoint);
							cout << "������ ���� " << endl;
							yutNum = 0;
							phase = 1;
							if (thwCnt < 1) {
								changeTurn();
								thwCnt = 1;
							}
						}
					}

					/*������ ��*/
					else if (obj->kind == FinishPoint && phase == 3) {
						if (30 == selectedPoint->canGo(getYutNum())) {
							finishGamePiece(selectedPoint);
							cout << "���� ���� " << endl;
							yutNum = 0;
							phase = 1;
							if (thwCnt < 1) {
								changeTurn();
								thwCnt = 1;
							}
						}
					}

					/*�ٸ� ���� �ִ� ������ �̵��� ��*/
					else if (obj->kind == Piece && phase == 3) {
						nextPoint = (Point*)obj;
						if (nextPoint->team == turn && selectedPoint->canGo(yutNum) == nextPoint->pos) {
							/*����*/
							moveGamePiece(selectedPoint, nextPoint);
							cout << "���� ���� " << endl;
							yutNum = 0;
							phase = 1;
							if (thwCnt < 1) {
								changeTurn();
								thwCnt = 1;
							}
						}
						else if(nextPoint->team != turn && selectedPoint->canGo(yutNum) == nextPoint->pos){
							/*���*/
							getEnemy()->catched(nextPoint->count);
							nextPoint->team = Neutral;
							nextPoint->count = 0;
							nextPoint->kind = BlankPoint;
							moveGamePiece(selectedPoint, nextPoint);
							thwCnt++;
							cout << "��� ���� " << endl;
							yutNum = 0;
							phase = 1;
						}
					}

					/*������*/
					if (obj->kind == BlankPoint) {
						Point* p = (Point*)obj;
						cout << p->pos << "����Ʈ Ŭ����" << endl;
					}
					else if (obj->kind == FinishPoint) {
						cout << "������ Ŭ����" << endl;
					}

					switch (phase) {
					case 1:	cout << "�÷��̾� " << turn << "�� ���� ���� �����Դϴ�." << endl;	break;
					case 2:	cout << "�÷��̾� " << turn << "�� �ű� ���� ������ �����Դϴ�" << endl;	break;
					case 3:	cout << "�÷��̾� " << turn << "�� ���� �ű� ��ġ�� ������ �����Դϴ�." << endl;	break;
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
