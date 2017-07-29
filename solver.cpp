#include <iostream>
#include <vector>
#include <list>
#include <string>
//constexpr double compilePow(double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow(mantissa, exponent - 1); }
//const double EPS = compilePow(0.1,9);

//todo
/*


接触判定・Frameの更新 <-Frameクラスをリスト化して全てにCrossing numberをかける<-wn をXOR?
--多分AC

探索<-とりあえず面積とBottom-leftで貪欲

ピースごとの面積の算出 OK
使ってないピースの中から面積が一番大きいものを選ぶ(毎回全走査、ピースをソート、使ったものからerase)
左上から置けるか探索、最初の置ける地点に配置
置けるピースがなくなればループ抜け

フレームを全部表示すれば一応できてるかはわかる。
*/

//split、なぜ標準実装じゃないのか。
std::vector<std::string> split(std::string str, char sep)
{
	std::vector<std::string> v;
	auto first = str.begin();
	while (first != str.end()) {
		auto last = first;
		while (last != str.end() && *last != sep)
			++last;
		v.push_back(std::string(first, last));
		if (last != str.end())
			++last;
		first = last;
	}
	return v;
}

//x,y座標のpair
class Position {
public:

	short x, y;//shortで持っているので内外判定の切り捨てでバグるかも。

	void renewPosition(int a, int b) { x = a; y = b; return; }//DON'T USE
	Position() {};
	Position(int xValue, int yValue) { x = xValue; y = yValue; }
};

double cross(Position a, Position b) {
	return a.x*b.y - a.y * b.x;
}


//各辺の直線の式の傾きと切片のpair
/*
class ExpressionElement {

private
:double slope, intercept;
public:
double slope() { return slope; }
double intercept() { return intercept; }
ExpressionElement(){};
ExpressionElement(double slopeValue, double interceptValue) { slope = slopeValue; intercept = interceptValue; return; }
};
*/

/*Pieceクラス　頂点数と各点の座標*/
class Piece {
	
	//ある点からの相対座標で表示
	Position relativePosition(Position standard, Position object) {
		double x, y;
		x = object.x - standard.x;
		y = object.y - standard.y;
		return Position(x, y);
	};
	
	//外積による面積の算出
	double areaCalucurasion() {
		double tmp=0;
		for (int i = 0; i < this->vertexPositionList.size() - 1; ++i) {
			tmp += cross(this->vertexPositionList.at(i), this->vertexPositionList.at(i + 1));
		}
		return tmp/2;
	}

public:

	double area;
	bool canUse;


	std::vector<Position> vertexPositionList;

	void spin() { //+pi/2 (r cos(a+pi/2)=-r sin(a) (r sin(a+pi/2)=r cos(a))
		for (int i = 0; i < vertexPositionList.size(); ++i)
			vertexPositionList.at(i).renewPosition(-vertexPositionList.at(i).y, vertexPositionList.at(i).x);
		return;
	}

	void turn() {
		for (int i = 0; i < vertexPositionList.size(); ++i)
			vertexPositionList.at(i).renewPosition(-vertexPositionList.at(i).x, vertexPositionList.at(i).y);
		return;
	}

	Piece() {};
	Piece(std::vector<Position>positionList) {
		vertexPositionList.reserve(positionList.size() + 1);
		for (int i = 0; i < positionList.size(); ++i)
			vertexPositionList.push_back(relativePosition(positionList.at(0), positionList.at(i)));
		vertexPositionList.push_back(Position(0, 0));

		area = areaCalucurasion();

		this->canUse = true;
		return;
	}
	Piece absolutePiecePosition(Position base) {//引数のPositionを基準に合わせる形で全点をずらす。(まともな関数名募集中)
		Piece tmpPiece;
		for (int i = 0; i < this->vertexPositionList.size(); ++i)
			tmpPiece.vertexPositionList.push_back(Position(this->vertexPositionList.at(i).x + base.x, this->vertexPositionList.at(i).y + base.y));
		return tmpPiece;
	}
};

class Frame {

public:
	std::vector<Position> vertexPositionList;
	Frame() {};
	Frame(std::vector<Position>positionList) {
		vertexPositionList.reserve(positionList.size() + 1);
		for (int i = 0; i < positionList.size(); ++i)
			vertexPositionList.push_back(positionList.at(i));
		vertexPositionList.push_back(positionList.at(0));
		return;
	};
	Frame(Piece p) {
		vertexPositionList.reserve(p.vertexPositionList.size());
		for (int i = 0; i < p.vertexPositionList.size(); ++i)
			vertexPositionList.push_back(p.vertexPositionList.at(i));
	}

};

