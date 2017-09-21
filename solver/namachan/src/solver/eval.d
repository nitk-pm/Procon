module procon28.solver.eval;

import procon28.basic_data : P;
import procon28.solver.datamanip : equal_slope, move, protrude_frame, judge_on_line, merge;
import std.typecons : Tuple, tuple;

/++
 + 線分の角度の一致度と重複した点の数で評価する評価関数
 + 衝突していた場合は-float.inifinityを返す
 +/
@safe
pure nothrow Tuple!(float, P[][]) eval_basic (in P[] frame, in P[] piece) {
	float point_conflict = 0.0f;
	float segment_sum = 0.0f;
	float tooshort = 0.0f;
	float frame_num = 0.0f;
	if (protrude_frame (frame, piece))
		return tuple(-float.infinity, cast(P[][])[]);
	auto merged = merge (frame, piece);
	if (merged.length == 0) return tuple(float.infinity, cast(P[][])[]);
	if (merged.length > 1) {
		frame_num += merged.length * 2;
	}
	foreach (merged_frame; merged) {
		foreach (f_idx, f_point1; merged_frame) {
			auto f_point2 = merged_frame[(f_idx+1)%merged_frame.length];
			auto norm = (f_point1 - f_point2).norm;
			segment_sum += norm;
			if (norm < 4.0f)
				tooshort += 10.0f;
			else if (norm < 10.0f)
				tooshort += 3.0f;
		}
	}

	foreach (f_idx, frame_point1; frame) {
		auto frame_point2 = frame[(f_idx+1)%frame.length];
		foreach (piece_point; piece) {
			if (frame_point1 == piece_point)
				++point_conflict;
			else if (judge_on_line(piece_point, frame_point1, frame_point2))
				++point_conflict;
		}
	}
	if (point_conflict == 1) return tuple(-float.infinity, cast(P[][])[]);	
	return tuple(point_conflict - segment_sum / 200.0f - tooshort - frame_num, merged);
}

@safe
pure nothrow Tuple!(float, P[][]) simple_is_best (in P[] frame, in P [] piece) {
	float point_conflict = 0.0f;
	if (protrude_frame (frame, piece))
		return tuple(-float.infinity, cast(P[][])[]);
	auto merged = merge (frame, piece);
	if (merged.length == 0) return tuple(float.infinity, cast(P[][])[]);
	foreach (frame_point; frame) {
		foreach (piece_point; piece) {
			if (frame_point == piece_point)
				++point_conflict;
		}
	}
	return tuple(point_conflict, merged);
}
