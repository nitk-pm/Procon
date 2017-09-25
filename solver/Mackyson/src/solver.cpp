#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>
#include <queue>
#include <deque>
#include <assert.h>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include "data.h"
#include <chrono>

#define time_point std::chrono::system_clock::time_point

constexpr double compilePow(double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow(mantissa, exponent - 1); }

#define rep(i,n) for (int i=0;i<n;++i) //デバッグ専用！！！！！！！！！！！！！！

//todoなど
/*

全てピースの基準点を左上と右下で二つ<-精度氏is死なら右上左下も追加-Maintain
JSONで入出力
単体テストの追加
*/

std::hash<std::bitset<101 * 65>> hash_fn;

bool timeCheck(time_point start) {
	auto now = std::chrono::system_clock::now();
	return (now < start + std::chrono::minutes(10));
}

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

bool isInFrameBit(std::vector<Position> p, Position pos, std::bitset<101 * 65> f) {

	for (auto itr = p.begin(); itr != p.end(); ++itr)
		if (!f[(itr->y + pos.y) * 101 + itr->x + pos.x])
			return false;
	return true;
}
std::bitset<101 * 65>frameMerge(std::bitset<101 * 65> f, Piece p, Position pos) {
	for (auto itr = p.insidePositionList.begin(); itr != p.insidePositionList.end(); ++itr) {
		f[(itr->y + pos.y) * 101 + itr->x + pos.x] = 0;
	}
	for (auto itr = p.onlinePositionList.begin(); itr != p.onlinePositionList.end(); ++itr) {
		f[(itr->y + pos.y) * 101 + itr->x + pos.x] = 0;
	}
	return f;
}

class Solver {

	std::vector<bool> samePieceDisable(std::vector<bool> usableList, int i) {
		std::vector<bool>tmp(usableList);
		i /= 16;
		for (int j = 0; j < 16; ++j)
			tmp.at(16 * i + j) = false;
		return tmp;
	}

	void climbForBeam(ParStruct par, std::vector<std::pair<int, ParStruct>> *chi, int width) {

		std::vector<bool> usableList = par.usableList;
		std::vector<Position> evalPos = par.evalPos;

		auto currentFrame = par.frame;

		std::priority_queue<Evalution>evalutionList; //左から、評価値、インデックス値、絶対座標の始点

		for (int currentPieceID = 0; currentPieceID < piece.size() - 1; ++currentPieceID) {//全てのusableなピースに対して探索を行う
			if (!usableList.at(currentPieceID))
				continue;

			for (auto posCandidate = evalPos.begin(); posCandidate != evalPos.end(); ++posCandidate) {//探索座標(i,j)を指定
				int i = posCandidate->x, j = posCandidate->y;
				Position tmpPos(i, j);
				bool prevIsMatch = false;
				if (piece.at(currentPieceID).minX + i >= fminX && piece.at(currentPieceID).maxX + i <= fmaxX &&
					piece.at(currentPieceID).minY + j >= fminY && piece.at(currentPieceID).maxY + j <= fmaxY &&
					isInFrameBit(piece.at(currentPieceID).insidePositionList, tmpPos, currentFrame)) {//ぼくのかんがえたさいきょうのないがいはんてい

					int evalPointCnt = 0;
					for (int currentPieceVertexIdx = 0; currentPieceVertexIdx < piece.at(currentPieceID).vertexPositionList.size() - 1; ++currentPieceVertexIdx)//いくら何でも名前が長すぎる
						if (std::find(evalPos.begin(), evalPos.end(), piece.at(currentPieceID).vertexPositionList.at(currentPieceVertexIdx) + tmpPos) != evalPos.end()) //フレームの構成頂点に重なる数を評価
						{

							evalPointCnt += 5000;
							if (prevIsMatch)
								evalPointCnt += 500;
							prevIsMatch = true;
						}
						else prevIsMatch = false;
						evalPointCnt += piece.at(currentPieceID).area>0 ? piece.at(currentPieceID).area:-piece.at(currentPieceID).area;
						evalutionList.push(Evalution(evalPointCnt, currentPieceID, tmpPos));
				}
			}

		}
		for (int cnt = 0; cnt < width && !evalutionList.empty(); ++cnt) {
			int match = evalutionList.top().point;
			int itmp = evalutionList.top().idx;
			Position ptmp = evalutionList.top().pos;
			evalutionList.pop();

			ParStruct tmp(par);

			tmp.frame = frameMerge(currentFrame, piece.at(itmp), ptmp);
			for (int i = 0; i < piece.at(itmp).vertexPositionList.size(); ++i) {
				auto tmpPos = std::find(tmp.evalPos.begin(), tmp.evalPos.end(), piece.at(itmp).vertexPositionList.at(i) + ptmp);
				if (tmpPos == tmp.evalPos.end())
					tmp.evalPos.push_back(piece.at(itmp).vertexPositionList.at(i) + ptmp);
			}
			tmp.usableList = samePieceDisable(usableList, itmp);
			tmp.log.push_back(std::pair<int, Position>(itmp, ptmp));

			chi->push_back(std::pair<int, ParStruct>(match, tmp));
		}
		return;

	}

public:
	int pieceNumber;
	std::vector<Piece> piece;
	std::vector<Frame> frame;
	std::bitset<101 * 65>frameStatus;
	int fminY = 65, fmaxY = -65, fminX = 101, fmaxX = -101;

