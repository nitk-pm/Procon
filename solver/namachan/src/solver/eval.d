module procon28.solver.eval;

import procon28.basic_data : P;
import procon28.solver.datamanip : equal_slope, move, protrude_frame, judge_on_line, merge;
import std.typecons : Tuple, tuple;

/++
 + 線分の角度の一致度と重複した点の数で評価する評価関数
 + 衝突していた場合は-float.inifinityを返す
 +/
@safe @nogc
pure nothrow Tuple!(float, P[][]) eval_basic (in P[] frame, in P[] piece) {
	float point_conflict = 0.0f;
	float segment_sum = 0.0f;
	float tooshort = 0.0f;
	if (protrude_frame (frame, piece))
		return tuple(-float.infinity, cast(P[][])[]);
	auto merged = merge (frame, piece);
	foreach (f_idx, frame_point1; frame) {
		auto frame_point2 = frame[(f_idx+1)%frame.length];
		foreach (piece_point; piece) {
			if (frame_point1 == piece_point)
				++point_conflict;
			else if (judge_on_line(piece_point, frame_point1, frame_point2))
				++point_conflict;
		}
	}
	return tuple(point_conflict, merged);
}
