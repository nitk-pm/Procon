module procon28.solver.eval;

import procon28.basic_data : P, S, Shape;
import procon28.solver.collison : is_hit;
import procon28.solver.data : move, vertexies2shape;

/++
 + 線分の角度の一致度と重複した点の数で評価する評価関数
 + 衝突していた場合は-float.inifinityを返す
 +/
float angle_and_points (Shape frame, Shape piece, Point pos) {
	float val = 0.0f;
	if (is_hit (frame, piece))
		return -float.infinity;
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
