module procon28.solver.collison;

import procon28.basic_data;
import procon28.solver.data;

import armos.math.vector;

/++
 + 線分の交差判定
 + 始点と終点が線分に接触していた場合は交差していないとみなす
 +/
bool judge_intersected (in Segment seg1, in Segment seg2) {
	auto ta = (seg2.start.x - seg2.end.x) * (seg1.start.y - seg2.start.y) +  (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.start.x);
	auto tb = (seg2.start.x - seg2.end.x) * (seg1.end.y - seg2.start.y) +  (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.end.x);
	auto tc = (seg1.start.x - seg1.end.x) * (seg2.start.y - seg1.start.y) +  (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.start.x);
	auto td = (seg1.start.x - seg1.end.x) * (seg2.end.y - seg1.start.y) +  (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.end.x);
	return tc * td < 0 && ta * tb < 0;
}
unittest {
	auto seg1 = Segment (Vector2i (-2, 3), Vector2i (4, -1));
	auto seg2 = Segment (Vector2i ( 3, 1), Vector2i (1, -3));
	assert (judge_intersected (seg1, seg2));

	auto seg3 = Segment (Vector2i ( 1, 3), Vector2i( 2, 8));
	auto seg4 = Segment (Vector2i ( 1, 2), Vector2i( 3, 8));
	assert (!judge_intersected (seg3, seg4));
}

/++
 + 一次方程式を解き、交点を返す
 + 解けない場合の動作は未定義
 +/
Vector2f solve_linear (Segment a, Segment b) {
	if (a.vec.x == 0 || b.vec.x == 0) return Vector2f (a.start.x, 0.0f);
	if (a.vec.x == 0) {
		immutable x = a.start.x;
		immutable y = b.line.slope.toFloat * x + b.line.seg.toFloat;
		return Vector2f (x, y);
	}
	if (b.vec.x == 0) {
		immutable x = b.start.x;
		immutable y = a.line.slope.toFloat * x + a.line.seg.toFloat;
		return Vector2f (x, y);
	}
	else {
		immutable a_slope = a.line.slope.toFloat;
		immutable a_seg = a.line.seg.toFloat;
		immutable b_slope = b.line.slope.toFloat;
		immutable b_seg = b.line.seg.toFloat;
		immutable x = (b_seg - a_seg) / (a_slope - b_slope);
		immutable y = a_slope * x + a_seg;
		return Vector2f (x, y);
	}
}

///点の図形に対する内外判定
bool judge_inclusion (in Point p, in Segment[] segments) {
	import std.stdio;
	auto horizontal_seg  = Segment (Vector2i(-1, p.y), Vector2i (102, p.y));
	size_t cross_cnt;
	foreach (ref seg; segments) {
		if (judge_intersected (horizontal_seg, seg)) {
			immutable cross_point = solve_linear (seg, horizontal_seg);
			if (cross_point.x > p.x)
				++cross_cnt;
		}
	}
	return cross_cnt % 2 == 1;
}
unittest {
	auto shape = [
		S( P(0, 0), P(0, 2)),
		S( P(0, 2), P(2, 2)),
		S( P(2, 2), P(2, 0)),
		S( P(2, 0), P(0, 0)),

		S( P(4, 0), P(4, 4)),
		S( P(4, 4), P(6, 4)),
		S( P(6, 4), P(6, 0)),
		S( P(6, 0), P(4, 0))
	];
	auto pt1 = P (1, 1);
	auto pt2 = P (2, 2);
	auto pt3 = P (5, 2);
	assert (judge_inclusion(pt1, shape));
	assert (!judge_inclusion(pt2, shape));
	assert (judge_inclusion(pt3, shape));
}

///図形同士の当たり判定
bool is_hit (in Shape frame, in Shape shape, in Point pos) {
	foreach (shape_seg; shape) {
		foreach (frame_seg; frame) {
			if (judge_intersected (frame_seg, shape_seg))
				return true;
		}
		if (!judge_inclusion(shape_seg.start, frame))
			return true;
	}
	return false;
}
