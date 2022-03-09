#ifndef VEC_MAT_MATHS_H
#define VEC_MAT_MATHS_H

#include <cmath>
#include <math.h>
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;
using std::sqrtf;
#define _X xyzw[0]
#define _Y xyzw[1]
#define _Z xyzw[2]
#define _W xyzw[3]
#define _R xyzw[0]
#define _G xyzw[1]
#define _B xyzw[2]
#define _L xyzw[0]
#define _A xyzw[1]

enum matInitType {Zeros, Ones, Diagonal};

struct vec4 {
    float xyzw[4];

    vec4() {
        _X = _Y = _Z = 0.0;
        _W = 1.0;
    }

    vec4(float f) {
        _X = _Y = _Z = _W = f;
    }

    vec4(double f) {
        _X = _Y = _Z = _W = static_cast<float>(f);
    }

    vec4(double x, double y, double z = 0.0, double w = 0.0) {
        _X = static_cast<float>(x);
        _Y = static_cast<float>(y);
        _Z = static_cast<float>(z);
        _W = static_cast<float>(w);
    }

    vec4(float x, float y, float z = 0.0, float w = 0.0) {
        _X = x;
        _Y = y;
        _Z = z;
        _W = w;
    }

    vec4(float v[4]) {
        _X = v[0];
        _Y = v[1];
        _Z = v[2];
        _W = v[3];
    }

    vec4(double v[4]) {
        _X = static_cast<float>(v[0]);
        _Y = static_cast<float>(v[1]);
        _Z = static_cast<float>(v[2]);
        _W = static_cast<float>(v[3]);
    }

    vec4(vec4 *v) {
        _X = v->_X;
        _Y = v->_Y;
        _Z = v->_Z;
        _W = v->_W;
    }

    void operator = (const double d) {
        _X = static_cast<float>(d);
        _Y = static_cast<float>(d);
        _Z = static_cast<float>(d);
        _W = static_cast<float>(d);
    }

    void operator = (float arr[4]) {
        _X = arr[0];
        _Y = arr[1];
        _Z = arr[2];
        _W = arr[3];
    }

    void operator = (double arr[4]) {
        _X = static_cast<float>(arr[0]);
        _Y = static_cast<float>(arr[1]);
        _Z = static_cast<float>(arr[2]);
        _W = static_cast<float>(arr[3]);
    }

    float operator * (const vec4 &v) {
        return _X * v._X + _Y * v._Y + _Z * v._Z + _W * v._W;
    }

    vec4 operator * (const double d) {
        float f = static_cast<float>(d);
        return vec4(_X * f, _Y * f, _Z * f, _W * f);
    }

    vec4 operator / (const double d) {
        float f = static_cast<float>(d);
        return vec4(_X / f, _Y / f, _Z / f, _W / f);
    }

    vec4 operator + (const vec4 &v) {
        return vec4(_X + v._X, _Y + v._Y, _Z + v._Z, _W + v._W);
    }

    vec4 operator + (const double d) {
        float f = static_cast<float>(d);
        return vec4(_X + f, _Y + f, _Z + f, _W + f);
    }

    vec4 operator - (const vec4 &v) {
        return vec4(_X - v._X, _Y - v._Y, _Z - v._Z, _W - v._W);
    }

    vec4 operator - (const double d) {
        float f = static_cast<float>(d);
        return vec4(_X - f, _Y - f, _Z - f, _W - f);
    }

    void operator += (vec4 &v) {
        *this = *this + v;
    }

    void operator += (double d) {
        *this = *this + d;
    }

    void operator -= (vec4 &v) {
        *this = *this - v;
    }

    void operator -= (double d) {
        *this = *this - d;
    }

    void operator *= (double d) {
        *this = *this * d;
    }

    void operator /= (double d) {
        *this = *this / d;
    }

    float operator [] (const int i) {
        return xyzw[i];
    }

    // unchecked for axis < 0 or > 3
    void set(int i, float val) {
        xyzw[i] = val;
    }

    float get(int i) {
        return xyzw[i];
    }

    float dot(vec4 v) {
        return _X * v._X + _Y * v._Y + _Z * v._Z + _W * v._W;
    }

    vec4 cross(vec4 v) {
        float i = _Y * v._Z - _Z * v._Y;
        float j = _Z * v._X - _X * v._Z;
        float k = _X * v._Y - _Y * v._X;
        return vec4(i, j, k);
    }

    void crossWith(vec4 v) {
        float i = _Y * v._Z - _Z * v._Y;
        float j = _Z * v._X - _X * v._Z;
        float k = _X * v._Y - _Y * v._X;
        _X = i;
        _Y = j;
        _Z = k;
    }

