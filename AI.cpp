#include "AI.h"
#include "boardTree.h"

const EXPECTED_VALUE AI::evaluate(const Board &b)
{
  EXPECTED_VALUE teams[2];// = new EXPECTED_VALUE [b.getSides()];
  if(b.won() != NULL)
  {
    if(b.won()->getSide() == side)
      return 100000.0;
    else
      return -100000.0;
  }
  for(SIDE_t s = 0; s < b.getSides(); s++)
  {
    vector<Piece> &pieces = b.getPieceList(s);
    for(auto i = pieces.begin(); i != pieces.end(); i++)
    {
      Uint8 pVal = i->getValue();
      auto validLocations = b.getValid(&*i);
      for(auto c = validLocations.begin(); c != validLocations.end(); c++)
      {
	Point loc = b.getPoint(i->getCell());
	if(i->getCell() == NULL)
	{
	  loc = (s == 0? Point(-1, 0):Point(10, 9));
	}
	Point dest = b.getPoint(*c);
	EXPECTED_VALUE multiplier = 1.0;
	multiplier *= (i->hasFlag() >= 0? 5.0:1.0) * ((i->getStrength()+5.0)/2.0);
	if((i->hasFlag() >= 0) + s == 1)
	{
	  multiplier *= (Y_t(s? 10:20) - dest.y())/((3.0-pVal < 0.0? pVal-3.0:3.0-pVal)+1.0);
	}
	else
	{
	  multiplier *= (dest.y()+Y_t(s? 10:1))/((3.0-pVal < 0.0? pVal-3.0:3.0-pVal)+1.0);
	}
	if((*c)->getSide() != i->getSide() && (*c)->getType() == Cell::CAP_ZONE)
	{
	  multiplier *= 2;
	}
	teams[s] += multiplier/3;
      }
    }
  }
  
  EXPECTED_VALUE ourSide;
  EXPECTED_VALUE sum_a_ther_side = 0.0;
  
  for(Uint8 t = 0; t < b.getSides(); t++)
  {
    if(t == side)
    {
      ourSide = teams[t];
    }
    else
    {
      sum_a_ther_side += teams[t];
    }
  }
  
  return ourSide - sum_a_ther_side/(b.getSides()-1);
}

/*
const EXPECTED_VALUE AI::findBestMove(BoardNode &n)
{
  EXPECTED_VALUE evaluation = evaluate(n.b);
  
#ifdef DEBUG
  const char * const merp = n.b.toString();
  cout<<endl<<endl<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < n.b.getDim().y(); y++)
  {
    cout<<char('A'+y)<<' '<<(merp + y*20)<<endl;
  }
  cout<<"Value for current game: "<<evaluation<<endl;
#endif
  
  EXPECTED_VALUE rtrn=0.0;
  Node next = n;
  vector<EXPECTED_VALUE> children;
  if(n.b.won() || (n.prev - evaluation > -12 && n.prev - evaluation < 12 && n.p != NULL && n.c != NULL) || n.depth == 0)
  {
    return evaluation;
  }
  
  vector<Piece> &pieces = next.b.getPieceList(next.b.whosTurn());
  for(auto i = pieces.begin(); i != pieces.end(); i++)
  {
    if(next.p = next.b.nextIn(next.b.whosTurn()))
    {
      if(next.p != &*i)
        continue;
    }
    auto validLocations = next.b.getValid(&*i);
    for(auto c = validLocations.begin(); c != validLocations.end(); c++)
    {
      next.p = &*i;
      next.c = *c;
      next.prev = evaluation;
      if(next.b.nextIn(next.b.whosTurn()))
      {
	if(next.b.playIn(next.p, next.c))
	  continue;
	next.depth = n.depth;
      }
      else
      {
	if(!next.b.move(next.p, next.c))
	  continue;
	next.depth = n.depth - 1;
      }
      EXPECTED_VALUE child = findBestMove(next);
      rtrn += child;
      if((n.b.whosTurn()==0? (child > n.prev):(child < n.prev)) || children.size() == 0)
      {
	n.prev = child;
	n.p = n.b.getPiece(i->getValue(), i->getSide());
	n.c = n.b.getCell(next.b.getPoint(*c));
      }
      children.push_back(child);
      /*if(children.size() == 0)
	children.push_back(child);
      else if(child == 0.0 || rtrn == 0.0)
	children.push_back(0.0);
      else
      {
	children.pus
        if(n.b.whosTurn()==0? (child > 0.0):(child < 0.0))
          children.push_back(child*(child*(children.size()/rtrn)));
        else
	  children.push_back((children.size()/rtrn));
      }
    }
  }
  
  EXPECTED_VALUE sum = 0.0;
  for(auto chld = children.begin(); chld != children.end(); chld++)
  {
    if(n.b.whosTurn()==0? (*chld > rtrn/children.size()):(*chld < rtrn/children.size()))
      sum += *chld + *chld - rtrn/children.size();
    else
      sum += *chld;
  }
  
  return sum/children.size();
}

*/

