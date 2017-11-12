#pragma once
#ifndef OTF_hpp__2b9f070e5256418a89a5d649c5a761ba
#define OTF_hpp__2b9f070e5256418a89a5d649c5a761ba   1

#include <cmath>
#include "Tristimulus.hpp"

namespace ColorSystem {

class OTF
{
  public:
    typedef enum {
        LINEAR,
        GAMMA, // simplest gamma
        SRGB,
        BT709,
        ST2084,
        SLOG2,
        // OTF_HLG // Hybrid-log-gamma
    } TYPE;

    static float gamma(const float &v, const float &g) { return powf(v, 1.f / g); }
    static float degamma(const float &v, const float &g) { return powf(v, g); }
    static const float ST2084_to_Y(const float &pixel) // pixel should be 0-1
    {
        const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
        const float pq_m2 = 78.84375;        // ( 2523.0 / 4096.0 ) * 128.0;
        const float pq_c1 = 0.8359375;       // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
        const float pq_c2 = 18.8515625;      // ( 2413.0 / 4096.0 ) * 32.0;
        const float pq_c3 = 18.6875;         // ( 2392.0 / 4096.0 ) * 32.0;
        const float pq_C  = 10000.0;

        // Note that this does NOT handle any of the signal range
        // considerations from 2084 - this assumes full range (0 - 1)
        float Np = powf(pixel, 1.0f / pq_m2);
        float L  = Np - pq_c1;
        if (L < 0.0)
            L = 0.0;
        L     = L / (pq_c2 - pq_c3 * Np);
        L     = powf(L, 1.0f / pq_m1);
        return L * pq_C; // returns cd/m^2
    }
    static const float Y_to_ST2084(const float &nit) // nit should be 0-10000(cd/m^2)
    {
        if (nit <= 0.f)
            return 0.f;
        const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
        const float pq_m2 = 78.84375;        // ( 2523.0 / 4096.0 ) * 128.0;
        const float pq_c1 = 0.8359375;       // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
        const float pq_c2 = 18.8515625;      // ( 2413.0 / 4096.0 ) * 32.0;
        const float pq_c3 = 18.6875;         // ( 2392.0 / 4096.0 ) * 32.0;
        const float pq_C  = 10000.0;

        // Note that this does NOT handle any of the signal range
        // considerations from 2084 - this returns full range (0 - 1)
        float L  = nit / pq_C;
        float Lm = powf(L, pq_m1);
        float N  = (pq_c1 + pq_c2 * Lm) / (1.0f + pq_c3 * Lm);
        N        = powf(N, pq_m2);
        return N;
    }
    static const float Y_to_sRGB(const float &nits) // returns signal, 0-1, input nits [0-100]
    {
        const float C = nits / 100.f;
        return (C < 0.0031308f) ? C * 12.92f : (1.055f * powf(C, 1.0f / 2.4f) - 0.055f);
    }
    static const float sRGB_to_Y(const float &C) // returns nits, 0-100[cd/m^2]
    {
        return (C < 0.04045f) ? C / 12.92f : powf((C + 0.055f) / 1.055f, 2.4f) * 100.f;
    }
    static const float Y_to_BT709(const float &nits) // returns signal, 0-1, input nits [0-100]
    {
        const float C = nits / 100.f;
        return (C < 0.018f) ? C * 4.50f : (1.099f * powf(C, 0.45f) - 0.099f);
    }
    static const float BT709_to_Y(const float &C) // returns nits, 0-100[cd/m^2]
    {
        return (C < 0.081f) ? C / 4.50f : powf((C + 0.099f) / 1.099f, 1.f / 0.45f) * 100.f;
    }

    static const float CV_to_IRE_SLog2(const float &cv)
    {
        const float BLACK = 64.f / 1024.f;
        const float WV    = 876.f / 1024.f; // 940-64
        return (cv - BLACK) / WV;
    }
    static const float IRE_to_CV_SLog2(const float &ire)
    {
        const float BLACK = 64.f / 1024.f;
        const float WV    = 876.f / 1024.f; // 940-64
        return (ire * WV) + BLACK;
    }
    static const float Y_to_SLog2(const float &nits) // returns signal, 0-1, input nits [0-100]
    {
        const float x = nits / 100.f;
        const float y = (x < 0.f) ? x * 3.53881278538813f + 0.030001222851889303f : (0.432699f * log10(155.0f * x / 219.0f + 0.037584f) + 0.616596f) + 0.03f;
        return IRE_to_CV_SLog2(y);
    }
    static const float SLog2_to_Y(const float &C) // returns nits, 0-100[cd/m^2]
    {
        const float x = CV_to_IRE_SLog2(C);
        const float y = (x >= 0.030001222851889303f) ? 219.0f * (pow(10.0f, ((x - 0.616596f - 0.03f) / 0.432699f)) - 0.037584f) / 155.0f : (x - 0.030001222851889303f) / 3.53881278538813f;
        return (y > 0.f) ? y * 100.f : 0.f;
    }
    static const Tristimulus toScreen(TYPE type, const Tristimulus &scene, const float g = 1.f)
    {
        switch (type)
        {
            case GAMMA:
            {
                return Tristimulus(gamma(scene[0], g), gamma(scene[1], g), gamma(scene[2], g));
            }
                break;
            case SRGB:
            {
                return Tristimulus(Y_to_sRGB(scene[0]), Y_to_sRGB(scene[1]), Y_to_sRGB(scene[2]));
            }
                break;
            case BT709:
            {
                return Tristimulus(Y_to_BT709(scene[0]), Y_to_BT709(scene[1]), Y_to_BT709(scene[2]));
            }
                break;
            case ST2084:
            {
                return Tristimulus(Y_to_ST2084(scene[0]), Y_to_ST2084(scene[1]), Y_to_ST2084(scene[2]));
            }
                break;
            case SLOG2:
            {
                return Tristimulus(Y_to_SLog2(scene[0]), Y_to_SLog2(scene[1]), Y_to_SLog2(scene[2]));
            }
                break;
            case LINEAR:
            default:
                return scene;
        }
    }
    static const Tristimulus toScene(TYPE type, const Tristimulus &screen, const float g = 1.f)
    {
        switch (type)
        {
            case GAMMA:
            {
                return Tristimulus(degamma(screen[0], g), degamma(screen[1], g), degamma(screen[2], g));
            }
                break;
            case SRGB:
            {
                return Tristimulus(sRGB_to_Y(screen[0]), sRGB_to_Y(screen[1]), sRGB_to_Y(screen[2]));
            }
                break;
            case BT709:
            {
                return Tristimulus(BT709_to_Y(screen[0]), BT709_to_Y(screen[1]), BT709_to_Y(screen[2]));
            }
                break;
            case ST2084:
            {
                return Tristimulus(ST2084_to_Y(screen[0]), ST2084_to_Y(screen[1]), ST2084_to_Y(screen[2]));
            }
            case SLOG2:
            {
                return Tristimulus(SLog2_to_Y(screen[0]), SLog2_to_Y(screen[1]), SLog2_to_Y(screen[2]));
            }
                break;
            case LINEAR:
            default:
                return screen;
        }
    }
} ;

}
#endif /* OTF_hpp__2b9f070e5256418a89a5d649c5a761ba */
