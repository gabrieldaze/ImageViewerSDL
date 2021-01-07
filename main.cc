#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void log(const std::string &msg) {
  std::cerr << msg << std::endl;
}

void log_sdl(const std::string &msg) {
  std::cerr << msg << " SDL_Error: " << SDL_GetError() << std::endl;
}

void log_img(const std::string &msg) {
  std::cerr << msg << " IMG_Error: " << IMG_GetError() << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) { log("Missing file name"); exit(1); }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) { log_sdl("SDL2 failed to initialize"); exit(1); }

  const char *FILENAME = argv[1];
  SDL_Window *wnd = SDL_CreateWindow(FILENAME,
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     800, 600,
                                     SDL_WINDOW_SHOWN);
  if (wnd == nullptr) { log_sdl("SDL2 failed to create window"); exit(1); }
  SDL_Renderer *rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
  if (rnd == nullptr) { log_sdl("SDL2 failed to create renderer"); exit(1); }
  SDL_Surface *tmpsrf = IMG_Load(FILENAME);
  if (tmpsrf == nullptr) { log_img("SDL2_image failed to load file"); exit(1); }
  SDL_Texture *image = SDL_CreateTextureFromSurface(rnd, tmpsrf);
  if (image == nullptr) { log_sdl("SDL2 failed to create image texture"); exit(1); }
  SDL_FreeSurface(tmpsrf);

  int width, height;
  SDL_QueryTexture(image, nullptr, nullptr, &width, &height);
  SDL_Rect src, dst;
  src.w = width; src.h = height;
  src.x = dst.x = src.y = dst.y = 0;
  SDL_DisplayMode display;
  SDL_GetDesktopDisplayMode(0, &display);
  
  while (width > display.w || height > display.h) { width /= 2; height /= 2; }
  dst.w = width; dst.h = height;

  SDL_SetWindowSize(wnd, width, height);
  SDL_SetWindowPosition(wnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  SDL_RenderClear(rnd);
  SDL_RenderCopy(rnd, image, &src, &dst);
  SDL_RenderPresent(rnd);

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true; break;
        case SDL_KEYUP:
          if (event.key.keysym.sym == SDLK_q) quit = true;
          break;
      }
    }
  }

  SDL_DestroyRenderer(rnd);
  SDL_DestroyWindow(wnd);
  SDL_Quit();
  return 0;
}
