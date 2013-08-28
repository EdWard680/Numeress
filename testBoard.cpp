#include "board.h"

void display(const Board &b)
{
  Piece *temp;
  cout<<endl<<endl<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < b.getDim().y(); y++)
  {
    cout<<char(y+'A')<<' ';
    for(Uint8 x = 0; x < b.getDim().x(); x++)
    {
      if((temp = b.getCell(Point(x, y))->getPiece()) == NULL)
	cout<<"- ";
      else
	cout<<int(temp->getStrength())<<' ';
    }
    cout<<endl;      
  }
}

const Point retrievePoint(const Dimensions &d)
{
  int xTemp;
  char yTemp;
  cin.clear();
  cin.ignore(9001, '\n');
  cout<<endl;
  do
  {
    cout<<"Coordinates: ";
    scanf("%c%d", &yTemp, &xTemp);
    yTemp -= 'A';
  } while(xTemp < 0 || xTemp > d.x()-0.5 || yTemp < 0 || yTemp > d.y()-0.5);
  
  return Point(xTemp, yTemp);
}

const Uint8 retrievePiece(const Uint8 nPieces)
{
  int rtrn;
  cin.clear();
  cin.ignore(9001, '\n');
  do
  {
    cout<<"Piece Value: ";
    cin>>rtrn;
  } while(rtrn < 0 || rtrn > nPieces);
  return rtrn;
}

void playIns(Board &b, const SIDE_t s)
{
  Piece * next = b.nextIn(s);
  if(next == NULL)
    return;
  do
  {
    display(b);
    cout<<endl<<"Player "<<int(s)<<": Play in piece "<<int(next->getStrength())<<endl;
  } while(next = b.playIn(next, b.getCell(retrievePoint(b.getDim()))));
}


int main(void)
{
  Piece * winner;
  Board b(Dimensions(10.0, 10.0), 2, 5);
  for(SIDE_t s = 0; s < b.getSides(); s++)
    playIns(b, s);  //lol
  Uint8 turn;
  for(turn=0; !(winner = b.won()); turn++)
  {
    cout<<"Turn "<<(turn/2+1)<<endl;
    cout<<"Player "<<(turn%2)<<endl;
    display(b);
    playIns(b, turn%2);
    display(b);
    Piece *p;
    do
    {
      cout<<"Move Player "<<int(turn%2)<<endl;
      p = b.getPiece(retrievePiece(b.getNumPieces()), turn%2);
      cout<<endl<<"Valid Cells"<<endl;
      auto v = b.getValid(p);
      for(auto i = v.begin(); i != v.end(); i++)
      {
	cout<<char(b.getPoint(*i).y() + 'A')<<int(b.getPoint(*i).x())<<endl;
      }
      
      
      
    } while(!b.move(p, b.getCell(retrievePoint(b.getDim()))));
  }
  
  cout<<endl<<"Player "<<(turn-1)%2<<" capped"<<endl;
}