#include "graphics.h"

const int Numeress::init()
{
  Running = true;
  Board &b = MyDM->board;
  b.playIn(b.getPiece(1, 0), b.getCell(Point(4, 0)));
  b.playIn(b.getPiece(1, 1), b.getCell(Point(5, 9)));
  b.playIn(b.getPiece(2, 0), b.getCell(Point(8, 0)));
  b.playIn(b.getPiece(2, 1), b.getCell(Point(1, 9)));
  b.playIn(b.getPiece(3, 0), b.getCell(Point(2, 0)));
  b.playIn(b.getPiece(3, 1), b.getCell(Point(7, 9)));
  b.playIn(b.getPiece(4, 0), b.getCell(Point(0, 0)));
  b.playIn(b.getPiece(4, 1), b.getCell(Point(9, 9)));
  b.playIn(b.getPiece(5, 0), b.getCell(Point(6, 0)));
  b.playIn(b.getPiece(5, 1), b.getCell(Point(3, 9)));
  return 0;
}

const int Numeress::loop()
{
  if(MyDM->board.won() != NULL)
  {
    Running = false;
    cout<<"Side "<<(int)MyDM->board.won()->getSide()<<" won"<<endl;
    return 1;
  }
  MyDM->available.reset();
  Cell *first = MyDM->board.getCell(Point(0, 0));
  if(MyDM->playingIn)
  {
    if((MyDM->select = MyDM->board.nextIn(MyDM->board.whosTurn())) == NULL)
      MyDM->playingIn = false;
  }
  
  if(MyDM->select == NULL)
  {
    for(Uint8 i = 0; i < MyDM->board.getNumPieces(); i++)
    {
      Cell *pieceLoc = MyDM->board.getPiece(i+1, MyDM->board.whosTurn())->getCell();
      if(pieceLoc == NULL)
	continue;
      MyDM->available[pieceLoc - first] = 1;
    }
  }
  else
  {
    auto valid = MyDM->board.getValid(MyDM->select);
    for(auto i = valid.begin(); i != valid.end(); i++)
    {
      MyDM->available[*i - first] = 1;
    }
  }
  return 0;
}

const int Numeress::render() const
{
  MyDM->background.draw(MyDM->Display, Point(0, 0));
  Cell *first = MyDM->board.getCell(Point(0, 0));
  Point start(100, 100);
  for(Uint8 i = 0; i < MyDM->available.size(); start.y()+=MyDM->tiles[0]->h)
  {
    Uint8 j;
    for(start.x(100), j=i; i < j + MyDM->board.getDim().x(); i++, start.x()+=MyDM->tiles[0]->w)
    {
      MyDM->tiles[MyDM->available[i]? 4:(((first + i)->getSide() << 1) | (first + i)->getType())].draw(MyDM->Display, start);
      Piece *p = (first + i)->getPiece();
      if(p)
      {
	MyDM->pieceSprites[p->getSide()][p->getValue() - 1].draw(MyDM->Display, start);
	if(p->hasFlag() >= 0)
	{
	  MyDM->flags[p->hasFlag()].draw(MyDM->Display, start);
	}
      }
    }
  }
  
  MyDM->tiles[((MyDM->board.whosTurn()) << 1) | 1].draw(MyDM->Display, Point(MyDM->Display->w - 200, 100));
  if(MyDM->select)
    MyDM->pieceSprites[MyDM->select->getSide()][MyDM->select->getValue() - 1].draw(MyDM->Display, Point(MyDM->Display->w - 200, 100));
  
  SDL_Flip(MyDM->Display);
  return 0;
}

const int Numeress::cleanup()
{
  return 0;
}

const EVENT_RESULT Numeress::exited()
{
  Running = false;
  return EVENT_SUCCESS;
}

const EVENT_RESULT Numeress::mouseButtonPressed(const Uint8 button, const int mX, const int mY)
{
  if(button != SDL_BUTTON_LEFT)
    return EVENT_SUCCESS;
  
  if(mX >= 100 + MyDM->board.getDim().x()*MyDM->tiles[0]->w || mX <= 100)
    return EVENT_SUCCESS;
  
  if(mY >= 100 + MyDM->board.getDim().y()*MyDM->tiles[0]->h || mY <= 100)
    return EVENT_SUCCESS;
  
  Point selectedPoint(int((mX - 100)/MyDM->tiles[0]->w), int((mY - 100)/MyDM->tiles[0]->h));
  Cell * selectedCell = MyDM->board.getCell(selectedPoint);
  Cell *first = MyDM->board.getCell(Point(0, 0));
  if(MyDM->available[selectedCell - first])
  {
    if(MyDM->select != NULL)
    {
      if(MyDM->playingIn)
      {
	MyDM->board.playIn(MyDM->select, selectedCell);
      }
      else
      {
        MyDM->board.move(MyDM->select, selectedCell);
        MyDM->playingIn = true;
      }
    }
    else if(selectedCell->getPiece() != NULL)
    {
      if(MyDM->select == NULL)
        MyDM->select = selectedCell->getPiece();
    }
  }
  else if(MyDM->select != NULL)
  {
    if(selectedCell == MyDM->select->getCell())
      MyDM->select = NULL;
  }
  
  return EVENT_SUCCESS;
}

const EVENT_RESULT Numeress::resized(const int W, const int H)
{
  MyDM->Display = SDL_SetVideoMode(W, H, 32, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_RESIZABLE);
  return EVENT_SUCCESS;
}

const EVENT_RESULT Numeress::keyPressed(const SDLKey sym, const SDLMod mod, const Uint16 unicode)
{
  switch(sym)
  {
  case SDLK_ESCAPE:
    resized(MyDM->Display->w, MyDM->Display->h);
    break;
  case SDLK_q:
    exited();
    break;
  default:
    break;
  }
}
