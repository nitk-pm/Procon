module procon28.solver.datamanip;

import armos.math.vector;

import procon28.basic_data : Segment, Shape, S, P, PlacedShape, Data, Situation, ShapeBits, Height, Width;

import std.math : approxEqual;

import std.algorithm.iteration : map;
import std.algorithm.sorting : sort;
import std.range : array, zip;

int segment_of_line (in Segment seg) {
	//y = ax + b
	if (seg.vec.x == 0) {
		return 0;
	}
	immutable ax = seg.vec.y * seg.start.x / seg.vec.x;
	return seg.start.y - ax;
}
unittest {
	alias S = Segment;
	alias V = Vector2i;
	assert (segment_of_line(S (V(1,3), V(3, 7))) == 1);
	assert (segment_of_line(S (V(1,1), V(1, 5))) == 0);
	assert (segment_of_line(S (V(1,1), V(5, 1))) == 1);
}

Segment[] segment_sort (in Segment[] segs)
in {
}
out (sorted){
	assert (sorted.length == segs.length);
}
body{
	import std.stdio;
	auto used = new bool[segs.length];
	used[0] = true;
	Shape sorted;
	sorted ~= segs[0];
	while (sorted.length < segs.length) {
		bool found;
		foreach (idx, seg; segs) {
			if (used[idx]) continue;
			else if (sorted[$-1].end == seg.start) {
				sorted ~= seg;
				used[idx] = true;
				found = true;
				break;
			}
			else if (sorted[$-1].end == seg.end) {
				sorted ~= Segment (seg.end, seg.start);
				used[idx] = true;
				found = true;
				break;
			}
		}
		if (!found) {
			sorted ~= segs[0];
		}
	}
	return sorted;
}
unittest {
	import procon28.basic_data : S, P;
	auto segs = [
		S(P(0,10), P(0,0)),
		S(P(0,10), P(10,10)),
		S(P(10,0), P(0,0)),
		S(P(10,10),P(10,0))
	];
	auto ans = [
		S(P(0,10), P(0,0)),
		S(P(0,0), P(10,0)),
		S(P(10,0), P(10,10)),
		S(P(10,10), P(0,10))
	];
	assert (segment_sort(segs) == ans);
}

bool equal_slope (in Vector2i v1, in Vector2i v2) {
	if (v1.x == 0 && v2.x == 0) {
		return v1.y * v2.y > 0;
	}
	return v1.x * v2.y == v2.x * v1.y;
}
unittest {
	alias V = Vector2i;
	assert (equal_slope (V (0,1), V(0,5)));
	assert (equal_slope (V (1, 0), V (5, 0)));
	assert (equal_slope (V (2, -1), V (-2, 1)));
	assert (equal_slope (V (9, 3), V (3, 1)));
	assert (!equal_slope (V (100, 99), V(100, 101)));
}

bool equal_slope (in Segment seg1, in Segment seg2) {
	return equal_slope (seg1.vec, seg2.vec) || equal_slope (seg1.vec, -seg2.vec);
}
unittest {
	alias S = Segment;
	alias V = Vector2i;
	assert (equal_slope(S(V(0, 0), V(2, 2)), S (V(3, 3), V(1, 1))));
	assert (!equal_slope(S(V(0, 0), V(2, 2)), S (V(3, 4), V(1, 1))));
	assert (equal_slope(S(V(0,0), V(0,1)),S(V(0,1),V(0,0))));
}

///頂点座標の列を線分の集合に変換
Shape vertexies2shape (Vector2i[] vertexies) {
	Shape shape;
	for (size_t i; i < vertexies.length; ++i) {
		auto start_idx = i;
		auto end_idx = (i + 1) % vertexies.length;
		if (vertexies[start_idx] != vertexies[end_idx])
			shape ~= Segment (vertexies[start_idx], vertexies[end_idx]);
	}
	return shape;
}
unittest {
	alias V = Vector2i;
	alias S = Segment;
	assert ([V(0, 0), V(2, 0), V(2, 2), V(0, 2)].vertexies2shape
		== [S (V(0, 0), V(2, 0)), S (V(2, 0), V(2, 2)), S (V (2, 2), V(0, 2)), S (V (0, 2), V (0, 0))]);
}



///線分のxorを取る。重なって居ない線分同士には使えない
Segment[2] xor (in Segment seg1, in Segment seg2) {
	auto xs = [seg1.start.x, seg1.end.x, seg2.start.x, seg2.end.x].dup.sort!"a>b".array;
	int[] ys;
	if (seg1.vec.x * seg1.vec.y > 0)
		 ys = [seg1.start.y, seg1.end.y, seg2.start.y, seg2.end.y].dup.sort!"a>b".array;
	else
		 ys = [seg1.start.y, seg1.end.y, seg2.start.y, seg2.end.y].dup.sort!"a<b".array;
	return [Segment(Vector2i(xs[0], ys[0]), Vector2i(xs[1], ys[1])), Segment(Vector2i(xs[2], ys[2]), Vector2i(xs[3], ys[3]))];
}
unittest {
	alias S = Segment;
	alias V = Vector2i;
	auto seg1 = S(V(0,0), V(4,4));
	auto seg2 = S(V(1,1), V(2,2));
	auto seg3 = S(V(0,0), V(10,10));
	auto seg4 = S(V(0,0), V(5, 5));
	import std.stdio;
}