const Move AI::getBestMove(const int depth)
{
  //Node rtrn = {*board, depth, NULL, NULL, quiEvaluate(*board)};
  Board temp(*game);
  Move rtrn(alphaBeta(temp, -10000000.0, 10000000.0, depth).bestMove, game);
  return rtrn;
}

/*
const EXPECTED_VALUE AI::quiesce(ABNode &n)
{
  EXPECTED_VALUE standPat = -quiEvaluate(n.b), score;
#if DEBUG
  const char * const merp = n.b.toString();
  cout<<endl<<endl;
  
  for(Uint8 space = 6 - n.depth; space > 0; space--)
    cout<<' ';
  cout<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < n.b.getDim().y(); y++)
  {
    for(Uint8 space = 6 - n.depth; space > 0; space--)
      cout<<' ';
    cout<<char('A'+y)<<' '<<(merp + y*20)<<endl;
  }
  
  for(Uint8 space = 6 - n.depth; space > 0; space--)
    cout<<' ';
  cout<<"Value for current game: "<<standPat<<endl;
#endif
  if(n.b.won() != NULL || n.depth < 1)
    return -standPat;
  
  if(standPat >= n.beta)
    return n.beta;
  
  if(n.alpha < standPat)
    n.alpha = standPat;
  
  ABNode next {n.b, n.depth, NULL, NULL, n.b.nextIn(n.b.whosTurn())==NULL? -n.beta:n.alpha, n.b.nextIn(n.b.whosTurn())==NULL? -n.alpha:n.beta};
  
  vector<Piece> &pieces = n.b.getPieceList(n.b.whosTurn());
  for(auto i = pieces.begin(); i != pieces.end(); i++)
  {
    if(next.p = n.b.nextIn(n.b.whosTurn()))
    {
      if(next.p != &*i)
        continue;
    }
    auto validLocations = n.b.getValid(&*i);
    for(auto c = validLocations.begin(); c != validLocations.end(); c++)
    {
      next.b = n.b;
      next.p = next.b.getPiece(i->getValue(), i->getSide());
      next.c = next.b.getCell(n.b.getPoint(*c));
      next.alpha = n.b.nextIn(n.b.whosTurn())==NULL? -n.beta:n.alpha;
      next.beta = n.b.nextIn(n.b.whosTurn())==NULL? -n.alpha:n.beta;
      next.depth = n.depth - 1;
      if(next.b.nextIn(next.b.whosTurn()))
      {
	if(next.b.playIn(next.p, next.c))
	  continue;
      }
      else
      {
	if(!next.b.move(next.p, next.c))
	  continue;
      }
      
      score = -quiesce(next);
      
      if(score >= n.beta)
	return n.beta;
      if(score > n.alpha)
      {
	n.alpha = -(next.beta = -score);
	n.p = &*i;
	n.c = *c;
      }
      
    }
  }
  
  return n.alpha;
}
*/


