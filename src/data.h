#pragma once
//x,y座標のpair
const int INF = 10000000;


class Position {
public:
        bool operator<(const Position& p)const;
        bool operator>(const Position& p)const;
        bool operator==(const Position& p)const;
        bool operator!=(const Position& p)const;


        int x, y;

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

//各辺の直線の式の傾きと切片の
class Line {

public:
        double slope, intercept, length;
        Line() {};

        Line(double slopeValue, double interceptValue, double lengthValue) { slope = slopeValue; intercept = interceptValue; length = lengthValue; return; }
        Line(Position a, Position b) {
                double x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
                slope = (x1 == x2) ? INF : (y2 - y1) / (x2 - x1);
                intercept = (slope == INF) ? x1 : -slope*x1 + y1;
                length = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
        }

};

/*Pieceクラス　頂点数と各点の座標*/
class Piece {

        void lineCalculation() {
                lineList.reserve(vertexPositionList.size() - 1);
                for (int i = 0; i < vertexPositionList.size() - 1; ++i) {
                        lineList.push_back(Line(this->vertexPositionList.at(i), this->vertexPositionList.at(i + 1)));
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

        Position centerCaluculation() {//重心の算出
                double x = 0, y = 0;
                for (int i = 0; i < vertexPositionList.size() - 1; ++i) {
                        x += vertexPositionList.at(i).x;
                        y += vertexPositionList.at(i).y;
                }
                return Position(x / (vertexPositionList.size() - 1), y / (vertexPositionList.size() - 1));
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

        Piece absolutePiecePosition(Position base) {//引数のPositionを基準に他の点を合わせる形で全点をずらす。
                Piece tmpPiece;
                for (int i = 0; i < this->vertexPositionList.size(); ++i)
                        tmpPiece.vertexPositionList.push_back(Position(this->vertexPositionList.at(i).x + base.x, this->vertexPositionList.at(i).y + base.y));
                tmpPiece.center = tmpPiece.centerCaluculation();
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
                lineList.reserve(vertexPositionList.size() - 1);
                for (int i = 0; i < vertexPositionList.size() - 1; ++i) {
                        lineList.push_back(Line(this->vertexPositionList.at(i), this->vertexPositionList.at(i + 1)));
                }
        }

public:

        std::deque<Position> vertexPositionList;
        std::vector<Line> lineList;

        Frame() {};
        Frame(std::vector<Position>positionList) {
                for (int i = 0; i < positionList.size(); ++i)
                        vertexPositionList.push_back(positionList.at(i));
                vertexPositionList.push_back(positionList.at(0));
                lineCalculation();
                return;
        };

        void addFrame(Piece p) {
                for (int k = 0; k < this->vertexPositionList.size() - 1; ++k) {//Frameのなす線分のループ
                        Position c(this->vertexPositionList.at(k).x, this->vertexPositionList.at(k).y), d(this->vertexPositionList.at(k + 1).x, this->vertexPositionList.at(k + 1).y);
                        std::vector<Position>onlinePositionList;
                        int addID;
                        bool isDirectionRight;
                        for (int i = 0; i < p.vertexPositionList.size() - 1; ++i) {//ピースのなす線分のループ
                                Position a(p.vertexPositionList.at(i).x, p.vertexPositionList.at(i).y);

                                int
                                        ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x);


                                if (ta == 0 && ((c.x <= a.x && d.x >= a.x) || (c.x >= a.x&&d.x <= a.x) && (c.y <= a.y && d.y >= a.y) || (c.y >= a.y&&d.y <= a.y)))
                                {
                                        onlinePositionList.push_back(a);
                                        addID = k + 1;
                                        isDirectionRight = c.x == d.x ? c.y < d.y : c.x < d.x;
                                }

                        }

                                if (!onlinePositionList.empty()) {
                                        isDirectionRight ? sort(onlinePositionList.begin(), onlinePositionList.end()) : sort(onlinePositionList.rbegin(), onlinePositionList.rend());
                                        for (int i = 0; i < p.vertexPositionList.size(); ++i) {
                                                Piece tmpPiece;
                                                if (p.vertexPositionList.at(i) == onlinePositionList.at(0)) {

                                                        for (int cnt = 0; cnt < p.vertexPositionList.size() - 1; ++cnt) {//挿入する頂点のリストをピースとして作成
                                                                tmpPiece.vertexPositionList.push_back(p.vertexPositionList.at(i));
                                                                i -= i <= 0
                                                                        ? -((signed)p.vertexPositionList.size() - 2)//基準点は始点と終点に二つあるため二度座標を置かれるのを防ぐための-2
                                                                        : 1;
                                                        }

                                                        std::vector<bool>willDeletedList;//同一座標リストを作成
                                                        for (int cnt = tmpPiece.vertexPositionList.size() - 1; cnt >= 1; --cnt) {
                                                                for (int j = 0; j < this->vertexPositionList.size(); ++j) {
                                                                        if (tmpPiece.vertexPositionList.at(cnt) == this->vertexPositionList.at(j))
                                                                                willDeletedList.push_back(true);
                                                                        else
                                                                                willDeletedList.push_back(false);
                                                                }
                                                        }

                                                        for (int cnt = 1; cnt < tmpPiece.vertexPositionList.size(); ++cnt)//同一座標リストに該当する中で一個前の番号も同一座標の場合頂点を削除
                                                                if (willDeletedList.at(cnt) == true)
                                                                        if (willDeletedList.at(cnt - 1) == true)
                                                                                tmpPiece.vertexPositionList.erase(tmpPiece.vertexPositionList.begin() + cnt);
                                                        for (int cnt = 0; cnt < tmpPiece.vertexPositionList.size(); ++cnt)
                                                                this->vertexPositionList.insert(this->vertexPositionList.begin() + addID + cnt, tmpPiece.vertexPositionList.at(cnt));

                                                        goto END_FRAME_MARGE;//これは許されるgoto(˘ω˘)
                                                }
                                        }
                                }
                        }
        END_FRAME_MARGE:

                //同じ座標の点を削除
                for (int i = 0;i < vertexPositionList.size() - 2;++i)
                        if (vertexPositionList.at(i) == vertexPositionList.at(i + 1))
                                vertexPositionList.erase(vertexPositionList.begin() + i);

                return;
        }


};

