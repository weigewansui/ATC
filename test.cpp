#include <iostream>
#include <list>
using namespace std;
int main() {
  string b[] = {"aaa", "bbb", "ccc", "dddd"};
  list<string> a(b, b + sizeof(b)/sizeof(string));
  a.remove("aaa");

  for(list<string>::iterator itr = a.begin(); itr != a.end(); itr ++) {
    cout << *itr << endl;
  }
}
