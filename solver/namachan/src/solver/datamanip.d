module procon28.solver.datamanip;

import procon28.basic_data : P, PlacedShape, Situation, Height, Width, Pos, Vector;

import std.math : approxEqual;

import std.algorithm.iteration : map;
import std.algorithm.sorting : sort;
import std.range : array, zip;

@safe @nogc
nothrow pure bool equal_slope (in Pos v1, in Pos v2) {
	if (v1.x == 0 && v2.x == 0) {
		return v1.y * v2.y > 0;
	}
	return v1.x * v2.y == v2.x * v1.y;
}
unittest {
	alias V = Pos;
	assert (equal_slope (V (0,1), V(0,5)));
	assert (equal_slope (V (1, 0), V (5, 0)));
	assert (equal_slope (V (2, -1), V (-2, 1)));
	assert (equal_slope (V (9, 3), V (3, 1)));
	assert (!equal_slope (V (100, 99), V(100, 101)));
}

struct Point {
	P pos;
	bool is_junction;
	bool visited;
	@safe
	pure string toString() const {
		import std.format;
		return format("Point(%s, %s, %s, %s)", pos.x, pos.y, is_junction, visited);
	}
}

//頂点座標の配列を、ピースと接触している点と接触していない点からなる頂点座標の配列に
@safe
nothrow pure Point[] insert_junction (in P[] frame, in P[] piece) {
	import std.range : retro;
	Point[] frame_buf;
	auto piece_rev  = piece.retro.array;
	foreach (f_idx; 0..frame.length) {
		bool point_appended;
		//始点とピースの頂点の何処かが衝突していた場合追加
		foreach (p_idx,piece_p; piece_rev) {
			if (frame[f_idx] == piece_p || judge_on_line(frame[f_idx], piece_p, piece_rev[(p_idx+1)%piece_rev.length])) {
				frame_buf ~= Point(frame[f_idx], true, false);
				point_appended = true;
				break;
			}
		}
		//線分の間にピースの頂点があった場合追加
		foreach (p_idx,piece_p; piece_rev) {
			//judge_on_lineは線分の終点を含むので、除外
			if (frame[(f_idx+1)%frame.length] != piece_p
				&& judge_on_line(piece_p, frame[f_idx], frame[(f_idx+1)%frame.length])) {
				//まだ始点が追加されてなかった場合追加してから線分の間の点を追加
				if (!point_appended) {
					frame_buf ~= Point(frame[f_idx], false, false);
					point_appended = true;
				}
				frame_buf ~= Point(piece_p, true, false);
			}
		}
		//衝突してなかった場合、点を追加
		if (!point_appended){
			frame_buf ~= Point(frame[f_idx], false, false);
		}
	}
	return frame_buf;
}
unittest {
	auto s1 = [P(0,0), P(10,0), P(10,10), P(0,10)];
	auto s2 = [P(0,0),P(5,0),P(0,10)];
	auto ans =
		[Point(P(0,0),true,false),Point(P(5,0),true, false),Point(P(10,0),false,false),Point(P(10,10),false,false),Point(P(0,10),true,false)];
	assert (insert_junction(s1,s2)==ans);
}
@safe @nogc
nothrow pure size_t find_junction (in Point[] points, in P target) {
	foreach (idx, point; points) {
		if (point.pos == target)
			return idx;
	}
	return 0;
}

@safe @nogc
nothrow pure bool judge_cross_horizontal_line (V : Vector!T, T)(in V p, in V start, in V end) {
	return (start.y - p.y) * (end.y - p.y) < 0;
}

@safe @nogc
nothrow pure float on_right_side (V : Vector!T, T) (in V start, in V end, in V p) {
	if ((p.y - start.y) * (p.y - end.y) > 0) return false;
	if ((end - start).x == 0) {
		if (start.x <= p.x) return true;
		else return false;
	}
	immutable a = cast(float)(end.y - start.y) / cast(float)(end.x - start.x);
	immutable b = end.y - a * end.x;
	immutable cross_x = (p.y - b) / a;
	return cross_x <= p.x;
}

