#pragma once
#ifndef Delta_hpp__289aed8b50b443fb81e53d0402b2661d
#define Delta_hpp__289aed8b50b443fb81e53d0402b2661d 1

#include <cmath>
#include "Tristimulus.hpp"

namespace ColorSystem {
class Delta
{
  public:
    static const float UV(const Tristimulus &a_Yuv, const Tristimulus &b_Yuv) // a, b both are XYZ
    {
        return sqrtf((a_Yuv[1] - b_Yuv[1]) * (a_Yuv[1] - b_Yuv[1]) + (a_Yuv[2] - b_Yuv[2]) * (a_Yuv[2] - b_Yuv[2]));
    }
    static const float E76(const Tristimulus &a_LAB, const Tristimulus &b_LAB)
    {
        return sqrtf(
                (a_LAB[0] - b_LAB[0]) * (a_LAB[0] - b_LAB[0]) +
                (a_LAB[1] - b_LAB[1]) * (a_LAB[1] - b_LAB[1]) +
                (a_LAB[2] - b_LAB[2]) * (a_LAB[2] - b_LAB[2]));
    }
    static const float E00(const Tristimulus &lab1, const Tristimulus &lab2, const float &Kl = 1.f, const float &Kc = 1.f, const float &Kh = 1.f)
    {
        const float PI         = 3.14159265358979323846264338327950288f;
        const float L1         = lab1[0];
        const float a1         = lab1[1];
        const float b1         = lab1[2];
        const float L2         = lab2[0];
        const float a2         = lab2[1];
        const float b2         = lab2[2];
        const float Lbar       = (L1 + L2) / 2.f;
        const float C1         = sqrtf(a1 * a1 + b1 * b1);
        const float C2         = sqrtf(a2 * a2 + b2 * b2);
        const float Cbar       = (C1 + C2) / 2.f;
        const float C7         = powf(Cbar, 7.f);
        const float pow25_7    = 25.f * 25.f * 25.f * 25.f * 25.f * 25.f * 25.f;
        const float G          = (1.f - sqrtf(C7 / (C7 + pow25_7))) / 2.f;
        const float ad1        = a1 * (1.f + G);
        const float ad2        = a2 * (1.f + G);
        const float Cd1        = sqrtf(ad1 * ad1 + b1 * b1);
        const float Cd2        = sqrtf(ad2 * ad2 + b2 * b2);
        const float CdBar      = (Cd1 + Cd2) / 2.f;
        const float h1         = fmod(360.f + atan2f(b1, ad1) * 180.0f / PI, 360.f);
        const float h2         = fmod(360.f + atan2f(b2, ad2) * 180.0f / PI, 360.f);
        const float HdBar      = (fabs(h1 - h2) > 180.f ? (h1 + h2 + 360.f) : (h1 + h2)) / 2.f;
        const float T          = 1.f - 0.17f * cosf(PI * (1.f * HdBar - 30.f) / 180.f) + 0.24f * cosf(PI * (2.f * HdBar) / 180.f) + 0.32f * cosf(PI * (3.f * HdBar + 6.f) / 180.f) - 0.20f * cosf(PI * (4.f * HdBar - 63.f) / 180.f);
        const float deltah     = (fabs(h2 - h1) <= 180.f) ? h2 - h1 : ((h2 <= h1) ? h2 - h1 + 360.f : h2 - h1 - 360.f);
        const float deltaL     = L2 - L1;
        const float deltaC     = Cd2 - Cd1;
        const float deltaH     = 2.f * sqrtf(Cd1 * Cd2) * sinf(PI * deltah / (180.f * 2.f));
        const float Lbar2      = (Lbar - 50.f) * (Lbar - 50.f);
        const float Sl         = 1.f + 0.015f * Lbar2 / sqrtf(20.f + Lbar2);
        const float Sc         = 1.f + 0.045f * CdBar;
        const float Sh         = 1.f + 0.015f * CdBar * T;
        const float HdBar2     = (HdBar - 275.f) * (HdBar - 275.f) / (25.f * 25.f);
        const float deltaTheta = 30.f * expf(-HdBar2);
        const float CdBar7     = powf(CdBar, 7.f);
        const float Rc         = 2.f * sqrtf(CdBar7 / (CdBar7 + pow25_7));
        const float Rt         = -Rc * sinf(2.f * deltaTheta * PI / 180.f);
        const float dl         = deltaL / (Kl * Sl);
        const float dc         = deltaC / (Kc * Sc);
        const float dh         = deltaH / (Kh * Sh);
        return sqrtf(dl * dl + dc * dc + dh * dh + Rt * dc * dh);
    }
} ;

}
#endif /* Delta_hpp__289aed8b50b443fb81e53d0402b2661d */
