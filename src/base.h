#pragma once

#include "vec.h"
#include <vector>
#include <deque>
#include <iostream>

namespace MyAvatar
{
    namespace Help
    {
        inline Vec3 help_min(const Vec3 &v0, const Vec3 &v1)
        {
            Vec3 res = v0;
            res.a = res.a < v1.a ? res.a : v1.a;
            res.b = res.b < v1.b ? res.b : v1.b;
            res.c = res.c < v1.c ? res.c : v1.c;

            return res;
        }

        inline Vec3 help_min(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
        {
            Vec3 res = v0;
            res.a = res.a < v1.a ? res.a : v1.a;
            res.b = res.b < v1.b ? res.b : v1.b;
            res.c = res.c < v1.c ? res.c : v1.c;
            res.a = res.a < v2.a ? res.a : v2.a;
            res.b = res.b < v2.b ? res.b : v2.b;
            res.c = res.c < v2.c ? res.c : v2.c;

            return res;
        }

        inline Vec3 help_max(const Vec3 &v0, const Vec3 &v1)
        {
            Vec3 res = v0;
            res.a = res.a > v1.a ? res.a : v1.a;
            res.b = res.b > v1.b ? res.b : v1.b;
            res.c = res.c > v1.c ? res.c : v1.c;

            return res;
        }

        inline Vec3 help_max(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
        {
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
};    // namespace MyAvatar

class Ray
{
public:
    Ray() : origin(), direction() {}
    Ray(Vec3 &o, Vec3 &d) : origin(o), direction(d) {}
    Vec3 origin;
    Vec3 direction;
    friend std::ostream &operator<<(std::ostream &output, const Ray &r)
    {
        output << r.origin << " " << r.direction;
        return output;
    }
};

class Triangle
{
public:
    Vec3 v0, v1, v2;
    Vec3 n;
    double offset;
    Triangle(Vec3 &iv0, Vec3 &iv1, Vec3 &iv2) : v0(iv0), v1(iv1), v2(iv2)
    {
        n = cross(v1 - v0, v2 - v0);
        offset = dot(v0, n);
    }
    Triangle(const Triangle &t) : v0(t.v0), v1(t.v1), v2(t.v2), n(t.n), offset(t.offset) {}

    inline friend std::ostream &operator<<(std::ostream &output, Triangle &triangle)
    {
        output << triangle.v0 << ", " << triangle.v1 << ", " << triangle.v2;
        return output;
    }

    bool contain(Vec3 &v)
    {
        // check if v is in the same side with v0, v1, v2
        double t0 = is_contain_help(v0, v1, v);
        double t1 = is_contain_help(v1, v2, v);
        double t2 = is_contain_help(v2, v0, v);

        return (t0 >= 0 && t1 >= 0 && t2 >= 0) || (t0 <= 0 && t1 <= 0 && t2 <= 0);
    }

    bool interset(Ray &ray, double &res)
    {
        // check if ray interset with primitive
        // res is t for ray
        // return bool
        double temp = dot(ray.direction, n);
        if (temp > 0.001 || temp < -0.001)
        {
            double t = (offset - dot(ray.origin, n)) / temp;
            res = t;
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    double is_contain_help(Vec3 &v0, Vec3 &v1, Vec3 &p)
    {
        return dot(cross(v1 - v0, p - v0), n);
    }
};

using namespace MyAvatar::Help;

class Box
{
public:
    Vec3 min, max;
    Box *lChild, *rChild;
    std::vector<Triangle *> leaf;

    Box(const Vec3 &imin, const Vec3 &imax) : min(imin), max(imax), lChild(nullptr), rChild(nullptr), leaf() {}

    Box(const Box &box) : min(box.min), max(box.max), lChild(box.lChild), rChild(box.rChild), leaf(box.leaf) {}

    Box(const Triangle &triangle) : min(), max(), lChild(nullptr), rChild(nullptr), leaf()
    {
        min = help_min(triangle.v0, triangle.v1, triangle.v2);
        max = help_max(triangle.v0, triangle.v1, triangle.v2);
    }

    ~Box()
    {
        delete lChild;
        delete rChild;
    }

    Box &combine(const Vec3 &v)
    {
        min = help_min(min, v);
        max = help_max(max, v);
        return *this;
    }

    friend Box combine_box(const Box &b1, const Box &b2)
    {
        return Box(Vec3(help_min(b1.min, b2.min)), Vec3(help_max(b1.max, b2.max)));
    }
};

void build_box(std::vector<Triangle> &triangles, Box &root)
{

    /*
    if nTriangles < define
        add to child
    else
        choose axis
        sort triangles
        split triangles into 2 groups
        recurrisive
    */
}