@safe @nogc
pure nothrow crossing_number (V : Vector!T, T) (in V p, in V[] shape, in bool include_on_line = true) {
	size_t cross_cnt;
	foreach (idx, vertex1; shape) {
		auto vertex2 = shape[(idx+1)%shape.length];
		if ((judge_on_line(p, vertex1,vertex2)))
			return include_on_line;
		if (vertex1.y == p.y) {
			//並行の場合は無視
			if (vertex1.y == vertex2.y)
				continue;
			if (vertex1.y < vertex2.y && vertex1.x <= p.x)
				++cross_cnt;
		}
		else if (vertex2.y == p.y) {
			//並行の場合は無視
			if (vertex1.y == vertex2.y)
				continue;
			if (vertex1.y > vertex2.y && vertex2.x <= p.x)
				++cross_cnt;
		}
		else if (on_right_side(vertex1,vertex2, p))
			++cross_cnt;
	}
	return cross_cnt % 2 == 1;
} 
unittest {
	auto shape1 = [
		P(0, 0),
		P(0, 2),
		P(2, 2),
		P(2, 0)
	];
	auto pt1 = P (1, 1);
	auto pt2 = P (2, 3);
	auto pt4 = P (3, 2);

	auto shape2 = [
		P(0,0), P(3,0), P(0,3)
	];
	auto pt5 = P(2,0);
	auto pt6 = P(1,1);

	auto shape3 = [P(0,0), P(-10,0), P(-10, 10)];
	auto pt7 = P(10,10);

	auto shape4 = [P (0, 0), P (20, 0), P(20, 40), P(0, 40), P(0, 0)];
	auto pt8 = P (0,20);
	auto pt9 = P (20,0);
	assert (crossing_number(pt1, shape1));
	assert (!crossing_number(pt2, shape1));
	assert (!crossing_number(pt4, shape1));

	assert (crossing_number(pt5, shape2));
	assert (crossing_number(pt6, shape2));

	assert (!crossing_number(pt7, shape3));

	assert (crossing_number(pt8,shape4));
	assert (crossing_number(pt9,shape4));
}

@safe
pure nothrow bool same(in P[] s1, in P[] s2) {
	if (s1.length != s2.length) return false;
	if (s1.length == 0) return true;
	auto v2 = s2[0];
	long[] start_points;
	foreach (idx1, v1; s1) {
		if (v1 == v2) start_points ~= idx1;
	}
	if (start_points.length == 0) return false;
	bool same_any;
	foreach (start_idx; start_points) {
		bool same = true;
		for (size_t idx2, idx1 = start_idx; idx2 < s2.length; ++idx2, idx1 = (idx1+1)%s1.length) {
			if (s1[idx1] != s2[idx2]) {
				same = false;
				break;
			}
		}
		same_any |= same;
	}
	return same_any;
}
unittest {
	auto s1 = [P(0,0), P(5,2), P(8,9)];
	auto s2 = [P(8,9), P(0,0), P(5,2)];
	assert (same(s1,s2));
	auto s3 = [P(0,0), P(5,2), P(8,8)];
	assert (!same(s1,s3));

	auto s4 = [P(0, 0), P(10, 0), P(10, 10), P(0, 10), P(0, 5), P(5, 7), P(5, 3), P(0, 5)];
	auto s5 = [P(0, 5), P(5, 7), P(5, 3), P(0, 5), P(0, 0), P(10, 0), P(10, 10), P(0, 10)];
	assert (same(s4,s5));
}