    float mag3() {
        return sqrt(_X * _X + _Y * _Y + _Z * _Z);
    }

    vec4 norm() {
        float sr = sqrt(_X * _X + _Y * _Y + _Z * _Z);
        return vec4(_X / sr, _Y / sr, _Z / sr, _W);
    }

    void normalize() {
        float sr = sqrt(_X * _X + _Y * _Y + _Z * _Z);
        _X /= sr;
        _Y /= sr;
        _Z /= sr;
    }

    void print() {
        for (float f : xyzw)
            cout << f << "\t";
        cout << endl;
    }

};

struct mat4 {
    vec4 m[4];

    mat4(matInitType initType) {
        float n = static_cast<float>(initType);
        for (int i = 0; i < 4; ++i) {
            if (initType == Diagonal) {
                m[i] = vec4(0.0);
                m[i].xyzw[i] = 1.0;
            }
            else
                m[i] = vec4(n);
        }
    }

    mat4(mat4 *M) {
        for (int i = 0; i < 4; ++i)
            m[i] = M->m[i];
    }

    mat4(float f) {
        for (int i = 0; i < 4; ++i)
            m[i] = vec4(f);
    }

    mat4(double d) {
        for (int i = 0; i < 4; ++i)
            m[i] = vec4(d);
    }

    mat4(vec4 col1, vec4 col2, vec4 col3, vec4 col4) {
        m[0] = col1;
        m[1] = col2;
        m[2] = col3;
        m[3] = col4;
    }

    mat4(vec4 v[4]) {
        for (int i = 0; i < 4; ++i)
            m[i] = v[i];
    }

    mat4(float M[4][4]) {
        for (int i = 0; i < 4; ++i)
            m[i] = vec4(M[i]);
    }

    mat4(float M[3][3]) {
        for (int i = 0; i < 3; ++i)
            m[i] = vec4(M[i][0], M[i][1], M[i][2], 0.0f);
        m[3] = vec4(0.0);
    }

    vec4 operator * (const vec4 &v) {
        float x, y, z, w;
        x = m[0].dot(v);
        y = m[1].dot(v);
        z = m[2].dot(v);
        w = m[3].dot(v);
        return vec4(x, y, z, w);
    }

    mat4 operator * (mat4 &M) {
        mat4 ret(Zeros);
        for (int i = 0; i < 4; ++i) {
            vec4 temp(m[0][i], m[1][i], m[2][i], m[3][i]);
            ret.m[i] = M * temp;
        }
        ret.transpose();
        return ret;
    }

    mat4 operator * (double d) {
        mat4 ret = *this;
        for (int i = 0; i < 4; ++i)
            ret.m[i] = ret[i] * d;
        return &ret;
    }

    mat4 operator + (double d) {
        mat4 ret = *this;
        for (int i = 0; i < 4; ++i)
            ret.m[i] = ret[i] + d;
        return ret;
    }

    mat4 operator - (double d) {
        mat4 ret = *this;
        for (int i = 0; i < 4; ++i)
            ret.m[i] = ret[i] - d;
        return ret;
    }

    mat4 operator / (double d) {
        mat4 ret = *this;
        for (int i = 0; i < 4; ++i)
            ret.m[i] = ret[i] / d;
        return &ret;
    }

    void operator *= (mat4 &M) {
        *this = *this * M;
    }

    void operator *= (double d) {
        *this = *this * d;
    }

    void operator += (mat4 &M) {
        *this = *this + M;
    }

    void operator += (double d) {
        *this = *this + d;
    }

    void operator /= (double d) {
        *this = *this / d;
    }

    void operator -= (double d) {
        *this = *this - d;
    }

    void operator -= (mat4 &M) {
        *this = *this - M;
    }

    mat4 operator + (mat4 &M) {
        return mat4(m[0] + M[0], m[1] + M[1], m[2] + M[2], m[3] + M[3]);
    }

    mat4 operator - (mat4 &M) {
        return mat4(m[0] - M[0], m[1] - M[1], m[2] - M[2], m[3] - M[3]);
    }

    vec4 operator [] (const int i) {
        return m[i];
    }

    void set(int col, int row, float val) {
        m[col].xyzw[row] = val;
    }

    void setCol(int col, vec4 v) {
        m[col] = v;
    }

    void setRow(int row, vec4 c) {
        for (int i = 0; i < 4; ++i)
            m[i].xyzw[row] = c[i];
    }

