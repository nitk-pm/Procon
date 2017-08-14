#include <iostream>
#include <vector>
#include <list>
#include <string>
//constexpr double compilePow(double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow(mantissa, exponent - 1); }
//const double EPS = compilePow(0.1,9);
const int INF = 10000000;
//todoなど

/*


接触判定・Frameの更新 <-Frameクラスをリスト化して全てにCrossing numberをかける<-wn をXOR?
--多分AC
線分の交差判定でやってはどうか？
----どちらも必要、両方使って判定

WN->凹みをまたいだ時無反応
交差->(x1,y1)to(x2,y2)の線分があったとき横がdxかつ縦がdy以上の正方形は無反応(始点と終点は無反応なので枠外に突き抜ける)


探索		フレームを全部表示すれば一応できてるかはわかる。

pieceにboolのcanUse置くよりも現在のノードで使用済みのIDのリスト作ったほうがいいのでは…?要検討。

貪欲
面積とBottom-left
ピースごとの面積の算出
使ってないピースの中から面積が一番大きいものを選ぶ(毎回全走査、面積でピースをソート、使ったものからerase)<-貪欲をわざわざそこまで最適化する意味は…
左上から右に向かって置けるか探索、最初の置ける地点に配置
置けるピースがなくなればループ抜け
--AC?<-それっぽい出力だけどまだ検証はしてない

山登り
評価値に線分の一致する部分の長さ
ピースとフレームを構成する辺に線分の長さ、傾き、切片を定義
切片と傾きの一致する二線分の長さのmin



枝刈り
・No Fit Polygonで配置場所を枝刈り
・ピースの最小の角度で枝刈り
・

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


double sqrt(double s) {
	double x = s / 2.0;
	double lastX = 0.0;
	while (x != lastX) {//数値に変化がなくなるまで継続
		lastX = x;
		x = (x + s / x) / 2.0;//相加平均
	}
	return x;
}

//x,y座標のpair
class Position {
public:

	double x, y;

	void renewPosition(double a, double b) { x = a; y = b; return; }//DON'T USE
	Position() {};
	Position(double xValue, double yValue) { x = xValue; y = yValue; }
};

double cross(Position a, Position b) {
	return a.x*b.y - a.y * b.x;
}

//各辺の直線の式の傾きと切片のpair

class Line {

public:
	double slope, intercept, length;
	Line() {};
	Line(double slopeValue, double interceptValue, double lengthValue) { slope = slopeValue; intercept = interceptValue; length = lengthValue; return; }
};


/*Pieceクラス　頂点数と各点の座標*/
class Piece {

	void lineCalculation() {
		double slope, intercept, length;
		lineList.reserve(vertexPositionList.size() - 1);
		for (int i = 0; i < vertexPositionList.size() - 1; ++i) {
			double x1 = vertexPositionList.at(i).x, x2 = vertexPositionList.at(i + 1).x, y1 = vertexPositionList.at(i).y, y2 = vertexPositionList.at(i + 1).y;
			slope = (x1 == x2) ? INF : (y2 - y1) / (x2 - x1);
			intercept = (slope == INF) ? x1 : -slope*x1 + y1;

			//評価値にしか使わないのでルートを外した二乗の値で長さを保持、のちに修正するかも
			length = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);

			lineList.push_back(Line(slope, intercept, length));
		}

	}

	//ある点からの相対座標で表示
	Position relativePosition(Position standard, Position object) {
		double x, y;
		x = object.x - standard.x;
		y = object.y - standard.y;
		return Position(x, y);
	};

	//外積による面積の算出、
	double areaCalculation() {

		double tmp = 0;
		for (int i = 0; i < this->vertexPositionList.size() - 1; ++i) {
			tmp += cross(this->vertexPositionList.at(i), this->vertexPositionList.at(i + 1));
		}
		return tmp / 2;
	}

public:

	std::vector<Line> lineList;
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
			//vertexPositionList.at(i).renewPosition(-vertexPositionList.at(i).x, vertexPositionList.at(i).y);
			vertexPositionList.at(i).renewPosition(-vertexPositionList.at(i).x, -vertexPositionList.at(i).y);
		return;
	}

	Piece() {};
	Piece(std::vector<Position>positionList) {
		vertexPositionList.reserve(positionList.size() + 1);
		for (int i = 0; i < positionList.size(); ++i)
			vertexPositionList.push_back(relativePosition(positionList.at(0), positionList.at(i)));
		vertexPositionList.push_back(Position(0, 0));

		area = areaCalculation();
		lineCalculation();
		this->canUse = true;
		return;
	}



	Piece absolutePiecePosition(Position base) {//引数のPositionを基準に合わせる形で全点をずらす。
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

bool lineHitCheck(Position a, Position b, Position c, Position d) {
	double
		tc = (a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x),
		td = (a.x - b.x)*(d.y - a.y) + (a.y - b.y)*(a.x - d.x),
		ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x),
		tb = (c.x - d.x)*(b.y - c.y) + (c.y - d.y)*(c.x - b.x);

	return  ta*tb < 0 && tc*td < 0;
}
//線分の交差判定による内外判定
bool isInFrame(Piece p, std::vector<Frame> frameList) {
	bool tmp1 = false;
	for (int i = 0; i < p.vertexPositionList.size() - 1; ++i) {//ピースのなす線分のループ
		Position a(p.vertexPositionList.at(i).x, p.vertexPositionList.at(i).y), b(p.vertexPositionList.at(i + 1).x, p.vertexPositionList.at(i + 1).y);

		for (int j = 0; j < frameList.size(); ++j) {//Frameの数のループ
			for (int k = 0; k < frameList.at(j).vertexPositionList.size() - 1; ++k) {//currentなFrameのなす線分のループ
				Position c(frameList.at(j).vertexPositionList.at(k).x, frameList.at(j).vertexPositionList.at(k).y), d(frameList.at(j).vertexPositionList.at(k + 1).x, frameList.at(j).vertexPositionList.at(k + 1).y);

				tmp1 |= lineHitCheck(a, b, c, d);



			}
		}
	}


	//widing number algorithmによる内外判定

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
	bool tmp2 = true;
	for (int j = 0; j < p.vertexPositionList.size(); ++j) {
		tmp2 &= (wn[0][j] != 0);
	}
	if (tmp2)
		for (int i = 1; i < frameList.size(); ++i) {
			for (int j = 0; j < p.vertexPositionList.size(); ++j) {
				tmp2 &= (wn[i][j] == 0);
			}
		}
	return tmp2&&!tmp1;
}

