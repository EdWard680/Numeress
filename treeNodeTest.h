#ifndef TREENODETEST_H_
#define TREENODETEST_H_

#include <stdlib.h>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

class B;

class A
{
public:
  int part1;
  int part2;
public:
  A(int p1, int p2): part1(p1), part2(p2) {};
  const bool operator== (const A& a) const {return a.part1 == part1 && a.part2 == part2;};
  const bool operator!= (const A& a) const {return !(*this == a);};
};

namespace std
{
  template<>
  struct hash<A>
  {
    size_t operator() (const A& a) const
    {
      return ((hash<int>()(a.part1)) ^ (hash<int>()(a.part2) << 1));
    };
  };
}

class B
{
private:
  unordered_map<A, B> *Children;  // this is dynamically allocated. for reasons
public:
  int value;
  unordered_map<A, B> &children() {return *Children;};  // not safe for NULL, but whatever, example
  B(const int val=0): value(val)
  {
    Children = new unordered_map<A, B>;
  };
};

#endif