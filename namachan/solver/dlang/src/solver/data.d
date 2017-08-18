module procon28.solver.data;

import armos.math.vector;

import procon28.basic_data : Segment, Shape;

import std.algorithm.iteration : map;
import std.algorithm.sorting : sort;
import std.range : array, zip;

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

///線分が重複しているかどうかを判定
bool judge_overlap(in Segment seg1, in Segment seg2) {
	auto ta = (seg2.start.x - seg2.end.x) * (seg1.start.y - seg2.start.y) +  (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.start.x);
	auto tb = (seg2.start.x - seg2.end.x) * (seg1.end.y - seg2.start.y) +  (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.end.x);
	auto tc = (seg1.start.x - seg1.end.x) * (seg2.start.y - seg1.start.y) +  (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.start.x);
	auto td = (seg1.start.x - seg1.end.x) * (seg2.end.y - seg1.start.y) +  (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.end.x);
	return tc * td <= 0 && ta * tb <= 0;

}
unittest {
	auto seg1 = Segment(Vector2i(0,0), Vector2i(4,4));
	auto seg2 = Segment(Vector2i(1,1), Vector2i(2,2));
	assert (judge_overlap(seg1, seg2));
}

///線分のxorを取る。重複していないものには使えない
Segment[2] xor (in Segment seg1, in Segment seg2) {
	auto xs = [seg1.start.x, seg1.end.x, seg2.start.x, seg2.end.x].dup.sort!"a>b".array;
	int[] ys;
	if (seg1.line.slope.denom > 0)
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
}

Shape move (in Shape shape,in Vector2i pos) {
	auto copy = shape.dup;
	foreach (ref seg; copy) {
		seg.start += pos;
		seg.end += pos;
	}
	return copy;
}

///ShapeとShapeをマージ。重複部分は取り除かれる
Shape merge (in Shape x, in Shape y) {
	auto x_hit = new bool[x.length];
	auto y_hit = new bool[y.length];
	Segment[] splited;
	foreach (x_idx, x_seg; x) {
		foreach (y_idx, y_seg; y) {
			if (x_seg.line == y_seg.line && judge_overlap (x_seg, y_seg)) {
				x_hit[x_idx] = true;
				y_hit[y_idx] = true;
				splited ~= xor(x_seg, y_seg);
			}
		}
	}
	Shape mixed = splited;
	foreach (x_idx, x_seg; x) {
		if (!x_hit[x_idx])
			mixed ~= x_seg;
	}
	foreach (y_idx, y_seg; y) {
		if (!y_hit[y_idx])
			mixed ~= y_seg;
	}
	int[Segment] cnt;
	foreach (seg; mixed) {
		++cnt[seg];
	}
	Shape result;
	foreach (seg; mixed) {
		if (cnt[seg] < 2 && seg.start != seg.end)
			result ~= seg;
	}
	return result;
}
unittest {
	import procon28.basic_data : S, P;
	auto s1 = [P(0,0), P(10,0),P(0, 10)].vertexies2shape;
	auto s2 = [P(0,0), P(10,0),P(0, 10)].vertexies2shape;
	assert (merge(s1,s2) == []);
}