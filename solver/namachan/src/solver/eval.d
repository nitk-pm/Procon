module procon28.solver.eval;

import procon28.basic_data : P;
import procon28.solver.datamanip : equal_slope, move, protrude_frame, judge_on_line;

/++
 + 線分の角度の一致度と重複した点の数で評価する評価関数
 + 衝突していた場合は-float.inifinityを返す
 +/
@safe @nogc
pure nothrow float eval_basic (in P[] frame, in P[] piece) {
	float point_conflict = 0.0f;
	if (protrude_frame (frame, piece))
		return -float.infinity;
	foreach (piece_point; piece) {
		auto frame_point2 = frame[$-1];
		foreach (frame_point1; frame) {
			if (frame_point1 == piece_point)
				++point_conflict;
			else if (judge_on_line(piece_point, frame_point1, frame_point2))
				++point_conflict;
		}
	}
	return point_conflict;
}