#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
    this->r1 = r1;
    this->r2 = r2;
    this->r3 = r3;
    this->r4 = r4;
}

Float4& Matrix::operator[](int idx) {
    assert(idx>=0 && idx<=3);
    if(idx == 0)
        return this->r1;
    else if(idx == 1)
        return this->r2;
    else if(idx == 2)
        return this->r3;
    else
        return this->r4;
}

Float4 Matrix::operator[](int idx) const {
    assert(idx>=0 && idx<=3);
    if(idx == 0)
        return this->r1;
    else if(idx == 1)
        return this->r2;
    else if(idx == 2)
        return this->r3;
    else
        return this->r4;
}

Matrix Matrix::operator+(const Matrix& b) const {
    Matrix result;
    result[0] = this->r1 + b[0];
    result[1] = this->r2 + b[1];
    result[2] = this->r3 + b[2];
    result[3] = this->r4 + b[3];
    return result;
}

Matrix Matrix::operator-(const Matrix& b) const {
    Matrix result;
    result[0] = this->r1 - b[0];
    result[1] = this->r2 - b[1];
    result[2] = this->r3 - b[2];
    result[3] = this->r4 - b[3];
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result;
    result[0] = Float4(this->r1[0], this->r2[0], this->r3[0], this->r4[0]);
    result[1] = Float4(this->r1[1], this->r2[1], this->r3[1], this->r4[1]);
    result[2] = Float4(this->r1[2], this->r2[2], this->r3[2], this->r4[2]);
    result[3] = Float4(this->r1[3], this->r2[3], this->r3[3], this->r4[3]);
    return result;
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    const Matrix& m = *this;
    for(int r=0; r<=3; r++) {
        if(m[r] != b[r])
            return false;
    }
    return true;
}

bool Matrix::operator!=(const Matrix& b) const {
    const Matrix& m = *this;
    if(m==b)
        return false;
    else
        return true;
}

Matrix product(const Matrix& a, const Matrix& b) {
    Matrix result = Matrix::zero();
    Matrix bt = b.transpose();
    for(int i=0; i<=3; i++) {
        for(int j=0; j<=3; j++) {
            result[i][j] = dot(a[i], bt[j]);
        }
    }
    return result;
}

Matrix operator*(const Matrix& a, float scalar) {
    Matrix result;
    for(int r=0; r<=3; r++) {
        result[r] = scalar * a[r];
    }
    return result;
}

Matrix operator*(float scalar, const Matrix& a) {
    Matrix result;
    for(int r=0; r<=3; r++) {
        result[r] = a[r] * scalar;
    }
    return result;
}

Float4 Matrix::operator*(const Float4& b) const {
    float f1 = dot(this->r1, b);
    float f2 = dot(this->r2, b);
    float f3 = dot(this->r3, b);
    float f4 = dot(this->r4, b);
    return (Float4(f1, f2, f3, f4));
}

Vector Matrix::operator*(const Vector& b) const {
    Float4 f = Float4(b);
    float f1 = dot(this->r1, f);
    float f2 = dot(this->r2, f);
    float f3 = dot(this->r3, f);
    return (Vector(f1, f2, f3));
}

Point Matrix::operator*(const Point& b) const {
    Float4 f = Float4(b);
    float p1 = dot(this->r1, f);
    float p2 = dot(this->r2, f);
    float p3 = dot(this->r3, f);
    float p4 = dot(this->r4, f);
    return (Point(p1/p4, p2/p4, p3/p4)); // normalize
}

float Matrix::det() const {
    Matrix result;
    const Matrix& m = *this;

    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    return det;
}

Matrix Matrix::zero() {
    Float4 f = Float4::rep(0.f);
    return (Matrix(f, f, f, f));
}

Matrix Matrix::identity() {
    Float4 r1(1.f, 0.f, 0.f, 0.f);
    Float4 r2(0.f, 1.f, 0.f, 0.f);
    Float4 r3(0.f, 0.f, 1.f, 0.f);
    Float4 r4(0.f, 0.f, 0.f, 1.f);
    return (Matrix(r1, r2, r3, r4));
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    Matrix m = {
        Float4(e1.x, e2.x, e3.x, 0.f),
        Float4(e1.y, e2.y, e3.y, 0.f),
        Float4(e1.z, e2.z, e3.z, 0.f),
        Float4(0.f , 0.f , 0.f , 1.f)
    };
    return m;
}

}
