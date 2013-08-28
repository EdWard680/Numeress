#include "graphics.h"

int main(int argc, char **argv)
{
  NumeressDM NumeressDataManager;
  App myApp(&NumeressDataManager, SDL_INIT_EVERYTHING, 1366, 768, 32, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN);
  myApp.run();
  return 0;
}