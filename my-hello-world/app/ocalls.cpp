#include "ocalls.hpp"

#include <iostream>

using namespace std;

void ocall_show_who_wins(const char *who) {
  cout << who << " wins" << endl;
}

void ocall_show_double_num(int num){
  cout << num << endl;
}
