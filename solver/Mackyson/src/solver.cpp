#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <tuple>
#include <string>
#include <queue>
#include <deque>
#include <assert.h>
#include "data.h"
#include "colisionCheck.h"

constexpr double compilePow(double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*compilePow(mantissa, exponent - 1); }
//const double EPS = compilePow(0.1,9);

#define rep(i,n) for (int i=0;i<n;++i) //デバッグ専用！！！！！！！！！！！！！！

//todoなど
/*
setで探索座標を制限
WNをCNに
ビームサーチ化
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

class Solver {

        std::vector<bool> samePieceDisable(std::vector<bool> usableList,int i) {
                std::vector<bool>tmp(usableList);
                i /= 8;
                for (int j = 0;j < 8;++j)
                        tmp.at(8*i + j) = false;

                return tmp;
        }

        public:
        int pieceNumber;
        std::deque<Piece> piece;
        std::vector<Frame> frame;
        std::string infomationString = "8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 13 0 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0";
        //とりあえずのサンプル、入力形式に応じてinfomationString弄る

        //形状情報読み込み
        void loadShapeInfomation() {

                std::vector <std::string> shapeInfoTmp = split(infomationString, ':');
                pieceNumber = stoi(shapeInfoTmp.at(0));

                //ピースの読み込み
                for (int i = 1; i < pieceNumber + 1; i++) {
                        std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(i), ' ');
                        int vertexNumberTmp = stoi(shapeInfoTmp2.at(0));
                        std::vector<Position>piecePositionTmp; piecePositionTmp.reserve(vertexNumberTmp);

                        for (int j = 1; j <= vertexNumberTmp * 2; j += 2) {
                                piecePositionTmp.push_back(Position(stoi(shapeInfoTmp2.at(j)), stoi(shapeInfoTmp2.at(j + 1))));
                        }

                        Piece tmpPiece(piecePositionTmp);
                        for (int turning = 0;turning < 2;++turning) {
                                for (int spining = 0;spining < 4;++spining) {
                                        piece.push_back(tmpPiece);
                                        tmpPiece.spin();
                                }
                                tmpPiece.turn();
                        }

                }

                //枠の読み込み
                if (shapeInfoTmp.at(pieceNumber + 1).c_str() != '\0') {
                        std::vector<std::string> shapeInfoTmp2 = split(shapeInfoTmp.at(pieceNumber + 1), ' ');//shapeInfoTmp[pieceNumber+1]に枠の形状情報

                        int frameVertexNumberTmp = stoi(shapeInfoTmp2.at(0));

                        std::vector<Position> framePositionTmp; framePositionTmp.reserve((frameVertexNumberTmp + 1));

                        for (int i = 1; i < frameVertexNumberTmp * 2; i += 2) {
                                //std::cout << shapeInfoTmp2.at(i) << "," << shapeInfoTmp2.at(i + 1) << std::endl;
                                framePositionTmp.push_back(Position(stoi(shapeInfoTmp2.at(i)), stoi(shapeInfoTmp2.at(i + 1))));
                        }
                        for (auto i = shapeInfoTmp2.begin(); i != shapeInfoTmp2.end(); ++i)
                                ;
                        frame.push_back(Frame(framePositionTmp));
                }
        pieceNumber *= 8;
                return;
        }

        //配置情報読み込み
        void loadLayoutInfomation() {}

        //貪欲探索
        void greedySearch() {
                std::vector<bool> usableList(this->pieceNumber, true);

                int currentPieceID;
                while (true) {
                        currentPieceID = -1;

                        for (int i = 0; i < pieceNumber - 1; ++i) {
                                if (!usableList.at(i + 1) && usableList.at(i) || piece.at(i).area < piece.at(i + 1).area&&usableList.at(i))
                                        currentPieceID = i;
                        }

                        if (currentPieceID == -1)	return;
                        std::cout << currentPieceID << "\n";

                        for (int i = 0; i < 16; ++i) {//x方向
                                for (int j = 0; j < 10 && usableList.at(currentPieceID); ++j) {//y方向
                                        Position tmpPos(i, j);
                                        if (isInFrame(piece.at(currentPieceID).absolutePiecePosition(tmpPos), frame)) {
                                                frame.push_back(piece.at(currentPieceID).absolutePiecePosition(tmpPos));


                                                for (int tm = 0; tm < piece.at(currentPieceID).vertexPositionList.size(); ++tm)

                                                        std::cout << "     " << piece.at(currentPieceID).vertexPositionList.at(tm).x << "," << piece.at(currentPieceID).vertexPositionList.at(tm).y << std::endl;
                                                usableList.at(currentPieceID) = false;
                                        }
                                }
                        }
                        usableList.at(currentPieceID) = false;
                }
        }

        //山登り探索
        void hillClimbingsearch() {

                std::vector<bool> usableList(this->pieceNumber, true);

                while (true) {
                        for (int cnt = 0;cnt < piece.size()/8;++cnt) {

                                std::priority_queue<std::tuple<double, int, Position>>EvalutionList;

                                for (int currentPieceID = 0; currentPieceID < piece.size()-1; ++currentPieceID) {
                                        if (!usableList.at(currentPieceID))
                                                continue;

                                        for (int i = 0; i <= 16; ++i) {//x方向
                                                for (int j = 0; j <= 10; ++j) {//y方向

                                                        Position tmpPos(i, j);

                                                        if (isInFrame(piece.at(currentPieceID).absolutePiecePosition(tmpPos), frame)) {

                                                                for (int k = 0; k < piece.at(currentPieceID).lineList.size(); ++k) {//currentなピースの線分に対するループ
                                                                        for (int l = 0; l < frame.size(); ++l) {//フレームの数に対するループ
                                                                                for (int m = 0; m < frame.at(l).lineList.size(); ++m) {//フレームの線分に対するループ


                                                                                        Line tmpLine(piece.at(currentPieceID).lineList.at(k));

                                                                                        tmpLine.intercept += (tmpLine.slope == INF) ? i : (tmpLine.slope == 0) ? j : (tmpLine.slope*i - j);


                                                                                        if (tmpLine.intercept == frame.at(l).lineList.at(m).intercept &&
                                                                                                tmpLine.slope == frame.at(l).lineList.at(m).slope) {//傾きと切片が等しいとき(＝同じ式で表せるとき）
                                                                                                double tmpEval = 0;

                                                                                                double tmpLength = std::min(piece.at(currentPieceID).lineList.at(k).length, frame.at(l).lineList.at(m).length);

                                                                                                //piece.at(currentPieceID).lineList.at(k).length -= tmpLength;
                                                                                                //frame.at(l).lineList.at(m).length -= tmpLength;

                                                                                                tmpEval += pow(tmpLength, 2);
                                                                                                EvalutionList.push(std::tuple<double, int, Position>(tmpEval, currentPieceID, tmpPos));

                                                                                                double d_dtmp = std::get<0>(EvalutionList.top());
                                                                                                int d_itmp = std::get<1>(EvalutionList.top());
                                                                                                Position d_ptmp = std::get<2>(EvalutionList.top());
                                                                                        }
                                                                                }
                                                                        }
                                                                }
                                                        }
                                                }
                                        }
                                }
                                if (!EvalutionList.empty()) {
                                        int itmp = std::get<1>(EvalutionList.top());
                                        Position ptmp = std::get<2>(EvalutionList.top());
                                        std::cout << itmp << "\n";
                                        std::cout << ptmp.x << "," << ptmp.y << std::endl;
                                        /*/
                                        if (!isInFrame(piece.at(itmp).absolutePiecePosition(ptmp), frame)) {
                                        std::cout << std::endl << itmp << ":" << ptmp.x << "," << ptmp.y << std::endl;
                                        rep(i, piece.at(itmp).vertexPositionList.size())	std::cout << piece.at(itmp).vertexPositionList.at(i).x << "," << piece.at(itmp).vertexPositionList.at(i).y << std::endl;	puts("");
                                        }
                                        assert(isInFrame(piece.at(itmp).absolutePiecePosition(ptmp), frame));*/

                                        frame.push_back(piece.at(itmp).absolutePiecePosition(ptmp));

                                        usableList = samePieceDisable(usableList,itmp);
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
        /*/int cnt = 0;
        for (auto i = solve.piece.begin(); i != solve.piece.end(); ++i) {
                std::cout << cnt << "\n\n";
        //for (int turning = 0; turning < 2; ++turning) {
        //i->turn();
        //for (int spining = 0; spining < 4; ++spining) {
        //i->spin();
        for (auto j = i->vertexPositionList.begin(); j != i->vertexPositionList.end(); ++j) {
        std::cout << j->x << "," << j->y << std::endl;
        }
        std::cout << std::endl;
        //}
        //}
        cnt++;
        }
        /**/

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

        std::cout << isInFrame(b.absolutePiecePosition(Position(13,2)), solve.frame) << "," << a.area << "\n\n";



        for (auto i = solve.frame.begin(); i != solve.frame.end(); ++i) {
        for (int j = 0; j < i->vertexPositionList.size(); ++j)
        std::cout << i->vertexPositionList.at(j).x << "," << i->vertexPositionList.at(j).y << std::endl;
        puts("");
        }
        /**/

        return 0;
}
