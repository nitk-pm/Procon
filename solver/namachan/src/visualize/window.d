module procon28.visualize.window;

import procon28.basic_data : Segment;

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
