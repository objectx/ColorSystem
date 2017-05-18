#include <rapidcheck.h>

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iterator>
#include <matrix.hpp>

#include "TestUtilities.hpp"

namespace {
    using M3 = ColorSystem::detail::Matrix3x3_<float> ;

    bool is_almost_identity (const M3 &m) {
        auto approx = [](float actual, float expected) -> bool {
            constexpr float eps = 1e-3 ;
            return fabsf (actual - expected) < eps;
        } ;
        return (  approx (m.get (0, 0), 1.0f) && approx (m.get (1, 0), 0.0f) && approx (m.get (2, 0), 0.0f)
               && approx (m.get (0, 1), 0.0f) && approx (m.get (1, 1), 1.0f) && approx (m.get (2, 1), 0.0f)
               && approx (m.get (0, 2), 0.0f) && approx (m.get (1, 2), 0.0f) && approx (m.get (2, 2), 1.0f)) ;
    }
}

//std::ostream &  operator << (std::ostream &o, const M3 &m) {
//    o << "|" << putFloat (m [0]) << " " << putFloat (m [1]) << " " << putFloat (m [2]) << "|\n"
//      << "|" << putFloat (m [3]) << " " << putFloat (m [4]) << " " << putFloat (m [5]) << "|\n"
//      << "|" << putFloat (m [6]) << " " << putFloat (m [7]) << " " << putFloat (m [8]) << "|\n";
//    return o;
//}

namespace rc {
    template <>
        struct Arbitrary<M3> {
            static Gen<M3> arbitrary () {
                return gen::construct<M3> ( gen::arbitrary<float> (), gen::arbitrary<float> (), gen::arbitrary<float> ()
                                          , gen::arbitrary<float> (), gen::arbitrary<float> (), gen::arbitrary<float> ()
                                          , gen::arbitrary<float> (), gen::arbitrary<float> (), gen::arbitrary<float> ()) ;
            }
        } ;
}

int main (int argc, const char **argv) {
    rc::check ([] (const M3 &m) {
        auto det = m.det () ;
        RC_PRE (std::isfinite (det)) ;
        RC_PRE (det != 0) ;
        RC_LOG () << "Detaminant: " << m.det () ;
        auto ident = ColorSystem::detail::multiply (m, M3 { m }.invert ()) ;
        RC_ASSERT (is_almost_identity (ident)) ;
        return true ;
    });
    return 0 ;
}
