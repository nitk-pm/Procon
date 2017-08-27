module procon28.visualize.window;

static import ar = armos;

import armos.math.vector : Vector3f;

import procon28.basic_data : Segment;

static this () {
	DerelictSDL2.load();
	SDL_Init(SDL_INIT_EVERYTHING);
}

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
	void addLines (in Segment[] segs) {
		foreach (seg; segs) {
			lines ~= ar.graphics.linePrimitive ([
				Vector3f (seg.start.x, seg.start.y, 0),
				Vector3f (seg.end.x  , seg.end.y  , 0),
			]);
		}
	}
}

void launch(in Segment[] segs) {
	auto win = new Visualizer;
	win.addLines (segs);
	ar.app.run(win);
}

import derelict.sdl2.sdl;

class Window {
	SDL_Window *win;
	SDL_Renderer *ren;
	bool live;
	bool dead;
	this () {
		import std.string;
		SDL_CreateWindowAndRenderer (500,500, SDL_WINDOW_SHOWN, &win, &ren);
		live = true;
	}
	void show (in Segment[] segs) {
		if (live){
			SDL_SetRenderDrawColor (ren, 10, 10, 10, 255);
			SDL_RenderClear (ren);
			SDL_SetRenderDrawColor (ren, 255, 255, 255, 255);
			foreach (seg; segs) {
				SDL_RenderDrawLine (ren, seg.start.x, seg.start.y, seg.end.x, seg.end.y);
			}
			SDL_RenderPresent(ren);
			SDL_Event e;
			bool run = true;
			while (run) {
				while(SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT) {
						live = false;
						run = false;
						break;
					}
					if (e.type == SDL_KEYDOWN) {
						run = false;
						break;
					}
				}
				SDL_Delay(10);
			}
		}
		if (!live && !dead) {
			quit ();
			dead = true;
		}
	}

	void quit () {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
	}
}
