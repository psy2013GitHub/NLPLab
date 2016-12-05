
#include<iostream>
#include<vector>
#include<string>
#include<limits>

using namespace std;

template <class T>
int cmp(const vector<T>& x, const vector<T>& y) {
  return x[0] - y[0];
}

int main () {
 vector<double> x; x.push_back(1);
 vector<double> y; y.push_back(2);
 cout << cmp(x,y) << endl;
 cout << string("test_toy") << endl;
 string src("你好;no;ok;");
 cout << src.find(string(";"), 0) << endl;
 const long double a = -3.14e+100;
 const long double b = a-10;
 cout << b-a << endl;
 //cout << DOUBLE_MIN << endl;
}
