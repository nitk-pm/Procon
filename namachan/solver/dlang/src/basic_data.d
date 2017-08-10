module procon28.basic_data;

import armos.math.vector;

import std.algorithm.iteration : uniq;
import std.algorithm.sorting : sort;
import std.conv : to;
import std.format : format;
import std.math : abs;
import std.numeric : gcd;
import std.range : array;

///有理数クラス
struct Rational {
	///分母
	int denom;
	///分子
	int num;

	///自身を約分したものを返す
	Rational normalized () const {
		if (num == 0) {
			if (denom < 0)	return Rational (-1, 0);
			else			return Rational (+1, 0);
		}
		int sign = 1;
		if (denom * num < 0) sign = -1;
		auto gcd_num = gcd (abs(denom), abs(num));
		return Rational (abs(denom) * sign / gcd_num, abs(num) / gcd_num);
	}

	///自身を約分
	void normalize () {
		if (num == 0) {
			if (denom < 0) denom = -1;
			else denom = 1;
		}
		int sign = 1;
		if (denom * num < 0) sign = -1;
		auto gcd_num = gcd (abs(denom), abs(num));
		denom = abs(denom) * sign / gcd_num;
		num = abs(num) / gcd_num;
	}

	bool opEquals(in Rational rational) const {
		auto x = this.normalized;
		auto y = rational.normalized;
		return x.denom == y.denom && x.num == y.num;
	}
	
	/++
	 + Rational同士の二項演算子
	 + 四則演算のみ対応
	 +/
	Rational opBinary(string op)(in Rational rational) const {
		static if (op == "+" || op == "-") {
			auto common_num = this.num * rational.num;
			mixin ("return Rational (this.denom * rational.num "~op~"rational.denom * this.num, common_num);");
		}
		else static if (op == "*") {
			return Rational (this.denom * rational.denom, this.num * rational.num);
		}
		else static if (op == "/") {
			return Rational (this.denom * rational.num, this.num * rational.denom);
		}
	}

	/++
	 + intとの二項演算子
	 + Rationalが左に来た場合
	 + 四則演算のみ対応
	 +/
	Rational opBinary(string op)(in int n) const {
		static if (op == "+") {
			return Rational (this.denom + n * this.num, this.num);
		}
		else static if (op == "-") {
			return Rational (this.denom - n * this.num, this.num);
		}
		else static if (op == "*") {
			return Rational (this.denom * n, this.num);
		}
		else static if (op == "/") {
			return Rational (this.denom, this.num * n);
		}
	}

	/++
	 + intとの二項演算子
	 + Rationalが右に来た場合
	 + 四則演算のみ対応
	 +/
	Rational opBinaryRight(string op)(in int n) const {
		static if (op == "-") {
			return Rational (n * this.num - this.denom, this.num);
		}
		else static if (op == "/") {
			return Rational (this.num * n, this.denom);
		}
		else {
			return this.opBinary!op(n);
		}
	}

	///doubleへ変換
	double toDouble () const {
		return denom.to!double / num.to!double;
	}

	///floatへ変換
	float toFloat () const {
		return denom.to!float / num.to!float;
	}
}
unittest {
	auto x = Rational (3,4) - Rational (7,8);
	x.normalize;
	assert (x == Rational(-1, 8));
}

///Vector2iの傾きをRationalとして返す
Rational slope (Vector2i vec) {
	return Rational (vec.y, vec.x);
}

///一次関数
struct Line {
	///傾き
	Rational slope;
	///切片
	Rational seg;

	///xに値を代入し、yを計算	
	Rational y (in int x) const {
		return slope * x + seg;
	}

	///yに値を代入し、xを計算
	Rational x (in int y) const {
		return (y - seg) / slope;
	}
}

///線分
struct Segment {

	///始点
	Vector2i start;
	///終点
	Vector2i end;

	///その線分と重なる軌跡を描く一次関数
	Line line () const {
		auto slope = vec.slope.normalized;
		if (slope.num == 0)
			return Line (Rational(1,0), Rational(0,0));
		auto seg = start.y - slope * start.x;
		seg.normalize;
		return Line (slope, seg);
	}

	///等価比較演算子
	bool opEquals(in Segment seg) const {
		return (this.start == seg.start && this.end == seg.end) || (this.start == seg.end && this.end == seg.start);
	}

	@safe
	size_t toHash () const {
		import std.typecons;
		auto start_hash = tuple(start.x, start.y).toHash;
		auto end_hash = tuple(start.x, start.y).toHash;
		if (start_hash > end_hash)
			return tuple(start_hash, end_hash).toHash;
		else
			return tuple(end_hash, start_hash).toHash;
	}
	unittest {
		assert (Segment(Vector2i(0, 5), Vector2i(3, 2)).toHash == Segment(Vector2i(3, 2), Vector2i(0, 5)).toHash );
	}

	///線分の傾きを計算
	@property
	Rational slope () const {
		return vec.slope;
	}

	///線分からベクトルを計算
	@property
	Vector2i vec () const {
		return end - start;
	}
}

unittest {
	auto p1 = Vector2i ( 4,  8);
	auto p2 = Vector2i ( 6, 7);
	auto seg = Segment(p1, p2);
	import std.stdio;
	assert (seg.line == Line (Rational (-1,2), Rational (10, 1)));
}

alias R = Rational;
alias P = Vector2i;
alias S = Segment;
alias L = Line;

alias Shape = Segment[];
alias Piece = Shape[];