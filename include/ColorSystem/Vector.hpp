#pragma once
#ifndef Vector_hpp__3ad9e760201443538057b1eb45a0b8b8
#define Vector_hpp__3ad9e760201443538057b1eb45a0b8b8    1

#include <array>

namespace ColorSystem {

class Vector3
{
  public:
    typedef std::array<float, 3> vec3;
    vec3 v_;
    constexpr Vector3(const float a, const float b, const float c) : v_({a, b, c}) { ; }
    constexpr float operator[](const int &i) const
    {
        return v_[i];
    }
    constexpr float x() const { return v_[0]; }
    constexpr float y() const { return v_[1]; }
    constexpr float z() const { return v_[2]; }
    constexpr auto  size() const { return v_.size(); }
    auto            begin() const { return v_.begin(); }
    auto            end() const { return v_.end(); }
    constexpr float dot(const Vector3 &a) const
    {
        return v_[0] * a[0] + v_[1] * a[1] + v_[2] * a[2];
    }
    static constexpr float dot(const Vector3 &a, const Vector3 &b)
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
} ;

}
#endif /* Vector_hpp__3ad9e760201443538057b1eb45a0b8b8 */
