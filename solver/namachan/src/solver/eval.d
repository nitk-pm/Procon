module procon28.solver.eval;

import procon28.basic_data : P;
import procon28.solver.datamanip : equal_slope, move, protrude_frame, judge_on_line, merge, insert_junction, Point;
import std.typecons : Tuple, tuple;

//接触をの検出
@safe
pure nothrow bool has_point_contact (in P[] s1, in P[] s2) {
	auto ps1 = insert_junction(s1,s2);
	auto ps2 = insert_junction(s2,s1);
	@safe @nogc
	pure nothrow bool include_isolated_junction (in Point[] ps) {
		foreach (idx, pt; ps) {
			//非接触点-接触点-非接触点 があるかどうかで孤立した接触点があるかを調べ、孤立した接触点があれば点接触になっているのでtrue
			if (!pt.is_junction && ps[(idx+1)%ps.length].is_junction && !ps[(idx+2)%ps.length].is_junction)
				return true;
		}
		return false;
	}
	return include_isolated_junction(ps1) || include_isolated_junction(ps2);
}
unittest {
	auto box = [P(0,0),P(10,0),P(10,10),P(0,10)];
	auto triangle1 = [P(2,2), P(10,5),P(2,7)];
	assert (point_contact(box, triangle1));
	auto small_box = [P(0,0),P(5,0),P(5,5),P(0,5)];
	assert (!point_contact(box, small_box));
	auto triangle2 = [P(2,0), P(10,5),P(2,7)];
	assert (point_contact(box, triangle2));
	auto triangle3 = [P(0,0),P(5,7),P(10,10)];
	assert (point_contact(box, triangle3));
}

/++
 + 線分の角度の一致度と重複した点の数で評価する評価関数
 + 衝突していた場合は-float.inifinityを返す
 +/
@safe
pure nothrow Tuple!(float, P[][]) eval_basic (in P[] frame, in P[] piece) {
	float point_conflict = 0.0f;
	float pt_on_line = 0.0f;
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
				++pt_on_line;
		}
	}
	if (point_conflict < 1.1f && pt_on_line == 0.0f) return tuple(-float.infinity, cast(P[][])[]);	
	return tuple(point_conflict - segment_sum / 200.0f - tooshort - frame_num, merged);
}

@safe
pure nothrow Tuple!(float, P[][]) simple_is_best (in P[] frame, in P [] piece) {
	float point_conflict = 0.0f;
	if (protrude_frame (frame, piece))
		return tuple(-float.infinity, cast(P[][])[]);
	auto merged = merge (frame, piece);
	if (merged.length == 0) return tuple(float.infinity, cast(P[][])[]);
	bool pt_on_line = false;
	if (has_point_contact (frame, piece))
		return tuple(-float.infinity, cast(P[][])[]);
	foreach (f_idx, frame_point1; frame) {
		auto frame_point2 = frame[(f_idx+1)%frame.length];
		foreach (piece_point; piece) {
			if (frame_point1 == piece_point)
				++point_conflict;
		}
	}
	return tuple(point_conflict, merged);
}