	/*template*///std::string infomationString = "";

	/*miniSample*///std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 13 0 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";//形状情報読み込み
	/*sample*/std::string infomationString = "38:9 0 0 26 0 26 5 18 5 18 17 15 24 10 24 4 20 0 20:9 8 0 32 0 32 7 24 7 24 12 8 12 0 17 0 5 8 5:4 0 5 19 4 19 0 0 0:6 10 4 9 8 4 6 0 6 0 0 4 0:6 9 2 8 6 6 16 0 16 0 0 4 0:4 7 0 9 8 0 8 2 0:5 10 0 12 8 5 7 0 5 1 0:5 2 0 1 5 0 9 7 11 7 2:4 7 1 9 10 0 9 0 0:5 17 3 20 14 0 8 1 0 8 2:5 12 7 9 0 0 4 2 12 6 11:6 10 10 12 6 19 0 10 0 0 10 1 14:11 38 9 38 17 46 17 46 22 0 22 0 0 6 0 5 4 4 12 24 18 34 12:5 0 5 4 2 8 0 13 10 3 16:4 0 2 4 0 9 9 5 12:6 10 0 14 7 6 11 2 14 0 5 4 4:8 0 12 9 19 14 19 39 5 39 0 8 0 8 7 0 7:9 36 7 35 12 27 6 11 6 1 16 0 12 3 5 11 0 27 0:5 7 0 9 10 0 9 1 5 2 0:5 1 0 0 5 7 8 12 9 10 1:5 2 0 0 9 5 11 9 12 9 3:4 0 9 8 11 5 1 0 0:10 7 24 20 24 22 15 23 10 24 6 16 0 9 0 2 6 0 10 3 17:4 2 0 0 9 7 11 7 2:4 12 3 15 11 0 9 0 0:6 44 0 44 5 25 5 1 20 0 15 25 1:7 0 17 12 17 12 9 8 0 27 0 27 22 0 22:6 6 18 3 11 0 6 0 0 25 0 25 18:5 18 15 26 6 38 0 24 0 0 15:5 0 15 25 15 25 0 20 0 8 6:4 3 10 10 9 7 0 0 2:5 0 3 4 2 11 0 15 9 3 12:10 52 9 52 17 5 17 5 9 0 0 13 0 11 9 18 11 33 13 40 12:4 6 0 6 6 0 6 0 0:6 12 6 14 12 3 13 1 5 0 0 12 0:4 2 6 9 5 6 0 0 0:4 10 7 3 9 0 1 7 0:5 0 1 11 0 14 8 10 9 3 11:9 46 59 62 59 73 59 73 64 100 64 100 0 0 0 0 64 46 64";//形状情報読み込み
	/*sunameri*///std::string infomationString = "23:9 7 16 5 12 3 10 0 6 0 0 8 1 14 1 14 27 12 21:11 11 16 16 21 12 21 5 17 2 14 0 10 1 3 4 0 4 6 7 10 9 12:7 19 7 21 13 0 13 0 8 5 0 8 3 15 7:12 24 12 21 15 20 22 14 18 3 5 0 1 11 0 20 3 32 1 38 4 38 13 32 13:11 44 10 39 18 26 18 16 16 8 18 0 15 12 4 16 5 14 0 27 8 34 11:13 31 28 33 32 23 33 16 30 3 22 0 8 4 6 2 2 6 0 7 4 11 7 14 11 25 24:10 13 18 15 23 11 22 8 19 6 15 3 10 1 7 0 3 8 0 10 4:4 2 6 0 2 4 0 6 4:12 7 12 3 9 2 5 0 1 7 0 20 1 36 0 45 1 45 15 39 12 27 14 18 11:7 6 12 3 8 0 2 3 0 7 0 12 4 14 8:8 13 10 5 13 1 15 0 11 5 8 5 4 7 0 10 6:7 18 4 6 15 6 9 0 9 1 4 8 0 15 1:9 12 2 13 6 15 9 8 11 3 9 0 6 2 1 7 0 8 4:4 6 6 0 6 0 0 6 0:7 20 0 28 3 36 1 46 3 42 8 0 8 14 0:11 29 9 22 8 15 12 9 8 4 10 0 6 8 1 12 0 17 2 24 0 27 5:8 17 4 15 8 4 7 0 5 0 0 28 0 24 2 20 2:6 8 8 3 9 0 4 2 0 13 1 13 5:8 5 6 0 2 4 0 50 0 50 6 41 5 25 6 12 5:7 15 6 14 11 9 14 0 12 0 0 4 4 9 2:10 10 19 6 20 2 16 3 12 0 8 4 0 8 2 6 6 9 11 7 16:8 6 5 2 13 5 17 4 21 8 25 0 30 0 0 6 0:5 0 1 9 3 14 0 14 6 0 14:8 10 5 90 5 90 60 69 60 69 55 56 55 52 60 10 60";//形状情報読み込み
	/*50*///std::string infomationString = "49:4 15 8 0 8 3 0 19 4:7 14 15 7 15 6 11 3 6 0 0 11 0 17 7:5 13 15 5 13 0 0 8 4 17 11:6 17 17 8 10 0 6 4 0 11 5 17 5:5 14 13 6 11 0 4 3 4 9 0:5 20 9 0 9 7 3 16 0 19 5:6 13 13 7 13 0 8 0 4 7 3 13 0:6 13 12 0 12 1 3 9 2 16 0 20 6:5 10 4 13 10 4 13 0 7 3 0:9 25 32 11 32 4 28 0 22 0 5 12 0 6 6 6 19 11 25:5 15 9 0 9 2 2 9 2 16 0:9 24 25 20 31 14 35 0 28 12 28 17 22 17 9 21 0 24 8:6 13 9 7 12 0 13 0 0 6 4 13 5:9 19 14 14 14 6 13 0 13 2 6 1 0 6 2 10 6 15 8:8 5 25 0 19 0 6 6 0 16 0 22 6 22 19 17 25:6 15 11 8 13 1 13 0 6 6 6 14 0:5 22 8 0 8 8 5 10 0 17 2:7 12 17 7 11 0 9 6 6 5 0 13 3 14 10:8 13 27 6 26 0 22 7 19 3 13 7 10 9 4 13 0:9 3 22 0 14 2 6 2 0 16 0 12 4 10 10 6 13 10 19:8 20 17 15 15 11 11 6 9 0 4 11 4 15 1 20 0:4 0 14 0 2 6 0 8 11:4 10 8 0 3 6 0 12 0:4 6 0 6 6 0 6 0 0:4 15 13 0 5 2 0 17 5:5 20 8 15 9 11 12 0 12 5 0:5 4 9 2 14 0 3 9 0 10 6:6 0 16 0 4 9 5 15 0 18 8 15 11:4 0 21 0 0 8 8 4 18:6 0 0 11 0 14 9 14 14 4 18 8 8:6 0 11 0 7 4 4 14 0 14 9 6 10:6 0 2 6 1 14 0 10 5 5 9 0 17:7 0 17 5 9 10 5 14 0 14 9 15 13 9 18:4 0 0 4 0 4 16 0 18:4 0 0 7 0 16 9 3 9:4 0 0 11 0 10 4 0 10:4 0 0 25 0 23 5 0 5:4 0 0 21 0 15 9 9 9:4 3 0 11 0 6 12 0 12:4 0 6 10 0 8 8 0 12:8 0 5 10 0 16 0 19 6 17 11 13 14 8 14 3 13:8 1 10 0 6 12 0 18 0 24 5 20 11 17 5 11 5:4 6 0 15 0 6 9 0 9:11 8 14 13 14 17 11 19 6 23 0 24 6 22 13 14 19 8 19 0 16 3 13:4 0 3 16 0 24 3 15 8:5 0 9 9 0 18 0 23 3 16 6:8 12 8 0 13 2 5 11 0 22 0 32 5 28 14 22 8:7 0 6 7 3 2 0 13 0 17 4 19 9 8 9:5 0 0 12 0 12 6 6 9 4 4:5 0 0 100 0 100 56 92 64 0 64";//形状情報読み込み
	/*takamatsu*///std::string infomationString = "23:7 5 0 16 0 16 14 11 10 11 4 5 4 0 0:4 7 0 7 14 0 14 0 0:6 0 0 5 4 5 10 11 10 16 14 0 14:8 0 0 30 0 30 13 25 13 25 4 5 4 5 13 0 13:4 0 5 0 0 12 0 12 5:11 5 24 5 10 12 10 12 5 0 5 0 0 27 0 27 5 21 5 21 10 12 24:4 6 6 0 6 0 0 6 0:8 0 14 9 0 15 0 7 16 12 18 15 11 15 27 0 27:8 30 10 0 10 0 5 12 5 12 0 18 0 18 5 30 5:8 6 0 11 0 11 5 17 5 17 10 0 10 0 5 6 5:5 0 0 13 0 6 10 6 5 0 5:4 0 5 0 0 12 0 12 5:10 8 27 8 11 5 18 0 16 8 0 13 0 20 13 17 16 13 9 13 27:7 0 18 0 0 4 7 7 4 9 15 27 15 30 18:4 7 0 10 3 3 13 0 10:6 4 0 15 0 12 3 21 13 0 13 7 3:6 6 0 9 3 19 3 9 24 7 13 0 0:4 3 0 12 10 9 13 0 3:3 0 0 9 0 9 10:4 9 12 0 3 3 0 12 9:6 4 6 0 6 3 0 9 0 6 3 9 6:8 10 18 15 18 18 21 0 21 10 0 13 3 9 12 6 18:7 0 15 4 6 1 3 12 3 15 0 15 24 6 15:5 9 9 92 9 92 43 89 46 9 46";//形状情報読み込み
	/*grid*///std::string infomationString = "37:8 12 0 12 6 18 6 18 12 6 12 6 6 0 6 0 0:8 12 6 0 6 0 0 24 0 24 6 18 6 18 12 12 12:9 0 6 0 0 6 0 6 6 18 6 18 0 24 0 24 12 0 12:8 0 0 6 0 6 6 12 6 12 12 18 12 18 18 0 18:9 6 0 18 0 18 18 12 18 12 12 6 12 6 6 0 6 0 0:4 0 0 12 0 12 12 0 12:4 0 0 12 0 12 6 0 6:5 6 0 12 0 12 12 0 12 0 0:8 0 0 6 0 6 12 12 12 12 18 6 18 6 24 0 24:8 12 0 12 6 24 6 24 12 0 12 0 6 6 6 6 0:8 0 0 6 0 6 6 12 6 12 18 6 18 6 12 0 12:10 0 0 12 0 12 6 18 6 18 18 12 18 12 12 6 12 6 6 0 6:4 0 0 6 0 6 6 0 6:9 0 6 0 0 6 0 6 6 12 6 12 18 6 18 6 12 0 12:11 0 12 0 6 6 6 6 0 18 0 18 6 12 6 12 12 6 12 6 18 0 18:6 0 0 18 0 18 12 12 12 12 6 0 6:8 0 12 0 0 24 0 24 12 18 12 18 6 6 6 6 12:6 0 0 12 0 12 6 6 6 6 18 0 18:8 0 0 18 0 18 6 12 6 12 12 6 12 6 18 0 18:8 12 0 12 24 0 24 0 18 6 18 6 6 0 6 0 0:8 0 12 0 6 6 6 6 0 18 0 18 6 12 6 12 12:6 0 6 6 6 6 0 12 0 12 18 0 18:6 18 6 18 12 0 12 0 0 12 0 12 6:5 5 0 11 0 11 6 0 6 0 0:4 0 0 13 0 13 12 0 12:6 0 0 18 0 18 6 12 6 12 12 0 12:8 0 24 0 0 6 0 6 6 12 6 12 12 6 12 6 24:8 0 12 0 0 6 0 6 6 18 6 18 0 24 0 24 12:8 0 6 6 6 6 0 12 0 12 6 18 6 18 12 0 12:8 6 24 6 12 0 12 0 6 6 6 6 0 12 0 12 24:5 0 6 0 0 12 0 12 12 0 12:6 6 18 6 12 0 12 0 0 12 0 12 18:4 0 0 6 0 6 6 0 6:4 0 6 0 0 18 0 18 6:8 6 0 12 0 12 24 6 24 6 18 0 18 0 12 6 12:4 0 0 6 0 6 12 0 12:4 0 6 0 0 6 0 6 6:8 2 2 98 2 98 62 26 62 26 56 15 56 15 62 2 62";																																																																																																																																																																																																																														 //std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 13 0 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";
	
