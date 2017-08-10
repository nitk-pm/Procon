module procon28.visualize.window;

static import ar = armos;

import armos.math.vector : Vector3f;

import procon28.basic_data : Segment;

///armosを使って作られたデバッグ用簡易ビジュアライザ
class Visualizer : ar.app.BaseApp {
private:
	ar.graphics.Mesh[] lines;
public:
	override void setup () {
	}

	override void update () {
	}

	override void draw () {
		foreach (line; lines) {
			line.drawFill;
		}
	}

///描画する線分の集合をセット
	void addLines (Segment[] segs) {
		foreach (seg; segs) {
			lines ~= ar.graphics.linePrimitive ([
				Vector3f (seg.start.x, seg.start.y, 0),
				Vector3f (seg.end.x  , seg.end.y  , 0),
			]);
		}
	}
}
