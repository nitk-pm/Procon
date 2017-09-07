module procon28.solver.eval;

import procon28.basic_data : P, S, Shape, Segment;
import procon28.solver.datamanip : equal_slope, move, vertexies2shape, protrude_frame, judge_on_line;

/++
 + 線分の角度の一致度と重複した点の数で評価する評価関数
 + 衝突していた場合は-float.inifinityを返す
 +/
float angle_and_points (in Shape frame,in Shape piece) {
	import std.stdio;
	float val_by_point = 0.0f;
	float val_by_slope = 0.0f;
	if (protrude_frame (frame, piece))
		return -float.infinity;
	foreach (piece_seg; piece) {
		foreach (frame_seg; frame) {
			if (piece_seg.start == frame_seg.end || piece_seg.start == frame_seg.start) {
				val_by_point += 1.0;
				if (equal_slope(piece_seg.vec, frame_seg.vec)){
					val_by_slope += 1.0;
				}
			}
			if (piece_seg.end == frame_seg.end || piece_seg.end == frame_seg.start) {
				val_by_point += 1.0;
				if (equal_slope(piece_seg.vec, frame_seg.vec)){
					val_by_slope += 1.0;
				}
			}
		}
	}
	return val_by_point / 4 + val_by_slope;
}
unittest {
	auto frame = [P (10, 10), P (110, 10), P (60, 110)].vertexies2shape;
	auto shape = [P (0, 0), P (20, 0), P(10, 20)].vertexies2shape;
	assert (angle_and_points (frame, shape.move(P (10, 10))) == 3);
}

@safe @nogc
pure nothrow float eval_basic (in P[] frame, in P[] piece) {
	float point_conflict = 0.0f;
	foreach (piece_point; piece) {
		auto frame_point2 = frame[$-1];
		foreach (frame_point1; frame) {
			if (frame_point1 == piece_point)
				++point_conflict;
			else if (judge_on_line(piece_point, Segment(frame_point1, frame_point2)))
				++point_conflict;
		}
	}
	return point_conflict;
}