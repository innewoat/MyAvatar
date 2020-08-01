#include "font.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

#define FILENAME "pic0.ppm"

char c = 'a';

const int PIC_SIZE = 512;

class Vec3 {
public:
  Vec3(double na, double nb, double nc) : a(na), b(nb), c(nc) {}
  Vec3() : a(0), b(0), c(0) {}
  Vec3(const Vec3 &v) : a(v.a), b(v.b), c(v.c) {}
  double a, b, c;
  double length() { return sqrt(a * a + b * b + c * c); }
  Vec3 &normalize() {
    double l = length();
    a /= l;
    b /= l;
    c /= l;
    return *this;
  }

  inline Vec3 operator+(const Vec3 &v) {
    return Vec3(this->a + v.a, this->b + v.b, this->c + v.c);
  }

  inline Vec3 operator-(const Vec3 &v) {
    return Vec3(this->a - v.a, this->b - v.b, this->c - v.c);
  }

  inline Vec3 operator/(double d) {
    return Vec3(this->a / d, this->b / d, this->c / d);
  }

  inline Vec3 operator=(const Vec3 &v) {
    this->a = v.a;
    this->b = v.b;
    this->c = v.c;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &output, const Vec3 &v) {
    output << v.a << " " << v.b << " " << v.c;
    return output;
  }

  inline friend Vec3 operator*(double d, const Vec3 &v) {
    return Vec3(v.a * d, v.b * d, v.c * d);
  }

  inline friend Vec3 operator*(const Vec3 &v, double d) {
    return Vec3(v.a * d, v.b * d, v.c * d);
  }
};

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
  return Vec3(v1.b * v2.c - v1.c * v2.b, v1.c * v2.a - v1.a * v2.c,
              v1.a * v2.b - v1.b * v2.a);
}

inline double dot(const Vec3 &v1, const Vec3 &v2) {
  return v1.a * v2.a + v1.b * v2.b + v1.c * v2.c;
}

class Vec4 {
public:
  double a, b, c, d;

  Vec4() : a(0), b(0), c(0), d(0) {}
  Vec4(double na, double nb, double nc, double nd)
      : a(na), b(nb), c(nc), d(nd) {}
  Vec4(const Vec4 &v) : a(v.a), b(v.b), c(v.c), d(v.d) {}

  friend ostream &operator<<(ostream &output, const Vec4 &v) {
    output << v.a << " " << v.b << " " << v.c << " " << v.d;
    return output;
  }

  inline Vec4 operator+(const Vec4 &v) {
    return Vec4(this->a + v.a, this->b + v.b, this->c + v.c, this->d + v.d);
  }

  inline Vec4 operator-(const Vec4 &v) {
    return Vec4(this->a - v.a, this->b - v.b, this->c - v.c, this->d + v.d);
  }

  inline Vec4 operator=(const Vec4 &v) {
    this->a = v.a;
    this->b = v.b;
    this->c = v.c;
    this->d = v.d;
    return *this;
  }

  inline Vec4 operator/(double d) {
    return Vec4(this->a / d, this->b / d, this->c / d, this->d / d);
  }

  inline friend Vec4 operator*(double d, const Vec4 &v) {
    return Vec4(v.a * d, v.b * d, v.c * d, v.d * d);
  }

  inline friend Vec4 operator*(const Vec4 &v, double d) {
    return Vec4(v.a * d, v.b * d, v.c * d, v.d * d);
  }
};

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

class Mat4x4 {
public:
  double value[4][4];

