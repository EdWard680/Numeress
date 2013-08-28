#include "graphics.h"

int main(int argc, char **argv)
{
  NumeressDM NumeressDataManager;
  App myApp(&NumeressDataManager, SDL_INIT_EVERYTHING, 950, 768, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
  myApp.run();
  return 0;
}