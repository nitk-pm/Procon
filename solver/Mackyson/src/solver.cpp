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
#include <thread>
#include "decoder.h"

#define time_point std::chrono::system_clock::time_point

constexpr double compilePow (double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow (mantissa, exponent - 1); }

#define rep(i,n) for (int i=0;i<n;++i) //デバッグ専用

std::hash<std::bitset<101 * 65>> hash_fn;

bool timeCheck (time_point start) {
	auto now = std::chrono::system_clock::now ();
	return(now < start + std::chrono::minutes (10));
}

bool isInFrameBit (std::vector<Position> p, Position pos, std::bitset<101 * 65> f) {

	for (auto itr = p.begin (); itr != p.end (); ++itr)
		if (!f[(itr->y + pos.y) * 101 + itr->x + pos.x])
			return false;
	return true;
}
std::bitset<101 * 65>frameMerge (std::bitset<101 * 65> f, Piece p, Position pos) {
	for (auto itr = p.insidePositionList.begin (); itr != p.insidePositionList.end (); ++itr) {
		f[(itr->y + pos.y) * 101 + itr->x + pos.x] = 0;
	}
	for (auto itr = p.onlinePositionList.begin (); itr != p.onlinePositionList.end (); ++itr) {
		f[(itr->y + pos.y) * 101 + itr->x + pos.x] = 0;
	}
	return f;
}


int IndependentArea (std::bitset<101 * 65> f) {
	_queue tmp;
	int cnt = 0;

	for (int i = 1; i<64; ++i)
		for (int j = 1; j < 100; ++j) {
			if (f[i * 101 + j]) {
				if (!(f[(i - 1) * 101 + j] || f[(i + 1) * 101 + j]))
					++cnt;
				if (!(f[i * 101 + j - 1] || f[i * 101 + j + 1]))
					++cnt;
			}
		}
	for (int i = 0; i<65; ++i)
		for (int j = 0; j < 101; ++j) {
			if (f[i * 101 + j]) {
				tmp.push (i * 101 + j);
				cnt += 3;
			}

			while (!tmp.empty ()) {
				int nowPos = tmp.front (); tmp.pop ();

				if (nowPos / 101 == 0) { //上端
					if (f[nowPos + 101]) {
						tmp.push (nowPos + 101);
						f[nowPos + 101] = 0;
					}
				}
				else if (nowPos / 101 == 64) {//下端
					if (f[nowPos - 101]) {
						tmp.push (nowPos - 101);
						f[nowPos - 101] = 0;
					}
				}
				else {
					if (f[nowPos + 101]) {
						tmp.push (nowPos + 101);
						f[nowPos + 101] = 0;
					}
					if (f[nowPos - 101]) {
						tmp.push (nowPos - 101);
						f[nowPos - 101] = 0;
					}
				}

				if (nowPos % 101 == 0) {//左端
					if (f[nowPos + 1]) {
						tmp.push (nowPos + 1);
						f[nowPos + 1] = 0;
					}
				}
				else if (nowPos % 101 == 100) {//右端
					if (f[nowPos - 1]) {
						tmp.push (nowPos - 1);
						f[nowPos - 1] = 0;
					}
				}
				else {
					if (f[nowPos - 1]) {
						tmp.push (nowPos - 1);
						f[nowPos - 1] = 0;
					}
					if (f[nowPos + 1]) {
						tmp.push (nowPos + 1);
						f[nowPos + 1] = 0;
					}
				}
			}
		}
	return cnt;

}

std::unordered_set<size_t> existingFrame;

class Solver {

	std::vector<bool> samePieceDisable (std::vector<bool> usableList, int i) {
		std::vector<bool>tmp (usableList);
		i /= 16;
		for (int j = 0; j < 16; ++j)
			tmp.at (16 * i + j) = false;
		return tmp;
	}

