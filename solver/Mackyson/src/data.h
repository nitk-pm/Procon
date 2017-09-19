#pragma once

//x,y座標のpair
const int INF = 10000000;

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

	void renewPosition (int a, int b) { x = a; y = b; return; }
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

//各辺の直線の式の傾きと切片の
class Line {

public:
	double slope, intercept, length;
	Line () {};

	Line (double slopeValue, double interceptValue, double lengthValue) { slope = slopeValue; intercept = interceptValue; length = lengthValue; return; }
	Line (Position a, Position b) {
		double x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
		slope = (x1 == x2) ? INF : (y2 - y1) / (x2 - x1);
		intercept = (slope == INF) ? x1 : -slope*x1 + y1;
		length = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
	}

};

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

	int wn = 0;
	for (int k = 0; k < shape.size () - 1; ++k) {//currentな枠の頂点数に対応するループ
		double vt = (double)(p.y - shape.at (k).y) / (shape.at (k + 1).y - shape.at (k).y);//k,k+1のなす線分の傾き

		if ((shape.at (k).y <= p.y)//始点以上終点未満のy値
			&& (shape.at (k + 1).y > p.y)) {
			if (p.x < (shape.at (k).x + (vt * (shape.at (k + 1).x - shape.at (k).x))))
			{
				wn++;
			}
		}
		else if ((shape.at (k).y > p.y)//あるいは終点以下始点超過のy値
			&& (shape.at (k + 1).y <= p.y)) {
			if (p.x < (shape.at (k).x + (vt * (shape.at (k + 1).x - shape.at (k).x))))
			{
				wn--;
			}
		}
	}
	if (wn != 0) {
		return 2;
	}

	return 0;
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

	void lineCalculation () {
		lineList.reserve (vertexPositionList.size () - 1);
		for (int i = 0; i < vertexPositionList.size () - 1; ++i) {
			lineList.push_back (Line (this->vertexPositionList.at (i), this->vertexPositionList.at (i + 1)));
		}

	}

	//ある点からの相対座標で表示
	Position relativePosition (Position standard, Position object) {
		int x, y;
		x = object.x - standard.x;
		y = object.y - standard.y;
		return Position (x, y);
	};

	//外積による面積の算出、
	double areaCalculation () {

		double tmp = 0;
		for (int i = 0; i < this->vertexPositionList.size () - 1; ++i) {
			tmp += cross (this->vertexPositionList.at (i), this->vertexPositionList.at (i + 1)) / 2;
		}
		return tmp;
	}

	Position centerCaluculation () {//重心の算出
		double x = 0, y = 0;
		for (int i = 0; i < vertexPositionList.size () - 1; ++i) {
			x += vertexPositionList.at (i).x;
			y += vertexPositionList.at (i).y;
		}
		return Position (x / (vertexPositionList.size () - 1), y / (vertexPositionList.size () - 1));
	}

