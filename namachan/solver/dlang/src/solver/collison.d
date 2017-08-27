module procon28.solver.collison;

import armos.math.vector;

import std.stdio;

import procon28.basic_data : Segment, P, S, Shape;
import procon28.solver.data : move, segment_of_line,vertexies2shape, segment_sort;

/++
 + 線分の交差判定
 + 始点と終点が線分に接触していた場合は交差していないとみなす
 + 結構簡単にオーバーフローしてバグを生むので注意
 +/
bool judge_intersected (in Segment seg1, in Segment seg2) {
	auto ta = (seg2.start.x - seg2.end.x) * (seg1.start.y - seg2.start.y) + (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.start.x);
	auto tb = (seg2.start.x - seg2.end.x) * (seg1.end.y   - seg2.start.y) + (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.end.x);
	auto tc = (seg1.start.x - seg1.end.x) * (seg2.start.y - seg1.start.y) + (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.start.x);
	auto td = (seg1.start.x - seg1.end.x) * (seg2.end.y   - seg1.start.y) + (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.end.x);
	return tc * td < 0 && ta * tb < 0;
}
unittest {
	auto seg1 = S (P (-2, 3), P (4, -1));
	auto seg2 = S (P ( 3, 1), P (1, -3));
	assert (judge_intersected (seg1, seg2));

	auto seg3 = S (P ( 1, 3), P ( 2, 8));
	auto seg4 = S (P ( 1, 2), P ( 3, 8));
	assert (!judge_intersected (seg3, seg4));

	auto seg5 = S(P(20,40),P(0,40));
	auto seg6 = S(P(0,20),P(20,0));
	assert (!judge_intersected(seg5, seg6));
}

bool judge_on_line (in Vector2i p, in Segment seg) {
	immutable v1 = seg.vec;
	immutable v2 = p - seg.start;
	immutable v1_abs_2 = (v1.x^^2+v1.y^^2);
	immutable v2_abs_2 = (v2.x^^2+v2.y^^2);
	immutable dot = v1.dotProduct(v2);
	//内積が|v1|*|v2|かつ内積が正(角度が同じ)で、v2の長さがv1より短い(線分の中に含まれている)場合は線の上にあるとみなす。
	return dot > 0 && dot^^2 == v1_abs_2*v2_abs_2 && v1_abs_2 >= v2_abs_2;
}

/++
 + heightを高さとする水平線とsegを通る直線の一次方程式を解き、交点を返す
 + 解けない場合の動作は未定義
 +/
Vector2f cross_point_of_horizontal_line (in Segment seg, in int height)  {
	if (seg.vec.x == 0)
		return Vector2f (seg.start.x, height);
	if (seg.vec.y == 0)
		return Vector2f (float.infinity, height);
	return Vector2f ((height - segment_of_line(seg)) * seg.vec.x / seg.vec.y, height);
}

///heightを高さとする平行線とsegが交差しているかを判定する。
bool judge_cross_horizontal_line (in Segment seg, in int height) {
	if ((seg.start - seg.end).y == 0) return false;
	if ((seg.start.y > height && seg.end.y < height) || (seg.start.y < height && seg.end.y > height))
		return true;
	return false;
}

Vector2i mid_point (in Segment seg) {
	return (seg.start + seg.end) / 2;
}

