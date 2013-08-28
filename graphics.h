#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "GameControl/App.h"
#include "GameControl/DataManager.h"
#include "SurfaceX/DoubleSurf.h"

#include "board.h"

#include <bitset>

using namespace Space2D;
using namespace SurfaceX;
using namespace GameControl;

class NumeressDM: public DataManager
{
public:
  SafeSurf pieceSprites[2][5];
  SafeSurf tiles[5];
  SafeSurf flags[2];
  bitset<100> available;
  Piece *select;
  bool playingIn;
  SafeSurf background;
  Board board;
  
  Uint8 turn;  
  
public:
  NumeressDM() {currentGL = NULL;};
  const int init();
  GameLoop * const firstGameLoop(App * parent);
  
  ~NumeressDM() {
    delete currentGL;
  };
};

class Numeress: public GameLoop
{
  NumeressDM *MyDM;
protected:
  const int init();
  const int loop();
  const int render() const;
  const int cleanup();
  
  const EVENT_RESULT exited();
  const EVENT_RESULT mouseMove(const int mX, const int mY, const int relX, const int relY,
			      const bool lDown, const bool rDown, const bool mDown) {return EVENT_SUCCESS;};
  const EVENT_RESULT mouseButtonPressed(const Uint8 button, const int mX, const int mY);
  const EVENT_RESULT resized(const int W, const int H);
public:
  Numeress(NumeressDM * const dataM, GameLoop * const parent): GameLoop(dataM, parent) {
    MyDM = dataM;
  };
  Numeress(NumeressDM &dataM, GameLoop &parent): GameLoop(&dataM, &parent) {};
};

#endif