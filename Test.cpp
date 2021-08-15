#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

class Program {
public:
  bool quit = false;
  TTF_Font *font;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Event event;

  Program() {
    std::cout << "Program Begin." << std::endl;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("SDL_ttf in SDL2", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 512, 512, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    font = TTF_OpenFont("assets/monofonto.ttf", 25);
    SDL_Color color = {255, 255, 255};
    surface = TTF_RenderText_Solid(font, "Welcome to TTF Test", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
  }

  ~Program() {
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    std::cout << "Program End." << std::endl;
  }

  void start() {
    while (!quit) {
      loop();
    }
  }

  void loop() {
    SDL_WaitEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      quit = true;
      break;
    }
    SDL_RenderClear(renderer);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect rect = {50, 50, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
  }
};

Program program;

#ifdef __EMSCRIPTEN__
void loop(void *arg) { program.loop(); }
#endif

int main(int argc, char *argv[]) {

  try {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&loop, NULL, -1, 1);
#else
    program.start();
#endif
    std::cout << "Program End." << std::endl;
  } catch (const std::string &e) {
    std::cout << e;
  }
  return 0;
}
