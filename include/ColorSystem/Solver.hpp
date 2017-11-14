//
// copyright 2017 (c) Hajime UCHIMURA / @nikq
// all rights reserved
//
#pragma once
#ifndef Solver_hpp__4bc40c38b3a7429f99feb1f71851f659
#define Solver_hpp__4bc40c38b3a7429f99feb1f71851f659    1

#include <cmath>
#include <vector>

namespace ColorSystem {
namespace SOLVER
{
/*
* svdcomp - SVD decomposition routine.
* Takes an mxn matrix a and decomposes it into udv, where u,v are
* left and right orthogonal transformation matrices, and d is a
* diagonal matrix of singular values.
*
* This routine is adapted from svdecomp.c in XLISP-STAT 2.1 which is
* code from Numerical Recipes adapted by Luke Tierney and David Betz.
*
* Input to dsvd is as follows:
*   a = mxn matrix to be decomposed, gets overwritten with u
*   m = row dimension of a
*   n = column dimension of a
*   w = returns the vector of singular values of a
*   v = returns the right orthogonal transformation matrix
*/

class Vector
{
  public:
    std::vector<float> v_;

    Vector() { ; }
    Vector(int d) : v_(d) { ; }
    float &operator[](int i) { return v_[i]; }

    const float *data(void) { return v_.data(); }
};

class Matrix
{
  public:
    std::vector<std::vector<float>> v_;
    Matrix() { ; }
    Matrix(int r, int c)
    {
        v_.resize(r);
        for (auto &v : v_)
            v.resize(c);
    }
    const int rows(void) { return (int)(v_.size()); }
    const int cols(void) { return (int)(v_.size() ? v_[0].size() : 0); }
    float &v(int r, int c) { return v_[r][c]; }
    std::vector<float> &operator[](int r) { return v_[r]; }
};

static inline float DSIGN(float a, float b)
{
    return (b > 0.f) ? fabs(a) : -fabs(a);
}
static inline float PYTHAG(float a, float b)
{
    float at = fabs(a);
    float bt = fabs(b);
    if (at > bt)
    {
        float ct = bt / at;
        return at * sqrtf(1.0f + ct * ct);
    }
    else if (bt > 0.0)
    {
        float ct = at / bt;
        return bt * sqrtf(1.0f + ct * ct);
    }
    return 0.f;
}

static int svdcmp(Matrix &a, Vector &w, Matrix &v)
{
    int   flag, its, j, jj, k, l, nm;
    float c, f, h, s, x, y, z;
    float anorm = 0.0, g = 0.0, scale = 0.0;

    int m = a.rows();
    int n = a.cols();

    if (m < n)
    {
        return (-1);
    }

    Vector rv1(n);

    /* Householder reduction to bidiagonal form */
    for (int i = 0; i < n; i++)
    {
        /* left-hand reduction */
        l      = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i < m)
        {
            for (k = i; k < m; k++)
                scale += fabs(a[k][i]);
            if (scale)
            {
                for (k = i; k < m; k++)
                {
                    a[k][i] = (a[k][i] / scale);
                    s += (a[k][i] * a[k][i]);
                }
                f       = a[i][i];
                g       = -DSIGN(sqrt(s), f);
                h       = f * g - s;
                a[i][i] = (f - g);
                if (i != n - 1)
                {
                    for (j = l; j < n; j++)
                    {
                        for (s = 0.0, k = i; k < m; k++)
                            s += (a[k][i] * a[k][j]);
                        f = s / h;
                        for (k = i; k < m; k++)
                            a[k][j] += (f * a[k][i]);
                    }
                }
                for (k      = i; k < m; k++)
                    a[k][i] = (a[k][i] * scale);
            }
        }
        w[i] = (scale * g);

        /* right-hand reduction */
        g = s = scale = 0.0;
        if (i < m && i != n - 1)
        {
            for (k = l; k < n; k++)
                scale += fabs(a[i][k]);
            if (scale)
            {
                for (k = l; k < n; k++)
                {
                    a[i][k] = (a[i][k] / scale);
                    s += (a[i][k] * a[i][k]);
                }
                f       = a[i][l];
                g       = -DSIGN(sqrt(s), f);
                h       = f * g - s;
                a[i][l] = (f - g);
                for (k     = l; k < n; k++)
                    rv1[k] = a[i][k] / h;
                if (i != m - 1)
                {
                    for (j = l; j < m; j++)
                    {
                        for (s = 0.0, k = l; k < n; k++)
                            s += (a[j][k] * a[i][k]);
                        for (k = l; k < n; k++)
                            a[j][k] += (s * rv1[k]);
                    }
                }
                for (k      = l; k < n; k++)
                    a[i][k] = (a[i][k] * scale);
            }
        }
        anorm = std::max<float>(anorm, (fabs(w[i]) + fabs(rv1[i])));
    }

