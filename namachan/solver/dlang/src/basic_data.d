module procon28.basic_data;

import std.format;
import std.math : abs, sqrt, acos;
import std.numeric : gcd;
import std.algorithm.sorting : sort;
import std.algorithm.iteration : uniq;
import std.range : array;
import std.conv : to;
import armos.math.vector;

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

Rational slope (Vector2i vec) {
	return Rational (vec.y, vec.x);
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
	Vector2i start, end;

	Line line () const {
		auto slope = vec.slope.normalized;
		if (slope.num == 0)
			return Line (Rational(1,0), Rational(0,0));
		auto seg = start.y - slope * start.x;
		seg.normalize;
		return Line (slope, seg);
	}

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

	@property
	Rational slope () const {
		return vec.slope;
	}

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