	void loadShapeInfomation() {

		std::vector <std::string> shapeInfoTmp = split(infomationString, ':');
		pieceNumber = stoi(shapeInfoTmp.at(0));

		//ピースの読み込み
		for (int i = 1; i < pieceNumber + 1; i++) {
			std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(i), ' ');
			int vertexNumberTmp = stoi(shapeInfoTmp2.at(0));

			for (int edge = 0; edge < 2; ++edge) { //基準点設定用のループ
				std::vector<Position>piecePositionTmp; piecePositionTmp.reserve(vertexNumberTmp);

				for (int j = 1; j <= vertexNumberTmp * 2; j += 2) {
					piecePositionTmp.push_back(Position(stoi(shapeInfoTmp2.at(j)), stoi(shapeInfoTmp2.at(j + 1))));
				}


				for (int turning = 0; turning < 2; ++turning) {
					for (int spining = 0; spining < 4; ++spining) {
						Piece tmpPiece(piecePositionTmp, edge);

						piece.push_back(tmpPiece);
						piecePositionTmp = spin(piecePositionTmp);
					}
					piecePositionTmp = turn(piecePositionTmp);
				}
			}
		}

		//枠の読み込み
		if (shapeInfoTmp.at(pieceNumber + 1).c_str() != NULL) {
			std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(pieceNumber + 1), ' ');//shapeInfoTmp[pieceNumber+1]に枠の形状情報

