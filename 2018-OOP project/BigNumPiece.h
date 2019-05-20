
class BigNumPiece : public Piece {
private:
	static const int MORE = 2;
public:
	BigNumPiece(short int team, short int id) : Piece(team, id) {}
	virtual vector<Pos> canGo(int yutNum) override;
};

vector<Pos> BigNumPiece::canGo(int yutNum) {
	vector<Pos> retPos;
	Pos tempPos;
	/*back-do*/
	if (yutNum == -1) {
		switch (curPos) {
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
		tempPos = curPos + yutNum + MORE;
		if (tempPos > 26)	tempPos -= 12;
		retPos.push_back(tempPos);
		tempPos = curPos + 2 + yutNum + MORE;
		if (tempPos > 28)	tempPos = 29;
		retPos.push_back(tempPos);
		return retPos;
	}
	/*first edge*/
	else if (curPos == 5) {
		tempPos = curPos + 14 + yutNum + MORE;
		if (tempPos > 26)	tempPos -= 12;
		retPos.push_back(tempPos);
	}
	/*second edge*/
	else if (curPos == 10) {
		tempPos = curPos + 11 + yutNum + MORE;
		if (tempPos > 28)	tempPos = 29;
		retPos.push_back(tempPos);
	}
	tempPos += curPos + yutNum + MORE;
	if (tempPos > 28)	tempPos = 29;
	retPos.push_back(tempPos);
	return retPos;
}