  Mat4x4() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (i == j) {
          value[i][j] = 1;
        } else {
          value[i][j] = 0;
        }
      }
    }
  }

  Mat4x4(const Mat4x4 &m) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        value[i][j] = m.value[i][j];
      }
    }
  }

  friend ostream &operator<<(ostream &output, Mat4x4 &mat) {
    output << mat.value[0][0] << " " << mat.value[0][1] << " "
           << mat.value[0][2] << " " << mat.value[0][3] << std::endl;
    output << mat.value[1][0] << " " << mat.value[1][1] << " "
           << mat.value[1][2] << " " << mat.value[1][3] << std::endl;
    output << mat.value[2][0] << " " << mat.value[2][1] << " "
           << mat.value[2][2] << " " << mat.value[2][3] << std::endl;
    output << mat.value[3][0] << " " << mat.value[3][1] << " "
           << mat.value[3][2] << " " << mat.value[3][3] << std::endl;
    return output;
  }

  Mat4x4 &scale(const Vec3 &v);

  Mat4x4 &translate(const Vec3 &v);

  Mat4x4 &rotate_x(double angle);

  Mat4x4 &rotate_y(double angle);

  Mat4x4 &rotate_z(double angle);

  friend inline Vec3 operator*(Mat4x4 &mat, Vec3 &vec) {
    return Vec3(mat.value[0][0] * vec.a + mat.value[0][1] * vec.b +
                    mat.value[0][2] * vec.c + mat.value[0][3],
                mat.value[1][0] * vec.a + mat.value[1][1] * vec.b +
                    mat.value[1][2] * vec.c + mat.value[1][3],
                mat.value[2][0] * vec.a + mat.value[2][1] * vec.b +
                    mat.value[2][2] * vec.c + mat.value[2][3]);
  }

  friend Mat4x4 operator*(Mat4x4 &m1, Mat4x4 &m2) {
    Mat4x4 res;
    res.value[0][0] =
        m1.value[0][0] * m2.value[0][0] + m1.value[0][1] * m2.value[1][0] +
        m1.value[0][2] * m2.value[2][0] + m1.value[0][3] * m2.value[3][0];
    res.value[0][1] =
        m1.value[0][0] * m2.value[0][1] + m1.value[0][1] * m2.value[1][1] +
        m1.value[0][2] * m2.value[2][1] + m1.value[0][3] * m2.value[3][1];
    res.value[0][2] =
        m1.value[0][0] * m2.value[0][2] + m1.value[0][1] * m2.value[1][2] +
        m1.value[0][2] * m2.value[2][2] + m1.value[0][3] * m2.value[3][2];
    res.value[0][3] =
        m1.value[0][0] * m2.value[0][3] + m1.value[0][1] * m2.value[1][3] +
        m1.value[0][2] * m2.value[2][3] + m1.value[0][3] * m2.value[3][3];
    res.value[1][0] =
        m1.value[1][0] * m2.value[0][0] + m1.value[1][1] * m2.value[1][0] +
        m1.value[1][2] * m2.value[2][0] + m1.value[1][3] * m2.value[3][0];
    res.value[1][1] =
        m1.value[1][0] * m2.value[0][1] + m1.value[1][1] * m2.value[1][1] +
        m1.value[1][2] * m2.value[2][1] + m1.value[1][3] * m2.value[3][1];
    res.value[1][2] =
        m1.value[1][0] * m2.value[0][2] + m1.value[1][1] * m2.value[1][2] +
        m1.value[1][2] * m2.value[2][2] + m1.value[1][3] * m2.value[3][2];
    res.value[1][3] =
        m1.value[1][0] * m2.value[0][3] + m1.value[1][1] * m2.value[1][3] +
        m1.value[1][2] * m2.value[2][3] + m1.value[1][3] * m2.value[3][3];
    res.value[2][0] =
        m1.value[2][0] * m2.value[0][0] + m1.value[2][1] * m2.value[1][0] +
        m1.value[2][2] * m2.value[2][0] + m1.value[2][3] * m2.value[3][0];
    res.value[2][1] =
        m1.value[2][0] * m2.value[0][1] + m1.value[2][1] * m2.value[1][1] +
        m1.value[2][2] * m2.value[2][1] + m1.value[2][3] * m2.value[3][1];
    res.value[2][2] =
        m1.value[2][0] * m2.value[0][2] + m1.value[2][1] * m2.value[1][2] +
        m1.value[2][2] * m2.value[2][2] + m1.value[2][3] * m2.value[3][2];
    res.value[2][3] =
        m1.value[2][0] * m2.value[0][3] + m1.value[2][1] * m2.value[1][3] +
        m1.value[2][2] * m2.value[2][3] + m1.value[2][3] * m2.value[3][3];
    res.value[3][0] =
        m1.value[3][0] * m2.value[0][0] + m1.value[3][1] * m2.value[1][0] +
        m1.value[3][2] * m2.value[2][0] + m1.value[3][3] * m2.value[3][0];
    res.value[3][1] =
        m1.value[3][0] * m2.value[0][1] + m1.value[3][1] * m2.value[1][1] +
        m1.value[3][2] * m2.value[2][1] + m1.value[3][3] * m2.value[3][1];
    res.value[3][2] =
        m1.value[3][0] * m2.value[0][2] + m1.value[3][1] * m2.value[1][2] +
        m1.value[3][2] * m2.value[2][2] + m1.value[3][3] * m2.value[3][2];
    res.value[3][3] =
        m1.value[3][0] * m2.value[0][3] + m1.value[3][1] * m2.value[1][3] +
        m1.value[3][2] * m2.value[2][3] + m1.value[3][3] * m2.value[3][3];

    return res;
  }
};