///Widing Number Algorithm
///FIXME
bool widing_number (in Vector2i p, in Segment[] segments, bool include_on_line = true) {
	int wn;
	auto sorted = segment_sort(segments);
	foreach (seg; segments) {
		if (judge_on_line(p, seg))
			return include_on_line;
		if (seg.start.y <= p.y && seg.end.y > p.y) {
			float vt = (cast(float)p.y - cast(float)seg.start.y) / (cast(float)seg.end.y - cast(float)seg.start.y);
			if (p.x < (seg.start.x + (vt * (seg.end.x - seg.start.x)))) wn++;
		}
		else if (seg.start.y > p.y && seg.end.y <= p.y) {
			float vt = (cast(float)p.y - cast(float)seg.start.y) / (cast(float)seg.end.y - cast(float)seg.start.y);
			if (p.x < (seg.start.x + (vt * (seg.end.x - seg.start.x)))) wn--;
		}
	}
	return wn != 0;
}
unittest {
	auto shape1 = [
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
	auto pt2 = P (2, 3);
	auto pt3 = P (5, 2);
	auto pt4 = P (3, 2);

	auto shape2 = [
		S (P(0,0), P(0,3)),
		S (P(3,0), P(0,3)),
		S (P(3,0), P(0,0))
	];
	auto pt5 = P(2,0);
	auto pt6 = P(1,1);

	auto shape3 = [P(0,0), P(-10,0), P(-10, 10)].vertexies2shape;
	auto pt7 = P(10,10);

	auto shape4 = [S(P (0, 0), P(20, 0)), S(P (20, 0), P(20, 40)), S(P (20, 40), P(0, 40)), S(P (0, 40), P(0, 0))];
	auto pt8 = P (0,20);
	auto pt9 = P (20,0);
	assert (widing_number(pt1, shape1));
	assert (!widing_number(pt2, shape1));
	assert (widing_number(pt3, shape1));
	assert (!widing_number(pt4, shape1));

	assert (widing_number(pt5, shape2));
	assert (widing_number(pt6, shape2));

	assert (!widing_number(pt7, shape3));

	assert (widing_number(pt8,shape4));
	assert (widing_number(pt9,shape4));
}
///点の図形に対する内外判定
///Crossing Number Algorithm
bool crossing_number (in Vector2i p, in Segment[] segments, bool include_on_line = true) {
	size_t cross_cnt;
	size_t point_cnt;
	foreach (ref seg; segments) {
		//線分の始点と終点には当たり判定がない
		//線分が交差していて、交点のx座標がpより大きい時、カウント
		if (judge_cross_horizontal_line (seg, p.y)) {
			immutable cross_point = cross_point_of_horizontal_line(seg, p.y);
			if (cross_point.x > p.x)
				++cross_cnt;
		}
		//始点又は終点と重なっていた場合は、境界線上なので含まれるとする
		if (seg.end == p || seg.start == p){
			if (include_on_line)
				return true;
			else
				return false;
		}
		//線分と点の重なりを判定
		immutable v1 = seg.vec;
		immutable v2 = p - seg.start;
		immutable v1_abs_2 = (v1.x^^2+v1.y^^2);
		immutable v2_abs_2 = (v2.x^^2+v2.y^^2);
		immutable dot = v1.dotProduct(v2);
		//内積が|v1|*|v2|かつ内積が正(角度が同じ)で、v2の長さがv1より短い(線分の中に含まれている)場合は線の上にあるとみなす。
		if (dot > 0 && dot^^2 == v1_abs_2*v2_abs_2 && v1_abs_2 >= v2_abs_2){
			if (include_on_line)
				return true;
			else
				return false;
		}
		//同じ高さで右側に始点もしくは交点があればカウント
		//向きによって始点か終点どちらかだけをカウントするアルゴリズムが紹介されていたが、
		//向きの判定は難しいので始点と終点両方をカウントして2で割る
		if ((seg.start.x > p.x && seg.start.y == p.y) || (seg.end.x > p.x && seg.end.y == p.y))
			++point_cnt;
	}
	return (cross_cnt + (point_cnt / 2)) % 2 == 1;
}
unittest {
	auto shape1 = [
		S( P(0, 0), P(0, 2)),
		S( P(0, 2), P(2, 2)),
		S( P(2, 2), P(2, 0)),
		S( P(2, 0), P(0, 0)),

		S( P(6, 4), P(6, 0)),
		S( P(4, 4), P(6, 4)),
		S( P(6, 0), P(4, 0)),
		S( P(4, 0), P(4, 4))
	];
	auto pt1 = P (1, 1);
	auto pt2 = P (2, 3);
	auto pt3 = P (5, 2);
	auto pt4 = P (3, 2);

	auto shape2 = [P(0,0), P(-10,0), P(-10, 10)].vertexies2shape;
	auto pt5 = P(10,10);
	assert (crossing_number(pt1, shape1));
	assert (!crossing_number(pt2, shape1));
	assert (crossing_number(pt3, shape1));
	assert (!crossing_number(pt4, shape1));
	assert (!crossing_number(pt5, shape2));
}

//枠と図形の当たり判定
bool is_hit (in Shape frame, in Shape shape) {
	foreach (shape_seg; shape) {
		foreach (frame_seg; frame) {
			if (judge_intersected (frame_seg, shape_seg))
				return true;
		}
		if (!crossing_number(shape_seg.start, frame) || !crossing_number(shape_seg.end, frame)) {
			return true;
		}
	}
	//ピースの中を横切る線分の対処
	//線分の中点がピースの中にあればピースの中を線分が横切っている
	foreach (seg; frame) {
		if (crossing_number(mid_point(seg), shape, false)) {
			return true;
		}
	}
	return false;
}
unittest {
	auto shape1 = [P(0,0), P(0,20), P(20,0)].vertexies2shape;
	auto frame1 = [P(0,0), P(20,0), P(20,40), P(0,40)].vertexies2shape;
	auto shape2 = [P(0,0), P(10,10), P(10,0)].vertexies2shape;
	auto frame2 = [P(0,0), P(-10,0), P(-10, 10)].vertexies2shape;
	assert (!is_hit (frame1, shape1)); 
	assert (is_hit(frame2, shape2));
}