@safe
pure nothrow bool same (in P[][] ss1, in P[][] ss2) {
	if (ss1.length != ss2.length) return false;
	auto used_mask = new bool[ss2.length];
	foreach (s1; ss1) {
		bool same_found;
		foreach(s2_idx, s2; ss2) {
			if (used_mask[s2_idx]) continue;
			if (same(s1, s2)) {
				same_found = true;
				used_mask[s2_idx] = true;
			}
		}
		if (!same_found) return false;
	}
	return true;
}
unittest {
	auto ss1 = [[P(0,0), P(5,2), P(8,9)], [P(4,2),P(1,1),P(31,4)]];
	auto ss2 = [[P(4,2),P(1,1),P(31,4)], [P(8,9), P(0,0), P(5,2)]];
	assert (same(ss1,ss2));
}

@safe
pure nothrow P[][] merge(in P[] frame, in P[] piece) {
	import std.range : retro, array;
	Point[] frame_buf;
	Point[] piece_buf;
	frame_buf = insert_junction(frame, piece);
	piece_buf = insert_junction(piece, frame).retro.array;
	P[][] shapes;
	P[] take (size_t start, ref Point[] looking, ref Point[] not_looking) {
		P[] took;
		auto idx = start;
		looking[idx].visited = true;
		took ~= looking[idx].pos;
		for (;;) {
			idx = (idx+1)%looking.length;
			if (looking[idx].is_junction)
				return took ~ take (find_junction(not_looking, looking[idx].pos), not_looking, looking);
			if (looking[idx].visited)
				return took;
			looking[idx].visited = true;
			took ~= looking[idx].pos;
		}
	}
	foreach (f_idx, frame_point; frame_buf) {
		if (frame_point.is_junction || frame_point.visited) continue;
		auto took = take (f_idx, frame_buf, piece_buf);
		if (took.length >= 3)
			shapes ~= took;
	}
	foreach (p_idx, piece_point; piece_buf) {
		if (piece_point.is_junction || piece_point.visited) continue;
		auto took = take (p_idx, piece_buf, piece_buf);
		if (took.length >= 3)
			shapes ~= took;
	}
	return shapes;
}
unittest {
	auto s1 = [P(0,0),P(10,0),P(10,20),P(0,20)];
	auto s2 = [P(0,0),P(10,0),P(0,10)];
	assert (same(merge (s1, s2),[[P(0,10),P(10,0),P(10,20),P(0,20)]]));
	assert (same(merge (s1, s1), []));
	
	auto square = [P(0,0),P(10,0),P(10,10),P(0,10)];
	auto triangle1 = [P(0,5),P(5,3),P(5,7)];
	assert (same(merge (square,triangle1), [[P(0, 5), P(5, 7), P(5, 3), P(0, 5), P(0, 0), P(10, 0), P(10, 10), P(0, 10)]]));
	auto triangle2 = [P(0,5),P(5,0),P(5,7)];
	assert (same(merge (square,triangle2), [[P(5, 0), P(0, 5), P(0, 0)], [P(0, 5), P(5, 7), P(5, 0), P(10, 0), P(10, 10), P(0, 10)]]));
}

@safe
nothrow pure P[] move (in P[] shape, in Pos pos) {
	auto copy =  shape.dup;
	foreach (ref vertex; copy)
		vertex += pos;
	return copy;
}

@safe
nothrow pure P[] move (in P[] shape, in int x, in int y) {
	return shape.move(P(x,y));
}


/++
 + 線分の交差判定
 + 始点と終点が線分に接触していた場合は交差していないとみなす
 + 結構簡単にオーバーフローしてバグを生むので注意
 +/
