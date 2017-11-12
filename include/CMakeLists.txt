
cmake_minimum_required (VERSION 3.9)

add_custom_target(_colorsystem_sources
                  SOURCES colorsystem.hpp
                          ColorSystem/Vector.hpp
                          ColorSystem/Matrix.hpp
                          ColorSystem/Tristimulus.hpp
                          ColorSystem/Delta.hpp
                          ColorSystem/Gamut.hpp
                          ColorSystem/OTF.hpp
                          ColorSystem/Spectrum.hpp
                          ColorSystem/Observer.hpp
                          ColorSystem/Macbeth.hpp
                          ColorSystem/Solver.hpp
                          ColorSystem/Corrector.hpp
                  COMMENT Workaround for https://youtrack.jetbrains.com/issue/CPP-270)