bool isInFrame(Piece p, std::vector<Frame> frameList) {
	/*std::cout <<p.vertexPositionList.size();*/

	std::vector<std::vector<int>> wn(frameList.size(), std::vector<int>(p.vertexPositionList.size()));
	for (int i = 0; i < frameList.size(); ++i)
		for (int j = 0; j < p.vertexPositionList.size() - 1; ++j)
			wn[i][j] = 0;

	for (int i = 0; i < frameList.size(); ++i) {//枠の数に対応するループ
		for (int j = 0; j < p.vertexPositionList.size(); ++j) {//ピースの頂点数に対応するループ
															   //std::cout << p.vertexPositionList.at(i).x << "," << p.vertexPositionList.at(i).y << std::endl;
			for (int k = 0; k < frameList.at(i).vertexPositionList.size() - 1; ++k) {//currentな枠の頂点数に対応するループ
				double vt = (double)(p.vertexPositionList.at(j).y - frameList.at(i).vertexPositionList.at(k).y) / (frameList.at(i).vertexPositionList.at(k + 1).y - frameList.at(i).vertexPositionList.at(k).y);//k,k+1のなす線分の傾き
																																																				//std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k).y << "," << frameList.at(i).vertexPositionList.at(k).x << std::endl;
																																																				//std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k + 1).y << "," << frameList.at(i).vertexPositionList.at(k + 1).x << std::endl;

				if ((frameList.at(i).vertexPositionList.at(k).y <= p.vertexPositionList.at(j).y) //始点以上終点未満のy値
					&& (frameList.at(i).vertexPositionList.at(k + 1).y > p.vertexPositionList.at(j).y)) {
					if (p.vertexPositionList.at(j).x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
					{

						//std::cout << "a " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
						wn.at(i).at(j)++;
					}
				}
				else if ((frameList.at(i).vertexPositionList.at(k).y > p.vertexPositionList.at(j).y)//あるいは終点以下始点超過のy値
					&& (frameList.at(i).vertexPositionList.at(k + 1).y <= p.vertexPositionList.at(j).y)) {
					if (p.vertexPositionList.at(j).x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
					{
						//std::cout << "b " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
						wn.at(i).at(j)--;
					}
				}
			}
		}
	}
	bool tmp = true;
	for (int j = 0; j < p.vertexPositionList.size(); ++j) {
		tmp &= (wn[0][j] != 0);
	}
	if (tmp)
	for (int i = 1; i < frameList.size(); ++i) {
		for (int j = 0; j < p.vertexPositionList.size(); ++j) {
			tmp &= (wn[i][j] == 0);
		}
	}
	return tmp;
}

class Solver {
public:
	int pieceNumber;
	std::vector<Piece> piece;
	std::vector<Frame> frame;
	std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 0 13 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";
	//とりあえずのサンプル、入力形式に応じてinfomationString弄る

	//形状情報読み込み
	void loadShapeInfomation() {

		std::vector <std::string> shapeInfoTmp = split(infomationString, ':');
		pieceNumber = stod(shapeInfoTmp.at(0));
		piece.reserve(pieceNumber);

		//ピースの読み込み
		for (int i = 1; i < pieceNumber + 1; i++) {
			std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(i), ' ');
			int vertexNumberTmp = stod(shapeInfoTmp2.at(0));
			std::vector<Position>piecePositionTmp; piecePositionTmp.reserve(vertexNumberTmp);

			for (int j = 1; j <= vertexNumberTmp * 2; j += 2) {
				piecePositionTmp.push_back(Position(stod(shapeInfoTmp2.at(j)), stod(shapeInfoTmp2.at(j + 1))));
			}
			piece.push_back(Piece(piecePositionTmp));
		}

		//枠の読み込み
		std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(pieceNumber + 1), ' ');//shapeInfoTmp[pieceNumber+1]に枠の形状情報

		int frameVertexNumberTmp = stod(shapeInfoTmp2.at(0));

		std::vector<Position> framePositionTmp; framePositionTmp.reserve((frameVertexNumberTmp + 1));

		for (int i = 1; i < frameVertexNumberTmp * 2; i += 2) {
			//std::cout << shapeInfoTmp2.at(i) << "," << shapeInfoTmp2.at(i + 1) << std::endl;
			framePositionTmp.push_back(Position(stod(shapeInfoTmp2.at(i)), stod(shapeInfoTmp2.at(i + 1))));
		}
		for (auto i = shapeInfoTmp2.begin(); i != shapeInfoTmp2.end(); ++i)
			;
		frame.push_back(Frame(framePositionTmp));

		return;
	}

	//配置情報読み込み
	void loadLayoutInfomation() {}

	//貪欲探索
	void greedySearch() {
		int biggestPieceID;
		while (true) {
			biggestPieceID = -1;

			for (int i = 1; i < pieceNumber-1; ++i) {
				if (piece.at(i).area < piece.at(i + 1).area&&piece.at(i).canUse)
					biggestPieceID = i;
			}

			if (biggestPieceID == -1)	return;
			for (int l = 0; l < 4 && piece.at(biggestPieceID).canUse; ++l) {//spin
				for (int k = 0; k < 1; ++k) {//turn
					for (int i = 0; i < 16; ++i) {//x方向
						for (int j = 0; j < 10 && piece.at(biggestPieceID).canUse; ++j) {//y方向
							Position tmpPos(i, j);
							if (isInFrame(piece.at(biggestPieceID).absolutePiecePosition(tmpPos), frame)) {
								frame.push_back(piece.at(biggestPieceID).absolutePiecePosition(tmpPos));
								piece.at(biggestPieceID).canUse = false;
							}
						}
					}
					piece.at(biggestPieceID).turn();
				}
				piece.at(biggestPieceID).spin();
			}
			piece.at(biggestPieceID).canUse = false;
		}
	}
};

int main() {
	Solver solve;
	solve.loadShapeInfomation();
	/*
	for (auto i = solve.piece.begin(); i != solve.piece.end(); ++i) {
	for (int turning = 0; turning < 2; ++turning) {
	i->turn();
	for (int spining = 0; spining < 4; ++spining) {
	i->spin();
	for (auto j = i->vertexPositionList.begin(); j != i->vertexPositionList.end(); ++j) {
	std::cout << j->x << "," << j->y << std::endl;
	}
	std::cout << std::endl;
	}
	}
	}*/
	solve.greedySearch();
	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
		for (int j = 0; j < i->vertexPositionList.size(); ++j)
			std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
		puts("");
	}
	//std::vector<Position> p(0);
	//p.push_back(Position(4, 0));	p.push_back(Position(4, 3));	p.push_back(Position(0, 3));
	//Piece a(p);
	//puts("");
	//std::cout << isInFrame(a.absolutePiecePosition(Position(4, 0)), solve.frame) << "," << a.area << std::endl;
	return 0;

}
