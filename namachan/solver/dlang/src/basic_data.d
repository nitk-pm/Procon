module procon28.basic_data;

import std.format;
import std.math : abs, sqrt, acos;
import std.numeric : gcd;
import std.algorithm.sorting : sort;
import std.algorithm.iteration : uniq;
import std.range : array;
import std.conv : to;

struct Rational {
	int denom, num;

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

	double toDouble () const {
		return denom.to!double / num.to!double;
	}
	float toFloat () const {
		return denom.to!float / num.to!float;
	}
}
unittest {
	auto x = Rational (3,4) - Rational (7,8);
	x.normalize;
	assert (x == Rational(-1, 8));
}

struct Vector {
	int x, y;
	Vector opBinary(string op)(in Vector pt) const {
		mixin (format!"return Vector (x %1$s pt.x, y %1$s pt.y);"(op));
	}
	void opOpAssign(string op)(in Vector pt) {
		mixin (format!"this.x %s= pt.x;"(op));
		mixin (format!"this.y %s= pt.y;"(op));
	}

	@property
	double length () const {
		return sqrt (cast(float)(x ^^ 2 + y ^^ 2));
	}

	int cross (in Vector pt) const {
		return x * pt.x + y * pt.y;
	}

	//外積の大きさ
	float dotL (in Vector pt) const {
		return x * pt.y - pt.x * y;
	}

	Rational slope () const {
		return Rational (y, x);
	}

	double angel (in Vector pt) const {
		return acos (cross(pt) / (length * pt.length));
	}
}

unittest {
	assert (Vector (2, 5) + Vector (3, -4) == Vector (5, 1));
	auto pt1 = Vector (6, 10);

	pt1 += Vector (2, 6);
	assert (pt1 == Vector (8, 16));

	auto pt2 = Vector (4, 2);
	assert (pt2.cross (Vector (4, 1)) == 18);
}

//一次関数
struct Line {
	Rational slope;
	Rational seg;

	Rational y (in int x) const {
		return slope * x + seg;
	}

	Rational x (in int y) const {
		return (y - seg) / slope;
	}
}

struct Segment {
	Vector start, end;

	Line line () const {
		auto slope = vec.slope.normalized;
		if (slope.num == 0)
			return Line (Rational(1,0), Rational(0,0));
		auto seg = start.y - slope * start.x;
		seg.normalize;
		return Line (slope, seg);
	}

	@property
	Rational slope () const {
		return vec.slope;
	}

	@property
	Vector vec () const {
		return end - start;
	}

	bool addable(in Segment seg) const {
		if (line != seg.line) return false;
		return
			(end.x - start.x) * (seg.end.x - end.x) < 0 ||
			(end.x - start.x) * (seg.start.x - end.x) < 0;

	}

	Segment[] split (in Segment seg) const {
		auto line = line;
		auto xs = [start.x, end.x, seg.start.x, seg.end.x].dup.sort!"a>b".uniq.array;
		Segment[] segs;
		for (size_t idx; idx < xs.length - 1; ++idx) {
			segs ~= Segment (Vector(xs[idx], line.y(xs[idx].to!int).toFloat.to!int), Vector (xs[idx+1], line.y(xs[idx+1]).toFloat.to!int));
		}
		return segs;
	}
}

unittest {
	auto p1 = Vector ( 4,  8);
	auto p2 = Vector ( 6, 7);
	auto seg = Segment(p1, p2);
	assert (seg.line == Line (Rational (-1,2), Rational (10, 1)));
}
