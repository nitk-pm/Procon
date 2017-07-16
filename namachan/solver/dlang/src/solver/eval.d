module procon28.solver.eval;

import procon28.basic_data;
import procon28.solver.data;
import procon28.solver.collison;

float angle_and_points (Shape frame, Shape piece, Point pos) {
	float val = 0.0f;
	if (is_hit (frame, piece, pos)) return -float.infinity;
	foreach (piece_seg; piece) {
		foreach (frame_seg; frame) {
			if (piece_seg.start + pos == frame_seg.end || piece_seg.start + pos == frame_seg.start) {
				val += 1.0;
				if (piece_seg.slope == frame_seg.slope)
					val += 1.0;
			}
			if (piece_seg.end + pos == frame_seg.end || piece_seg.end + pos == frame_seg.start) {
				val += 1.0;
				if (piece_seg.slope == frame_seg.slope)
					val += 1.0;
			}
		}
	}
	return val;
}
