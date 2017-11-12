#pragma once
#ifndef Tristimulus_hpp__456dbe052ac84feaa9c8a7b36384e647
#define Tristimulus_hpp__456dbe052ac84feaa9c8a7b36384e647   1

#include "Vector.hpp"

namespace ColorSystem {

static const float PI = 3.14159265358979323846f;

class Tristimulus
{
  public:
    Vector3 v_;
    constexpr Tristimulus(const float &a, const float &b, const float &c) : v_(a, b, c)
    {
        ;
    }
    constexpr Tristimulus() : v_(0, 0, 0) { ; }
    constexpr Tristimulus(const Vector3 &v) : v_(v) { ; }
    constexpr Tristimulus(const float &v) : v_(v, v, v) { ; }
    constexpr float operator[](const int &i) const
    {
        return v_[i];
    }
    constexpr auto               size() const { return v_.size(); }
    auto                         begin() const { return v_.begin(); }
    auto                         end() const { return v_.end(); }
    constexpr const Vector3 &    vec3(void) const { return v_; }
    constexpr float              a() const { return v_[0]; }
    constexpr float              b() const { return v_[1]; }
    constexpr float              c() const { return v_[2]; }
    static constexpr Tristimulus scale(const Tristimulus &t, const float &s)
    {
        return Tristimulus(t[0] * s, t[1] * s, t[2] * s);
    }
    constexpr Tristimulus scale(const float &s) const
    {
        return scale(*this, s);
    }
    constexpr Tristimulus operator*(const float &s) const
    {
        return scale(s);
    }
    constexpr Tristimulus operator/(const float &s) const
    {
        return scale(1.f / s);
    }

    // apply color transform matrix
    static constexpr Tristimulus apply(const Tristimulus &t, const Matrix3 &m)
    {
        return Tristimulus(m.apply(t.vec3()));
    }
    constexpr Tristimulus apply(const Matrix3 &m) const
    {
        return apply(*this, m);
    }

