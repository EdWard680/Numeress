#include "treeNodeTest.h"

int main(void)
{
  B root;
  root.children()[A(0, 0)].value = 3;
  root.children()[A(0, 0)].children()[A(0, 0)].value = 9;
  root.children()[A(0, 0)].children()[A(0, 1)].value = -1;
  root.children()[A(0, 1)].value = 6;
  root.children()[A(0, 1)].children()[A(0, 0)].value = -1;
  root.children()[A(0, 1)].children()[A(0, 1)].value = 0;
  
  cout<<root.children()[A(0, 1)].children()[A(0, 0)].value<<endl;
 
  return 0;
}