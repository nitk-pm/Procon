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


constexpr double compilePow(double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow(mantissa, exponent - 1); }

#define rep(i,n) for (int i=0;i<n;++i) //デバッグ専用！！！！！！！！！！！！！！

//todoなど
/*

全てピースの基準点を左上と右下で二つ<-精度氏is死なら右上左下も追加-Maintain
JSONで入出力
単体テストの追加
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

bool isInFrameBit(std::unordered_set<Position> p, Position pos, std::bitset<101 * 65> f) {

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

	void climbForBeam(ParStruct par, std::priority_queue<std::pair<int, ParStruct>> *chi, int width) {

		std::vector<bool> usableList = par.usableList;
		std::unordered_set<Position> evalPos = par.evalPos;

		auto currentFrame = par.frame;

		std::priority_queue<std::tuple<int, int, Position>>evalutionList; //左から、評価値、インデックス値、絶対座標の始点

		for (int currentPieceID = 0; currentPieceID < piece.size() - 1; ++currentPieceID) {//全てのusableなピースに対して探索を行う
			if (!usableList.at(currentPieceID))
				continue;

			for (auto posCandidate = evalPos.begin(); posCandidate != evalPos.end(); ++posCandidate) {//探索座標(i,j)を指定
				int i = posCandidate->x, j = posCandidate->y;
				Position tmpPos(i, j);

				if (piece.at(currentPieceID).minX + i >= 0 && piece.at(currentPieceID).maxX + i <= 100 &&
					piece.at(currentPieceID).minY + j >= 0 && piece.at(currentPieceID).maxY + j <= 64 &&
					isInFrameBit(piece.at(currentPieceID).insidePositionList, tmpPos, currentFrame)) {//ぼくのかんがえたさいきょうのないがいはんてい

					int matchPointCnt = 0;
					for (int currentPieceVertexIdx = 0; currentPieceVertexIdx < piece.at(currentPieceID).vertexPositionList.size() - 1; ++currentPieceVertexIdx)//いくら何でも名前が長すぎる
						if (evalPos.find(piece.at(currentPieceID).vertexPositionList.at(currentPieceVertexIdx) + tmpPos) != evalPos.end()) //フレームの構成頂点に重なる数を評価
							++matchPointCnt;


					evalutionList.push(std::tuple<int, int, Position>(matchPointCnt, currentPieceID, tmpPos));
				}
			}

		}
		for (int cnt = 0; cnt < width && !evalutionList.empty(); ++cnt) {
			int match = std::get<0>(evalutionList.top());
			int itmp = std::get<1>(evalutionList.top());
			Position ptmp = std::get<2>(evalutionList.top());
			evalutionList.pop();

			ParStruct tmp(par);

			tmp.frame = frameMerge(currentFrame, piece.at(itmp), ptmp);
			for (int i = 0; i < piece.at(itmp).vertexPositionList.size(); ++i)
				tmp.evalPos.emplace(piece.at(itmp).vertexPositionList.at(i) + ptmp);

			tmp.usableList = samePieceDisable(usableList, itmp);
			tmp.log.push_back(std::pair<int, Position>(itmp, ptmp));

			chi->push(std::pair<int, ParStruct>(match, tmp));
		}
		return;

	}

public:
	int pieceNumber;
	std::deque<Piece> piece;
	std::vector<Frame> frame;
	std::bitset<101 * 65>frameStatus;

	std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 13 0 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";//形状情報読み込み
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
						if (edge == 1) {

							Piece minmax = piece.at(piece.size() - 8);
							sort(minmax.vertexPositionList.begin(), minmax.vertexPositionList.end());
							Position difference = minmax.vertexPositionList.back() - minmax.vertexPositionList.front();

							auto onlinePositionTmp = piece.at(piece.size() - 8).onlinePositionList;
							for (auto itr = onlinePositionTmp.begin(); itr != onlinePositionTmp.end(); ++itr)
								tmpPiece.onlinePositionList.emplace(*itr + difference);

							auto insidePositionTmp = piece.at(piece.size() - 8).insidePositionList;
							for (auto itr = insidePositionTmp.begin(); itr != insidePositionTmp.end(); ++itr)
								tmpPiece.insidePositionList.emplace(*itr + difference);
						}
						piece.push_back(tmpPiece);
						piecePositionTmp = spin(piecePositionTmp);
					}
					piecePositionTmp = turn(piecePositionTmp);
				}
			}
		}

		//枠の読み込み
		if (shapeInfoTmp.at(pieceNumber + 1).c_str() != '\0') {
			std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(pieceNumber + 1), ' ');//shapeInfoTmp[pieceNumber+1]に枠の形状情報

			int frameVertexNumberTmp = stoi(shapeInfoTmp2.at(0));

			std::vector<Position> framePositionTmp; framePositionTmp.reserve((frameVertexNumberTmp + 1));

			for (int i = 1; i < frameVertexNumberTmp * 2; i += 2) {
				framePositionTmp.push_back(Position(stoi(shapeInfoTmp2.at(i)), stoi(shapeInfoTmp2.at(i + 1))));
			}
			frame.push_back(Frame(framePositionTmp));

			int minY = 65, maxY = -65, minX = 101, maxX = -101;
			for (int i = 0; i < framePositionTmp.size() - 1; ++i) {
				minY = std::min(minY, frame.at(0).vertexPositionList.at(i).y);
				maxY = std::max(maxY, frame.at(0).vertexPositionList.at(i).y);
				minX = std::min(minX, frame.at(0).vertexPositionList.at(i).x);
				maxX = std::max(maxX, frame.at(0).vertexPositionList.at(i).x);
			}
			for (int i = minX; i <= maxX; ++i) {
				for (int j = minY; j <= maxY; ++j) {
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
	void beamSearch(int width) {

		std::unordered_set<Position> evalPosInit;
		std::vector<bool> usableInit(pieceNumber, true);
		std::vector<std::pair<int, Position>> logInit;

		for (int j = 0; j < frame.at(0).vertexPositionList.size(); ++j) {
			evalPosInit.emplace(frame.at(0).vertexPositionList.at(j));
		}

		size_t minCount = frameStatus.count();
		std::pair<std::bitset<101 * 65>, std::vector<std::pair<int, Position>>>  best;

		std::queue<ParStruct> parEval;//フレームの状況、使えるピースのリスト、探索座標、履歴

		parEval.push(ParStruct(frameStatus, usableInit, evalPosInit, logInit));

		for (int cnt = 0; cnt < pieceNumber / 16; ++cnt) {
			std::priority_queue<std::pair<int, ParStruct>> tmpEvalList;
			while (!parEval.empty()) {
				climbForBeam(parEval.front(), &tmpEvalList, width);
				if (parEval.front().frame.count() < minCount) {
					minCount = parEval.front().frame.count();
					best = std::pair<std::bitset<101 * 65>, std::vector<std::pair<int, Position>>>(parEval.front().frame, parEval.front().log);
				}
				parEval.pop();
			}
			for (int i = 0; i < width && !tmpEvalList.empty(); ++i) {
				parEval.push(tmpEvalList.top().second); tmpEvalList.pop();
			}

		}

		if (parEval.front().frame.count() < minCount) {
			minCount = parEval.front().frame.count();
			best = std::pair<std::bitset<101 * 65>, std::vector<std::pair<int, Position>>>(parEval.front().frame, parEval.front().log);
		}

		auto a = best.second;
		for (auto i = a.begin(); i != a.end(); ++i) {
			frame.push_back(piece.at(i->first).absolutePiecePosition(i->second));
		}
		//d
		rep(j, 13) {
			rep(i, 19) {
				std::cout << best.first[j * 101 + i];
			}
			puts("");
		}
	}/**/
};


int main(int argc,char* argv[]) {
	Solver solve;
	solve.loadShapeInfomation();

	//ビーム
	solve.beamSearch(argc);
	puts("");
	for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
		for (int j = 0; j < i->vertexPositionList.size(); ++j)
			std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
		puts("");
	}
	return 0;
}
