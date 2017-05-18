#pragma once
#ifndef matrix_hpp__c4f06633ade24b3e9ff239f4fdc881cc
#define matrix_hpp__c4f06633ade24b3e9ff239f4fdc881cc    1

namespace ColorSystem {
    namespace detail {

        template <typename T_>
            class Vector3_ {
            public:
                using self_t = Vector3_<T_>;
            private:
                T_  v_ [3] ;
            public:
                constexpr Vector3_ (const T_ &a, const T_ &b, const T_ &c) : v_ { a, b, c } { /* NO-OP */ }

                constexpr const T_ &get (size_t idx) const {
                    assert (idx < 3);
                    return v_[idx];
                }

                constexpr self_t &set (size_t idx, const T_ &value) {
                    assert (idx < 3);
                    v_[idx] = value;
                    return *this;
                }

                constexpr const T_ &    at (size_t idx) const { return v_ [idx]; }
                constexpr       T_ &    at (size_t idx)       { return v_ [idx]; }

                constexpr const T_ &    operator[] (size_t idx) const { return at (idx); }
                constexpr       T_ &    operator[] (size_t idx)       { return at (idx); }

                constexpr const T_ &    x () const { return at (0); }
                constexpr const T_ &    y () const { return at (1); }
                constexpr const T_ &    z () const { return at (2); }

                constexpr T_ & x () { return at (0); }
                constexpr T_ & y () { return at (1); }
                constexpr T_ & z () { return at (2); }

                constexpr auto size () const { return 3; }

                auto begin () const { using std::begin ; return begin (v_); }
                auto begin ()       { using std::begin ; return begin (v_); }

                auto end () const { using std::end ; return end (v_); }
                auto end ()       { using std::end ; return end (v_); }
            };
    }

    namespace detail {
        template <typename T_, typename U_>
            constexpr const T_ into (const U_ &value) {
                return static_cast<T_> (value) ;
            }
        /**
         * @brief 3x3 Matrix
         * @tparam T_ The value type.
         */
        template <typename T_>
            class Matrix3x3_ {
            public:
                using self_t = Matrix3x3_<T_> ;
            private:
                T_  m_ [9] ;
            public:
                constexpr Matrix3x3_ ( const T_ &a00, const T_ &a01, const T_ &a02
                                     , const T_ &a10, const T_ &a11, const T_ &a12
                                     , const T_ &a20, const T_ &a21, const T_ &a22)
                        : m_ { a00, a01, a02
                             , a10, a11, a12
                             , a20, a21, a22
                             } {
                    /* NO-OP */
                }
//                template <typename U_>
//                    constexpr Matrix3x3_ (std::initializer_list<U_> args) {
//                        //static_assert (args.size () == size (), "Wrong number of arguments") ;
//                        using namespace std ;
//                        using std::begin ;
//                        auto it = begin (m_) ;
//                        for (auto const &v : args) {
//                            *it++ = from (v) ;
//                        }
//                    }
                constexpr Matrix3x3_ (const Matrix3x3_ &src)
                        : Matrix3x3_ { src.at (0, 0), src.at (1, 0), src.at (2, 0)
                                     , src.at (0, 1), src.at (1, 1), src.at (2, 1)
                                     , src.at (0, 2), src.at (1, 2), src.at (2, 2) } {
                }

                template <typename U_>
                    constexpr Matrix3x3_ (const U_ &v0, const U_ &v1, const U_ &v2)
                            : Matrix3x3_ { from (v0), from ( 0), from ( 0)
                                         , from ( 0), from (v1), from ( 0)
                                         , from ( 0), from ( 0), from (v2) } {
                        /* NO-OP */
                    }

                template <typename U_>
                    explicit constexpr Matrix3x3_ (const Vector3_<U_> &v)
                            : Matrix3x3_ { from (v [0]), from (v [1]), from (v [2]) } {
                        /* NO-OP */
                    }

                constexpr Matrix3x3_ () : Matrix3x3_ { identity () } { /* NO-OP */ }

                constexpr const T_ &    get (size_t idx) const {
                    return m_ [idx] ;
                }

                constexpr const T_ &    get (size_t col, size_t row) const {
                    return m_ [col + 3 * row] ;
                }

                constexpr self_t &      set (size_t idx, const T_ &value) {
                    m_ [idx] = value ;
                    return *this ;
                }

                constexpr self_t &      set (size_t col, size_t row, const T_ &value) {
                    m_ [col + 3 * row] = value ;
                    return *this ;
                }

                constexpr const T_ &    at (size_t col, size_t row) const { return m_ [col + 3 * row] ; }
                constexpr       T_ &    at (size_t col, size_t row)       { return m_ [col + 3 * row] ; }

                constexpr const T_ &    at (size_t idx) const { return m_ [idx] ; }
                constexpr       T_ &    at (size_t idx)       { return m_ [idx] ; }

