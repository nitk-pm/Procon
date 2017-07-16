module procon28.visualize.window;

import derelict.sdl2.sdl;
import std.exception;
import procon28.basic_data;
import procon28.solver.data;

static this () {
	DerelictSDL2.load();
	assert (SDL_Init (SDL_INIT_EVERYTHING) == 0);
}
static ~this() {
	SDL_Quit();
}

struct Color {
	ubyte r, g, b, a;
}

class Window {
	SDL_Window* win;
	SDL_Renderer* ren;

	this () {
		this (505, 325);
	}

	this (int w, int h) {
		win = SDL_CreateWindow (
			"solver",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			w,
			h,
			SDL_WINDOW_SHOWN
		);

		ren = SDL_CreateRenderer (
			win,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);
	}

	void renderShape (Shape shape, Color color = Color (255, 255, 255, 255)) {
		ren.SDL_SetRenderDrawColor (color.r, color.g, color.b, color.a);
		foreach (seg; shape) {
			ren.SDL_RenderDrawLine (seg.start.x, seg.start.y, seg.end.x, seg.end.y);
		}
		ren.SDL_RenderPresent();
	}
	
	void clear (Color color = Color (0, 0, 0, 255)) {
		ren.SDL_SetRenderDrawColor (color.r, color.g, color.b, color.a);
		ren.SDL_RenderClear ();
	}

	~this () {
		if (ren != null)
			ren.SDL_DestroyRenderer();
		if (win != null)
			win.SDL_DestroyWindow();
	}
}
