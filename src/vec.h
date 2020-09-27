#include <cmath>
#include <iostream>

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
  Vec4(const Vec3 &v, double nd) : a(v.a), b(v.b), c(v.c), d(nd) {}

  friend std::ostream &operator<<(std::ostream &output, const Vec4 &v) {
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

  friend std::ostream &operator<<(std::ostream &output, const Mat4x4 &mat) {
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

  friend Mat4x4 inverse_mat(Mat4x4 &m) {
    int n = 4;

    double *raw = m.value[0];
    double *nums = new double[n * n * 2];
    double **rows = new double *[n];
    Mat4x4 res;

    // init
    rows[0] = nums;
    for (int i = 0; i < n; i++) {
      if (i != 0) {
        rows[i] = rows[i - 1] + (n * 2);
        raw = raw + n;
      }
      for (int j = 0; j < n * 2; j++) {
        if (j < n) {
          rows[i][j] = raw[j];
        } else {
          if (j - n == i) {
            rows[i][j] = 1;
          } else {
            rows[i][j] = 0;
          }
        }
      }
    }

    for (int i = 0; i < n; i++) {
      // find first not 0 line
      double *currentRow = rows[i];
      int j = 0;

      if (currentRow[i] == 0) {
        for (j = i + 1; j < n; i++) {
          if (rows[j][i] != 0) {
            currentRow = rows[j];
            double *t = rows[j];
            rows[j] = rows[i];
            rows[i] = t;
            break;
          }
        }
      }
      if (j == n) {
        std::cout << "return in " << j << std::endl;
        return Mat4x4();
      }

      // normalize current row
      double div = currentRow[i];
      for (int k = 0; k < n * 2; k++) {
        currentRow[k] /= div;
      }

      // normalize other row
      for (int p = 0; p < n; p++) {
        if (p == i) {
          continue;
        }
        double mul = -rows[p][i];
        for (int q = 0; q < 2 * n; q++) {
          rows[p][q] = rows[p][q] + mul * currentRow[q];
        }
      }
    }

    // save to res
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        res.value[i][j] = rows[i][j + n];
      }
    }

    delete nums;
    return res;
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
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        res.value[i][j] =
            m1.value[i][0] * m2.value[0][j] + m1.value[i][1] * m2.value[1][j] +
            m1.value[i][2] * m2.value[2][j] + m1.value[i][3] * m2.value[3][j];
      }
    }
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