    void transpose() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < i + 1; ++j) {
                float f = m[i][j];
                m[i].xyzw[j] = m[j][i];
                m[j].xyzw[i] = f;
            }
    }

    vec4 mult(vec4 v) {
        float x, y, z, w;
        x = m[0].dot(v);
        y = m[1].dot(v);
        z = m[2].dot(v);
        w = m[3].dot(v);
        return vec4(x, y, z, w);
    }

    mat4 mult(mat4 M) {
        mat4 ret(Zeros);
        for (int i = 0; i < 4; ++i) {
            vec4 temp(m[0][i], m[1][i], m[2][i], m[3][i]);
            ret.setCol(i, M * temp);
        }
        ret.transpose();
        return ret;
    }

    float det3() {
        return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    mat4 inverse3() {
        float det = det3();
        if (det == 0.0)
            return mat4(Diagonal);
        float invdet = 1 / det;
        float arr[3][3];
        arr[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invdet;
        arr[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invdet;
        arr[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invdet;
        arr[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invdet;
        arr[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invdet;
        arr[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invdet;
        arr[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invdet;
        arr[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invdet;
        arr[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invdet;
        return mat4(arr) + 0.0;
    }

    float det4() {
        float a = m[1][1]  * m[2][2] * m[3][3] -
                  m[1][1]  * m[3][2] * m[2][3] -
                  m[1][2]  * m[2][1]  * m[3][3] +
                  m[1][2]  * m[3][1]  * m[2][3] +
                  m[1][3] * m[2][1]  * m[3][2] -
                  m[1][3] * m[3][1]  * m[2][2];

        float b = -m[0][1]  * m[2][2] * m[3][3] +
                   m[0][1]  * m[3][2] * m[2][3] +
                   m[0][2]  * m[2][1]  * m[3][3] -
                   m[0][2]  * m[3][1]  * m[2][3] -
                   m[0][3] * m[2][1]  * m[3][2] +
                   m[0][3] * m[3][1]  * m[2][2];

        float c = m[0][1]  * m[1][2] * m[3][3] -
                  m[0][1]  * m[3][2] * m[1][3] -
                  m[0][2]  * m[1][1] * m[3][3] +
                  m[0][2]  * m[3][1] * m[1][3] +
                  m[0][3] * m[1][1] * m[3][2] -
                  m[0][3] * m[3][1] * m[1][2];

        float d = -m[0][1]  * m[1][2] * m[2][3] +
                   m[0][1]  * m[2][2] * m[1][3] +
                   m[0][2]  * m[1][1] * m[2][3] -
                   m[0][2]  * m[2][1] * m[1][3] -
                   m[0][3] * m[1][1] * m[2][2] +
                   m[0][3] * m[2][1] * m[1][2];
        return m[0][0] * a + m[1][0] * b + m[2][0] * c + m[3][0] * d;
    }

    mat4 inverse4() {
        float a = m[1][1]  * m[2][2] * m[3][3] -
                  m[1][1]  * m[3][2] * m[2][3] -
                  m[1][2]  * m[2][1]  * m[3][3] +
                  m[1][2]  * m[3][1]  * m[2][3] +
                  m[1][3] * m[2][1]  * m[3][2] -
                  m[1][3] * m[3][1]  * m[2][2];

        float b = -m[0][1]  * m[2][2] * m[3][3] +
                   m[0][1]  * m[3][2] * m[2][3] +
                   m[0][2]  * m[2][1]  * m[3][3] -
                   m[0][2]  * m[3][1]  * m[2][3] -
                   m[0][3] * m[2][1]  * m[3][2] +
                   m[0][3] * m[3][1]  * m[2][2];

        float c = m[0][1]  * m[1][2] * m[3][3] -
                  m[0][1]  * m[3][2] * m[1][3] -
                  m[0][2]  * m[1][1] * m[3][3] +
                  m[0][2]  * m[3][1] * m[1][3] +
                  m[0][3] * m[1][1] * m[3][2] -
                  m[0][3] * m[3][1] * m[1][2];

        float d = -m[0][1]  * m[1][2] * m[2][3] +
                   m[0][1]  * m[2][2] * m[1][3] +
                   m[0][2]  * m[1][1] * m[2][3] -
                   m[0][2]  * m[2][1] * m[1][3] -
                   m[0][3] * m[1][1] * m[2][2] +
                   m[0][3] * m[2][1] * m[1][2];
        float det = m[0][0] * a + m[1][0] * b + m[2][0] * c + m[3][0] * d;
        if (det == 0.0)
            return mat4(Diagonal);
        float a1 = -m[1][0]  * m[2][2] * m[3][3] +
                  m[1][0]  * m[3][2] * m[2][3] +
                  m[1][2]  * m[2][0] * m[3][3] -
                  m[1][2]  * m[3][0] * m[2][3] -
                  m[1][3] * m[2][0] * m[3][2] +
                  m[1][3] * m[3][0] * m[2][2];

        float b1 = m[0][0]  * m[2][2] * m[3][3] -
                 m[0][0]  * m[3][2] * m[2][3] -
                 m[0][2]  * m[2][0] * m[3][3] +
                 m[0][2]  * m[3][0] * m[2][3] +
                 m[0][3] * m[2][0] * m[3][2] -
                 m[0][3] * m[3][0] * m[2][2];

        float c1 = -m[0][0]  * m[1][2] * m[3][3] +
                  m[0][0]  * m[3][2] * m[1][3] +
                  m[0][2]  * m[1][0] * m[3][3] -
                  m[0][2]  * m[3][0] * m[1][3] -
                  m[0][3] * m[1][0] * m[3][2] +
                  m[0][3] * m[3][0] * m[1][2];

        float d1 = m[0][0]  * m[1][2] * m[2][3] -
                  m[0][0]  * m[2][2] * m[1][3] -
                  m[0][2]  * m[1][0] * m[2][3] +
                  m[0][2]  * m[2][0] * m[1][3] +
                  m[0][3] * m[1][0] * m[2][2] -
                  m[0][3] * m[2][0] * m[1][2];

        float a2  = m[1][0]  * m[2][1] * m[3][3] -
                 m[1][0]  * m[3][1] * m[2][3] -
                 m[1][1]  * m[2][0] * m[3][3] +
                 m[1][1]  * m[3][0] * m[2][3] +
                 m[1][3] * m[2][0] * m[3][1] -
                 m[1][3] * m[3][0] * m[2][1];

        float b2 = -m[0][0]  * m[2][1] * m[3][3] +
                  m[0][0]  * m[3][1] * m[2][3] +
                  m[0][1]  * m[2][0] * m[3][3] -
                  m[0][1]  * m[3][0] * m[2][3] -
                  m[0][3] * m[2][0] * m[3][1] +
                  m[0][3] * m[3][0] * m[2][1];

        float c2 = m[0][0]  * m[1][1] * m[3][3] -
                  m[0][0]  * m[3][1] * m[1][3] -
                  m[0][1]  * m[1][0] * m[3][3] +
                  m[0][1]  * m[3][0] * m[1][3] +
                  m[0][3] * m[1][0] * m[3][1] -
                  m[0][3] * m[3][0] * m[1][1];

        float d2 = -m[0][0]  * m[1][1] * m[2][3] +
                   m[0][0]  * m[2][1] * m[1][3] +
                   m[0][1]  * m[1][0] * m[2][3] -
                   m[0][1]  * m[2][0] * m[1][3] -
                   m[0][3] * m[1][0] * m[2][1] +
                   m[0][3] * m[2][0] * m[1][1];

        float a3 = -m[1][0] * m[2][1] * m[3][2] +
                  m[1][0] * m[3][1] * m[2][2] +
                  m[1][1] * m[2][0] * m[3][2] -
                  m[1][1] * m[3][0] * m[2][2] -
                  m[1][2] * m[2][0] * m[3][1] +
                  m[1][2] * m[3][0] * m[2][1];

        float b3 = m[0][0] * m[2][1] * m[3][2] -
                 m[0][0] * m[3][1] * m[2][2] -
                 m[0][1] * m[2][0] * m[3][2] +
                 m[0][1] * m[3][0] * m[2][2] +
                 m[0][2] * m[2][0] * m[3][1] -
                 m[0][2] * m[3][0] * m[2][1];

        float c3 = -m[0][0] * m[1][1] * m[3][2] +
                   m[0][0] * m[3][1] * m[1][2] +
                   m[0][1] * m[1][0] * m[3][2] -
                   m[0][1] * m[3][0] * m[1][2] -
                   m[0][2] * m[1][0] * m[3][1] +
                   m[0][2] * m[3][0] * m[1][1];

        float d3 = m[0][0] * m[1][1] * m[2][2] -
                  m[0][0] * m[2][1] * m[1][2] -
                  m[0][1] * m[1][0] * m[2][2] +
                  m[0][1] * m[2][0] * m[1][2] +
                  m[0][2] * m[1][0] * m[2][1] -
                  m[0][2] * m[2][0] * m[1][1];
        mat4 inv(vec4(a, b, c, d), vec4(a1, b1, c1, d1), vec4(a2, b2, c2, d2), vec4(a3, b3, c3, d3));
        inv /= det;
        inv += 0.0;
        return inv;
    }

    void print() {
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row)
                cout << m[row].get(col) << "\t";
            cout << endl;
        }
    }
};

#endif // VEC_MAT_MATHS_H
