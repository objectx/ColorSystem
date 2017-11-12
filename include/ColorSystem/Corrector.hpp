#pragma once
#ifndef Corrector_hpp__14711dd2fc0f4efb9d0fdf2e952c89a1
#define Corrector_hpp__14711dd2fc0f4efb9d0fdf2e952c89a1 1

#include "Matrix.hpp"
#include "Gamut.hpp"
#include "Spectrum.hpp"
#include "Tristimulus.hpp"
#include "Observer.hpp"
#include "Macbeth.hpp"
#include "Solver.hpp"

namespace ColorSystem {

// --------------- Color correction.
class Corrector
{
  public:
    Corrector() { ; }
    virtual ~Corrector() { ; }

    static Matrix3 solve(std::vector<Tristimulus> &patch, const std::vector<Tristimulus> &target)
    {
        // M*A = B, M and B are known. solve A.
        int row_count = (int)(patch.size() + 1) * 3; // forex, (24+1)*3 = 75
        int col_count = 9;

        SOLVER::Matrix matrix(row_count, col_count); // M. (75*9)
        SOLVER::Vector result(row_count);            // B.  75
        SOLVER::Vector x(col_count);                 // A.     9

        for (int i = 0; i < patch.size() * 3; i += 3)
        {
            // each patch
            for (int c = 0; c < col_count; c++)
            {
                matrix[i + 0][c] = 0.f;
                matrix[i + 1][c] = 0.f;
                matrix[i + 2][c] = 0.f;
            }
            matrix[i + 0][3 * 0 + 0] = patch[i / 3][0];
            matrix[i + 0][3 * 0 + 1] = patch[i / 3][1];
            matrix[i + 0][3 * 0 + 2] = patch[i / 3][2];
            matrix[i + 1][3 * 1 + 0] = patch[i / 3][0];
            matrix[i + 1][3 * 1 + 1] = patch[i / 3][1];
            matrix[i + 1][3 * 1 + 2] = patch[i / 3][2];
            matrix[i + 2][3 * 2 + 0] = patch[i / 3][0];
            matrix[i + 2][3 * 2 + 1] = patch[i / 3][1];
            matrix[i + 2][3 * 2 + 2] = patch[i / 3][2];
            result[i + 0]            = target[(i / 3)][0];
            result[i + 1]            = target[(i / 3)][1];
            result[i + 2]            = target[(i / 3)][2];
        }

        // 0 restrction.
        {
            int i = (int)patch.size() * 3;
            for (int c = 0; c < col_count; c++)
            {
                matrix[i + 0][c] = 0.f;
                matrix[i + 1][c] = 0.f;
                matrix[i + 2][c] = 0.f;
            }
            matrix[i + 0][3 * 0 + 0] = 0.f;
            matrix[i + 0][3 * 0 + 1] = 0.f;
            matrix[i + 0][3 * 0 + 2] = 0.f;
            matrix[i + 1][3 * 1 + 0] = 0.f;
            matrix[i + 1][3 * 1 + 1] = 0.f;
            matrix[i + 1][3 * 1 + 2] = 0.f;
            matrix[i + 2][3 * 2 + 0] = 0.f;
            matrix[i + 2][3 * 2 + 1] = 0.f;
            matrix[i + 2][3 * 2 + 2] = 0.f;
            result[i + 0]            = 0.f;
            result[i + 1]            = 0.f;
            result[i + 2]            = 0.f;
        }

        SOLVER::solve(matrix, result, x);
        return Matrix3::fromArray(x.data());
    }

    static Matrix3 solve(std::vector<Tristimulus> &patch, const Spectrum &light = CIE_D65, const Observer &observer = CIE1931)
    {
        return solve(patch, Macbeth::reference(light, observer));
    }

    static Gamut makeGamut(const char *name, std::vector<Tristimulus> &patch, const Spectrum &light = CIE_D65, const Observer &observer = CIE1931)
    {
        return Gamut(name, solve(patch, light, observer));
    }
} ;

}

#endif /* Corrector_hpp__14711dd2fc0f4efb9d0fdf2e952c89a1 */