    /* accumulate the right-hand transformation */
    for (int i = n - 1; i >= 0; i--)
    {
        if (i < n - 1)
        {
            if (g)
            {
                for (j      = l; j < n; j++)
                    v[j][i] = ((a[i][j] / a[i][l]) / g);
                /* float division to avoid underflow */
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < n; k++)
                        s += (a[i][k] * v[k][j]);
                    for (k = l; k < n; k++)
                        v[k][j] += (s * v[k][i]);
                }
            }
            for (j      = l; j < n; j++)
                v[i][j] = v[j][i] = 0.0;
        }
        v[i][i] = 1.0;
        g       = rv1[i];
        l       = i;
    }

    /* accumulate the left-hand transformation */
    for (int i = n - 1; i >= 0; i--)
    {
        l = i + 1;
        g = w[i];
        if (i < n - 1)
            for (j      = l; j < n; j++)
                a[i][j] = 0.0;
        if (g)
        {
            g = 1.0f / g;
            if (i != n - 1)
            {
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < m; k++)
                        s += (a[k][i] * a[k][j]);
                    f = (s / a[i][i]) * g;
                    for (k = i; k < m; k++)
                        a[k][j] += (f * a[k][i]);
                }
            }
            for (j      = i; j < m; j++)
                a[j][i] = (a[j][i] * g);
        }
        else
        {
            for (j      = i; j < m; j++)
                a[j][i] = 0.0;
        }
        ++a[i][i];
    }

    /* diagonalize the bidiagonal form */
    for (k = n - 1; k >= 0; k--)
    { /* loop over singular values */
        for (its = 0; its < 30; its++)
        { /* loop over allowed iterations */
            flag = 1;
            for (l = k; l >= 0; l--)
            { /* test for splitting */
                nm = l - 1;
                if (fabs(rv1[l]) + anorm == anorm)
                {
                    flag = 0;
                    break;
                }
                if (fabs(w[nm]) + anorm == anorm)
                    break;
            }
            if (flag)
            {
                c = 0.0;
                s = 1.0;
                for (int i = l; i <= k; i++)
                {
                    f = s * rv1[i];
                    if (fabs(f) + anorm != anorm)
                    {
                        g    = w[i];
                        h    = PYTHAG(f, g);
                        w[i] = h;
                        h    = 1.0f / h;
                        c    = g * h;
                        s    = (-f * h);
                        for (j = 0; j < m; j++)
                        {
                            y        = a[j][nm];
                            z        = a[j][i];
                            a[j][nm] = (y * c + z * s);
                            a[j][i]  = (z * c - y * s);
                        }
                    }
                }
            }
            z = w[k];
            if (l == k)
            { /* convergence */
                if (z < 0.0)
                { /* make singular value nonnegative */
                    w[k] = (-z);
                    for (j      = 0; j < n; j++)
                        v[j][k] = (-v[j][k]);
                }
                break;
            }
            if (its >= 30)
            {
                return (-1);
            }

            /* shift from bottom 2 x 2 minor */
            x  = w[l];
            nm = k - 1;
            y  = w[nm];
            g  = rv1[nm];
            h  = rv1[k];
            f  = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0f * h * y);
            g  = PYTHAG(f, 1.0f);
            f  = ((x - z) * (x + z) + h * ((y / (f + DSIGN(g, f))) - h)) / x;

            /* next QR transformation */
            c = s = 1.0;
            for (j = l; j <= nm; j++)
            {
                int i  = j + 1;
                g      = rv1[i];
                y      = w[i];
                h      = s * g;
                g      = c * g;
                z      = PYTHAG(f, h);
                rv1[j] = z;
                c      = f / z;
                s      = h / z;
                f      = x * c + g * s;
                g      = g * c - x * s;
                h      = y * s;
                y      = y * c;
                for (jj = 0; jj < n; jj++)
                {
                    x        = v[jj][j];
                    z        = v[jj][i];
                    v[jj][j] = (x * c + z * s);
                    v[jj][i] = (z * c - x * s);
                }
                z    = PYTHAG(f, h);
                w[j] = z;
                if (z)
                {
                    z = 1.0f / z;
                    c = f * z;
                    s = h * z;
                }
                f = (c * g) + (s * y);
                x = (c * y) - (s * g);
                for (jj = 0; jj < m; jj++)
                {
                    y        = a[jj][j];
                    z        = a[jj][i];
                    a[jj][j] = (y * c + z * s);
                    a[jj][i] = (z * c - y * s);
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k]   = x;
        }
    }
    return 1;
}

static void svbksb(Matrix &u, Vector &w, Matrix &v, Vector &b, Vector &x)
{
    int m = u.rows();
    int n = u.cols();

    Vector tmp(n);
    for (int j = 0; j < n; j++)
    {
        float s = 0.0;
        if (w[j])
        {
            for (int i = 0; i < m; i++)
                s += u[i][j] * b[i];
            s /= w[j];
        }
        tmp[j] = s;
    }
    for (int j = 0; j < n; j++)
    {
        float s = 0.0;
        for (int jj = 0; jj < n; jj++)
            s += v[j][jj] * tmp[jj];
        x[j] = (float)s;
    }
}

// solve x for Ax=b.
static int solve(Matrix A, Vector b, Vector &x, float TOL = 1e-5f)
{
    //Matrix TMP(rows, cols);
    int    rows = A.rows();
    int    cols = A.cols();
    Matrix V(cols, cols);
    Vector W(cols);
    float  wmax, thresh;

    int r = svdcmp(A, W, V);
    if (r == 0)
    {
        return r;
    }

    wmax = 0.0;
    for (int j = 0; j < cols; j++)
    {
        if (W[j] > wmax)
            wmax = W[j];
    }

    thresh = TOL * wmax;
    for (int j = 0; j < cols; j++)
    {
        if (W[j] < thresh)
            W[j] = 0.0;
    }

    svbksb(A, W, V, b, x);

    return 0;
}
}

}
#endif /* Solver_hpp__4bc40c38b3a7429f99feb1f71851f659 */
