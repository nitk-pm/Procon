#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <tuple>
#include <string>
#include <queue>
#include <assert.h>
//constexpr double compilePow(double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow(mantissa, exponent - 1); }
//const double EPS = compilePow(0.1,9);
const int INF = 10000000;
#define rep(i,n) for (int i=0;i<n;++i) //デバッグ専用！！！！！！！！！！！！！！

//todoなど
/*


接触判定・Frameの更新 <-Frameクラスをリスト化して全てにCrossing numberをかける<-wn をXOR?
--多分AC
線分の交差判定でやってはどうか？


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

！内外判定実装変更予定！
ピースの重心でWN 外側ならピースは入らない
内側なら線分の交差をチェック、この時枠の辺上にあればwn[i]の値を書き換える
線分が交差していなければWNを実行

！もっと！内外判定実装変更予定！
WNをCNにすれば

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
	bool operator<(const Position& p)const;
	bool operator>(const Position& p)const;
	bool operator==(const Position& p)const;
	bool operator!=(const Position& p)const;


	double x, y;

	void renewPosition(double a, double b) { x = a; y = b; return; }
	Position() {};
	Position(double xValue, double yValue) { x = xValue; y = yValue; }
};
bool Position::operator<(const Position& p)const {
	return (this->x == p.x ? false : this->x < p.x ? true : false);
}
bool Position::operator>(const Position& p)const {
	return (this->x == p.x ? false : this->x > p.x ? true : false);
}
bool Position::operator==(const Position& p)const {
	return (this->x == p.x&&this->y == p.y);
}
bool Position::operator!=(const Position& p)const {
	return (this->x != p.x || this->y != p.y);
}


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
			tmp += cross(this->vertexPositionList.at(i), this->vertexPositionList.at(i + 1)) / 2;
		}
		return tmp;
	}

	Position centerCaluculation() {
		double x = 0, y = 0;
		for (int i = 0;i < vertexPositionList.size() - 1;++i) {
			x += vertexPositionList.at(i).x;
			y += vertexPositionList.at(i).y;
		}
		return Position(x / (vertexPositionList.size() - 1), y/(vertexPositionList.size()-1));
	}

public:

	bool operator <(const Piece& p)const;
	bool operator >(const Piece& p)const;
	bool operator ==(const Piece& p)const;
	bool operator !=(const Piece& p)const;

	std::vector<Line> lineList;
	double area;
	Position center;

	std::vector<Position> vertexPositionList;

	void spin() { //+pi/2 (r cos(a+pi/2)=-r sin(a) (r sin(a+pi/2)=r cos(a))
		for (int i = 0; i < vertexPositionList.size(); ++i)
			vertexPositionList.at(i).renewPosition(-vertexPositionList.at(i).y, vertexPositionList.at(i).x);
		return;
	}

	void turn() {
		for (int i = 0; i < vertexPositionList.size(); ++i)
			vertexPositionList.at(i).renewPosition(vertexPositionList.at(i).x, -vertexPositionList.at(i).y);
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
		center = centerCaluculation();
		return;
	};

	Piece absolutePiecePosition(Position base) {//引数のPositionを基準に合わせる形で全点をずらす。
		Piece tmpPiece;
		for (int i = 0; i < this->vertexPositionList.size(); ++i)
			tmpPiece.vertexPositionList.push_back(Position(this->vertexPositionList.at(i).x + base.x, this->vertexPositionList.at(i).y + base.y));
		tmpPiece.center=tmpPiece.centerCaluculation();
		return tmpPiece;
	}
};

bool Piece::operator <(const Piece& p)const {
	return (this->area < p.area);
}
bool Piece::operator >(const Piece& p)const {
	return (this->area > p.area);
}
bool Piece::operator ==(const Piece& p)const {
	if (this->vertexPositionList.size() == p.vertexPositionList.size()) {
		bool tmp = true;
		for (int i = 0; i < this->vertexPositionList.size(); ++i)
			tmp &= (this->vertexPositionList.at(i) == p.vertexPositionList.at(i));
		return tmp;
	}
	else return false;
}
bool Piece::operator !=(const Piece& p)const {
	return !(*this == p);
}


class Frame {
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
public:

	std::vector<Position> vertexPositionList;
	std::vector<Line> lineList;

	Frame() {};
	Frame(std::vector<Position>positionList) {
		vertexPositionList.reserve(positionList.size() + 1);
		for (int i = 0; i < positionList.size(); ++i)
			vertexPositionList.push_back(positionList.at(i));
		vertexPositionList.push_back(positionList.at(0));
		lineCalculation();
		return;
	};
	Frame(Piece p) {
		vertexPositionList.reserve(p.vertexPositionList.size());
		for (int i = 0; i < p.vertexPositionList.size(); ++i)
			vertexPositionList.push_back(p.vertexPositionList.at(i));
		this->lineList = p.lineList;
	}


};

bool isInFrame(Piece p, std::vector<Frame> frameList) {
	/*/
	//重心のwinding number
	std::vector<int> wn_(frameList.size(), 0);
	Position centerOfGravity(p.center);

	for (int i = 0; i < frameList.size(); ++i) {//枠の数に対応するループ
												//std::cout << p.vertexPositionList.at(i).x << "," << p.vertexPositionList.at(i).y << std::endl;
		for (int k = 0; k < frameList.at(i).vertexPositionList.size() - 1; ++k) {//currentな枠の頂点数に対応するループ
			double vt = (double)(centerOfGravity.y - frameList.at(i).vertexPositionList.at(k).y) / (frameList.at(i).vertexPositionList.at(k + 1).y - frameList.at(i).vertexPositionList.at(k).y);//k,k+1のなす線分の傾き
																																																 //std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k).y << "," << frameList.at(i).vertexPositionList.at(k).x << std::endl;
																																																 //std::cout << "pos: " << frameList.at(i).vertexPositionList.at(k + 1).y << "," << frameList.at(i).vertexPositionList.at(k + 1).x << std::endl;

			if ((frameList.at(i).vertexPositionList.at(k).y <= centerOfGravity.y) //始点以上終点未満のy値
				&& (frameList.at(i).vertexPositionList.at(k + 1).y > centerOfGravity.y)) {
				if (centerOfGravity.x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
				{

					//std::cout << "a " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
					wn_.at(i)++;
				}
			}
			else if ((frameList.at(i).vertexPositionList.at(k).y > centerOfGravity.y)//あるいは終点以下始点超過のy値
				&& (frameList.at(i).vertexPositionList.at(k + 1).y <= centerOfGravity.y)) {
				if (centerOfGravity.x < (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))))
				{
					//std::cout << "b " << p.vertexPositionList.at(j).x << "," << (frameList.at(i).vertexPositionList.at(k).x + (vt * (frameList.at(i).vertexPositionList.at(k + 1).x - frameList.at(i).vertexPositionList.at(k).x))) << std::endl;
					wn_.at(i)--;
				}
			}
		}
	}
	bool tmp3 = true;

	tmp3 &= (wn_[0] != 0);

		for (int i = 1; i < frameList.size(); ++i) 
			tmp3 &= (wn_[i] == 0);

			if (tmp3) {
				/*/
				std::vector<std::vector<int>> wn(frameList.size(), std::vector<int>(p.vertexPositionList.size()));//辺上の点を検出するため先に宣言しておく

				//線分の交差判定による内外判定
				bool tmp1 = false;
				for (int i = 0; i < p.vertexPositionList.size() - 1; ++i) {//ピースのなす線分のループ
					Position a(p.vertexPositionList.at(i).x, p.vertexPositionList.at(i).y), b(p.vertexPositionList.at(i + 1).x, p.vertexPositionList.at(i + 1).y);

					for (int j = 0; j < frameList.size(); ++j) {//Frameの数のループ
						for (int k = 0; k < frameList.at(j).vertexPositionList.size() - 1; ++k) {//currentなFrameのなす線分のループ
							Position c(frameList.at(j).vertexPositionList.at(k).x, frameList.at(j).vertexPositionList.at(k).y), d(frameList.at(j).vertexPositionList.at(k + 1).x, frameList.at(j).vertexPositionList.at(k + 1).y);

							double
								tc = (a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x),
								td = (a.x - b.x)*(d.y - a.y) + (a.y - b.y)*(a.x - d.x),
								ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x),
								tb = (c.x - d.x)*(b.y - c.y) + (c.y - d.y)*(c.x - b.x);

							if (ta == 0)
								wn[j][i] += 99;
							if (tb == 0)
								wn[j][i + 1] += 99;

							tmp1 |= ta*tb < 0 && tc*td < 0;

						}
					}
				}


				//widing number algorithmによる全点の内外判定

				/*std::cout <<p.vertexPositionList.size();*/

				if (!tmp1) {
					
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

					return tmp2;
				}
		//}
		return false;
	}

