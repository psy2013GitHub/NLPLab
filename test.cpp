
#include<vector>
#include<iostream>

using namespace std;

class Foo {
public:
  Foo(int* p1): p(p1) {};
  ~Foo() {};
private:
  int* p;
};
class Foo1 {
public:
  Foo1(int& p1): p(p1) {};
  ~Foo1() {};
private:
  int& p;
};
int main() {
  //printf("1%s\n", ss.str().c_str());
  int x=8;
  int& p(x);
  Foo1* y = new Foo1(p);
  delete y;
  cout << p << endl; 
}
