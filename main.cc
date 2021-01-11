#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define CENTER SDL_WINDOWPOS_CENTERED
#define SHOWN  SDL_WINDOW_SHOWN
#define ACCEL  SDL_RENDERER_ACCELERATED

void error(const std::string &msg)
{ std::cerr << msg << "\n"; exit(1); }

void error_sdl(const std::string &msg)
{ std::cerr << msg << " SDL_Error: " << SDL_GetError() << "\n"; exit(1); }

void error_img(const std::string &msg)
{ std::cerr << msg << " IMG_Error: " << IMG_GetError() << "\n"; exit(1); }

int main(int argc, char *argv[]) {
  if (argc < 2) { error("Invalid arguments: Missing file name"); }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) { error_sdl("SDL2 failed to initialize"); }

  const char *FILENAME = argv[1];
  SDL_Window *wnd = SDL_CreateWindow(FILENAME, CENTER, CENTER, 800, 600, SHOWN);
  if (wnd == NULL) error_sdl("SDL2 failed to create window");
  SDL_Renderer *rnd = SDL_CreateRenderer(wnd, -1, ACCEL);
  if (rnd == NULL) error_sdl("SDL2 failed to create renderer");

  SDL_Surface *tmpsrf = IMG_Load(FILENAME);
  if (tmpsrf == NULL) error_img("SDL2_image failed to load file");

  SDL_Texture *txt = SDL_CreateTextureFromSurface(rnd, tmpsrf);
  if (txt == NULL) error_sdl("SDL2 failed to create texture");

  int width, height;
  SDL_Rect src, dst;
  SDL_QueryTexture(txt, NULL, NULL, &width, &height);
  src.w = width; src.h = height;

  // Place everything at origin
  src.x = src.y = 0;

  SDL_DisplayMode display;
  SDL_GetDesktopDisplayMode(0, &display);
  while (width > display.w || height > display.h) { width /= 2; height /= 2; }
  SDL_SetWindowSize(wnd, width, height);
  SDL_SetWindowPosition(wnd, CENTER, CENTER);
  dst.w = width; dst.h = height;
  dst.x = dst.y = 0;

  SDL_RenderClear(rnd);
  SDL_RenderCopy(rnd, txt, &src, &dst);
  SDL_RenderPresent(rnd);

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT: quit = true; break;
      case SDL_KEYUP:
	if (e.key.keysym.sym == SDLK_q) quit = true;
	break;
      }
    }
  }
  
  return 0;
}