Mat4x4 &Mat4x4::scale(const Vec3 &v) {
  Mat4x4 m;
  m.value[0][0] = v.a;
  m.value[1][1] = v.b;
  m.value[2][2] = v.c;
  (*this) = m * (*this);
  return *this;
}

Mat4x4 &Mat4x4::translate(const Vec3 &v) {
  Mat4x4 m;
  m.value[0][3] = v.a;
  m.value[1][3] = v.b;
  m.value[2][3] = v.c;
  (*this) = m * (*this);
  return *this;
}

Mat4x4 &Mat4x4::rotate_x(double angle) {
  Mat4x4 m;
  double angle_cos = cos(angle);
  double angle_sin = sin(angle);
  m.value[1][1] = angle_cos;
  m.value[1][2] = -angle_sin;
  m.value[2][1] = angle_sin;
  m.value[2][2] = angle_cos;
  (*this) = m * (*this);
  return *this;
}

Mat4x4 &Mat4x4::rotate_y(double angle) {
  Mat4x4 m;
  double angle_cos = cos(angle);
  double angle_sin = sin(angle);
  m.value[0][0] = angle_cos;
  m.value[0][2] = angle_sin;
  m.value[2][0] = -angle_sin;
  m.value[2][2] = angle_cos;
  (*this) = m * (*this);
  return *this;
}

Mat4x4 &Mat4x4::rotate_z(double angle) {
  Mat4x4 m;
  double angle_cos = cos(angle);
  double angle_sin = sin(angle);
  m.value[0][0] = angle_cos;
  m.value[0][1] = -angle_sin;
  m.value[1][0] = angle_sin;
  m.value[1][1] = angle_cos;
  *this = m * (*this);
  return *this;
}

class Primitive {
public:
  Vec3 n;
  double offset;

  Primitive(Vec3 &iv0, Vec3 &iv1, Vec3 &iv2)
      : v0(iv0), v1(iv1), v2(iv2), t_v0(), t_v1(), t_v2() {
    n = cross(v1 - v0, v2 - v0);
    offset = dot(v0, n);
  }

  Primitive &set_texture_coor(const Vec3 &it_v0, const Vec3 &it_v1,
                              const Vec3 &it_v2) {
    t_v0 = it_v0;
    t_v1 = it_v1;
    t_v2 = it_v2;
    return *this;
  }

  inline friend ostream &operator<<(ostream &output, Primitive &primitive) {
    output << primitive.v0 << ", " << primitive.v1 << ", " << primitive.v2;
    return output;
  }

  bool contain(Vec3 &v) {
    // check if v is in the same side with v0, v1, v2
    double t0 = is_contain_help(v0, v1, v);
    double t1 = is_contain_help(v1, v2, v);
    double t2 = is_contain_help(v2, v0, v);

    return (t0 >= 0 && t1 >= 0 && t2 >= 0) || (t0 <= 0 && t1 <= 0 && t2 <= 0);
  }

  Vec3 cal_barycentric_coor(Vec3 &v) {
    double a, b, c;

    a = ((v.b - v1.b) * (v2.a - v1.a) - (v.a - v1.a) * (v2.b - v1.b)) /
        ((v0.b - v1.b) * (v2.a - v1.a) - (v0.a - v1.a) * (v2.b - v1.b));

    b = ((v.b - v2.b) * (v0.a - v2.a) - (v.a - v2.a) * (v0.b - v2.b)) /
        ((v1.b - v2.b) * (v0.a - v2.a) - (v1.a - v2.a) * (v0.b - v2.b));

    c = 1 - a - b;
    return Vec3(a, b, c);
  }

  Vec3 get_texture_coor(Vec3 &v) {
    Vec3 baryCoor = cal_barycentric_coor(v);
    return baryCoor.a * t_v0 + baryCoor.b * t_v1 + baryCoor.c * t_v2;
  }