class Solver {
public:
	int pieceNumber;
	std::vector<Piece> piece;
	std::vector<Frame> frame;
	std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 13 0 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";
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
				piecePositionTmp.push_back(Position(stod(shapeInfoTmp2.at(j)), stod(shapeInfoTmp2.at(j + 1))));
			}
			piece.push_back(Piece(piecePositionTmp));
		}

		//枠の読み込み
		std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(pieceNumber + 1), ' ');//shapeInfoTmp[pieceNumber+1]に枠の形状情報

		int frameVertexNumberTmp = stoi(shapeInfoTmp2.at(0));

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
		std::vector<bool> usableList(this->pieceNumber, true);

		int biggestPieceID;
		while (true) {
			biggestPieceID = -1;

			for (int i = 0; i < pieceNumber - 1; ++i) {
				if (!usableList.at(i + 1) && usableList.at(i) || piece.at(i).area < piece.at(i + 1).area&&usableList.at(i))
					biggestPieceID = i;
			}

			if (biggestPieceID == -1)	return;
			std::cout << biggestPieceID << "\n";

			for (int i = 0; i < 16; ++i) {//x方向
				for (int j = 0; j < 10 && usableList.at(biggestPieceID); ++j) {//y方向
					Position tmpPos(i, j);
					if (isInFrame(piece.at(biggestPieceID).absolutePiecePosition(tmpPos), frame)) {
						frame.push_back(piece.at(biggestPieceID).absolutePiecePosition(tmpPos));


						for (int tm = 0; tm < piece.at(biggestPieceID).vertexPositionList.size(); ++tm)

							std::cout << "     " << piece.at(biggestPieceID).vertexPositionList.at(tm).x << "," << piece.at(biggestPieceID).vertexPositionList.at(tm).y << std::endl;
						usableList.at(biggestPieceID) = false;
					}
				}
			}
			usableList.at(biggestPieceID) = false;
		}
	}

	//山登り探索
	void hillClimbingsearch() {

		std::vector<bool> usableList(this->pieceNumber, true);

		while (true) {
			for (int cnt = 0;cnt < piece.size();++cnt) {

				std::priority_queue<std::tuple<double,int,Position>>EvalutionList;

				for (int biggestPieceID = 0; biggestPieceID < piece.size(); ++biggestPieceID) {
					if (!usableList.at(biggestPieceID))
						continue;
					//std::cout << biggestPieceID << "\n";

					for (int i = 0; i < 16; ++i) {//x方向
						for (int j = 0; j < 10; ++j) {//y方向

							Position tmpPos(i, j);
						
							//for (int spin = 0;spin<4;++spin) {
								//piece.at(biggestPieceID).spin();
								//for (int turn = 0;turn < 4;++turn) {
								//	piece.at(biggestPieceID).turn();
									if (isInFrame(piece.at(biggestPieceID).absolutePiecePosition(tmpPos), frame)) {

										//if (biggestPieceID == 6) { std::cout << tmpPos.x << "," << tmpPos.y << std::endl; }

										for (int k = 0; k < piece.at(biggestPieceID).lineList.size(); ++k) {//currentなピースの線分に対するループ
											for (int l = 0; l < frame.size(); ++l) {//フレームの数に対するループ
												for (int m = 0; m < frame.at(l).lineList.size(); ++m) {//フレームの線分に対するループ
													
													
													Line tmpLine(piece.at(biggestPieceID).lineList.at(k));

													tmpLine.intercept += (tmpLine.slope == INF) ? i : (tmpLine.slope == 0) ? j : (tmpLine.slope*i - j);


													if (tmpLine.intercept == frame.at(l).lineList.at(m).intercept &&
														tmpLine.slope == frame.at(l).lineList.at(m).slope) {//傾きと切片が等しいとき(＝同じ式で表せるとき）
														double tmpEval = 0;

														double tmpLength = std::min(piece.at(biggestPieceID).lineList.at(k).length, frame.at(l).lineList.at(m).length);

														piece.at(biggestPieceID).lineList.at(k).length -= tmpLength;
														frame.at(l).lineList.at(m).length -= tmpLength;

														tmpEval += pow(tmpLength, 2);
														EvalutionList.push(std::tuple<double, int, Position>(tmpEval, biggestPieceID, tmpPos));
														
														double d_dtmp = std::get<0>(EvalutionList.top());
														int d_itmp = std::get<1>(EvalutionList.top());
														Position d_ptmp = std::get<2>(EvalutionList.top())
;														
														/*std::cout << d_itmp << ":" << d_ptmp.x << "," << d_ptmp.y << std::endl;

														if (!isInFrame(piece.at(d_itmp).absolutePiecePosition(d_ptmp), frame)) {
															std::cout << std::endl << d_itmp << ":" << d_ptmp.x << "," << d_ptmp.y << std::endl;
															rep(i, piece.at(d_itmp).vertexPositionList.size())	std::cout << piece.at(d_itmp).vertexPositionList.at(i).x << "," << piece.at(d_itmp).vertexPositionList.at(i).y << std::endl;	puts("");
														}

														assert(isInFrame(piece.at(d_itmp).absolutePiecePosition(d_ptmp),frame));
														assert(isInFrame(piece.at(EvalutionList.top().second.first).absolutePiecePosition(EvalutionList.top().second.second), frame));
														if (biggestPieceID == 6){std::cout << i << "," << j<< std::endl;}
														std::cout << "a\n";
														*/
													}
												}
											}
										}
									}
								}
								//for (int tm = 0;tm < piece.at(biggestPieceID).vertexPositionList.size();++tm)
								//	std::cout << "     " << piece.at(biggestPieceID).vertexPositionList.at(tm).x << "," << piece.at(biggestPieceID).vertexPositionList.at(tm).y << std::endl;
							//}
						//}
					}
				}
				if (!EvalutionList.empty()) {
					int itmp = std::get<1>(EvalutionList.top());
					Position ptmp = std::get<2>(EvalutionList.top());
					/*std::cout << itmp << "\n";
					std::cout << ptmp.x << "," << ptmp.y << std::endl;
					
					if (!isInFrame(piece.at(itmp).absolutePiecePosition(ptmp), frame)) {
						std::cout << std::endl << itmp << ":" << ptmp.x << "," << ptmp.y << std::endl;
						rep(i, piece.at(itmp).vertexPositionList.size())	std::cout << piece.at(itmp).vertexPositionList.at(i).x << "," << piece.at(itmp).vertexPositionList.at(i).y << std::endl;	puts("");
					}
					assert(isInFrame(piece.at(itmp).absolutePiecePosition(ptmp), frame));*/

					frame.push_back(piece.at(itmp).absolutePiecePosition(ptmp));
					usableList.at(itmp) = false;
				}
			}
			return;
		}
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
	/*
	solve.greedySearch();
	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
	for (int j = 0; j < i->vertexPositionList.size(); ++j)
	std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
	puts("");
	}
	/**/

	//山登り探索確認
	//

	solve.hillClimbingsearch();
	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
	for (int j = 0; j < i->vertexPositionList.size(); ++j)
	std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
	puts("");
	}
	/**/

	//線分確認
	/*
	for (int i = 0; i < solve.piece.size() - 1; ++i) {
	for (int j = 0; j < solve.piece.at(i).lineList.size() - 1; ++j) {
	std::cout << "m=" << solve.piece.at(i).lineList.at(j).slope << "\nc=" << solve.piece.at(i).lineList.at(j).intercept << "\nl=" << solve.piece.at(i).lineList.at(j).length << "\n\n";
	}
	}
	/**/

	//内外判定確認
	/*/
	std::vector<Position> p(0);
	std::vector<Position> f(0);
	//p.push_back(Position(6, 2));	p.push_back(Position(0, 7));	p.push_back(Position(0, 0));
	p.push_back(Position(4, 0));	p.push_back(Position(4, 3));	p.push_back(Position(0, 3));

	f.push_back(Position(5, 1));f.push_back(Position(5, 0));f.push_back(Position(7, 0));f.push_back(Position(7, 3));f.push_back(Position(0, 3));f.push_back(Position(0, 0));f.push_back(Position(2, 0));f.push_back(Position(2, 1));
	Piece a(p);
	Piece b(f);
	//solve.frame.push_back(Frame(b));
	//a.turn();
	//rep(i, 4)		a.spin();
	//rep(i, a.vertexPositionList.size())	std::cout << a.vertexPositionList.at(i).x << "," << a.vertexPositionList.at(i).y << std::endl;	puts("");

	std::cout << isInFrame(b.absolutePiecePosition(Position(5,7)), solve.frame) << "," << a.area << "\n\n";



	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
		for (int j = 0; j < i->vertexPositionList.size(); ++j)
			std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
		puts("");
	}
	/**/

	return 0;
}
