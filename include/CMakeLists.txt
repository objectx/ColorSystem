
cmake_minimum_required (VERSION 3.9)

add_custom_target(_colorsystem_sources
                  SOURCES colorsystem.hpp
                  COMMENT Workaround for https://youtrack.jetbrains.com/issue/CPP-270)
