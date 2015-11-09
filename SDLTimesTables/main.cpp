#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

enum { Width = 1920, Height = 1080 };

struct Color {
	
	int r = 255;
	int g = 255;
	int b = 255;

	void update(){
		
		static std::mt19937 rand(time(nullptr));
		std::uniform_int_distribution<int> dist(-1, 1);
		
		int newR = r + dist(rand);
		int newG = g + dist(rand);
		int newB = b + dist(rand);
		
		r = std::min(std::max(0, newR), 255);
		g = std::min(std::max(0, newG), 255);
		b = std::min(std::max(0, newB), 255);

	}
};

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cerr << "SDL_Init error!\n";
		return -1;
	}
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_CreateWindowAndRenderer(Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN, &window, &renderer);
	
	Color color;
	bool running = true;
	auto k = 1.0f;

	while (running){
		
		k += 0.01f;

		color.update();

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
		
		for (auto i = 0; i < 200; ++i){
			auto x1 = cos(2 * M_PI * i / 200);
			auto y1 = sin(2 * M_PI * i / 200);
			auto x2 = cos(2 * M_PI * (i * k) / 200);
			auto y2 = sin(2 * M_PI * (i * k) / 200);
			SDL_RenderDrawLine(renderer,
				(x1 + 1.0f) * Height / 2 + (Width - Height) / 2, (y1 + 1.0f) * Height / 2,
				(x2 + 1.0f) * Height / 2 + (Width - Height) / 2, (y2 + 1.0f) * Height / 2);
		}

		SDL_RenderPresent(renderer);

		SDL_Event keyevent; 
		while (SDL_PollEvent(&keyevent)){
			if (keyevent.type == SDL_KEYDOWN && keyevent.key.keysym.sym == SDLK_ESCAPE){
				running = false;
			}
		}

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
		
	return 0;
}
