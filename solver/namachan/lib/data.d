module procon28.data;

import procon28.geometry : shape_idx;

import std.algorithm.iteration : uniq;
import std.algorithm.sorting : sort;
import std.conv : to;
import std.format : format;
import std.range : array;

enum Width = 101;
enum Height = 65;

struct Vector (T) {
public:
	T x, y;
	@safe @nogc
	pure nothrow this (in T x,in T y) const {
		this.x = x;
		this.y = y;
	}
	
	@safe @nogc
	pure nothrow auto opBinary (string op)(in Vector!T p) const {
		static if (op == "+") {
			return Vector!T (x+p.x, y+p.y);
		}
		else static if (op == "-") {
			return Vector!T (x-p.x, y-p.y);
		}
		else static if (op == "*") {
			return x*p.x+y*p.y;
		}
		else static if (op == "%") {
			return Vector!T (x%p.x, y%p.y);
		}
	}

	@safe @nogc
	pure nothrow auto opBinary (string op)(in T n) const {
		static if (op == "+") {
			return Vector!T (x+n, y+n);
		}
		else static if (op == "-") {
			return Vector!T (x-n, y-n);
		}
		else static if (op == "*") {
			return Vector!T (x*n, y*n);
		}
		else static if (op == "/") {
			return Vector!T (x/n, y/n);
		}
		else static if (op == "%") {
			return Vector!T (x%n, y%n);
		}
	}

	@safe @nogc
	pure nothrow void opOpAssign (string op)(in Vector!T p) {
		static if (op == "+") {
			x += p.x;
			y += p.y;
		}
		else static if (op == "-") {
			x -= p.x;
			y -= p.y;
		}
		else static if (op == "%") {
			x %= p.x;
			y %= p.y;
		}
	}

	@safe @nogc
	pure nothrow void opOpAssign (string op)(in T n) {
		static if (op == "+") {
			x += n;
			y += n;
		}
		else static if (op == "-") {
			x -= n;
			y -= n;
		}
		else static if (op == "*") {
			x *= n;
			y *= n;
		}
		else static if (op == "/") {
			x /= n;
			y /= n;
		}
		else static if (op == "%") {
			x %= n;
			y %= n;
		}
	}

	@safe @nogc
	pure nothrow Pos opUnary (string op)() const {
		static if (op == "+") {
			return this;
		}
		else static if (op == "-") {
			return Vector!T (-x, -y);
		}
	}

	@safe @nogc
	pure nothrow float norm() const {
		import std.math : sqrt;
		return sqrt (cast(float)(x^^2+y^^2));
	}

	@safe @nogc
	pure nothrow T norm2() const {
		return x^^2+y^^2;
	}

	@safe @nogc
	pure nothrow Vector!T2 opCast(V : Vector!T2, T2) () const {
		return Vector!T2 (cast(T2)x, cast(T2)y);
	}
}
unittest {
	auto v1 = Vector!int (107, -122);
	//キャスト
	assert (cast(Vector!float)(v1) == Vector!float(107.0f, -122.0f));
	//Vector!T型以外へのキャストは不可能
	static assert (!__traits(compiles, cast(float)v1));

	assert (v1 % 3 == Vector!int(2, -2));
	assert (v1 % Vector!int(3, 7) == Vector!int(2, -3));
}

alias Pos = Vector!int;
alias P = Pos;

struct Shapes {
	P[][][] pieces;
	P[][] frames;
}

struct BitField(alias size) {
private:
	enum array_elem_num = size % 64 == 0 ? size / 64 : size / 64 + 1;
	ulong[array_elem_num] array;
public:
	@safe @nogc
	nothrow pure bool opIndex (in int idx) const {
		immutable shift_num = idx % 64;
		immutable bits = array[idx / 64];
		return (bits & (1UL << shift_num)) != 0UL;
	}

	@safe @nogc
	nothrow bool opIndexAssign (in bool b, in size_t idx) {
		immutable shift_num = idx % 64;
		immutable elem_idx = idx / 64;
		immutable bits = this.array[elem_idx];
		if (b)
			this.array[elem_idx] = (bits | (1UL << shift_num));
		else {
			this.array[elem_idx] = (bits & ~(1UL << (shift_num)));
		}
		return b;
	}
	@safe
	pure string toString () const {
		string[] strs;
		import std.string;
		for (int idx = array.length - 1; idx >= 0; --idx) {
			strs ~= [format ("%064b", array[idx])];
		}
		return format("BitField!(%d)[%s]", size, strs.join("_"));
	}

	///全てのビットが立っているとtrue
	@safe @nogc
	pure nothrow all () const {
		foreach (bits; array) {
			if (bits != ulong.max) return false;
		}
		return true;
	}

	///一つでもビットが立っていればtrue
	@safe @nogc
	pure nothrow any () const {
		foreach (bits; array) {
			if (bits != 0) return true;
		}
		return false;
	}
}
unittest {
	BitField!256 bf256;
	BitField!5 bf5;

	bf256[1] = true;
	bf256[2] = true;
	assert (bf256[1]);
	assert (bf256[2]);
	assert (!bf256[0]);

	bf256[2] = false;
	assert (bf256[1]);

	bf256[128] = true;
	assert (bf256[128]);

	bf256[255] = true;
	assert (bf256[255]);

	bf256[132] = true;
	assert (bf256[132]);

	bf256[255] = false;
	assert (!bf256[255]);

	BitField!256 bf256_2;
	bf256_2[132] = true;
}


struct PlacedShape {
	ubyte x;
	ubyte y;
	ubyte piece_idx;
	ubyte spin_level;
}

struct Situation {
	PlacedShape[] shapes;
	BitField!128 used_pieces;
	P[][] frames;
	float val;
}