public:

	bool operator <(const Piece& p)const;
	bool operator >(const Piece& p)const;
	bool operator ==(const Piece& p)const;
	bool operator !=(const Piece& p)const;

	std::vector<Line> lineList;
	double area;
	Position center;
	std::unordered_set<Position>onlinePositionList;
	std::unordered_set<Position>insidePositionList;
	int minY, maxY, minX, maxX;

	std::vector<Position> vertexPositionList;

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

		default:
			assert (false);
			break;
		}

		for (int cnt = 0, i = 0; cnt < positionList.size (); ++i, ++cnt) {
			vertexPositionList.push_back (relativePosition (positionList.at (idx), positionList.at (idx + i)));
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
				case 0:break;
				case 1:onlinePositionList.insert (tmpPos); break;
				case 2:insidePositionList.insert (tmpPos); break;
				default:assert (false);
				}
			}
		}

		area = areaCalculation ();
		lineCalculation ();
		center = centerCaluculation ();
		return;
	};

	Piece absolutePiecePosition (Position base) {//引数のPositionを基準に他の点を合わせる形で全点をずらす。
		Piece tmpPiece;
		for (int i = 0; i < this->vertexPositionList.size (); ++i)
			tmpPiece.vertexPositionList.push_back (Position (this->vertexPositionList.at (i).x + base.x, this->vertexPositionList.at (i).y + base.y));
		tmpPiece.center = tmpPiece.centerCaluculation ();
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

	void lineCalculation () {
		lineList.reserve (vertexPositionList.size () - 1);
		for (int i = 0; i < vertexPositionList.size () - 1; ++i) {
			lineList.push_back (Line (this->vertexPositionList.at (i), this->vertexPositionList.at (i + 1)));
		}
	}

public:

	std::deque<Position> vertexPositionList;
	std::vector<Line> lineList;

	Frame () {};
	Frame (std::vector<Position>positionList) {
		for (int i = 0; i < positionList.size (); ++i)
			vertexPositionList.push_back (positionList.at (i));
		vertexPositionList.push_back (positionList.at (0));
		lineCalculation ();
		return;
	};

	Frame (Piece p) {
		//vertexPositionList.reserve(p.vertexPositionList.size());
		for (int i = 0; i < p.vertexPositionList.size (); ++i)
			vertexPositionList.push_back (p.vertexPositionList.at (i));
		this->lineList = p.lineList;
	}

	void addFrame (Piece p) {
		for (int k = 0; k < this->vertexPositionList.size () - 1; ++k) {//Frameのなす線分のループ
			Position c (this->vertexPositionList.at (k).x, this->vertexPositionList.at (k).y), d (this->vertexPositionList.at (k + 1).x, this->vertexPositionList.at (k + 1).y);
			std::vector<Position>onlinePositionList;
			int addID;
			bool isDirectionRight;
			for (int i = 0; i < p.vertexPositionList.size () - 1; ++i) {//ピースのなす線分のループ
				Position a (p.vertexPositionList.at (i).x, p.vertexPositionList.at (i).y);

				int
					ta = (c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x);


				if (ta == 0 && ((c.x <= a.x && d.x >= a.x) || (c.x >= a.x&&d.x <= a.x) && (c.y <= a.y && d.y >= a.y) || (c.y >= a.y&&d.y <= a.y)))
				{
					onlinePositionList.push_back (a);
					addID = k + 1;
					isDirectionRight = c.x == d.x ? c.y < d.y : c.x < d.x;
				}

			}

			if (!onlinePositionList.empty ()) {
				isDirectionRight ? sort (onlinePositionList.begin (), onlinePositionList.end ()) : sort (onlinePositionList.rbegin (), onlinePositionList.rend ());
				for (int i = 0; i < p.vertexPositionList.size (); ++i) {
					Piece tmpPiece;
					if (p.vertexPositionList.at (i) == onlinePositionList.at (0)) {

						for (int cnt = 0; cnt < p.vertexPositionList.size () - 1; ++cnt) {//挿入する頂点のリストをピースとして作成
							tmpPiece.vertexPositionList.push_back (p.vertexPositionList.at (i));
							i -= i <= 0
								? -((signed)p.vertexPositionList.size () - 2)//基準点は始点と終点に二つあるため二度座標を置かれるのを防ぐための-2
								: 1;
						}

						std::vector<bool>willDeletedList;//同一座標リストを作成
						for (int cnt = tmpPiece.vertexPositionList.size () - 1; cnt >= 1; --cnt) {
							for (int j = 0; j < this->vertexPositionList.size (); ++j) {
								if (tmpPiece.vertexPositionList.at (cnt) == this->vertexPositionList.at (j))
									willDeletedList.push_back (true);
								else
									willDeletedList.push_back (false);
							}
						}

						for (int cnt = 1; cnt < tmpPiece.vertexPositionList.size (); ++cnt)//同一座標リストに該当する中で一個前の番号も同一座標の場合頂点を削除
							if (willDeletedList.at (cnt) == true)
								if (willDeletedList.at (cnt - 1) == true)
									tmpPiece.vertexPositionList.erase (tmpPiece.vertexPositionList.begin () + cnt);
						for (int cnt = 0; cnt < tmpPiece.vertexPositionList.size (); ++cnt)
							this->vertexPositionList.insert (this->vertexPositionList.begin () + addID + cnt, tmpPiece.vertexPositionList.at (cnt));

						goto END_FRAME_MARGE;//これは許されるgoto(˘ω˘)
					}
				}
			}
		}
	END_FRAME_MARGE:

		//同じ座標の点を削除
		for (int i = 0; i < vertexPositionList.size () - 2; ++i)
			if (vertexPositionList.at (i) == vertexPositionList.at (i + 1))
				vertexPositionList.erase (vertexPositionList.begin () + i);

		return;
	}


};

struct ParStruct{

	std::bitset<101 * 65> frame;
	std::vector<bool> usableList;
	std::unordered_set<Position> evalPos;
	std::vector<std::pair<int, Position>>log;

	ParStruct () {};
	ParStruct (std::bitset<101 * 65> f, std::vector<bool> u, std::unordered_set<Position> e, std::vector<std::pair<int, Position>> l) {
	
		frame = f;
		usableList = u;
		evalPos = e;
		log = l;
	}
	bool operator <(const ParStruct& p)const;
	bool operator >(const ParStruct& p)const;
};

bool ParStruct::operator <(const ParStruct& p)const {

	return this->frame.count() >p.frame.count();
}
bool ParStruct::operator >(const ParStruct& p)const {
	return(*this < p);
}
