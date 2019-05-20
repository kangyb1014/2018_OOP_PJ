
class GamePiece : public Object {
protected:
	int team;
	Pos curPos;
	int count;
public:
	GamePiece(short int team, short int id) : Object(0, 0, ) {
		this->team = team;
		kind = Piece;
		curPos = 0;
		count = 1;
	}
	short int getTeam() { return team; }
	vector<Pos> canGo(int yutnum);
};

/*----------------------------------------------------
다음으로 갈 수 있는 위치를 벡터로 리턴함.
위치 정보
0: 아직 올라가지 않은 상태, 1~29: 번호대로, 30: 완주함
-----------------------------------------------------*/
vector<Pos> GamePiece::canGo(int yutNum) {
	vector<Pos> retPos;
	Pos tempPos;
	/*back-do*/
	if (yutNum == -1) {
		switch (curPos) {
		case 1:		retPos.push_back(29);	break;
		case 20:	retPos.push_back(5);	break;
		case 22:	retPos.push_back(10);	break;
		case 27:	retPos.push_back(24);	break;
		default:
			retPos.push_back(curPos - 1);
			break;
		}
		return retPos;
	}
	/*center*/
	if (curPos == 24) {
		tempPos = curPos + yutNum;
		if (tempPos > 26)	tempPos -= 12;
		retPos.push_back(tempPos);
		tempPos = curPos + 2 + yutNum;
		if (tempPos > 30)	tempPos = 30;
		retPos.push_back(tempPos);
		return retPos;
	}
	/*first edge*/
	else if (curPos == 5) {
		tempPos = curPos + 14 + yutNum;
		if (tempPos > 26)	tempPos -= 12;
		retPos.push_back(tempPos);
	}
	/*second edge*/
	else if (curPos == 10) {
		tempPos = curPos + 11 + yutNum;
		if (tempPos > 30)	tempPos = 30;
		retPos.push_back(tempPos);
	}
	tempPos += curPos + yutNum;
	if (tempPos > 30)	tempPos = 30;
	retPos.push_back(tempPos);
	return retPos;
}
