module procon28.basic_data;

import armos.math.vector;

import procon28.solver.datamanip : shape_idx;

import std.algorithm.iteration : uniq;
import std.algorithm.sorting : sort;
import std.conv : to;
import std.format : format;
import std.range : array;
import core.simd;

enum Width = 101;
enum Height = 65;

///線分
struct Segment {

	///始点
	Vector2i start;
	///終点
	Vector2i end;

	///等価比較演算子
	@safe @nogc
	nothrow pure bool opEquals(in Segment seg) const {
		return (this.start == seg.start && this.end == seg.end) || (this.start == seg.end && this.end == seg.start);
	}

	@safe
	nothrow size_t toHash () const {
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
	@safe 
	pure string toString() const {
		import std.format : format;
		return format ("S(V (%d, %d), V(%d, %d))", start.x, start.y, end.x, end.y);
	}

	///線分からベクトルを計算
	@property @nogc @safe
	nothrow pure Vector2i vec () const {
		return end - start;
	}
}

alias P = Vector2i;
alias S = Segment;
alias Shape = Segment[];
alias Piece = Shape[];

struct BitField(alias size) {
private:
	enum xmm_length =
		size % 128 == 0 ? size / 128 : size / 128 + 1;
	union {
		__vector(ulong[2])[xmm_length] xmms;
		ulong[xmm_length * 2] array;
	}

	template expand_loop (alias times, alias code) {
		import std.algorithm.iteration : map;
		import std.range : iota, array;
		import std.array : join;
		import std.format : format;
		//わからん
		enum expand_loop =
			(){
				return times
					.iota
					.map!(idx => format(code, idx))
					.join;
			}();

	}
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
	@safe @nogc
	nothrow pure BitField!size opBinary (string op)(in BitField!size field) const {
		BitField!size ymm;
		static if (op == "|") {
			mixin(expand_loop!(xmm_length, "ymm.xmms[%1$s] = xmms[%1$s] | field.xmms[%1$s];"));
		}
		else static if (op == "&") {
			mixin(expand_loop!(xmm_length, "ymm.xmms[%1$s] = xmms[%1$s] & field.xmms[%1$s];"));
		}
		else static if (op == "^") {
			mixin(expand_loop!(xmm_length, "ymm.xmms[%1$s] = xmms[%1$s] ^ field.xmms[%1$s];"));
		}
		return ymm;
	}
	
	@safe @nogc
	nothrow BitField!size opOpAssign (string op)(in BitField!size field) {
		BitField!size ymm;
		static if (op == "|") {
			mixin(expand_loop!(xmm_length, "xmms[%1$s] = xmms[%1$s] | field.xmms[%1$s];"));
		}
		else static if (op == "&") {
			mixin(expand_loop!(xmm_length, "xmms[%1$s] = xmms[%1$s] & field.xmms[%1$s];"));
		}
		else static if (op == "^") {
			mixin(expand_loop!(xmm_length, "xmms[%1$s] = xmms[%1$s] ^ field.xmms[%1$s];"));
		}
		return ymm;
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
}
unittest {
	BitField!256 bf256;
	static assert (bf256.xmms.length == 2);
	BitField!5 bf5;
	static assert (bf5.xmms.length == 1);

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
	assert ((bf256_2 | bf256) == bf256);

	assert ((bf256_2 & bf256) == bf256_2);
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
}