			int frameVertexNumberTmp = stoi(shapeInfoTmp2.at(0));

			std::vector<Position> framePositionTmp; framePositionTmp.reserve((frameVertexNumberTmp + 1));

			for (int i = 1; i < frameVertexNumberTmp * 2; i += 2) {
				framePositionTmp.push_back(Position(stoi(shapeInfoTmp2.at(i)), stoi(shapeInfoTmp2.at(i + 1))));
			}
			frame.push_back(Frame(framePositionTmp));

			for (int i = 0; i < framePositionTmp.size() - 1; ++i) {
				fminY = std::min(fminY, frame.at(0).vertexPositionList.at(i).y);
				fmaxY = std::max(fmaxY, frame.at(0).vertexPositionList.at(i).y);
				fminX = std::min(fminX, frame.at(0).vertexPositionList.at(i).x);
				fmaxX = std::max(fmaxX, frame.at(0).vertexPositionList.at(i).x);
			}

			for (int i = fminX; i <= fmaxX; ++i) {
				for (int j = fminY; j <= fmaxY; ++j) {
					Position tmpPos(i, j);

					if (gridEvalution(tmpPos, framePositionTmp)) {
						frameStatus.set(j * 101 + i);
					}
				}
			}
		}

		pieceNumber *= 8 * 2; //ピース回転分8　基準点右上と左上で2
		return;
	}

	//配置情報読み込み
	void loadLayoutInfomation() {}

	//ビームサーチ
	void beamSearch(int width,time_point start) {
		int saveWidth = width;

		std::unordered_set<size_t> existingFrame;

		std::vector<Position> evalPosInit;
		std::vector<bool> usableInit(pieceNumber, true);
		std::vector<std::pair<int, Position>> logInit;

		for (int j = 0; j < frame.at(0).vertexPositionList.size(); ++j) {
			evalPosInit.push_back(frame.at(0).vertexPositionList.at(j));
		}

		size_t minCount = frameStatus.count();
		std::vector<std::pair<int, Position>>  best;

		std::vector<std::queue<ParStruct>> parEval((pieceNumber/16)+1);
		parEval[0].push(ParStruct(frameStatus, usableInit, evalPosInit, logInit));


		while (parEval[pieceNumber / 16].empty()) {
			if (!timeCheck(start))
				goto 	END_SEARCHING;
			for (int cnt = 0; cnt < pieceNumber / 16; ++cnt) {

				std::vector<std::pair<int, ParStruct>> tmpEvalList;

				if (parEval[cnt].empty())
					continue;

				for (int i = 0; i < width; ++i) {
					if (!parEval[cnt].empty()) {
						if (parEval[cnt].front().frame.count() < minCount) {
							minCount = parEval[cnt].front().frame.count();
							best = parEval[cnt].front().log;
						}
						climbForBeam(parEval[cnt].front(), &tmpEvalList, 500); //一回当たり、500個次の状態の候補を生成
						parEval[cnt].pop();
					}
				}


				std::sort(tmpEvalList.begin(), tmpEvalList.end());

				for (int i = 0;!tmpEvalList.empty(); ++i) {
					auto check = existingFrame.emplace(hash_fn(tmpEvalList.back().second.frame));
					if (check.second)
						parEval[cnt+1].push(tmpEvalList.back().second);
					tmpEvalList.pop_back();
				}

			}
		}

		best = parEval[pieceNumber / 16].front().log;
	END_SEARCHING:
		for (auto i = best.begin(); i != best.end(); ++i) {
			frame.push_back(piece.at(i->first).absolutePiecePosition(i->second));
		}

	}/**/
};

int main() {
	time_point start = std::chrono::system_clock::now();
	int argc;
	Solver solve;
	solve.loadShapeInfomation();
	argc = 1;
	//ビーム
	solve.beamSearch(argc,start);
	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
		for (int j = 0; j < i->vertexPositionList.size(); ++j)
			std::cout << i->vertexPositionList.at(j).x << std::endl << i->vertexPositionList.at(j).y << std::endl;
	}
	return 0;
}
