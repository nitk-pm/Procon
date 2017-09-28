module procon28.visualize.window;

import procon28.data : P, Situation;
import procon28.solver.geometry : move;

import derelict.sdl2.sdl;

static this () {
	DerelictSDL2.load();
	SDL_Init(SDL_INIT_EVERYTHING);
}

class Window {
	SDL_Window *win;
	SDL_Renderer *ren;
	bool live;
	bool dead;
	@trusted
	nothrow this () {
		import std.string;
		SDL_CreateWindowAndRenderer (500,500, SDL_WINDOW_SHOWN, &win, &ren);
		live = true;
	}

	@trusted
	nothrow void show (in P[][][] pieces, in P[][] merged, in P[] moved, in Situation acc) {
		import std.algorithm.iteration : map;
		import std.range : array;
		show (
			merged
			~ moved
			~ acc.shapes.map!(a => pieces[a.piece_idx][a.spin_level].move(a.x, a.y).move(0,100)).array
			~ moved.move(0,100));
	}

	@trusted
	nothrow void show (in P[][] shapes) {
		if (live){
			SDL_SetRenderDrawColor (ren, 10, 10, 10, 255);
			SDL_RenderClear (ren);
			SDL_SetRenderDrawColor (ren, 255, 255, 255, 255);
			foreach (shape; shapes) {
				if (shape.length >= 2) {
					SDL_Point[256] sdl_points;
					foreach (idx, pt; shape)
						sdl_points[idx] = SDL_Point(pt.x + 10, pt.y + 10);
					sdl_points[shape.length] = SDL_Point(shape[0].x+10, shape[0].y+10);
					SDL_RenderDrawLines(ren, cast(SDL_Point*)sdl_points, cast(int)shape.length+1);
				}
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

	@trusted
	nothrow void quit () {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
	}
}
