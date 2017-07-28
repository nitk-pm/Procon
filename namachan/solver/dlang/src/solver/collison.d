module procon28.solver.collison;

import procon28.basic_data;
import procon28.solver.data;

import armos.math.vector;

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

bool judge_inclusion (in Point p,in Segment[] segments) {
	double angel_sum = 0.0;
	foreach (seg; segments) {
		auto v1 = seg.start - p;
		auto v2 = seg.end - p;
		angel_sum += v1.angle(v2);
	}
	return angel_sum < 360.0;
}
unittest {
	auto shape = [
		Segment(Vector2i (0, 0), Vector2i(0, 2)),
		Segment(Vector2i (0, 2),Vector2i (2, 2)),
		Segment(Vector2i (2, 2), Vector2i(2, 0)),
		Segment(Vector2i (2, 0), Vector2i(0, 0))
	];
	auto pt1 = Vector2i (1, 1);
	auto pt2 = Vector2i (2, 2);
	assert (judge_inclusion(pt1, shape));
	assert (!judge_inclusion(pt2, shape));
}

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
