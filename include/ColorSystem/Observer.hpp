//
// copyright 2017 (c) Hajime UCHIMURA / @nikq
// all rights reserved
//
#pragma once
#ifndef Observer_hpp__aa9a852751704e0bbf4b8b5435921a69
#define Observer_hpp__aa9a852751704e0bbf4b8b5435921a69  1

#include "Spectrum.hpp"
#include "Tristimulus.hpp"

namespace ColorSystem {
class Observer
{
  public:
    Spectrum    X_, Y_, Z_;
    Tristimulus normalize_;
    constexpr Observer(const Spectrum &X, const Spectrum &Y, const Spectrum &Z) : X_(X), Y_(Y), Z_(Z), normalize_(1.f / X.sum(), 1.f / Y.sum(), 1.f / Z.sum()) { ; }
    static constexpr Tristimulus SpectrumIntegrate(const Spectrum &s, const Spectrum &x, const Spectrum &y, const Spectrum &z)
    {
        return Tristimulus(Spectrum::dot(s, x), Spectrum::dot(s, y), Spectrum::dot(s, z));
    }
    static constexpr Tristimulus SpectrumIntegrate3(const Spectrum &r, const Spectrum &l, const Spectrum &x, const Spectrum &y, const Spectrum &z)
    {
        return Tristimulus(Spectrum::dot3(r, l, x), Spectrum::dot3(r, l, y), Spectrum::dot3(r, l, z));
    }
    constexpr Tristimulus fromSpectrum(const Spectrum &s) const
    {
        return SpectrumIntegrate(s, X_, Y_, Z_) * normalize_;
    }
    constexpr Tristimulus fromReflectanceAndLight(const Spectrum &r, const Spectrum &l) const // r:reflectance, l:light
    {
        return SpectrumIntegrate3(r, l, X_, Y_, Z_) * normalize_;
    }
    constexpr float lumensFromMonochromaticFlux(const float &lambda, const float &watt) const // return lm
    {
        return Y_.fetch(lambda) * watt * 683.0f; // photoptic luminance efficiency
    }
    constexpr Tristimulus candellasFromMonochromaticRadiance(const float &lambda, const float &watt_per_sr_m2) const // return cd/m^2
    {
        return Tristimulus(X_.fetch(lambda), Y_.fetch(lambda), Z_.fetch(lambda)) * 683.0f * watt_per_sr_m2;
    }
} ;

// Standard observers
static constexpr Observer CIE1931(CIE1931_X, CIE1931_Y, CIE1931_Z);
static constexpr Observer CIE31JV(CIE1931_JuddVos_X, CIE1931_JuddVos_Y, CIE1931_JuddVos_Z);
static constexpr Observer CIE2012(CIE2012_X, CIE2012_Y, CIE2012_Z);

}
#endif /* Observer_hpp__aa9a852751704e0bbf4b8b5435921a69 */