	void climbForBeam (ParStruct par, std::vector<std::pair<int, ParStruct>> *chi, int width) {

		std::vector<bool> usableList = par.usableList;
		std::vector<Position> evalPos = par.evalPos;
		evalPos.reserve (par.evalPos.size () + 12);

		auto currentFrame = par.frame;

		std::priority_queue<Evalution>evalutionList; //左から、評価値、インデックス値、絶対座標の始点

		for (int currentPieceID = 0; currentPieceID < piece.size () - 1; ++currentPieceID) {//全てのusableなピースに対して探索を行う
			if (!usableList.at (currentPieceID))
				continue;

			for (auto posCandidate = evalPos.begin (); posCandidate != evalPos.end (); ++posCandidate) {//探索座標(i,j)を指定
				int i = posCandidate->x, j = posCandidate->y;
				Position tmpPos (i, j);
				bool prevIsMatch = false;
				if (piece.at (currentPieceID).minX + i >= fminX && piece.at (currentPieceID).maxX + i <= fmaxX &&
					piece.at (currentPieceID).minY + j >= fminY && piece.at (currentPieceID).maxY + j <= fmaxY &&
					isInFrameBit (piece.at (currentPieceID).insidePositionList, tmpPos, currentFrame)) {//ぼくのかんがえたさいきょうのないがいはんてい

					int evalPointCnt = 0;
					for (int currentPieceVertexIdx = 0; currentPieceVertexIdx < piece.at (currentPieceID).vertexPositionList.size () - 1; ++currentPieceVertexIdx)//いくら何でも名前が長すぎる
						if (std::find (evalPos.begin (), evalPos.end (), piece.at (currentPieceID).vertexPositionList.at (currentPieceVertexIdx) + tmpPos) != evalPos.end ()) //フレームの構成頂点に重なる数を評価
						{

							evalPointCnt += 5000;
							if (prevIsMatch)
								evalPointCnt += 4000;
							prevIsMatch = true;
						}
						else prevIsMatch = false;
						evalPointCnt += piece.at (currentPieceID).area>0 ? piece.at (currentPieceID).area : -piece.at (currentPieceID).area;
						evalutionList.push (Evalution (evalPointCnt, currentPieceID, tmpPos));
				}
			}

		}
		for (int cnt = 0; cnt < width && !evalutionList.empty (); ++cnt) {
			int match = evalutionList.top ().point;
			int itmp = evalutionList.top ().idx;
			Position ptmp = evalutionList.top ().pos;
			evalutionList.pop ();

			ParStruct tmp (par);

			tmp.frame = frameMerge (currentFrame, piece.at (itmp), ptmp);
			for (int i = 0; i < piece.at (itmp).vertexPositionList.size (); ++i) {
				auto tmpPos = std::find (tmp.evalPos.begin (), tmp.evalPos.end (), piece.at (itmp).vertexPositionList.at (i) + ptmp);
				if (tmpPos == tmp.evalPos.end ())
					tmp.evalPos.push_back (piece.at (itmp).vertexPositionList.at (i) + ptmp);
			}
			match -= IndependentArea (tmp.frame) * 10000;

			tmp.usableList = samePieceDisable (usableList, itmp);
			tmp.log.push_back (std::pair<int, Position> (itmp, ptmp));

			chi->push_back (std::pair<int, ParStruct> (match, tmp));
		}
		return;

	}

public:
	std::vector<Piece> piece;
	std::vector<Frame> frame;
	std::bitset<101 * 65>frameStatus;
	int fminY = 65, fmaxY = -65, fminX = 101, fmaxX = -101;
	void loadShapeInfomation () {

		auto tmpPieces = getPiecesShape ();

		for (auto itr = tmpPieces.begin (); itr != tmpPieces.end (); ++itr) {
			for (int edge = 0; edge < 2; ++edge) {
				piece.push_back (Piece (*itr, edge));
			}
		}

		auto tmpFrames = getFrameShape ();
		int cnt = 0;
		for (auto itr = tmpFrames.begin (); itr != tmpFrames.end (); ++itr, ++cnt) {
			frame.push_back (*itr);
			for (int i = 0; i < frame.at (cnt).vertexPositionList.size (); ++i) {
				fminY = std::min (fminY, frame.at (cnt).vertexPositionList.at (i).y);
				fmaxY = std::max (fmaxY, frame.at (cnt).vertexPositionList.at (i).y);
				fminX = std::min (fminX, frame.at (cnt).vertexPositionList.at (i).x);
				fmaxX = std::max (fmaxX, frame.at (cnt).vertexPositionList.at (i).x);
			}
			for (int i = 0; i < 65; ++i)
				for (int j = 0; j < 101; ++j)
					if (gridEvalution (Position (j, i), frame.at (cnt).vertexPositionList) == 2)
						frameStatus[i * 101 + j] = 1;
		}
	}

	//配置情報読み込み
	void loadLayoutInfomation () {}

	//chokudaiサーチ
	void beamSearch (int time, time_point start) {
		int width = 1;

		int maxTurn = (piece.size () - 1) / 16;

		std::vector<Position> evalPosInit;
		std::vector<bool> usableInit (piece.size (), true);
		std::vector<std::pair<int, Position>> logInit;
		for (int i = 0; i < frame.size (); ++i)
			for (int j = 0; j < frame.at (i).vertexPositionList.size (); ++j) {
				evalPosInit.push_back (frame.at (i).vertexPositionList.at (j));
			}

		size_t minCount = frameStatus.count ();
		std::vector<std::pair<int, Position>>  best;

		std::vector<std::queue<ParStruct>> parEval (maxTurn + 1);

		while (parEval[maxTurn].empty ()) {
			parEval[0].push (ParStruct (frameStatus, usableInit, evalPosInit, logInit));

			if (!timeCheck (start))
				goto 	END_SEARCHING;
			for (int cnt = 0; cnt < maxTurn; ++cnt) {

				std::vector<std::pair<int, ParStruct>> tmpEvalList;

				if (parEval[cnt].empty ())
					continue;

				for (int i = 0; i < width; ++i) {
					if (!parEval[cnt].empty ()) {

						if (parEval[cnt].front ().frame.count () < minCount) {
							minCount = parEval[cnt].front ().frame.count ();
							best = parEval[cnt].front ().log;
						}

						climbForBeam (parEval[cnt].front (), &tmpEvalList, 10); //N個次の状態の候補を生成
						parEval[cnt].pop ();
					}
				}


				std::sort (tmpEvalList.begin (), tmpEvalList.end ());

				for (int i = 0; !tmpEvalList.empty (); ++i) {
					parEval[cnt + 1].push (tmpEvalList.back ().second);
					tmpEvalList.pop_back ();
				}

			}
		}

		best = parEval[maxTurn].front ().log;
	END_SEARCHING:

		for (auto i = best.begin (); i != best.end (); ++i) {
			frame.push_back (piece.at (i->first).absolutePiecePosition (i->second));
		}

	}/**/
};

int main () {
	time_point start = std::chrono::system_clock::now ();
	int argc;
	Solver solve;
	solve.loadShapeInfomation ();
	argc = 1;
	//ビーム
	solve.beamSearch (argc, start);
	for (auto i = solve.frame.begin (); i != solve.frame.end (); ++i) {
		for (int j = 0; j < i->vertexPositionList.size (); ++j)
			std::cout << i->vertexPositionList.at (j).x << std::endl << i->vertexPositionList.at (j).y << std::endl;
	}
	return 0;
}
