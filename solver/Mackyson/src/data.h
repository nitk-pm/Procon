#pragma once

const int INF = 10000000;

constexpr double cPow (double mantissa, unsigned int exponent) { return exponent == 0 ? 1 : mantissa*cPow (mantissa, exponent - 1); }
class Position {
public:
        bool operator<(const Position& p)const;
        bool operator>(const Position& p)const;
        bool operator==(const Position& p)const;
        bool operator!=(const Position& p)const;

        Position operator+(const Position &p)const {
                return Position (this->x + p.x, this->y + p.y);
        }
        Position operator-(const Position &p)const {
                return Position (this->x - p.x, this->y - p.y);
        }

        int x, y;

        Position () {};
        Position (int xValue, int yValue) { x = xValue; y = yValue; }
};

bool Position::operator<(const Position& p)const {
        return (this->x == p.x ? this->y<p.y : this->x < p.x ? true : false);
}
bool Position::operator>(const Position& p)const {
        return (this->x == p.x ? this->y>p.y : this->x > p.x ? true : false);
}
bool Position::operator==(const Position& p)const {
        return (this->x == p.x&&this->y == p.y);
}
bool Position::operator!=(const Position& p)const {
        return (this->x != p.x || this->y != p.y);
}

namespace std { //std汚染罪 hashの特殊化
        template <>
        struct hash<Position> {
                size_t operator()(const Position &position) const {
                        size_t hash = position.x * 100 + position.y;
                        return hash;
                }

        };
}

double cross (Position a, Position b) {
        return a.x*b.y - a.y * b.x;
}


//shapeについての内外判定
int gridEvalution (Position p, std::vector<Position> shape) {

        Position a = p;
        for (int i = 0; i < shape.size () - 1; ++i) {
                Position c = shape.at (i), d = shape.at (i + 1);
                if ((c.x <= a.x&&a.x <= d.x) || (d.x <= a.x&&a.x <= c.x))
                {
                        if ((c.y <= a.y&&a.y <= d.y) || (d.y <= a.y&&a.y <= c.y)) {
                                int ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x);
                                if (ta == 0)
                                        return 1;
                        }
                }
        }

        int cn = 0;
        for (int k = 0; k < shape.size () - 1; ++k) {//currentな枠の頂点数に対応するループ
                double vt = (double)(p.y - shape.at (k).y) / (shape.at (k + 1).y - shape.at (k).y);//k,k+1のなす線分の傾き

                if ((shape.at (k).y <= p.y)//始点以上終点未満のy値
                        && (shape.at (k + 1).y > p.y)) {
                        if (p.x < (shape.at (k).x + (vt * (shape.at (k + 1).x - shape.at (k).x))))
                        {
                                cn++;
                        }
                }
                else if ((shape.at (k).y > p.y)//あるいは終点以下始点超過のy値
                        && (shape.at (k + 1).y <= p.y)) {
                        if (p.x < (shape.at (k).x + (vt * (shape.at (k + 1).x - shape.at (k).x))))
                        {
                                cn++;
                        }
                }
        }
        if (cn % 2 == 1) {
                return 2;
        }

        return 0;
}

Position concavePoint (std::vector<Position> p) {
        Position center(0,0),tmp;
        for (int i = 0; i < p.size (); ++i){
                center = center + p.at (i);
        }
        center.x /= p.size (); center.y /= p.size ();
        double distance = INF;
        for (int i = 0; i < p.size (); ++i) {
                double tmpdist = cPow (center.x - p.at (i).x, 2) + cPow (center.y - p.at (i).y, 2);
                if (tmpdist < distance) {
                        distance = tmpdist;
                        tmp = p.at (i);
                }

        }
        return tmp;

}
std::vector<Position> spin (std::vector<Position> vertexPositionList) { //+pi/2 (r cos(a+pi/2)=-r sin(a) (r sin(a+pi/2)=r cos(a))
        for (int i = 0; i < vertexPositionList.size (); ++i)
                vertexPositionList.at (i) = Position (-vertexPositionList.at (i).y, vertexPositionList.at (i).x);
        return vertexPositionList;

}

std::vector<Position> turn (std::vector<Position> vertexPositionList) {
        for (int i = 0; i < vertexPositionList.size (); ++i)
                vertexPositionList.at (i) = Position (vertexPositionList.at (i).x, -vertexPositionList.at (i).y);
        std::reverse (vertexPositionList.begin (), vertexPositionList.end ());
        return vertexPositionList;
}


/*Pieceクラス　頂点数と各点の座標*/
class Piece {

        //外積による面積の算出、
        double areaCalculation () {

                double tmp = 0;
                for (int i = 0; i < this->vertexPositionList.size () - 1; ++i) {
                        tmp += cross (this->vertexPositionList.at (i), this->vertexPositionList.at (i + 1)) / 2;
                }
                return tmp;
        }

public:

        bool operator <(const Piece& p)const;
        bool operator >(const Piece& p)const;
        bool operator ==(const Piece& p)const;
        bool operator !=(const Piece& p)const;

