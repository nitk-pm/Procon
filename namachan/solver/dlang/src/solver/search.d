module procon28.solver.search;

import armos.math.vector : Vector2i;

import procon28.basic_data : P, S, Piece, Shape;
import procon28.solver.data : merge, move, vertexies2shape;
import procon28.solver.eval : angle_and_points;

import std.algorithm.sorting : sort;
import std.range : array;

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
	size_t cnt;
	foreach (piece_idx,piece; pieces) {
		foreach (spin_level, pattern; piece) {
			foreach (piece_seg; pattern) {
				auto piece_vertex = piece_seg.start;
				foreach (frame_seg; frame) {
					cnt++;
					auto frame_vertex = frame_seg.start;
					auto diff = frame_vertex - piece_vertex;
					auto val = angle_and_points (frame, pattern.move(diff));
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
unittest {
	auto p1 = [[P(0,0), P(0,20), P(20,0)].vertexies2shape];
	auto p2 = [[P(20,0), P(20,20), P(0,20)].vertexies2shape];
	auto p3 = [[P(0,0), P(0,20), P(20,20), P (20, 0)].vertexies2shape];
	auto frame = [P(0,0), P(20,0), P(20,40), P(0,40)].vertexies2shape;
}