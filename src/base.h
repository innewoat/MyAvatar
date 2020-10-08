#ifndef BASE_H
#define BASE_H

#include "vec.h"
#include <algorithm>
#include <cfloat>
#include <deque>
#include <iostream>
#include <vector>

#define BOX_MAX_CHILD_NUM 4

namespace MyAvatar {
namespace Help {
inline Vec3 help_min(const Vec3 &v0, const Vec3 &v1) {
  Vec3 res = v0;
  res.a = res.a < v1.a ? res.a : v1.a;
  res.b = res.b < v1.b ? res.b : v1.b;
  res.c = res.c < v1.c ? res.c : v1.c;

  return res;
}

inline Vec3 help_min(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2) {
  Vec3 res = v0;
  res.a = res.a < v1.a ? res.a : v1.a;
  res.b = res.b < v1.b ? res.b : v1.b;
  res.c = res.c < v1.c ? res.c : v1.c;
  res.a = res.a < v2.a ? res.a : v2.a;
  res.b = res.b < v2.b ? res.b : v2.b;
  res.c = res.c < v2.c ? res.c : v2.c;

  return res;
}

inline Vec3 help_max(const Vec3 &v0, const Vec3 &v1) {
  Vec3 res = v0;
  res.a = res.a > v1.a ? res.a : v1.a;
  res.b = res.b > v1.b ? res.b : v1.b;
  res.c = res.c > v1.c ? res.c : v1.c;

  return res;
}

inline Vec3 help_max(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2) {
  Vec3 res = v0;
  res.a = res.a > v1.a ? res.a : v1.a;
  res.b = res.b > v1.b ? res.b : v1.b;
  res.c = res.c > v1.c ? res.c : v1.c;
  res.a = res.a > v2.a ? res.a : v2.a;
  res.b = res.b > v2.b ? res.b : v2.b;
  res.c = res.c > v2.c ? res.c : v2.c;

  return res;
}

} // namespace Help
}; // namespace MyAvatar

class Ray {
public:
  Ray() : origin(), direction() {}
  Ray(Vec3 &o, Vec3 &d) : origin(o), direction(d) {}
  Vec3 origin;
  Vec3 direction;
  friend std::ostream &operator<<(std::ostream &output, const Ray &r) {
    output << r.origin << " " << r.direction;
    return output;
  }
};

class Triangle {
public:
  Vec3 v0, v1, v2;
  Vec3 n;
  double offset;

  Triangle(const Vec3 &iv0, const Vec3 &iv1, const Vec3 &iv2)
      : v0(iv0), v1(iv1), v2(iv2) {
    n = cross(v1 - v0, v2 - v0);
    offset = dot(v0, n);
  }

  Triangle(const Triangle &t)
      : v0(t.v0), v1(t.v1), v2(t.v2), n(t.n), offset(t.offset) {}

  friend std::ostream &operator<<(std::ostream &output,
                                  const Triangle &triangle) {
    output << triangle.v0 << ", " << triangle.v1 << ", " << triangle.v2;
    return output;
  }

  bool contain(Vec3 &v) {
    // check if v is in the same side with v0, v1, v2
    double t0 = is_contain_help(v0, v1, v);
    double t1 = is_contain_help(v1, v2, v);
    double t2 = is_contain_help(v2, v0, v);

    return (t0 >= 0 && t1 >= 0 && t2 >= 0) || (t0 <= 0 && t1 <= 0 && t2 <= 0);
  }

  bool interset(Ray &ray, double &res) {
    // check if ray interset with primitive
    // res is t for ray
    // return bool
    double temp = dot(ray.direction, n);
    if (temp > 0.001 || temp < -0.001) {
      double t = (offset - dot(ray.origin, n)) / temp;
      res = t;
      return true;
    } else {
      return false;
    }
  }

private:
  double is_contain_help(Vec3 &v0, Vec3 &v1, Vec3 &p) {
    return dot(cross(v1 - v0, p - v0), n);
  }
};

using namespace MyAvatar::Help;

class Box {
public:
  Vec3 min, max;
  Box *lChild, *rChild;
  std::vector<Triangle *> leaf;

  Box()
      : min(1, 1, 1), max(0, 0, 0), lChild(nullptr), rChild(nullptr), leaf() {}

  Box(const Vec3 &imin, const Vec3 &imax)
      : min(imin), max(imax), lChild(nullptr), rChild(nullptr), leaf() {}