Shape move (in Shape shape,in Vector2i pos) {
	auto copy = shape.dup;
	foreach (ref seg; copy) {
		seg.start += pos;
		seg.end += pos;
	}
	return copy;
}

Shape zoom (in Shape shape, int rate) {
	auto copy = shape.dup;
	foreach (ref seg; copy) {
		seg.start *= rate;
		seg.end *= rate;
	}
	return copy;
}

Shape shape_xor (in Shape shape_origin) {
	auto shape = shape_origin.dup;
	import std.stdio;
	Shape acc;
	auto overlap_mask = new bool [shape.length];
	auto not_recalcuration_target_mask = new bool [shape.length];
	for (;;) {
		Shape recalcuration_target;
		for (size_t i; i < shape.length; ++i) {
			for (size_t j = i + 1; j < shape.length; ++j) {
				if (judge_overlap (shape[i], shape[j])) {
					if (!overlap_mask[i] && !overlap_mask[j]) {
						foreach (seg;xor (shape[i], shape[j])) {
							if (seg.start != seg.end)
								recalcuration_target ~= seg;
						}
						not_recalcuration_target_mask[i] = true;
						not_recalcuration_target_mask[j] = true;
					}
					overlap_mask[i] = true;
					overlap_mask[j] = true;
				}
			}
		}
		Shape not_overlapped;
		foreach(idx, ref seg; shape) {
			if (!overlap_mask[idx])
				not_overlapped ~= seg;
			else if (!not_recalcuration_target_mask[idx]) {
				recalcuration_target ~= shape[idx];
			}
		}
		if (recalcuration_target.length == 0)
			return acc ~ not_overlapped;
		acc ~= not_overlapped;
		not_overlapped = [];
		shape = recalcuration_target;
		foreach(ref flag; overlap_mask) flag = false;
		foreach(ref flag; not_recalcuration_target_mask) flag = false;
	}
}
unittest {
	import std.algorithm.searching : any;
	import procon28.basic_data : S, P;
	alias V = Vector2i;
	auto s1 = [P(0,0), P(10,0),P(0, 10)].vertexies2shape;
	auto s2 = [P(0,0), P(10,0),P(0, 10)].vertexies2shape;
	assert (shape_xor(s1 ~ s2) == []);
	auto s3 = [P(0,0), P(20,0), P(20,40), P(0,40)].vertexies2shape;
	auto s4 = [P(0,0), P(20,0), P(20,20), P(0,20)].vertexies2shape;
	auto s5 = [S(V (0, 0), V(100, 0)), S(V (100, 0), V(100, 64)), S(V (100, 64), V(0, 64)), S(V (0, 64), V(0, 0))];
	auto s6 = [S(V (0, 0), V(53, 40)), S(V (53, 40), V(67, 64)), S(V (67, 64), V(0, 64)), S(V (0, 64), V(0, 0))];
	auto s7 =  [S(V (47, 40), V(100, 0)), S(V (100, 0), V(0, 0)), S(V (0, 0), V(47, 40))];

	auto merged = shape_xor(s3~s4);
	import std.stdio;
	import procon28.visualize.window;
	assert (merged.length == 4);
}

///線分が重なっているか判定(交差は重なっていない)
bool judge_overlap(in Segment seg1, in Segment seg2) {
	bool is_overlap (in int x1, in int x2, in int y1, in int y2) {
		if ((x1 == y1 && x2 == y2) || (x1 == y2 && x2 == y1)) return 0;
		return
			(y2 - x1) * (y2 - x2) < 0
			|| (x2 - y1) * (x2 - y2) < 0
			|| (y1 - x1) * (y1 - x2) < 0
			|| (x1 - y1) * (x1 - y2) < 0;
	}
	//傾きが一致しないものを弾く
	if (!equal_slope (seg1, seg2)) return false;
	if (seg1 == seg2) return true;
	if (segment_of_line(seg1) != segment_of_line(seg2)) return false;
	if (seg1.vec.x == 0) {
		if (seg1.start.x != seg2.start.x) return false;
		return is_overlap (seg1.start.y, seg1.end.y, seg2.start.y, seg2.end.y);
	}
	else {
		return is_overlap (seg1.start.x, seg1.end.x, seg2.start.x, seg2.end.x);
	}
}
unittest {
	alias V = Vector2i;
	alias S = Segment;
	auto seg1 = S (V(0,0), V(1,1));
	auto seg2 = S (V(0,0), V(3,3));
	auto seg3 = S (V(3,3), V(4,4));
	auto seg4 = S (V(2,2), V(6,6));
	auto seg5 = S (V(2,2), V(0,0));
	auto seg6 = S (V(1,2), V(2,3));
	auto seg7 = S (V(0,4), V(0,0));
	auto seg8 = S (V(0,2), V(0,0));
	auto seg9 = S (V(0,64), V(0,0));
	auto seg10 = S (V(0,0),V(53,40));
	assert (!judge_overlap (seg1, seg4));
	assert (judge_overlap  (seg2, seg4));
	assert (judge_overlap  (seg3, seg4));
	assert (!judge_overlap (seg4, seg5));
	assert (!judge_overlap  (seg1, seg6));
	assert (judge_overlap (seg2, seg2));
	assert (judge_overlap (seg7, seg8));
	assert (!judge_overlap (seg9, seg10));
}

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