class Solver {
public:
	int pieceNumber;
	std::vector<Piece> piece;
	std::vector<Frame> frame;
	std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 0 13 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";//:5 6 5 0 0 0 13 9 2 9 5
	//とりあえずのサンプル、入力形式に応じてinfomationString弄る

	//形状情報読み込み
	void loadShapeInfomation() {

		std::vector <std::string> shapeInfoTmp = split(infomationString, ':');
		pieceNumber = stoi(shapeInfoTmp.at(0));
		piece.reserve(pieceNumber);

		//ピースの読み込み
		for (int i = 1; i < pieceNumber + 1; i++) {
			std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(i), ' ');
			int vertexNumberTmp = stoi(shapeInfoTmp2.at(0));
			std::vector<Position>piecePositionTmp; piecePositionTmp.reserve(vertexNumberTmp);

			for (int j = 1; j <= vertexNumberTmp * 2; j += 2) {
				piecePositionTmp.push_back(Position(stod(shapeInfoTmp2.at(j+1)), stod(shapeInfoTmp2.at(j))));
			}
			piece.push_back(Piece(piecePositionTmp));
		}

		//枠の読み込み
		std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(pieceNumber + 1), ' ');//shapeInfoTmp[pieceNumber+1]に枠の形状情報

		int frameVertexNumberTmp = stoi(shapeInfoTmp2.at(0));

		std::vector<Position> framePositionTmp; framePositionTmp.reserve((frameVertexNumberTmp + 1));

		for (int i = 1; i < frameVertexNumberTmp * 2; i += 2) {
			//std::cout << shapeInfoTmp2.at(i) << "," << shapeInfoTmp2.at(i + 1) << std::endl;
			framePositionTmp.push_back(Position(stod(shapeInfoTmp2.at(i+1)), stod(shapeInfoTmp2.at(i))));
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

			for (int i = 0; i < pieceNumber - 1; ++i) {
				if (piece.at(i).area < piece.at(i + 1).area&&piece.at(i).canUse)
					biggestPieceID = i;
			}

			if (biggestPieceID == -1)	return;
			for (int l = 0; l < 3 && piece.at(biggestPieceID).canUse; ++l) {//spin
				for (int k = 0; k < 1; ++k) {//turn
					for (int i = 0; i < 16; ++i) {//x方向
						for (int j = 0; j < 10 && piece.at(biggestPieceID).canUse; ++j) {//y方向
							Position tmpPos(i, j);
							if (isInFrame(piece.at(biggestPieceID).absolutePiecePosition(tmpPos), frame)) {
								frame.push_back(piece.at(biggestPieceID).absolutePiecePosition(tmpPos));

								std::cout << biggestPieceID << "\n";
								for (int tm = 0; tm < piece.at(biggestPieceID).vertexPositionList.size(); ++tm)

									std::cout << "     " << piece.at(biggestPieceID).vertexPositionList.at(tm).x << "," << piece.at(biggestPieceID).vertexPositionList.at(tm).y << std::endl;
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
	void hillClimbingSearch() {



		return;
	}


};

int main() {
	Solver solve;
	solve.loadShapeInfomation();
	//spin turn確認
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

	//貪欲探索確認

	solve.greedySearch();
	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
		for (int j = 0; j < i->vertexPositionList.size(); ++j)
			std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
		puts("");
	}


	//線分確認
	/*	for (int i = 0; i < solve.piece.size() - 1; ++i) {
	for (int j = 0; j < solve.piece.at(i).lineList.size() - 1; ++j) {
	std::cout << "m=" << solve.piece.at(i).lineList.at(j).slope << "\nc=" << solve.piece.at(i).lineList.at(j).intercept << "\nl=" << solve.piece.at(i).lineList.at(j).length << "\n\n";
	}
	}
	*/

	//std::vector<Position> p(0);
	//p.push_back(Position(4, 0));	p.push_back(Position(4, 3));	p.push_back(Position(0, 3));
	//Piece a(p);
	//puts("");
	//std::cout << isInFrame(a.absolutePiecePosition(Position(4, 0)), solve.frame) << "," << a.area << std::endl;
	return 0;

}
