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
	Point[] frame_buf;
	foreach (f_idx; 0..frame.length) {
		bool point_appended;
		//始点とピースの頂点の何処かが衝突していた場合追加
		foreach (p_idx,piece_p; piece) {
			if (frame[f_idx] == piece_p || judge_on_line(frame[f_idx], piece_p, piece[(p_idx+1)%piece.length])) {
				frame_buf ~= Point(frame[f_idx], true, false);
				point_appended = true;
				break;
			}
		}
		//線分の間にピースの頂点があった場合追加
		foreach (p_idx,piece_p; piece) {
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
		import std.stdio;
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
nothrow pure bool judge_cross_horizontal_line (in P p, in P start, in P end) {
	return (start.y - p.y) * (end.y - p.y) < 0;
}

@safe @nogc
nothrow pure float on_right_side (in P start, in P end, in P p) {
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
pure nothrow crossing_number (in P p, in P[] shape, in bool include_on_line = true) {
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
		else if (judge_cross_horizontal_line(p, vertex1, vertex2) && (vertex1.x > p.x || vertex2.x > p.x)) {
			++cross_cnt;
		}
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
pure nothrow P[][] merge(in P[] frame, in P[] piece) {
	import std.range : retro, array;
	Point[] frame_buf;
	Point[] piece_buf;
	frame_buf = insert_junction(frame, piece);
	piece_buf = insert_junction(piece, frame).retro.array;
	P[][] shapes;
	foreach (frame_point; frame_buf) {
		if (!frame_point.is_junction) continue;
		P[] take (in Point start, ref Point[] looking, ref Point[] out_of_looking, in Point before_start) {
			P[] shape;
			auto idx = find_junction (looking, start.pos);
			shape ~= looking[idx].pos;
			looking[idx].visited = true;
			for(;;) {
				idx = (idx + 1) % looking.length;
				// 中止条件 : 2個前のポイントに戻る(==無限ループに陥ったなら中止)
				if (before_start.pos == looking[idx].pos)
					return shape;
				// 終了条件 : 始めた場所に戻っていたら先読み
				if (looking[idx].pos == frame_point.pos) {
					auto out_of_looking_idx = find_junction (out_of_looking, looking[idx].pos);
					auto lookahead = out_of_looking[(out_of_looking_idx+1)%out_of_looking.length];
					//先読みした頂点が衝突しているか、訪れたことがあった場合は閉じていると見なして返す
					if (lookahead.is_junction || lookahead.visited)
						return shape;
				}
				//フレーム遷移
				if (looking[idx].is_junction)
					return shape ~ take(looking[idx], out_of_looking, looking, start);

				looking[idx].visited = true;
				shape ~= looking[idx].pos;
			}
		}
		auto not_exist_point = Point (P(int.max, int.max), false, false);
		auto shape = take (frame_point, piece_buf, frame_buf, not_exist_point);
		if (shape.length > 2)
			shapes ~= [shape];
	}
	return shapes;
}
unittest {
	auto s1 = [P(0,0),P(10,0),P(10,20),P(0,20)];
	auto s2 = [P(0,0),P(10,0),P(0,10)];
	assert (merge (s1, s2) == [[P(0,10),P(10,0),P(10,20),P(0,20)]]);
	assert (merge (s1, s1) == []);
	
	auto square = [P(0,0),P(10,0),P(10,10),P(0,10)];
	auto triangle1 = [P(0,5),P(5,3),P(5,7)];
	assert (merge (square,triangle1) == [[P(0, 5), P(5, 7), P(5, 3), P(0, 5), P(0, 0), P(10, 0), P(10, 10), P(0, 10)]]);
	auto triangle2 = [P(0,5),P(5,0),P(5,7)];
	assert (merge (square,triangle2) == [[P(5, 0), P(0, 5), P(0, 0)], [P(0, 5), P(5, 7), P(5, 0), P(10, 0), P(10, 10), P(0, 10)]]);
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
nothrow pure bool judge_on_line (in Pos p, in P start, in P end) {
	immutable v1 = end - start;
	immutable v2 = p - start;
	immutable v1_abs_2 = (v1.x^^2+v1.y^^2);
	immutable v2_abs_2 = (v2.x^^2+v2.y^^2);
	immutable dot = v1 * v2;
	//内積が|v1|*|v2|かつ内積が正(角度が同じ)で、v2の長さがv1より短い(線分の中に含まれている)場合は線の上にあるとみなす。
	return dot > 0 && dot^^2 == v1_abs_2*v2_abs_2 && v1_abs_2 >= v2_abs_2;
}

@safe @nogc
nothrow pure bool protrude_frame (in P[] frame,in P[] shape) {
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
		if (!crossing_number((shape[p_idx] + shape[(p_idx+1)%shape.length])/2, frame))
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