  Vec4 get_texture_color(Vec3 &v) {
    Vec3 textCoor = get_texture_coor(v);

    textCoor = textCoor * 1;

    double x = textCoor.a - trunc(textCoor.a);
    double y = textCoor.b - trunc(textCoor.b);

    x = x + 0.0;
    y = y + 0.0;
    if (x > 1)
      x = x - 1;
    if (y > 1)
      y = y - 1;

    int ix = (int)ceil(x * FONT_SIZE);
    int iy = (int)ceil(y * FONT_SIZE);

    if (ix == FONT_SIZE)
      ix = FONT_SIZE - 1;
    if (iy == FONT_SIZE)
      iy = FONT_SIZE - 1;

    if (need_draw(Fonts::get_instance().get_font(c), ix, iy)) {
      return Vec4(0, 0, 0, 0.6);
    } else {
      return Vec4(1, 1, 1, 0.3);
    }
  }

private:
  Vec3 v0;
  Vec3 v1;
  Vec3 v2;

  Vec3 t_v0;
  Vec3 t_v1;
  Vec3 t_v2;

  double is_contain_help(Vec3 &v0, Vec3 &v1, Vec3 &p) {
    return dot(cross(v1 - v0, p - v0), n);
  }
};

bool interset(Ray &ray, Primitive &p, double &res) {
  // check if ray interset with primitive
  // res is t for ray
  // return bool
  double temp = dot(ray.direction, p.n);
  if (temp > 0.001 || temp < -0.001) {
    double t = (p.offset - dot(ray.origin, p.n)) / temp;
    res = t;
    return true;
  } else {
    return false;
  }
}

class TransparentColor {
public:
  Vec4 color;
  double t;

  TransparentColor(const Vec4 &nc, const double nt) : color(nc), t(nt) {}

  static bool transparnet_color_comp(const TransparentColor &a,
                                     const TransparentColor &b) {
    return a.t > b.t;
  }
};

Vec4 cal_color(Vec3 &target, Vec3 &camera, vector<Primitive> &primitives) {
  vector<TransparentColor> colors;
  colors.clear();
  Ray r(camera, (target - camera).normalize());
  double t;
  for (Primitive &p : primitives) {
    if (interset(r, p, t)) {
      Vec3 temp = r.origin + r.direction * t;
      if (p.contain(temp)) {
        Vec4 c = p.get_texture_color(temp);
        colors.push_back(TransparentColor(c, t));
      }
    }
  }

  sort(colors.begin(), colors.end(), TransparentColor::transparnet_color_comp);

  Vec4 color(60.0 / 255.0, 240.0 / 255.0, 165.0 / 255.0, 0.1);

  for (auto c = colors.begin(); c != colors.end(); c++) {
    color = (1 - c->color.d) * color + c->color.d * c->color;
  }
  return color;
}

inline Vec3 get_sample_coor(const int x, const int y, const int w) {
  return Vec3(1.0 / (w * 2.0) + (double)x / (double)w,
              1.0 / (w * 2.0) + (double)y / (double)w, 0);
}

std::default_random_engine e;
std::uniform_real_distribution<double> u(0, 1);
double range = 4.0 / PIC_SIZE;
inline Vec3 random_offset() {
  return Vec3(range * u(e) - range / 2.0, range * u(e) - range / 2.0, 0);
}

