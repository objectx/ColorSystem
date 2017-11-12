//
// copyright 2017 (c) Hajime UCHIMURA / @nikq
// all rights reserved
//
#pragma once
#ifndef Gamut_hpp__c51f064858754f24b01c99f28de06abc
#define Gamut_hpp__c51f064858754f24b01c99f28de06abc 1

#include "Matrix.hpp"
#include "Tristimulus.hpp"

namespace ColorSystem {

class Gamut
{
  public:
    const char *name_;
    Matrix3     toXYZ_;
    Matrix3     fromXYZ_;

    static constexpr Matrix3 primMat(const float &xR, const float &yR, const float &xG, const float &yG, const float &xB, const float &yB)
    {
        return Matrix3(xR, xG, xB, yR, yG, yB, Tristimulus::z_from_xy(xR, yR), Tristimulus::z_from_xy(xG, yG), Tristimulus::z_from_xy(xB, yB));
    }
    static constexpr Matrix3 diag(const Vector3 &v)
    {
        return Matrix3(v[0], 0, 0, 0, v[1], 0, 0, 0, v[2]);
    }
    static constexpr Matrix3 mulDiag(const Matrix3 &m, const Vector3 &v)
    {
        return m.mul(diag(m.invert().apply(v)));
    }
    static constexpr Matrix3 fromPrimaries(const float &xR, const float &yR, const float &xG, const float &yG, const float &xB, const float &yB, const float &xW, const float &yW)
    {
        return mulDiag(primMat(xR, yR, xG, yG, xB, yB), Tristimulus::fromYxy(1.f, xW, yW).vec3());
    }
    constexpr Gamut(const char *name, const Matrix3 &fromXYZ) : name_(name), toXYZ_(fromXYZ.invert()), fromXYZ_(fromXYZ)
    {
        ;
    }
    constexpr Gamut(const char *name, const float &xR, const float &yR, const float &xG, const float &yG, const float &xB, const float &yB, const float &xW, const float &yW)
            : name_(name), toXYZ_(fromPrimaries(xR, yR, xG, yG, xB, yB, xW, yW)), fromXYZ_(fromPrimaries(xR, yR, xG, yG, xB, yB, xW, yW).invert())
    {
        ;
    }
    const char *name(void) const { return name_; }

    constexpr Matrix3     toXYZ(void) const { return toXYZ_; }
    constexpr Matrix3     fromXYZ(void) const { return fromXYZ_; }
    constexpr Tristimulus toXYZ(const Tristimulus &tri) const
    {
        return Tristimulus(toXYZ_.apply(tri.vec3()));
    }
    constexpr Tristimulus fromXYZ(const Tristimulus &tri) const
    {
        return Tristimulus(fromXYZ_.apply(tri.vec3()));
    }

    constexpr Vector3 primaryVector(void) const
    {
        return Vector3(
                toXYZ_[0] + toXYZ_[3] + toXYZ_[6],
                toXYZ_[1] + toXYZ_[4] + toXYZ_[7],
                toXYZ_[2] + toXYZ_[5] + toXYZ_[8]);
    }
    constexpr Matrix3 primaryMatrix(void) const
    {
        const Vector3 t(primaryVector());
        return Matrix3(
                toXYZ_[0] / t[0], toXYZ_[1] / t[1], toXYZ_[2] / t[2],
                toXYZ_[3] / t[0], toXYZ_[4] / t[1], toXYZ_[5] / t[2],
                toXYZ_[6] / t[0], toXYZ_[7] / t[1], toXYZ_[8] / t[2]);
    }
    constexpr Tristimulus primaryWhite() const
    {
        return Tristimulus(primaryMatrix().apply(primaryVector()));
    }
    constexpr Tristimulus primaryRed() const
    {
        const Matrix3 n(primaryMatrix());
        return Tristimulus(n[0], n[3], n[6]);
    }
    constexpr Tristimulus primaryGreen() const
    {
        const Matrix3 n(primaryMatrix());
        return Tristimulus(n[1], n[4], n[7]);
    }
    constexpr Tristimulus primaryBlue() const
    {
        const Matrix3 n(primaryMatrix());
        return Tristimulus(n[2], n[5], n[8]);
    }
} ;

//xR,yR,xG,yG,xB,yB,xW,yW
static constexpr Gamut AdobeRGB("AdobeRGB", 0.64f, 0.33f, 0.21f, 0.71f, 0.15f, 0.06f, 0.3127f, 0.3290f);
static constexpr Gamut Rec709("Rec.709", 0.64f, 0.33f, 0.30f, 0.60f, 0.15f, 0.06f, 0.3127f, 0.3290f);
static constexpr Gamut Rec2020("Rec.2020", 0.708f, 0.292f, 0.17f, 0.797f, 0.131f, 0.046f, 0.3127f, 0.3290f);
static constexpr Gamut DCI_P3("DCI P3", 0.68f, 0.32f, 0.265f, 0.69f, 0.15f, 0.06f, 0.314f, 0.351f);
static constexpr Gamut S_Gamut("S-Gamut", 0.73f, 0.28f, 0.14f, 0.855f, 0.10f, -0.05f, 0.3127f, 0.3290f);
static constexpr Gamut S_Gamut3_Cine("S-Gamut3.Cine", 0.766f, 0.275f, 0.225f, 0.800f, 0.089f, -0.087f, 0.3127f, 0.3290f);
static constexpr Gamut ACEScg("ACES cg", 0.713f, 0.293f, 0.165f, 0.830f, 0.128f, 0.044f, 0.32168f, 0.33767f);                   // AP1
static constexpr Gamut ACES2065("ACES 2065", 0.73470f, 0.26530f, 0.f, 1.f, 0.0001f, -0.077f, 0.32168f, 0.33767f);               // AP0
static constexpr Gamut LMS("LMS", Matrix3(0.8951f, 0.2664f, -0.1614f, -0.7502f, 1.7135f, 0.0367f, 0.0389f, -0.0685f, 1.0296f)); // fromXYZ matrix.
static constexpr Gamut XYZ("XYZ", Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1));

// returns Gamut convert matrix
static constexpr Matrix3 GamutConvert(const Gamut &src, const Gamut &dst)
{
    return dst.fromXYZ().mul(src.toXYZ());
}

// returns Bradford adaptation matrix
static constexpr Matrix3 Bradford(const Tristimulus &white_src, const Tristimulus &white_dst)
{
    const Tristimulus &lms_src(LMS.fromXYZ(white_src));
    const Tristimulus &lms_dst(LMS.fromXYZ(white_dst));
    const Matrix3      scale(
            Matrix3::diag(
                    Vector3(
                            lms_dst[0] / lms_src[0],
                            lms_dst[1] / lms_src[1],
                            lms_dst[2] / lms_src[2])));
    return LMS.toXYZ().mul(scale).mul(LMS.fromXYZ());
}

}
#endif /* Gamut_hpp__c51f064858754f24b01c99f28de06abc */
