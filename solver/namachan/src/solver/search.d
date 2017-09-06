module procon28.solver.search;

import armos.math.vector : Vector2i;

import procon28.basic_data : P, S, Piece, Shape, Segment;
import procon28.solver.datamanip : shape_xor, move, vertexies2shape;
import procon28.solver.eval : angle_and_points;

import std.algorithm.iteration : map;
import std.range : array;
import std.stdio;
import std.array : join;

///操作を記録する構造体
struct Op {
	///ピース番号
	const size_t piece_idx;
	const size_t shape_idx;
	///使われたピースの形状
	const Shape shape;
	const Shape frame;
	///位置
	const Vector2i pos;
	///評価値
	const float val;
}

struct Procedure {
	Op[] ops;
	bool[] used_mask;
	Shape frame;
	float val;
}

/++
 + なぜphobosのソートアルゴリズムは破壊的なのか。
 + その謎を探るため我々は南米の奥地へ飛んだ...
 +/
@safe
nothrow pure auto sort(alias compare, T)(inout T[] ops) {
	import std.traits;
	auto merge (inout T[] ops1, inout T[] ops2) {
		inout(T)[] merged;
		size_t ops1_idx, ops2_idx;
		for (;ops1_idx < ops1.length || ops2_idx < ops2.length;) {
			if (ops2_idx >= ops2.length || (ops1_idx < ops1.length && compare(ops1[ops1_idx], ops2[ops2_idx]))) {
				merged ~= ops1[ops1_idx];
				++ops1_idx;
			}
			else {
				merged ~= ops2[ops2_idx];
				++ops2_idx;
			}
		}
		return merged;
	}
	if (ops.length < 2)
		return ops;
	if (ops.length == 2) {
		if (compare(ops[0], ops[1]))
			return ops;
		else
			return [ops[1], ops[0]];
	}
	auto center_idx = ops.length / 2;
	return merge (sort!compare(ops[0..center_idx]), sort!compare(ops[center_idx..$]));
}

import std.typecons : Tuple, tuple;
alias key_t = Tuple!(const size_t, const size_t, const(Segment)[], const int, const int);
float[key_t] hash;
@safe
nothrow pure key_t toHash(in size_t piece_idx, in size_t spin_level, in Shape shape, in Vector2i pos) {
	return tuple(piece_idx, spin_level, shape, pos.x, pos.y);
}

Procedure[] eval_all(alias EvalFunc)(Piece[] pieces, Procedure acc)
in {
	assert (pieces.length == acc.used_mask.length);
}
body{
	Procedure[] procedures;
	foreach (piece_idx,piece; pieces) {
		if (acc.used_mask[piece_idx]) continue;
		foreach (spin_level, pattern; piece) {
			foreach (piece_seg; pattern) {
				auto piece_vertex = piece_seg.start;
				foreach (frame_seg; acc.frame) {
					foreach (frame_vertex; [frame_seg.start, frame_seg.end]) {
						auto diff = frame_vertex - piece_vertex;
						auto key = toHash(piece_idx, spin_level, acc.frame, diff);
						float val;
						auto moved = pattern.move(diff);
						if (key in hash) {
							continue;
						}
						else {
							val = EvalFunc (acc.frame, moved);
							hash[key] = val;
						}
						if (val == -float.infinity){
							continue;
						}
						auto used_mask_cpy = acc.used_mask.dup;
						used_mask_cpy[piece_idx] = true;
						auto merged_frame = shape_xor(acc.frame ~ moved);
						procedures ~=
							cast(inout)Procedure (
								Op(piece_idx, spin_level, pattern, acc.frame, diff, val) ~ acc.ops,
								used_mask_cpy,
								merged_frame,
								acc.val + val);
					}
				}
			}
		}
	}
	return procedures;
}

auto beam_search(alias EvalFunc)(Piece[] pieces, Shape frame, size_t beam_width) {
	auto sorted = eval_all!EvalFunc(pieces, Procedure([], new bool[pieces.length], frame, 0.0))
		.sort!((a, b) => a.val > b.val)
		.array;
	import procon28.visualize.window;
	auto win = new Window;
	size_t cnt;
	for (;;) {
		import procon28.visualize.window;
		++cnt;
		if (sorted.length > beam_width)
			sorted = sorted[0..beam_width];
		Procedure before;
		foreach (procedure; sorted) {
			if (procedure.frame.length == 0)
			return procedure;
		}
		sorted = sorted
			.map!(a => eval_all!EvalFunc(pieces, a))
			.array
			.join
			.sort!((a,b) => a.val > b.val);
		if (sorted.length == 0) throw new Exception("could'nt solve.");
	}
}

/++
 + 探索関数
 + 現在の実装は貪欲法
 +/
Op[] greedy_search (alias EvalFunc)(in Piece[] pieces, in Shape frame) {
	auto default_mask = new bool[pieces.length];
	if (pieces.length == 0) return [];
	auto sorted = eval_all (pieces, frame).sort;
	if (sorted[0].val == -float.infinity) return [];
	auto new_frame = merge (frame, sorted[0].shape.move(sorted[0].pos));
	return sorted[0] ~ greedy_search!EvalFunc (pieces, new_frame);
}
unittest {
	auto p1 = [[P(0,0), P(0,20), P(20,0)].vertexies2shape];
	auto p2 = [[P(20,0), P(20,20), P(0,20)].vertexies2shape];
	auto p3 = [[P(0,0), P(0,20), P(20,20), P (20, 0)].vertexies2shape];
	auto frame = [P(0,0), P(20,0), P(20,40), P(0,40)].vertexies2shape;
	auto ops = beam_search!angle_and_points([p1, p2, p3], frame, 1);
}