                constexpr const T_ &    at1 (size_t row, size_t col) const { return m_ [(col - 1) + 3 * (row - 1)] ; }
                constexpr       T_ &    at1 (size_t row, size_t col)       { return m_ [(col - 1) + 3 * (row - 1)] ; }

                constexpr const T_ &    operator [] (size_t idx) const { return m_ [idx] ; }
                constexpr       T_ &    operator [] (size_t idx)       { return m_ [idx] ; }

                constexpr auto size () const { return 9 ; }
                auto begin () const { using std::begin ; return begin (m_); }
                auto begin ()       { using std::begin ; return begin (m_); }

                auto end () const { using std::end ; return end (m_); }
                auto end ()       { using std::end ; return end (m_); }

                template <typename U_>
                    constexpr self_t &  multiply (const Matrix3x3_<U_> &other) {
                        auto v00 = at (0, 0) * other.at (0, 0) + at (1, 0) * other.at (0, 1) + at (2, 0) * other.at (0, 2) ;
                        auto v01 = at (0, 0) * other.at (1, 0) + at (1, 0) * other.at (1, 1) + at (2, 0) * other.at (1, 2) ;
                        auto v02 = at (0, 0) * other.at (2, 0) + at (1, 0) * other.at (2, 1) + at (2, 0) * other.at (2, 2) ;
                        auto v10 = at (0, 1) * other.at (0, 0) + at (1, 1) * other.at (0, 1) + at (2, 1) * other.at (0, 2) ;
                        auto v11 = at (0, 1) * other.at (1, 0) + at (1, 1) * other.at (1, 1) + at (2, 1) * other.at (1, 2) ;
                        auto v12 = at (0, 1) * other.at (2, 0) + at (1, 1) * other.at (2, 1) + at (2, 1) * other.at (2, 2) ;
                        auto v20 = at (0, 2) * other.at (0, 0) + at (1, 2) * other.at (0, 1) + at (2, 2) * other.at (0, 2) ;
                        auto v21 = at (0, 2) * other.at (1, 0) + at (1, 2) * other.at (1, 1) + at (2, 2) * other.at (1, 2) ;
                        auto v22 = at (0, 2) * other.at (2, 0) + at (1, 2) * other.at (2, 1) + at (2, 2) * other.at (2, 2) ;
                        m_ [0] = v00 ; m_ [1] = v01 ; m_ [2] = v02 ;
                        m_ [3] = v10 ; m_ [4] = v11 ; m_ [5] = v12 ;
                        m_ [6] = v20 ; m_ [7] = v21 ; m_ [8] = v22 ;
                        return *this ;
                    }

                template <typename U_>
                    constexpr self_t &  operator *= (const Matrix3x3_<U_> &other) {
                        return multiply (other) ;
                    }

                template <typename U_>
                    constexpr self_t &  multiply (const U_ &value) {
                        for (auto &v : m_) {
                            v *= value ;
                        }
                        return *this ;
                    }

                template <typename U_>
                    constexpr self_t &  operator *= (const U_ &value) {
                        return multiply (value) ;
                    }

                template <typename U_>
                    constexpr self_t &  divide (const U_ &value) {
                        for (auto &v : m_) {
                            v /= value ;
                        }
                    }

                template <typename U_>
                    constexpr self_t &  operator /= (const U_ &value) {
                        return divide (value) ;
                    }

                template <typename U_>
                    constexpr self_t &  add (const Matrix3x3_<U_> &other) {
                        for (int_fast32_t i = 0 ; i < m_.size () ; ++i) {
                            m_ [i] += from (other.m_ [i]) ;
                        }
                        return *this ;
                    }

                template <typename U_>
                    constexpr self_t &  operator += (const Matrix3x3_<U_> &other) {
                        return add (other) ;
                    }

                template <typename U_>
                    constexpr self_t &  sub (const Matrix3x3_<U_> &other) {
                        for (int_fast32_t i = 0 ; i < m_.size () ; ++i) {
                            m_ [i] -= from (other.m_ [i]) ;
                        }
                        return *this ;
                    }

                template <typename U_>
                    constexpr self_t &  operator -= (const Matrix3x3_<U_> &other) {
                        return sub (other);
                    }

                template <typename U_>
                    constexpr Vector3_<T_>  apply (const Vector3_<U_> &v) const {
                        return { at (0, 0) * from (v [0]) + at (1, 0) * from (v[1]) + at (2, 0) * from (v[2])
                               , at (0, 1) * from (v [0]) + at (1, 1) * from (v[1]) + at (2, 1) * from (v[2])
                               , at (0, 2) * from (v [0]) + at (1, 2) * from (v[1]) + at (2, 2) * from (v[2]) } ;
                    }

