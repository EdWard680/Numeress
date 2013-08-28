#include "graphics.h"

const int NumeressDM::init()
{
  playingIn = false;
  turn = 0;
  select = NULL;
  if(background.load("sprites/background.jpg") < 0)
    return -1;
  for(SIDE_t s = 0; s < board.getSides(); s++)
  {
    for(Uint8 i = 0; i < board.getNumPieces(); i++)
    {
      char fileName[255];
      sprintf(fileName, "sprites/pieces/%i_%i.gif", (int)s, (int)i+1);
      if(pieceSprites[s][i].load(fileName) < 0)
	return -1;
    }
  }
  for(Uint8 i = 0; i < 5; i++)
  {
    if(i == 2)
      tiles[2] = tiles[0];
    else
    {
      char fileName[255];
      sprintf(fileName, "sprites/cells/%i.gif", (int)i);
      if(tiles[i].load(fileName) < 0)
	return -1;
    }
  }
  for(SIDE_t s = 0; s < board.getSides(); s++)
  {
    char fileName[255];
    sprintf(fileName, "sprites/flags/%i.gif", (int)s);
    if(flags[s].load(fileName) < 0)
      return -1;
  }
  
  return 0;
}

GameLoop * const NumeressDM::firstGameLoop(App * parent)
{
  if(currentGL == NULL)
    currentGL = new Numeress(this, parent);
  return currentGL;
}