    static constexpr Tristimulus add(const Tristimulus &a, const Tristimulus &b)
    {
        return Tristimulus(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
    }
    constexpr const Tristimulus add(const Tristimulus &b) const { return add(*this, b); }
    constexpr const Tristimulus operator+(const Tristimulus &b) const { return add(*this, b); }
    // per-element re-lighting
    static constexpr Tristimulus mul(const Tristimulus &a, const Tristimulus &b)
    {
        return Tristimulus(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
    }
    constexpr const Tristimulus mul(const Tristimulus &b) const { return mul(*this, b); }
    constexpr const Tristimulus operator*(const Tristimulus &b) const
    {
        return mul(*this, b);
    }
    static constexpr float dot(const Tristimulus &a, const Tristimulus &b)
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
    constexpr float dot(const Tristimulus &b) const
    {
        return dot(*this, b);
    }
    static constexpr const float mini(const float &a, const float &b) { return (a < b) ? a : b; }
    static constexpr const float maxi(const float &a, const float &b) { return (a > b) ? a : b; }
    static constexpr Tristimulus min(const Tristimulus &a, const Tristimulus &b)
    {
        return Tristimulus(mini(a[0], b[0]), mini(a[1], b[1]), mini(a[2], b[2]));
    }
    static constexpr Tristimulus max(const Tristimulus &a, const Tristimulus &b)
    {
        return Tristimulus(maxi(a[0], b[0]), maxi(a[1], b[1]), maxi(a[2], b[2]));
    }
    constexpr Tristimulus min(const Tristimulus &a) const { return min(*this, a); }
    constexpr Tristimulus max(const Tristimulus &a) const { return max(*this, a); }

    constexpr float min3(void) const { return mini(mini(a(), b()), c()); }
    constexpr float max3(void) const { return maxi(maxi(a(), b()), c()); }

    constexpr Tristimulus clip(const float &l, const float &h) const
    {
        return max(min(*this, Tristimulus(h)), Tristimulus(l));
    }
    static constexpr Tristimulus clip(const Tristimulus &t, const float &l, const float &h) { return t.clip(l, h); }
    constexpr Tristimulus positive() const
    {
        return max(*this, Tristimulus(0.f));
    }
    static constexpr Tristimulus positive(const Tristimulus &t) { return t.positive(); }
    constexpr bool isNegative(const float &a) const
    {
        return (a < 0.f);
    }
    constexpr bool hasNegative() const
    {
        return isNegative(v_[0]) || isNegative(v_[1]) || isNegative(v_[2]);
    }
    static constexpr bool hasNegative(const Tristimulus &t)
    {
        return t.hasNegative();
    }
    static constexpr float z_from_xy(const float &x, const float &y) { return 1 - x - y; }
    static constexpr float X_from_Yxy(const float &Y, const float &x, const float &y) { return (Y < 1e-8f) ? 0.f : x * Y / y; }
    static constexpr float Y_from_Yxy(const float &Y, const float &x, const float &y)
    {
        (void)x;
        (void)y;
        return (Y < 1e-8f) ? 0.f : Y;
    }
    static constexpr float Z_from_Yxy(const float &Y, const float &x, const float &y) { return (Y < 1e-8f) ? 0.f : z_from_xy(x, y) * Y / y; }
    static constexpr float Y_from_XYZ(const float &x, const float &y, const float &z)
    {
        (void)x;
        (void)z;
        return (y < 1e-8f) ? 0.f : y;
    }
    static constexpr float x_from_XYZ(const float &x, const float &y, const float &z) { return (y < 1e-8f) ? 0.3127f : x / (x + y + z); }
    static constexpr float y_from_XYZ(const float &x, const float &y, const float &z) { return (y < 1e-8f) ? 0.3290f : y / (x + y + z); }
    static constexpr Tristimulus fromYxy(const float &Y, const float &x, const float &y)
    {
        return Tristimulus(X_from_Yxy(Y, x, y), Y_from_Yxy(Y, x, y), Z_from_Yxy(Y, x, y));
    }

    static constexpr Tristimulus toYxy(const float &X, const float &Y, const float &Z)
    {
        return Tristimulus(Y_from_XYZ(X, Y, Z), x_from_XYZ(X, Y, Z), y_from_XYZ(X, Y, Z));
    }
    static constexpr Tristimulus fromYxy(const Tristimulus &Yxy) { return fromYxy(Yxy[0], Yxy[1], Yxy[2]); }
    static constexpr Tristimulus toYxy(const Tristimulus &XYZ) { return toYxy(XYZ[0], XYZ[1], XYZ[2]); }
    //
    constexpr Tristimulus fromYxy(void) const { return fromYxy(*this); }
    constexpr Tristimulus toYxy(void) const { return toYxy(*this); }
    // support u'v'
    //  u = 4x / (-2x + 12y + 3)
    //  v = 6y / (-2x + 12y + 3)
    //  x = 3u / (2u - 8v + 4)
    //  y = 2v / (2u - 8v + 4)
    //  u' = 4x / (-2x + 12y + 3)    [ = u ]
    //  v' = 9y / (-2x + 12y + 3)    [ = 1.5v ]
    //  x = 9u' / (6u' - 16v' + 12)
    //  y = 4v' / (6u' - 16v' + 12)
    static constexpr float u_from_xy(const float &x, const float &y) { return 4.f * x / (-2.f * x + 12.f * y + 3.f); }
    static constexpr float v_from_xy(const float &x, const float &y) { return 6.f * y / (-2.f * x + 12.f * y + 3.f); }
    static constexpr float x_from_uv(const float &u, const float &v) { return 3.f * u / (2.f * u - 8.f * v + 4.f); }
    static constexpr float y_from_uv(const float &u, const float &v) { return 2.f * v / (2.f * u - 8.f * v + 4.f); }
    static constexpr Tristimulus YxyToYuv(const Tristimulus &Yxy) { return Tristimulus(Yxy[0], u_from_xy(Yxy[1], Yxy[2]), v_from_xy(Yxy[1], Yxy[2])); }
    static constexpr Tristimulus YuvToYxy(const Tristimulus &Yuv) { return Tristimulus(Yuv[0], x_from_uv(Yuv[1], Yuv[2]), y_from_uv(Yuv[1], Yuv[2])); }
    static constexpr Tristimulus toYuv(const Tristimulus &XYZ)
    {
        return YxyToYuv(toYxy(XYZ));
    }
    static constexpr Tristimulus toYuv(const float &X, const float &Y, const float &Z)
    {
        return toYuv(Tristimulus(X, Y, Z));
    }
    static constexpr Tristimulus fromYuv(const Tristimulus &Yuv)
    {
        return fromYxy(YuvToYxy(Yuv));
    }
    static constexpr Tristimulus fromYuv(const float &X, const float &Y, const float &Z)
    {
        return fromYuv(Tristimulus(X, Y, Z));
    }
    constexpr Tristimulus toYuv(void) const { return toYuv(*this); }
    constexpr Tristimulus fromYuv(void) const { return fromYuv(*this); }
    // Lab
    static constexpr float CIELAB_curve(const float &f)
    {
        const float threshold = 216.f / 24389.0f;
        const float K         = 24389.0f / 27.0f;
        return (f > 1.f) ? 1.f : ((f > threshold) ? powf(f, 1.f / 3.f) : ((K * f + 16.f) / 116.f));
    }
    static constexpr float CIELAB_decurve(const float &f)
    {
        const float K = (3.f / 29.f) * (3.f / 29.f) * (3.f / 29.f);
        return (f > 1.f) ? 1.f : ((f > 6.f / 29.f) ? powf(f, 3.f) : (116.f * f - 16.f) * K);
    }
    static constexpr Tristimulus toCIELAB(const Tristimulus &t, const Tristimulus &white)
    {
        const float x0 = white[0];
        const float y0 = white[1];
        const float z0 = white[2];
        const float x1 = CIELAB_curve(t[0] / x0);
        const float y1 = CIELAB_curve(t[1] / y0);
        const float z1 = CIELAB_curve(t[2] / z0);
        return Tristimulus(116.f * y1 - 16.f, 500.f * (x1 - y1), 200.f * (y1 - z1));
    }
    static constexpr Tristimulus fromCIELAB(const Tristimulus &t, const Tristimulus &white)
    {
        const float x0 = white[0];
        const float y0 = white[1];
        const float z0 = white[2];
        const float fy = (t[0] + 16.f) / 116.f;
        const float fx = fy + (t[1] / 500.f);
        const float fz = fy - (t[2] / 200.f);
        return Tristimulus(CIELAB_decurve(fx) * x0, CIELAB_decurve(fy) * y0, CIELAB_decurve(fz) * z0);
    }
    constexpr Tristimulus toCIELAB(const Tristimulus &white) const { return toCIELAB(*this, white); }
    constexpr Tristimulus fromCIELAB(const Tristimulus &white) const { return fromCIELAB(*this, white); }
    // CIELAB uses D50 by default.
    constexpr Tristimulus toCIELAB(void) const { return toCIELAB(*this, Tristimulus(0.9642f, 1.0f, 0.8249f)); }
    constexpr Tristimulus fromCIELAB(void) const { return fromCIELAB(*this, Tristimulus(0.9642f, 1.0f, 0.8249f)); }
    //HSV
    static constexpr float mod360(const float &r)
    {
        return (r < 0.f) ? mod360(r + 360.f) : ((r > 360.f) ? mod360(r - 360.f) : r);
    }
    static Tristimulus toHSV_atan(const Tristimulus &t)
    {
        const float max = maxi(maxi(t[0], t[1]), t[2]);
        const float min = mini(mini(t[0], t[1]), t[2]);
        return Tristimulus(
                mod360((180.f / PI) * atan2(sqrtf(3.f) * (t[1] - t[2]), 2.f * t[0] - t[1] - t[2])),
                (max == 0.f) ? 0.f : (max - min) / max,
                max);
    }
    Tristimulus                  toHSV_atan(void) const { return toHSV_atan(*this); }
    static constexpr Tristimulus toHSV(const Tristimulus &t)
    {
        const float max = maxi(maxi(t[0], t[1]), t[2]);
        const float min = mini(mini(t[0], t[1]), t[2]);
        const float d   = max - min;
        return Tristimulus(
                mod360(
                        ((max == min) ? 0.f : ((max == t[0]) ? (60.f * (t[1] - t[2]) / d) : ((max == t[1]) ? (60.f * (t[2] - t[0]) / d + 120.f) : (60.f * (t[0] - t[1]) / d + 240.f))))),
                (max == 0.f) ? 0.f : d / max,
                max);
    }

    static constexpr Tristimulus fromHSV(const Tristimulus &t)
    {
        const float h = mod360(t[0]);
        const int   d = (int)(h / 60.f);
        const int   i = d % 6;
        const float r = (h / 60.f) - d;

        const float s = t[1];
        const float v = t[2];
        const float m = v * (1.0f - s);
        const float n = v * (1.0f - s * r);
        const float p = v * (1.0f - s * (1.0f - r));

        return (i == 0) ? Tristimulus(v, p, m) : ((i == 1) ? Tristimulus(n, v, m) : ((i == 2) ? Tristimulus(m, v, p) : ((i == 3) ? Tristimulus(m, n, v) : ((i == 4) ? Tristimulus(p, m, v) : ((i == 5) ? Tristimulus(v, m, n) : Tristimulus(0, 0, 0))))));
    }

    constexpr Tristimulus toHSV(void) const { return toHSV(*this); }
    constexpr Tristimulus fromHSV(void) const { return fromHSV(*this); }
} ;

//standard illuminants
static constexpr Tristimulus Illuminant_A(1.09850f, 1.f, 0.35585f);
static constexpr Tristimulus Illuminant_B(1.99072f, 1.f, 0.85223f);
static constexpr Tristimulus Illuminant_C(0.98074f, 1.0f, 1.18232f);
static constexpr Tristimulus Illuminant_D50(0.96422f, 1.0f, 0.82521f);
static constexpr Tristimulus Illuminant_D55(0.95682f, 1.0f, 0.92149f);
static constexpr Tristimulus Illuminant_D60(0.95265f, 1.0f, 1.00883f); // by ACES TB-2014-004.pdf
static constexpr Tristimulus Illuminant_D65(0.95047f, 1.0f, 1.08883f);
static constexpr Tristimulus Illuminant_D75(0.94972f, 1.0f, 1.22638f);
static constexpr Tristimulus Illuminant_E(1.f, 1.f, 1.f);
static constexpr Tristimulus Illuminant_F2(0.99186f, 1.f, 0.67393f);
static constexpr Tristimulus Illuminant_F7(0.95041f, 1.f, 1.08747f);
static constexpr Tristimulus Illuminant_F11(1.00962f, 1.f, 0.64350f);

// cubic spline approx https://en.wikipedia.org/wiki/Planckian_locus#Approximation
static constexpr Tristimulus PlanckianLocus(const float &T, const float &Y = 1.f)
{
    const float x = (T < 4000.f) ? ((-0.2661239e9f / (T * T * T)) - (0.2343580e6f / (T * T)) + (0.8776956e3f / T) + 0.179910f)
                                 : ((-3.0258469e9f / (T * T * T)) + (2.1070379e6f / (T * T)) + (0.2226347e3f / T) + 0.240390f);
    const float y = (T < 2222.f) ? ((-1.1063814f * x * x * x) - (1.34811020f * x * x) + (2.18555832f * x) - 0.20219683f)
                                 : ((T < 4000.f) ? ((-1.1063814f * x * x * x - 1.34811020f * x * x + 2.18555832f * x - 0.20219683f))
                                                 : ((3.0817580f * x * x * x - 5.87338670f * x * x + 3.75112997f * x - 0.37001483f)));
    return Tristimulus(Y, x, y).fromYxy();
}

}
#endif /* Tristimulus_hpp__456dbe052ac84feaa9c8a7b36384e647 */