///heightを高さとする平行線とsegが交差しているかを判定する。
bool judge_cross_horizontal_line (in Segment seg, in int height) {
	if ((seg.start - seg.end).y == 0) return false;
	if ((seg.start.y > height && seg.end.y < height) || (seg.start.y < height && seg.end.y > height))
		return true;
	return false;
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
	foreach (ref seg; segments) {
		if (judge_on_line(p, seg))
			return include_on_line;
		if (seg.vec.y == 0) continue;
		if (seg.end.y > seg.start.y && seg.start.y == p.y && seg.start.x > p.x) {
			++cross_cnt;
			continue;
		}
		else if (seg.end.y == p.y && seg.end.x > p.x) {
				++cross_cnt;
				continue;
		}
		if (judge_cross_horizontal_line(seg, p.y)) {
			if (cross_point_of_horizontal_line(seg, p.y).x > p.x) {
				++cross_cnt;
				continue;
			}
		}
	}
	return cross_cnt % 2 == 1;
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
	assert (crossing_number(pt1, shape1));
	assert (!crossing_number(pt2, shape1));
	assert (crossing_number(pt3, shape1));
	assert (!crossing_number(pt4, shape1));

	assert (crossing_number(pt5, shape2));
	assert (crossing_number(pt6, shape2));

	assert (!crossing_number(pt7, shape3));

	assert (crossing_number(pt8,shape4));
	assert (crossing_number(pt9,shape4));
}

//枠と図形の当たり判定
bool protrude_frame (in Shape frame, in Shape shape_origin) {
	auto shape = segment_sort (shape_origin);
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
	assert (!protrude_frame (frame1, shape1)); 
	assert (protrude_frame(frame2, shape2));
}

size_t shape_idx (in size_t piece_idx, in size_t spin_level) {
	return piece_idx * 8 + spin_level;
}

int bits_idx (in int x, in int y) {
	return x * Height + y;
}

ShapeBits cache_make (in Shape shape, bool include_on_line) {
	import std.stdio;
	import procon28.solver.datamanip : crossing_number;
	ShapeBits bits;
	foreach (x; 0..Width) {
		foreach (y; 0..Height) {
			auto b = widing_number (P(x,y), shape, include_on_line);
			bits[bits_idx(x,y)] = b;
		}
	}
	return bits;
}

void show (in ShapeBits bits) {
	import std.stdio;
	foreach (y; 0..Height) {
		foreach (x; 0..Width) {
			if (bits[bits_idx(x,y)])
				write('o');
			else
				write('.');
		}
		writeln;
	}
}

bool judge_collision (in ShapeBits bits1, in ShapeBits bits2, in P p1, in P p2) {
	auto dp = p2 - p1;
	auto didx = bits_idx(dp.x, dp.y);
	ShapeBits emp;
	return ((bits2 << didx) & bits1) != emp;
}
unittest {
	import std.stdio;
	auto s1 = [P(0,0),P(10,0),P(10,10)].vertexies2shape;
	auto s2 = [P(0,0),P(0,10),P(10,10)].vertexies2shape;
	auto c1 = cache_make(s1, true);
	auto c2 = cache_make(s2, false);
	assert (!judge_collision(c1, c2, P(0,0), P(0,0)));
	assert (!judge_collision(c1, c2, P(1,0), P(0,0)));
	assert (judge_collision(c1, c2, P(-1,0), P(0,0)));
}

bool protrude_frame (in Data data, in Situation situation, in PlacedShape piece) {
	auto p_bits = data.piece_bits (piece.piece_idx, piece.spin_level);
	foreach (shape; situation.shapes) {
		auto f_bits = data.piece_inside_bits (shape.piece_idx, shape.spin_level);
		if (judge_collision (p_bits, f_bits, P(piece.x, piece.y), P(shape.x, shape.y)))
			return true;
	}
	return false;
}
