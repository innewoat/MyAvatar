#include "vec.h"
#include <cmath>
#include <iostream>
using namespace std;

int main() {
  Mat4x4 m;
  m.scale(Vec3(2, 2, 2));
  cout << m << endl;
  Mat4x4 im = inverse_mat(m);
  cout << im << endl;
  cout << (m * im) << endl;
  return 0;
}