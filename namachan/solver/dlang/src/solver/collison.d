module procon28.solver.collison;

import procon28.basic_data;
import procon28.solver.data;

bool judge_intersected (in Segment seg1, in Segment seg2) {
	auto ta = (seg2.start.x - seg2.end.x) * (seg1.start.y - seg2.start.y) +  (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.start.x);
	auto tb = (seg2.start.x - seg2.end.x) * (seg1.end.y - seg2.start.y) +  (seg2.start.y - seg2.end.y) * (seg2.start.x - seg1.end.x);
	auto tc = (seg1.start.x - seg1.end.x) * (seg2.start.y - seg1.start.y) +  (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.start.x);
	auto td = (seg1.start.x - seg1.end.x) * (seg2.end.y - seg1.start.y) +  (seg1.start.y - seg1.end.y) * (seg1.start.x - seg2.end.x);
	return tc * td < 0 && ta * tb < 0;
}

bool judge_inclusion (in Point p,in Segment[] segments) {
	double angel_sum;
	foreach (seg; segments) {
		auto v1 = seg.start - p;
		auto v2 = seg.end - p;
		angel_sum += v1.angel(v2);
	}
	return angel_sum < 360.0;
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
