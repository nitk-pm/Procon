module procon28.solver.search;

import procon28.solver.data;
import procon28.solver.eval;
import procon28.basic_data;

import std.algorithm.sorting : sort;
import std.range : array;
import armos.math.vector;

///操作を記録する構造体
struct Op {
	///ピース番号
	size_t piece_idx;
	///使われたピースの形状
	Shape shape;
	///位置
	Vector2i pos;
	///評価値
	float val;
}

///評価値付きで操作を記録する構造体
struct EvaledOp {
	///操作
	Op op;
	///評価値
	float val;
}

/++
 + 探索関数
 + 現在の実装は貪欲法
 +/
Op[] search (Piece[] pieces, Shape frame) {
	Op[] ops;
	if (pieces.length == 0) return [];
	foreach (piece; pieces) {
		foreach (spin_level, pattern; piece) {
			foreach (piece_idx, piece_seg; pattern) {
				auto piece_vertex = piece_seg.start;
				foreach (frame_seg; frame) {
					auto frame_vertex = frame_seg.start;
					auto diff = frame_vertex - piece_vertex;
					auto val = angle_and_points (pattern, frame, diff);
					ops ~= Op (piece_idx, pattern, diff, val);
				}
			}
		}
	}
	auto sorted = ops.sort!((a, b) => a.val > b.val).array;
	if (sorted[0].val == -float.infinity) return [];
	auto new_frame = merge (frame, sorted[0].shape);
	auto piece_set = pieces[0..sorted[0].piece_idx] ~ pieces[sorted[0].piece_idx+1..$];
	return sorted[0] ~ search (piece_set, new_frame);
}