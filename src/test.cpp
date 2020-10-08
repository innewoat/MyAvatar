#include "test.h"
#include "vec.h"
#include <iostream>
using namespace std;

int main() {
  std::vector<Triangle> triangles;
  test::generate_triangles(triangles);
  Box *root = new Box();
  build_box(triangles.begin(), triangles.end(), triangles.size(), root);
  delete root;
  return 0;
}