const SEARCH_RETURN AI::alphaBeta(Board& board, EXPECTED_VALUE alpha, EXPECTED_VALUE beta, int depth)
{
#ifdef DEBUG
  string merp(board.toString());
  cout<<endl<<endl;
  for(Uint8 space = 6 - depth; space > 0; space--)
    cout<<' ';
  cout<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < board.getDim().y(); y++)
  {
    for(Uint8 space = 6 - depth; space > 0; space--)
      cout<<' ';
    cout<<char('A'+y)<<' ';
    for(Uint8 x=0; merp[y*20+x] != '\n'; x++)
    {
      cout<<merp[y*20+x];
    }
    cout<<endl;
  }
  for(Uint8 space = 6 - depth; space > 0; space--)
    cout<<' ';
  cout<<"Value for current game: "<<evaluate(board)<<endl;
#endif
  
  if(depth == 0 || board.won() != NULL)
  {
    return SEARCH_RETURN(quiEvaluate(board) - depth, Move(NULL, NULL, NULL));
    //return quiesce(n);
  }
  
  EXPECTED_VALUE score, bestScore = -10000000;
  Move ret(NULL, NULL, NULL);
  
  //BoardNode next {n.b, n.depth, NULL, NULL, n.b.nextIn(n.b.whosTurn())==NULL? -n.beta:n.alpha, n.b.nextIn(n.b.whosTurn())==NULL? -n.alpha:n.beta};
  vector<Piece> &pieces = board.getPieceList(board.whosTurn());
  
  Move next(NULL, NULL, &board);
  
  for(auto i = pieces.begin(); i != pieces.end(); i++)
  {
    if(next.setPiece(board.nextIn(board.whosTurn())))
    {
      if(next.getPiece() != &*i)
        continue;
    }
    auto validLocations = board.getValid(&*i);
    for(auto c = validLocations.begin(); c != validLocations.end(); c++)
    {
      next.setPiece(&*i);
      next.setCell(*c);
      if(!next.doMove())
	continue;
      if(board.nextIn(board.whosTurn()))
      {
	score = alphaBeta(board, alpha, beta, depth-1).val;
      }
      else
      {
	score = -alphaBeta(board, -beta, -alpha, depth-1).val;
      }
      
      next.undoMove();
      
#ifdef DEBUG
  if(merp != string(board.toString()))
  {
    for(Uint8 fucksGiven = 99; fucksGiven > 0; fucksGiven--)
      cout<<"YOU FUCKED UP! ";
    cout<<endl<<"I guess you really don't give a fuck"<<endl;
    merp = board.toString();
    for(Uint8 space = 6 - depth; space > 0; space--)
      cout<<' ';
    cout<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
    for(Uint8 y = 0; y < board.getDim().y(); y++)
    {
      for(Uint8 space = 6 - depth; space > 0; space--)
        cout<<' ';
      cout<<char('A'+y)<<' ';
      for(Uint8 x=0; merp[y*20+x] != '\n'; x++)
      {
        cout<<merp[y*20+x];
      }
      cout<<endl;
    }
    for(Uint8 space = 6 - depth; space > 0; space--)
      cout<<' ';
    cout<<"Value for current game: "<<evaluate(board)<<endl;
    while(getchar() != '\n');
  }
  
  for(Uint8 space = 6 - depth; space > 0; space--)
    cout<<' ';
  cout<<"  0 1 2 3 4 5 6 7 8 9"<<endl<<endl;  
  for(Uint8 y = 0; y < board.getDim().y(); y++)
  {
    for(Uint8 space = 6 - depth; space > 0; space--)
      cout<<' ';
    cout<<char('A'+y)<<' ';
    for(Uint8 x=0; merp[y*20+x] != '\n'; x++)
    {
      cout<<merp[y*20+x];
    }
    cout<<endl;
  }
  for(Uint8 space = 6 - depth; space > 0; space--)
    cout<<' ';
  cout<<"Value for current game: "<<evaluate(board)<<endl;
#endif
      
      if(score >= beta)
	return SEARCH_RETURN(beta, next);
      
      if(score > bestScore)
      {
	bestScore = score;
	if(score > alpha)
	  alpha = score;
	ret = next;
      }
      
    }
  }
  
  return SEARCH_RETURN(bestScore, ret);
}