@safe @nogc
nothrow pure bool judge_intersected (in P s1, in P e1, in P s2, in P e2) {
	auto ta = (s2.x - e2.x) * (s1.y - s2.y) + (s2.y - e2.y) * (s2.x - s1.x);
	auto tb = (s2.x - e2.x) * (e1.y - s2.y) + (s2.y - e2.y) * (s2.x - e1.x);
	auto tc = (s1.x - e1.x) * (s2.y - s1.y) + (s1.y - e1.y) * (s1.x - s2.x);
	auto td = (s1.x - e1.x) * (e2.y - s1.y) + (s1.y - e1.y) * (s1.x - e2.x);
	return tc * td < 0 && ta * tb < 0;
}
unittest {
	assert (judge_intersected (P(-2,3),P(4,-1), P(3,1),P(1,-3)));
	assert (!judge_intersected (P(1,3),P(2,8), P(1,2),P(3,8)));
	assert (!judge_intersected(P(20,40),P(0,40), P(0,20),P(20,0)));
}
@safe @nogc
nothrow pure bool judge_on_line (V : Vector!T, T)(in V p, in V start, in V end) {
	immutable v1 = end - start;
	immutable v2 = p - start;
	immutable v1_abs_2 = (v1.x^^2+v1.y^^2);
	immutable v2_abs_2 = (v2.x^^2+v2.y^^2);
	immutable dot = v1 * v2;
	//内積が|v1|*|v2|かつ内積が正(角度が同じ)で、v2の長さがv1より短い(線分の中に含まれている)場合は線の上にあるとみなす。
	static if (__traits(isFloating, T)) {
		import std.math : approxEqual;
		return approxEqual(dot > 0 && dot^^2, v1_abs_2*v2_abs_2) && v1_abs_2 >= v2_abs_2;
	}
	else {
		return dot > 0 && dot^^2 == v1_abs_2*v2_abs_2 && v1_abs_2 >= v2_abs_2;
	}
}

@safe @nogc
nothrow pure bool protrude_frame (in P[] frame,in P[] shape) {
	import std.conv : to;
	P pos_sum;
	immutable gravity_point = pos_sum / cast(int)shape.length;
	if (crossing_number(gravity_point, shape) && !crossing_number(gravity_point, frame))
		return true;
	foreach (shape_vertex; shape) {
		if (!crossing_number(shape_vertex, frame))
			return true;
	}
	foreach (frame_vertex; frame) {
		if (crossing_number(frame_vertex, shape, false))
			return true;
	}
	for (int p_idx; p_idx < shape.length; ++p_idx) {
		for (int f_idx; f_idx < frame.length; ++f_idx) {
			if (judge_intersected (
				frame[f_idx], frame[(f_idx+1)%frame.length],
				shape[p_idx], shape[(p_idx+1)%shape.length]))
				return true;
		}
		pos_sum += shape[p_idx];
	}
	for (int p_idx1; p_idx1 < shape.length; ++p_idx1) {
		immutable p_idx2 = (p_idx1 + 1) % shape.length;
		immutable point_sum = shape[p_idx1] + shape[p_idx2];
		immutable int x1 = point_sum.x / 2;
		immutable int x2 = (point_sum.x + 1) / 2;
		immutable int y1 = point_sum.y / 2;
		immutable int y2 = (point_sum.y + 1) / 2;
		immutable P[4] ps = [
			P(x1, y1),
			P(x2, y1),
			P(x1, y2),
			P(x2, y2)
		];
		bool all_protruded = true;
		foreach (p; ps)
			if (crossing_number(p, frame))
				all_protruded = false;
		if (all_protruded)
			return true;
	}
	return false;
}
unittest {
	auto shape1 = [P(0,0), P(0,20), P(20,0)];
	auto frame1 = [P(0,0), P(20,0), P(20,40), P(0,40)];
	auto shape2 = [P(0,0), P(10,10), P(10,0)];
	auto frame2 = [P(0,0), P(-10,0), P(-10, 10)];
	auto frame3 = [P(0,0), P(10,0), P(10,20), P(0,20)];
	auto shape3 = [P(0,20), P(10,20), P(0,30)];
	auto shape4 = [P(0,0),P(10,0),P(0,10)];
	assert (!protrude_frame (frame1, shape1)); 
	assert (protrude_frame(frame2, shape2));
	assert (protrude_frame(frame3, shape3));
	assert (!protrude_frame(frame3, shape4));
}

@safe @nogc
nothrow pure size_t shape_idx (in size_t piece_idx, in size_t spin_level) {
	return piece_idx * 8 + spin_level;
}
