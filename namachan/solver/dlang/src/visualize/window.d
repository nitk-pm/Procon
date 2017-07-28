module procon28.visualize.window;

static import ar = armos;

import procon28.solver.data;
import procon28.basic_data;
import armos.math.vector;

class Visualizer : ar.app.BaseApp {
	ar.graphics.Mesh[] lines;

	override void setup () {
	}

	override void update () {
	}

	override void draw () {
		foreach (line; lines) {
			line.drawFill;
		}
	}

	void setLines (Segment[] segs) {
		foreach (seg; segs) {
			lines ~= ar.graphics.linePrimitive ([
				Vector3f (seg.start.x, seg.start.y, 0),
				Vector3f (seg.end.x  , seg.end.y  , 0),
			]);
		}
	}
}