int main() {
  ofstream file;
  Vec3 camera(0, 0, 6);
  int width = PIC_SIZE;
  int height = PIC_SIZE;
  Vec3 base(-2, -2, 0);
  Vec3 u(4, 0, 0);
  Vec3 v(0, 4, 0);

  int sample = 2;
  int randomOffsetTime = 10;

  Vec3 vertexs[8] = {Vec3(0.5, 0.5, 0.5),   Vec3(0.5, 0.5, -0.5),
                     Vec3(0.5, -0.5, 0.5),  Vec3(0.5, -0.5, -0.5),
                     Vec3(-0.5, 0.5, 0.5),  Vec3(-0.5, 0.5, -0.5),
                     Vec3(-0.5, -0.5, 0.5), Vec3(-0.5, -0.5, -0.5)};

  Mat4x4 mat;
  mat.rotate_x(40 * M_PI / 180)
      .rotate_y(45 * M_PI / 180)
      .rotate_z(50 * M_PI / 180)
      .scale(Vec3(2, 2, 2))
      .translate(Vec3(0.2, 0, 1.0));
  for (int i = 0; i < 8; i++) {
    vertexs[i] = mat * vertexs[i];
  }
  vector<Primitive> primitives;
  {
    primitives.push_back(
        Primitive(vertexs[0], vertexs[2], vertexs[6])
            .set_texture_coor(Vec3(0, 1, 0), Vec3(1, 1, 0), Vec3(1, 0, 0)));
    primitives.push_back(
        Primitive(vertexs[6], vertexs[4], vertexs[0])
            .set_texture_coor(Vec3(1, 0, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)));
    primitives.push_back(
        Primitive(vertexs[0], vertexs[1], vertexs[2])
            .set_texture_coor(Vec3(0, 1, 0), Vec3(0, 2, 0), Vec3(1, 1, 0)));
    primitives.push_back(
        Primitive(vertexs[3], vertexs[2], vertexs[1])
            .set_texture_coor(Vec3(1, 2, 0), Vec3(1, 1, 0), Vec3(0, 2, 0)));
    primitives.push_back(
        Primitive(vertexs[1], vertexs[0], vertexs[5])
            .set_texture_coor(Vec3(4, 2, 0), Vec3(4, 1, 0), Vec3(3, 2, 0)));
    primitives.push_back(
        Primitive(vertexs[0], vertexs[4], vertexs[5])
            .set_texture_coor(Vec3(4, 1, 0), Vec3(3, 1, 0), Vec3(3, 2, 0)));
    primitives.push_back(
        Primitive(vertexs[4], vertexs[5], vertexs[6])
            .set_texture_coor(Vec3(3, 1, 0), Vec3(3, 2, 0), Vec3(2, 1, 0)));
    primitives.push_back(
        Primitive(vertexs[5], vertexs[7], vertexs[6])
            .set_texture_coor(Vec3(3, 2, 0), Vec3(2, 2, 0), Vec3(2, 1, 0)));
    primitives.push_back(
        Primitive(vertexs[2], vertexs[6], vertexs[7])
            .set_texture_coor(Vec3(1, 1, 0), Vec3(2, 1, 0), Vec3(2, 2, 0)));
    primitives.push_back(
        Primitive(vertexs[3], vertexs[2], vertexs[7])
            .set_texture_coor(Vec3(1, 2, 0), Vec3(1, 1, 0), Vec3(2, 2, 0)));
    primitives.push_back(
        Primitive(vertexs[1], vertexs[3], vertexs[7])
            .set_texture_coor(Vec3(0, 2, 0), Vec3(1, 2, 0), Vec3(1, 3, 0)));
    primitives.push_back(
        Primitive(vertexs[7], vertexs[5], vertexs[1])
            .set_texture_coor(Vec3(1, 3, 0), Vec3(0, 3, 0), Vec3(0, 2, 0)));
  }

  e.seed(time(NULL));

  for (; c <= 'z'; c++) {
    string s("./output/pic_a.ppm");
    s[13] = c;
    cout << s << endl;
    file.open(s, std::fstream::out | fstream::trunc);
    file << "P3" << endl;
    file << width << " " << height << endl;
    file << "255" << endl;

    for (int i = width - 1; i >= 0; i--) {
      for (int j = 0; j < width; j++) {
        Vec4 color;
        Vec3 target;

        for (int k = 0; k < randomOffsetTime; k++) {
          for (int m = 0; m < sample; m++) {
            for (int n = 0; n < sample; n++) {
              Vec3 sample_coor = get_sample_coor(m, n, sample);
              target = base + u / width * ((double)i + sample_coor.a) +
                       v / height * ((double)j + sample_coor.b) +
                       random_offset();
              color = color + cal_color(target, camera, primitives);
            }
          }
        }

        color = color / (sample * sample * randomOffsetTime);

        // target =
        //     base + u / width * ((double)i + 0.5) + v / height * ((double)j +
        //     0.5);
        // color = color + cal_color(target, camera, primitives);

        int a = (int)(round(color.a * 255));
        int b = (int)(round(color.b * 255));
        int c = (int)(round(color.c * 255));
        file << a << " " << b << " " << c << "\n";
      }
    }
    file.close();
    cout << "finish render " << c << " " << time(NULL) << endl;
  }
  return 0;
}