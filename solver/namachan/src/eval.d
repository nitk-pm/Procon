module procon28.eval;

import procon28.data : P;
import procon28.geometry : equal_slope, move, protrude_frame, judge_on_line, merge, insert_junction, Point;
import std.typecons : Tuple, tuple;
import std.meta : AliasSeq;

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
	assert (has_point_contact(box, triangle1));
	auto small_box = [P(0,0),P(5,0),P(5,5),P(0,5)];
	assert (!has_point_contact(box, small_box));
	auto triangle2 = [P(2,0), P(10,5),P(2,7)];
	assert (has_point_contact(box, triangle2));
	auto triangle3 = [P(0,0),P(5,7),P(10,10)];
	assert (has_point_contact(box, triangle3));
}

@safe @nogc
pure nothrow float point_conflict (in P[] frame, in P[] piece, in P[][] merged) {
	float point_conflict = 0.0f;
	foreach (f_idx, frame_point1; frame) {
		auto frame_point2 = frame[(f_idx+1)%frame.length];
		foreach (piece_point; piece) {
			if (frame_point1 == piece_point)
				++point_conflict;
		}
	}
	return point_conflict;
}

@safe @nogc
pure nothrow tooshort_segments (alias short_threshold, alias val) (in P[] frame, in P[] piece, in P[][] merged) {
	float tooshort = 0.0f;
	foreach (merged_frame; merged) {
		foreach (f_idx, f_point1; merged_frame) {
			auto f_point2 = merged_frame[(f_idx+1)%merged_frame.length];
			auto norm = (f_point1 - f_point2).norm;
			if (norm < short_threshold)
				tooshort += val;
		}
	}
	return tooshort;
}

@safe @nogc
pure nothrow segment_length_total (in P[] frame, in P[] piece, in P[][] merged) {
	float segment_sum = 0.0f;
	foreach (merged_frame; merged) {
		foreach (f_idx, f_point1; merged_frame) {
			auto f_point2 = merged_frame[(f_idx+1)%merged_frame.length];
			auto norm = (f_point1 - f_point2).norm;
			segment_sum += norm;
		}
	}
	return segment_sum;
}

@safe @nogc
pure nothrow float frame_num (in P[] frame, in P[] piece, in P[][] merged) {
	return cast(float)merged.length;
}

//マージ後合計線長 - マージ前合計線長
//係数は負の値を指定すべき
@safe @nogc
pure nothrow float diff_length_total (in P[] frame, in P[] piece, in P[][] merged) {
	float before, after;
	foreach (seg; frame)
		before += seg.norm;

	foreach (shape; merged) {
		foreach (seg; shape) {
			after += seg.norm;
		}
	}
	return after - before;
}

@safe @nogc
pure nothrow float vanish_frame (alias v)(in P[] frame, in P[] piece, in P[][] merged) {
	if (merged.length == 0)
		return v;
}

template eval (Set...) {
	@safe
	pure nothrow Tuple!(float, P[][]) eval (in P[] frame, in P[] piece) {
		if (protrude_frame (frame, piece))
			return tuple(-float.infinity, cast(P[][])[]);
		if (has_point_contact(frame, piece))
			return tuple(-float.infinity, cast(P[][])[]);
		auto merged = merge (frame, piece);
		float score = 0.0f;
		@safe @nogc
		pure nothrow float score_acc (Set...)(in P[] frame, in P[] piece, in P[][] merged) {
			static if (Set.length > 0 && Set.length < 3) {
				static assert (false, "不正なテンプレート引数");
			}
			else static if (Set.length != 0) {
				return  Set[0](frame, piece, merged) * Set[2] + score_acc!(Set[3..$])(frame, piece, merged);
			}
			else {
				return 0.0f;
			}
		}
		return tuple(score_acc!Set(frame, piece, merged), merged);
	}
}

alias simple_is_best = AliasSeq!(point_conflict, 1, 1.0f);
