module procon28.basic_data;

import armos.math.vector;

import procon28.solver.datamanip : shape_idx, bits_idx, cache_make;

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
	nothrow pure BitField!size opShl (in int n) const {
		if (n < 0)
			return opShr (-n);
		immutable array_shift = n / 64;
		immutable elem_shift = n % 64;
		BitField!size bits;
		for (size_t i; i + array_shift < xmm_length * 2; ++i) {
			bits.array[i+array_shift] = array[i];
		}
		ulong carry;
		for (size_t i; i < xmm_length * 2 - 1; ++i) {
			immutable bits_i = bits.array[i];
			bits.array[i] = (bits.array[i] << elem_shift) ^ carry;
			if (elem_shift > 0)
				carry =  bits_i >> (64UL - elem_shift);
		}
		bits.array[$-1] = (bits.array[$-1] << elem_shift) | carry;
		return bits;
	}
	@safe @nogc
	nothrow pure BitField!size opShr (in int n) const {
			import std.stdio;
		if (n < 0)
			return opShl(-n);
		immutable array_shift = n / 64;
		immutable elem_shift = n % 64;
		BitField!size bits;
		for (long i = xmm_length * 2 - 1; i - array_shift >= 0; --i) {
			bits.array[i-array_shift] = array[i];
		}
		ulong carry;
		for (long i = xmm_length * 2 - 1; i >= 1; --i) {
			immutable bits_i = bits.array[i];
			bits.array[i] = (bits.array[i] >> elem_shift) | carry;
			if (elem_shift > 0)
				carry = bits_i << (64UL - elem_shift);
			else
				carry = 0;
		}
		bits.array[0] = (bits.array[0] >> elem_shift) | carry;
		return bits;
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

	BitField!1024 bf1024;
	foreach(i; 256..512) {
		bf1024[i] = true;
	}
	BitField!1024 ans1;
	foreach(i; 256..512){
		ans1[i+143] = true;
	}
	import std.stdio;
	assert (bf1024 << 143 == ans1);
	assert (bf1024 >> -143 == ans1);
	BitField!1024 ans2;
	foreach(i; 256..512){
		ans2[i-143] = true;
	}
	assert(bf1024 >> 143 == ans2);
	assert(bf1024 << -143 == ans2);
}


enum field_size = Width * Height;
alias ShapeBits = BitField!field_size;

struct PlacedShape {
	ubyte x;
	ubyte y;
	ubyte piece_idx;
	ubyte spin_level;
}

struct Situation {
	PlacedShape[] shapes;
	BitField!128 used_pieces;
}

class Data {
public:
	const Shape frame;
	const ShapeBits frame_inside_bits;
private:
	const ShapeBits[] pieces_bits;
	const ShapeBits[] pieces_inside_bits;
	const Shape[] shapes;


public:
	this (Shape[][] pieces, Shape frame) {
		Shape[] shapes_tmp;
		ShapeBits[] pieces_bits_tmp;
		ShapeBits[] pieces_inside_bits_tmp;
		this.frame = frame;
		this.frame_inside_bits = cache_make (frame, false);
		foreach (piece_idx, piece; pieces) {
			assert (piece.length == 8, "パターン数は8にしてください");
			foreach (spin_level, shape; piece) {
				shapes_tmp[shape_idx(piece_idx, spin_level)] ~= shape;
				pieces_bits_tmp ~= cache_make (shape, true);
				pieces_inside_bits_tmp ~= cache_make (shape, false);
			}
		}
		this.pieces_bits = pieces_bits_tmp;
		this.pieces_inside_bits = pieces_inside_bits_tmp;
		this.shapes = shapes_tmp;
	}
	@safe @nogc
	nothrow pure const(Shape) shape (in size_t piece_idx,in size_t spin_level) const {
		return shapes [shape_idx(piece_idx, spin_level)];
	}
	@safe @nogc
	nothrow pure const(ShapeBits) piece_bits (in size_t piece_idx,in size_t spin_level) const {
		return pieces_bits [shape_idx(piece_idx, spin_level)];
	}
	@safe @nogc
	nothrow pure const(ShapeBits) piece_inside_bits (in size_t piece_idx,in size_t spin_level) const {
		return pieces_inside_bits[shape_idx(piece_idx, spin_level)];
	}
}