                constexpr T_    det () const {
                    return ( at1 (1, 1) * at1 (2, 2) * at1 (3, 3)
                           + at1 (2, 1) * at1 (3, 2) * at1 (1, 3)
                           + at1 (3, 1) * at1 (1, 2) * at1 (2, 3)
                           - at1 (1, 1) * at1 (3, 2) * at1 (2, 3)
                           - at1 (3, 1) * at1 (2, 2) * at1 (1, 3)
                           - at1 (2, 1) * at1 (1, 2) * at1 (3, 3)) ;
                }

                constexpr self_t &  invert () {
                    auto d = 1 / det () ;
                    auto v00 = (at1 (2, 2) * at1 (3, 3) - at1 (2, 3) * at1 (3, 2)) * d ;
                    auto v01 = (at1 (1, 3) * at1 (3, 2) - at1 (1, 2) * at1 (3, 3)) * d ;
                    auto v02 = (at1 (1, 2) * at1 (2, 3) - at1 (1, 3) * at1 (2, 2)) * d ;
                    auto v10 = (at1 (2, 3) * at1 (3, 1) - at1 (2, 1) * at1 (3, 3)) * d ;
                    auto v11 = (at1 (1, 1) * at1 (3, 3) - at1 (1, 3) * at1 (3, 1)) * d ;
                    auto v12 = (at1 (1, 3) * at1 (2, 1) - at1 (1, 1) * at1 (2, 3)) * d ;
                    auto v20 = (at1 (2, 1) * at1 (3, 2) - at1 (2, 2) * at1 (3, 1)) * d ;
                    auto v21 = (at1 (1, 2) * at1 (3, 1) - at1 (1, 1) * at1 (3, 2)) * d ;
                    auto v22 = (at1 (1, 1) * at1 (2, 2) - at1 (1, 2) * at1 (2, 1)) * d ;
                    m_ [0] = v00 ; m_ [1] = v01 ; m_ [2] = v02 ;
                    m_ [3] = v10 ; m_ [4] = v11 ; m_ [5] = v12 ;
                    m_ [6] = v20 ; m_ [7] = v21 ; m_ [8] = v22 ;
                    return *this ;
                }

            public:
                static constexpr Matrix3x3_ identity () {
                    return { 1, 0, 0
                           , 0, 1, 0
                           , 0, 0, 1 } ;
                }
            private:
                template <typename U_>
                    static constexpr const T_ from (const U_ &v) {
                        return into<T_, U_> (v) ;
                    }
            };

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    diag (const Vector3_<U_> &v) {
                return { into<T_, U_> (v [0]), into<T_, U_> (    0), into<T_, U_> (    0)
                       , into<T_, U_> (    0), into<T_, U_> (v [1]), into<T_, U_> (    0)
                       , into<T_, U_> (    0), into<T_, U_> (    0), into<T_, U_> (v [2]) } ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    multiply (const Matrix3x3_<T_> &a, const Matrix3x3_<U_> &b) {
                return Matrix3x3_<T_> { a }.multiply (b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    operator * (const Matrix3x3_<T_> &a, const Matrix3x3_<U_> &b) {
                return multiply (a, b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    multiply (const Matrix3x3_<T_> &a, const U_ &b) {
                return Matrix3x3_<T_> { a }.multiply (b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    operator * (const Matrix3x3_<T_> &a, const U_ &b) {
                return multiply (a, b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    operator * (const U_ &a, const Matrix3x3_<T_> &b) {
                return multiply (b, a) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    divide (const Matrix3x3_<T_> &a, const U_ &b) {
                return Matrix3x3_<T_> { a }.divide (b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    operator / (const Matrix3x3_<T_> &a, const U_ &b) {
                return divide (a, b) ;
            }

        template <typename T_, typename U_>
            constexpr Vector3_<T_>      apply (const Matrix3x3_<T_> &m, const Vector3_<U_> &v) {
                return m.apply (v) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    add (const Matrix3x3_<T_> &a, const U_ &b) {
                return add (a, b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    operator + (const Matrix3x3_<T_> &a, const U_ &b) {
                return add (a, b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    sub (const Matrix3x3_<T_> &a, const U_ &b) {
                return sub (a, b) ;
            }

        template <typename T_, typename U_>
            constexpr Matrix3x3_<T_>    operator - (const Matrix3x3_<T_> &a, const U_ &b) {
                return sub (a, b) ;
            }

        template <typename T_>
            constexpr T_    det (const Matrix3x3_<T_> &m) {
                return m.det () ;
            }

        template <typename T_>
            constexpr Matrix3x3_<T_>    invert (const Matrix3x3_<T_> &m) {
                return Matrix3x3_<T_> { m }.invert () ;
            }

        template <typename T_>
            constexpr Matrix3x3_<T_>    operator ~ (const Matrix3x3_<T_> &m) {
                return invert (m) ;
            }
    }
}

#endif /* matrix_hpp__c4f06633ade24b3e9ff239f4fdc881cc */
