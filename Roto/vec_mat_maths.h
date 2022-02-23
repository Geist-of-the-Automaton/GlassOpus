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

struct vec4 {
    float xyzw[4];

    vec4() {
        _X = _Y = _Z = 0.0;
        _W = 1.0;
    }

    vec4(float f) {
        _X = _Y = _Z = _W = f;
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

    vec4(vector<float> v) {
        for (int i = 0; i < 4; ++i)
            xyzw[i] = v[i];
    }

    vec4(vec4 *v) {
        _X = v->_X;
        _Y = v->_Y;
        _Z = v->_Z;
        _W = v->_W;
    }

    float operator * (const vec4 &v) {
        return _X * v._X + _Y * v._Y + _Z * v._Z + _W * v._W;
    }

    vec4 operator += (vec4 &v) {
        return *this + v;
    }

    vec4 operator -= (vec4 &v) {
        return *this - v;
    }

    vec4 operator + (const vec4 &v) {
        return vec4(_X + v._X, _Y + v._Y, _Z + v._Z, _W + v._W);
    }

    vec4 operator - (const vec4 &v) {
        return vec4(_X - v._X, _Y - v._Y, _Z - v._Z, _W - v._W);
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

    mat4(int initType) {
        float n = static_cast<float>(initType);
        for (int i = 0; i < 4; ++i) {
            if (initType == 2) {
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

    mat4(vec4 col1, vec4 col2, vec4 col3, vec4 col4) {
        m[0] = col1;
        m[1] = col2;
        m[2] = col3;
        m[3] = col4;
    }

    mat4(vector<vec4> v) {
        for (int i = 0; i < 4; ++i)
            m[i] = v[i];
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
            m[i] = vec4(M[i][0], M[i][1], M[i][2], 0.0);
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
        mat4 ret(0);
        for (int i = 0; i < 4; ++i) {
            vec4 temp(m[0][i], m[1][i], m[2][i], m[3][i]);
            ret.setCol(i, M * temp);
        }
        ret.transpose();
        return ret;
    }

    void operator *= (mat4 &M) {
        *this = *this * M;
    }

    void operator += (mat4 &M) {
        *this = *this + M;
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
        mat4 ret(0);
        for (int i = 0; i < 4; ++i) {
            vec4 temp(m[0][i], m[1][i], m[2][i], m[3][i]);
            ret.setCol(i, M * temp);
        }
        ret.transpose();
        return ret;
    }

    mat4 inverse() {
        return mat4(2);
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
