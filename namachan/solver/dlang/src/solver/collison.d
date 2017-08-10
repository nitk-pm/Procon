module procon28.solver.collison;

import armos.math.vector;

import procon28.basic_data : Segment, P, S, Shape;
import procon28.solver.data : move, vertexies2shape;

/++
 + 線分の交差判定
 + 始点と終点が線分に接触していた場合は交差していないとみなす
 + 結構簡単にオーバーフローしてバグを生むので注意
 +/
bool judge_intersected (in Segment seg1, in Segment seg2) {
	auto ta = (seg2.start.x - seg2.end.x) * (seg1.start.y - seg2.start.y) + (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.start.x);
	auto tb = (seg2.start.x - seg2.end.x) * (seg1.end.y - seg2.start.y)   + (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.end.x);
	auto tc = (seg1.start.x - seg1.end.x) * (seg2.start.y - seg1.start.y) + (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.start.x);
	auto td = (seg1.start.x - seg1.end.x) * (seg2.end.y - seg1.start.y)   + (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.end.x);
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

/++
 + heightを高さとする水平線とsegを通る直線の一次方程式を解き、交点を返す
 + 解けない場合の動作は未定義
 +/
Vector2f cross_point_of_horizontal_line (in Segment seg, in int height)  {
	if (seg.vec.x == 0)
		return Vector2f (seg.start.x, height);
	if (seg.vec.y == 0)
		return Vector2f (float.infinity, height);
	float slope = seg.line.slope.toFloat;
	float line_seg   = seg.line.seg.toFloat;
	return Vector2f ((height - line_seg) / slope, height);
}

///heightを高さとする平行線とsegが交差しているかを判定する。
bool judge_cross_horizontal_line (in Segment seg, in int height) {
	if ((seg.start - seg.end).y == 0) return false;
	if ((seg.start.y > height && seg.end.y < height) || (seg.start.y < height && seg.end.y > height))
		return true;
	return false;
}

///点の図形に対する内外判定
bool judge_inclusion (in Vector2i p, in Segment[] segments) {
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
			return true;
		}
		//線分と点の重なりを判定
		immutable v1 = seg.vec;
		immutable v2 = p - seg.start;
		immutable v1_abs_2 = (v1.x^^2+v1.y^^2);
		immutable v2_abs_2 = (v2.x^^2+v2.y^^2);
		immutable dot = v1.dotProduct(v2);
		//内積が|v1|*|v2|かつ内積が正(角度が同じ)で、v2の長さがv1より短い(線分の中に含まれている)場合は線の上にあるとみなす。
		if (dot > 0 && dot^^2 == v1_abs_2*v2_abs_2 && v1_abs_2 >= v2_abs_2){
			return true;
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
	auto pt2 = P (2, 3);
	auto pt3 = P (5, 2);
	assert (judge_inclusion(pt1, shape));
	assert (!judge_inclusion(pt2, shape));
	assert (judge_inclusion(pt3, shape));
}

//枠と図形の当たり判定
bool is_hit (in Shape frame, in Shape shape) {
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
unittest {
	auto shape = [P(0,0), P(0,20), P(20,0)].vertexies2shape;
	auto frame = [P(0,0), P(20,0), P(20,40), P(0,40)].vertexies2shape;
	assert (!is_hit (frame, shape)); 
}
