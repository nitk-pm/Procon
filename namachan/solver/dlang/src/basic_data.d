module procon28.basic_data;

import armos.math.vector;

import std.algorithm.iteration : uniq;
import std.algorithm.sorting : sort;
import std.conv : to;
import std.format : format;
import std.range : array;

///線分
struct Segment {

	///始点
	Vector2i start;
	///終点
	Vector2i end;

	///等価比較演算子
	bool opEquals(in Segment seg) const {
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
	@property
	Vector2i vec () const {
		return end - start;
	}
}

alias P = Vector2i;
alias S = Segment;
alias Shape = Segment[];
alias Piece = Shape[];