        double area;
        std::vector<Position> vertexPositionList;

        std::vector<Position>onlinePositionList;
        std::vector<Position>insidePositionList;
        int minY, maxY, minX, maxX;


        Piece () {};
        Piece (std::vector<Position>positionList, int edge) {
                vertexPositionList.reserve (positionList.size () + 1);
                std::vector<Position>posListTmp = positionList;

                sort (posListTmp.begin (), posListTmp.end ());



                int idx;
                switch (edge) {
                case 0:
                        for (int i = 0; i < positionList.size (); ++i)
                                if (posListTmp.front () == positionList.at (i))
                                        idx = i;

                        break;

                case 1:
                        for (int i = 0; i < positionList.size (); ++i)
                                if (posListTmp.back () == positionList.at (i))
                                        idx = i;
                        break;
                case 2:
                        auto tmp = concavePoint (positionList);
                        for (int i = 0; i < positionList.size (); ++i)
                                if (positionList.at (i) == tmp)
                                        idx = i;
                        break;
                }

                for (int cnt = 0, i = 0; cnt < positionList.size (); ++i, ++cnt) {
                        vertexPositionList.push_back (positionList.at (idx + i) - positionList.at (idx));
                        if (idx + i >= positionList.size () - 1)
                                i -= positionList.size ();
                }

                vertexPositionList.push_back (Position (0, 0));

                minY = 65, maxY = -65, minX = 101, maxX = -101;
                for (int i = 0; i < vertexPositionList.size () - 1; ++i) {
                        minY = std::min (minY, vertexPositionList.at (i).y);
                        maxY = std::max (maxY, vertexPositionList.at (i).y);
                        minX = std::min (minX, vertexPositionList.at (i).x);
                        maxX = std::max (maxX, vertexPositionList.at (i).x);
                }

                for (int i = minX; i <= maxX; ++i) {
                        for (int j = minY; j <= maxY; ++j) {
                                Position tmpPos (i, j);
                                switch (gridEvalution (tmpPos, vertexPositionList)) {
                                case 2:insidePositionList.push_back (tmpPos); break;
                                case 0:break;
                                case 1:onlinePositionList.push_back (tmpPos); break;
                                }
                        }
                }

                area = areaCalculation ();
                area = area < 0 ? -area : area;
                return;
        };

        Piece absolutePiecePosition (Position base) {//引数のPositionを基準に他の点を合わせる形で全点をずらす。
                Piece tmpPiece;
                for (int i = 0; i < this->vertexPositionList.size (); ++i)
                        tmpPiece.vertexPositionList.push_back (Position (this->vertexPositionList.at (i) + base));
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
        if (this->vertexPositionList.size () == p.vertexPositionList.size ()) {
                bool tmp = true;
                for (int i = 0; i < this->vertexPositionList.size (); ++i)
                        tmp &= (this->vertexPositionList.at (i) == p.vertexPositionList.at (i));
                return tmp;
        }
        else return false;
}
bool Piece::operator !=(const Piece& p)const {
        return !(*this == p);
}

class Frame {

public:

        std::vector<Position> vertexPositionList;

        Frame () {};
        Frame (std::vector<Position>positionList) {
                for (int i = 0; i < positionList.size (); ++i)
                        vertexPositionList.push_back (positionList.at (i));
                vertexPositionList.push_back (positionList.at (0));
                return;
        };

        Frame (Piece p) {
                for (int i = 0; i < p.vertexPositionList.size (); ++i)
                        vertexPositionList.push_back (p.vertexPositionList.at (i));
        }

};


struct ParStruct {

        int point;
        std::bitset<101 * 65> frame;
        std::vector<bool> usableList;
        std::vector<Position> evalPos;
        std::vector<std::pair<int, Position>>log;

        ParStruct () {};
        ParStruct (int p, std::bitset<101 * 65> f, std::vector<bool> u, std::vector<Position> e, std::vector<std::pair<int, Position>> l) {

                point = p;
                frame = f;
                usableList = u;
                evalPos = e;
                log = l;
        }
        bool operator <(const ParStruct& p)const;
        bool operator >(const ParStruct& p)const;
};

bool ParStruct::operator <(const ParStruct& p)const {

        return this->point == p.point ? this->frame.count () <p.frame.count () : this->point<p.point;
}
bool ParStruct::operator >(const ParStruct& p)const {
        return(*this < p);
}

struct Evalution {

        int point;
        int idx;
        Position pos;

        Evalution () {};
        Evalution (int _point, int _idx, Position _pos) {
                point = _point; idx = _idx; pos = _pos;
        }

        bool operator <(const Evalution& p)const;
};
bool Evalution::operator <(const Evalution& e) const {
        return point == e.point ?
                idx == e.idx ?
                pos < e.pos :
                idx < e.idx :
                point < e.point;
}

class _queue {

private:
        int data[6565];
        int head = 0;
        int tail = 0;

public:
        void push (int a) { data[tail++] = a; }
        void pop () { head++; }
        int front () { return data[head]; }
        bool empty () { return head == tail ? true : false; }

};