  Box(const Box &box)
      : min(box.min), max(box.max), lChild(box.lChild), rChild(box.rChild),
        leaf(box.leaf) {}

  Box(const Triangle &triangle)
      : min(1, 1, 1), max(0, 0, 0), lChild(nullptr), rChild(nullptr), leaf() {
    min = help_min(triangle.v0, triangle.v1, triangle.v2);
    max = help_max(triangle.v0, triangle.v1, triangle.v2);
  }

  ~Box() {
    delete lChild;
    delete rChild;
  }

  bool hit(const Ray &r) {
    double aMin, aMax, bMin, bMax, cMin, cMax;
    if (r.direction.a != 0) {
      aMin = (min.a - r.origin.a) / r.direction.a;
      aMax = (max.a - r.origin.a) / r.direction.a;
    } else {
      aMin = DBL_MAX;
      aMax = DBL_MIN;
    }
    if (r.direction.b != 0) {
      bMin = (min.b - r.origin.b) / r.direction.b;
      bMax = (max.b - r.origin.b) / r.direction.b;
    } else {
      bMin = DBL_MAX;
      bMax = DBL_MIN;
    }
    if (r.direction.c != 0) {
      cMin = (min.c - r.origin.c) / r.direction.c;
      cMax = (max.c - r.origin.c) / r.direction.c;
    } else {
      cMin = DBL_MAX;
      cMax = DBL_MIN;
    }

    double hitMin, hitMax;
    hitMin = aMin;
    hitMin = hitMin < bMin ? hitMin : bMin;
    hitMin = hitMin < cMin ? hitMin : cMin;
    hitMax = aMax;
    hitMax = hitMax > bMax ? hitMax : bMax;
    hitMax = hitMax > cMax ? hitMax : cMax;

    return hitMin <= hitMax;
  }

  Box &combine(const Vec3 &v) {
    min = help_min(min, v);
    max = help_max(max, v);
    return *this;
  }

  Box &combine(Triangle *t) {
    min = help_min(min, help_min(t->v0, t->v1, t->v2));
    max = help_max(max, help_max(t->v0, t->v1, t->v2));
    leaf.push_back(t);
    return *this;
  }

  friend Box combine_box(const Box &b1, const Box &b2) {
    return Box(Vec3(help_min(b1.min, b2.min)), Vec3(help_max(b1.max, b2.max)));
  }
};

bool compare_triangle_a(const Triangle &t1, const Triangle &t2) {
  Vec3 v1 = (t1.v0 + t1.v1 + t1.v2) / 3.0;
  Vec3 v2 = (t2.v0 + t2.v1 + t2.v2) / 3.0;
  return v1.a < v2.a;
}

bool compare_triangle_b(const Triangle &t1, const Triangle &t2) {
  Vec3 v1 = (t1.v0 + t1.v1 + t1.v2) / 3.0;
  Vec3 v2 = (t2.v0 + t2.v1 + t2.v2) / 3.0;
  return v1.b < v2.b;
}

bool compare_triangle_c(const Triangle &t1, const Triangle &t2) {
  Vec3 v1 = (t1.v0 + t1.v1 + t1.v2) / 3.0;
  Vec3 v2 = (t2.v0 + t2.v1 + t2.v2) / 3.0;
  return v1.c < v2.c;
}

void build_box(const std::vector<Triangle>::iterator &start,
               const std::vector<Triangle>::iterator &end, int num, Box *box) {
  if (num < BOX_MAX_CHILD_NUM) {
    box->lChild = nullptr;
    box->rChild = nullptr;
    std::vector<Triangle>::iterator it = start;
    while (it < end) {
      box->combine(&*it);
      it++;
    }
  } else {
    int axis = rand() % 3;

    switch (axis) {
    case 0:
      sort(start, end, compare_triangle_a);
      break;
    case 1:
      sort(start, end, compare_triangle_b);
      break;
    case 2:
      sort(start, end, compare_triangle_c);
      break;
    }

    int newNum = (num + 1) / 2;

    std::vector<Triangle>::iterator mid = start + newNum;
    Box *l = new Box(), *r = new Box();

    build_box(start, mid, newNum, l);
    build_box(mid, end, num - newNum, r);
    box->lChild = l;
    box->rChild = r;
    box->min = help_min(l->min, r->min);
    box->max = help_max(l->max, r->max);
  }
}

#endif