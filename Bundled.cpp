#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#endif
#ifdef EVAL
#define ONLINE_JUDGE
#endif
#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <bit>      
#include <cstdlib>  
#include <cstring>  
#if __has_include(<unistd.h>)
#include <unistd.h>  
#endif
#ifndef _WIN32
#include <sys/mman.h>  
#include <sys/stat.h>  
#endif

namespace gsh {

namespace itype {
    using i8 = signed char;
    using u8 = unsigned char;
    using i16 = short;
    using u16 = unsigned short;
    using i32 = int;
    using u32 = unsigned;
    using ilong = long;
    using ulong = unsigned long;
    using i64 = long long;
    using u64 = unsigned long long;
#ifdef __SIZEOF_INT128__
    using i128 = __int128_t;
    using u128 = __uint128_t;
#endif
    using isize = i32;
    using usize = u32;
    struct i4dig {
        i16 val;
        constexpr bool check() const noexcept { return val < 10000 && -val < 10000; }
    };
    struct u4dig {
        u16 val;
        constexpr bool check() const noexcept { return val < 10000; }
    };
    struct i8dig {
        i32 val;
        constexpr bool check() const noexcept { return val < 100000000 && -val < 100000000; }
    };
    struct u8dig {
        u32 val;
        constexpr bool check() const noexcept { return val < 100000000; }
    };
    struct i16dig {
        i64 val;
        constexpr bool check() const noexcept { return val < 10000000000000000 && -val < 10000000000000000; }
    };
    struct u16dig {
        u64 val;
        constexpr bool check() const noexcept { return val < 10000000000000000; }
    };
}  

namespace ftype {
    
    using f32 = float;
    using f64 = double;
#ifdef __SIZEOF_FLOAT128__
    using f128 = __float128;
#endif
    using flong = long double;
}  

namespace ctype {
    using c8 = char;
    using wc = wchar_t;
    using utf8 = char8_t;
    using utf16 = char16_t;
    using utf32 = char32_t;
}  

namespace internal {
    template<class T, class U> constexpr bool IsSame = false;
    template<class T> constexpr bool IsSame<T, T> = true;
    template<class T, class U, class... V> constexpr bool IsSameAny = IsSame<T, U> || IsSameAny<T, V...>;
    template<class T, class U> constexpr bool IsSameAny<T, U> = IsSame<T, U>;
}  

namespace simd {

#if defined __GNUC__
    using i8x32 = __attribute__((vector_size(32))) itype::i8;
    using u8x32 = __attribute__((vector_size(32))) itype::u8;
    using i16x16 = __attribute__((vector_size(32))) itype::i16;
    using u16x16 = __attribute__((vector_size(32))) itype::u16;
    using i32x8 = __attribute__((vector_size(32))) itype::i32;
    using u32x8 = __attribute__((vector_size(32))) itype::u32;
    using i64x4 = __attribute__((vector_size(32))) itype::i64;
    using u64x4 = __attribute__((vector_size(32))) itype::u64;
    
    using f32x8 = __attribute__((vector_size(32))) ftype::f32;
    using f64x4 = __attribute__((vector_size(32))) ftype::f64;

    using i8x64 = __attribute__((vector_size(64))) itype::i8;
    using u8x64 = __attribute__((vector_size(64))) itype::u8;
    using i16x32 = __attribute__((vector_size(64))) itype::i16;
    using u16x32 = __attribute__((vector_size(64))) itype::u16;
    using i32x16 = __attribute__((vector_size(64))) itype::i32;
    using u32x16 = __attribute__((vector_size(64))) itype::u32;
    using i64x8 = __attribute__((vector_size(64))) itype::i64;
    using u64x8 = __attribute__((vector_size(64))) itype::u64;
    
    using f32x16 = __attribute__((vector_size(64))) ftype::f32;
    using f64x8 = __attribute__((vector_size(64))) ftype::f64;

    template<class T, class U> constexpr T VectorCast(U x) {
        return __builtin_convertvector(x, T);
    }

    template<class T> concept Is256BitVector = internal::IsSameAny<T, i8x32, i16x16, i32x8, i64x4, u8x32, u16x16, u32x8, u64x4, f32x8, f64x4>;
    template<class T> concept Is512BitVector = internal::IsSameAny<T, i8x64, i16x32, i32x16, i64x8, u8x64, u16x32, u32x16, u64x8, f32x16, f64x8>;
    template<class T> concept IsVector = Is256BitVector<T> || Is512BitVector<T>;
#endif

}  

class Byte {
    itype::u8 b = 0;
public:
    friend constexpr Byte operator&(Byte l, Byte r) noexcept { return Byte::from_integer(l.b & r.b); }
    constexpr Byte& operator&=(Byte r) noexcept {
        b &= r.b;
        return *this;
    }
    friend constexpr Byte operator|(Byte l, Byte r) noexcept { return Byte::from_integer(l.b | r.b); }
    constexpr Byte& operator|=(Byte r) noexcept {
        b |= r.b;
        return *this;
    }
    friend constexpr Byte operator^(Byte l, Byte r) noexcept { return Byte::from_integer(l.b ^ r.b); }
    constexpr Byte& operator^=(Byte r) noexcept {
        b ^= r.b;
        return *this;
    }
    template<class IntType> friend constexpr Byte operator<<(Byte l, IntType r) noexcept { return Byte::from_integer(l.b << r); }
    template<class IntType> constexpr Byte& operator<<=(IntType r) noexcept {
        b <<= r;
        return *this;
    }
    template<class IntType> friend constexpr Byte operator>>(Byte l, IntType r) noexcept { return Byte::from_integer(l.b >> r); }
    template<class IntType> constexpr Byte& operator>>=(IntType r) noexcept {
        b >>= r;
        return *this;
    }
    friend constexpr Byte operator~(Byte l) noexcept { return Byte::from_integer(~l.b); }
    template<class IntType> constexpr IntType to_integer() noexcept { return static_cast<IntType>(b); }
    template<class IntType> static constexpr Byte from_integer(IntType l) noexcept {
        Byte res;
        res.b = static_cast<itype::u8>(l);
        return res;
    }
};  

}  


#define GSH_INTERNAL_STR(s) #s
#if defined __clang__ || defined __INTEL_COMPILER
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(unroll n))
#elif defined __GNUC__
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(GCC unroll n))
#else
#define GSH_INTERNAL_UNROLL(n)
#endif
#ifdef __GNUC__
#define GSH_INTERNAL_INLINE [[gnu::always_inline]]
#elif _MSC_VER
#define GSH_INTERNAL_INLINE [[msvc::forceinline]]
#else
#define GSH_INTERNAL_INLINE inline
#endif
#if defined __clang__
#define GSH_INTERNAL_ASSUME(...) [&]() GSH_INTERNAL_INLINE { __builtin_assume(bool(__VA_ARGS__)); }()
#elif defined __GNUC__
#define GSH_INTERNAL_ASSUME(...) [&]() GSH_INTERNAL_INLINE { if (!(__VA_ARGS__)) __builtin_unreachable(); }()
#elif _MSC_VER
#define GSH_INTERNAL_ASSUME(...) [&]() GSH_INTERNAL_INLINE { __assume(bool(__VA_ARGS__)); }()
#else
namespace gsh { namespace internal { [[noreturn]] inline void unreachable() noexcept {} } }
#define GSH_INTERNAL_ASSUME(...) [&]() { if(!(__VA_ARGS__)) gsh::internal::unreachable(); }()
#endif


namespace gsh {

template<class T> class Parser;

namespace internal {
    template<class Stream> constexpr itype::u8 Parseu8(Stream& stream) {
        itype::u32 v;
        std::memcpy(&v, stream.current(), 4);
        v ^= 0x30303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0) >> 3;
        v <<= (32 - (tmp << 3));
        stream.skip(tmp + 1);
        v = (v * 10 + (v >> 8)) & 0x00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff;
        return v;
    }
    template<class Stream> constexpr itype::u16 Parseu16(Stream& stream) {
        itype::u64 v;
        std::memcpy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
        v <<= (64 - (tmp << 3));
        stream.skip(tmp + 1);
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        return v;
    }
    template<class Stream> constexpr itype::u32 Parseu32(Stream& stream) {
        itype::u32 res = 0;
        {
            itype::u64 v;
            std::memcpy(&v, stream.current(), 8);
            if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                res = v;
                stream.skip(8);
            }
        }
        itype::u64 buf;
        std::memcpy(&buf, stream.current(), 8);
        {
            itype::u32 v = buf;
            if (!((v ^= 0x30303030) & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res = 10000 * res + v;
                stream.skip(4);
            }
        }
        {
            itype::u16 v = buf;
            if (!((v ^= 0x3030) & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res = 100 * res + v;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = ctype::c8(buf) ^ 0x30;
            const bool f = !(v & 0xf0);
            res = f ? 10 * res + v : res;
            stream.skip(f + 1);
        }
        return res;
    };
    template<class Stream> constexpr itype::u64 Parseu64(Stream& stream) {
        itype::u64 res = 0;
        {
            itype::u64 v;
            std::memcpy(&v, stream.current(), 8);
            if (!((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                stream.skip(8);
                itype::u64 u;
                std::memcpy(&u, stream.current(), 8);
                if (!((u ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
                    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                    u = (u * 10 + (u >> 8)) & 0x00ff00ff00ff00ff;
                    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                    u = (u * 100 + (u >> 16)) & 0x0000ffff0000ffff;
                    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                    u = (u * 10000 + (u >> 32)) & 0x00000000ffffffff;
                    res = v * 100000000 + u;
                    stream.skip(8);
                } else {
                    v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
                    v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
                    v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
                    res = v;
                }
            }
        }
        itype::u64 buf;
        std::memcpy(&buf, stream.current(), 8);
        {
            itype::u32 v = buf;
            if (!((v ^= 0x30303030) & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res = 10000 * res + v;
                stream.skip(4);
            }
        }
        {
            itype::u16 v = buf;
            if (!((v ^= 0x3030) & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res = 100 * res + v;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = ctype::c8(buf) ^ 0x30;
            const bool f = !(v & 0xf0);
            res = f ? 10 * res + v : res;
            stream.skip(f + 1);
        }
        return res;
    }
    template<class Stream> constexpr itype::u128 Parseu128(Stream& stream) {
        itype::u128 res = 0;
        while (true) {
            itype::u64 v;
            std::memcpy(&v, stream.current(), 8);
            if (((v ^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) != 0) break;
            v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
            res = res * 100000000 + v;
            stream.skip(8);
        }
        itype::u64 buf;
        std::memcpy(&buf, stream.current(), 8);
        {
            itype::u32 v = buf;
            if (!((v ^= 0x30303030) & 0xf0f0f0f0)) {
                buf >>= 32;
                v = (v * 10 + (v >> 8)) & 0x00ff00ff;
                v = (v * 100 + (v >> 16)) & 0x0000ffff;
                res = 10000 * res + v;
                stream.skip(4);
            }
        }
        {
            itype::u16 v = buf;
            if (!((v ^= 0x3030) & 0xf0f0)) {
                buf >>= 16;
                v = (v * 10 + (v >> 8)) & 0x00ff;
                res = 100 * res + v;
                stream.skip(2);
            }
        }
        {
            const ctype::c8 v = ctype::c8(buf) ^ 0x30;
            const bool f = !(v & 0xf0);
            res = f ? 10 * res + v : res;
            stream.skip(f + 1);
        }
        return res;
    }
    template<class Stream> constexpr itype::u8dig Parseu8dig(Stream& stream) {
        itype::u64 v;
        std::memcpy(&v, stream.current(), 8);
        v ^= 0x3030303030303030;
        itype::i32 tmp = std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
        v <<= (64 - (tmp << 3));
        stream.skip(tmp + 1);
        v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
        v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
        v = (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
        return itype::u8dig{ static_cast<itype::u32>(v) };
    }
}  

template<> class Parser<itype::u8> {
public:
    template<class Stream> constexpr itype::u8 operator()(Stream& stream) const {
        stream.reload(8);
        return internal::Parseu8(stream);
    }
};
template<> class Parser<itype::i8> {
public:
    template<class Stream> constexpr itype::i8 operator()(Stream& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i8 tmp = internal::Parseu8(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u16> {
public:
    template<class Stream> constexpr itype::u16 operator()(Stream& stream) const {
        stream.reload(8);
        return internal::Parseu16(stream);
    }
};
template<> class Parser<itype::i16> {
public:
    template<class Stream> constexpr itype::i16 operator()(Stream& stream) const {
        stream.reload(8);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i16 tmp = internal::Parseu16(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u32> {
public:
    template<class Stream> constexpr itype::u32 operator()(Stream& stream) const {
        stream.reload(16);
        return internal::Parseu32(stream);
    }
};
template<> class Parser<itype::i32> {
public:
    template<class Stream> constexpr itype::i32 operator()(Stream& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i32 tmp = internal::Parseu32(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u64> {
public:
    template<class Stream> constexpr itype::u64 operator()(Stream& stream) const {
        stream.reload(32);
        return internal::Parseu64(stream);
    }
};
template<> class Parser<itype::i64> {
public:
    template<class Stream> constexpr itype::i64 operator()(Stream& stream) const {
        stream.reload(32);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i64 tmp = internal::Parseu64(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u128> {
public:
    template<class Stream> constexpr itype::u128 operator()(Stream& stream) const {
        stream.reload(64);
        return internal::Parseu128(stream);
    }
};
template<> class Parser<itype::i128> {
public:
    template<class Stream> constexpr itype::i128 operator()(Stream& stream) const {
        stream.reload(64);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i128 tmp = internal::Parseu128(stream);
        if (neg) tmp = -tmp;
        return tmp;
    }
};
template<> class Parser<itype::u8dig> {
public:
    template<class Stream> constexpr itype::u8dig operator()(Stream& stream) const {
        stream.reload(16);
        return internal::Parseu8dig(stream);
    }
};
template<> class Parser<itype::i8dig> {
public:
    template<class Stream> constexpr itype::i8dig operator()(Stream& stream) const {
        stream.reload(16);
        bool neg = *stream.current() == '-';
        if (neg) stream.skip(1);
        itype::i32 tmp = internal::Parseu8dig(stream).val;
        if (neg) tmp = -tmp;
        return itype::i8dig{ tmp };
    }
};
template<> class Parser<ctype::c8> {
public:
    template<class Stream> constexpr ctype::c8 operator()(Stream& stream) const {
        stream.reload(2);
        ctype::c8 tmp = *stream.current();
        stream.skip(2);
        return tmp;
    }
};



template<class T> class Formatter;

namespace internal {
    template<itype::u32> constexpr auto InttoStr = [] {
        struct {
            ctype::c8 table[40004] = {};
        } res;
        for (itype::u32 i = 0; i != 10000; ++i) {
            res.table[4 * i + 0] = (i / 1000 + '0');
            res.table[4 * i + 1] = (i / 100 % 10 + '0');
            res.table[4 * i + 2] = (i / 10 % 10 + '0');
            res.table[4 * i + 3] = (i % 10 + '0');
        }
        return res;
    }();
    template<class Stream> constexpr void Formatu16(Stream& stream, itype::u16 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu32(Stream& stream, itype::u32 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 100000000) {
            if (n < 10000) copy1(n);
            else {
                copy1(n / 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 100000000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu64(Stream& stream, itype::u64 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000000000000000) {
            if (n < 1000000000000) {
                if (n < 100000000) {
                    if (n < 10000) copy1(n);
                    else {
                        copy1(n / 10000);
                        copy2(n % 10000);
                    }
                } else {
                    copy1(n / 100000000);
                    copy2(n / 10000 % 10000);
                    copy2(n % 10000);
                }
            } else {
                copy1(n / 1000000000000);
                copy2(n / 100000000 % 10000);
                copy2(n / 10000 % 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 10000000000000000);
            copy2(n / 1000000000000 % 10000);
            copy2(n / 100000000 % 10000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu128(Stream& stream, itype::u128 n) {
        auto copy1 = [&](itype::u32 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u32 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        constexpr itype::u128 t = static_cast<itype::u128>(10000000000000000) * 10000000000000000;
        if (n >= t) {
            const itype::u32 r = n / t;
            n %= t;
            if (r >= 10000) {
                copy1(r / 10000);
                copy2(r % 10000);
            } else copy1(r / 10000);
            const itype::u64 a = n / 10000000000000000, b = n % 10000000000000000;
            const itype::u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
            copy2(c / 10000), copy2(c % 10000);
            copy2(d / 10000), copy2(d % 10000);
            copy2(e / 10000), copy2(e % 10000);
            copy2(f / 10000), copy2(f % 10000);
        } else {
            const itype::u64 a = n / 10000000000000000, b = n % 10000000000000000;
            const itype::u32 c = a / 100000000, d = a % 100000000, e = b / 100000000, f = b % 100000000;
            const itype::u32 g = c / 10000, h = c % 10000, i = d / 10000, j = d % 10000, k = e / 10000, l = e % 10000, m = f / 10000, n = f % 10000;
            if (a == 0) {
                if (e == 0) {
                    if (m == 0) copy1(n);
                    else copy1(m), copy2(n);
                } else {
                    if (k == 0) copy1(l), copy2(m), copy2(n);
                    else copy1(k), copy2(l), copy2(m), copy2(n);
                }
            } else {
                if (c == 0) {
                    if (i == 0) copy1(j), copy2(k), copy2(l), copy2(m), copy2(n);
                    else copy1(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                } else {
                    if (g == 0) copy1(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                    else copy1(g), copy2(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
                }
            }
        }
    }
    template<class Stream> constexpr void Formatu8dig(Stream& stream, itype::u8dig x) {
        const itype::u64 n = x.val;
        auto copy1 = [&](itype::u64 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u64 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 10000) copy1(n);
        else {
            copy1(n / 10000);
            copy2(n % 10000);
        }
    }
    template<class Stream> constexpr void Formatu16dig(Stream& stream, itype::u16dig x) {
        const itype::u64 n = x.val;
        auto copy1 = [&](itype::u64 x) {
            itype::u32 off = (x < 10) + (x < 100) + (x < 1000);
            std::memcpy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
            stream.skip(4 - off);
        };
        auto copy2 = [&](itype::u64 x) {
            std::memcpy(stream.current(), InttoStr<0>.table + 4 * x, 4);
            stream.skip(4);
        };
        if (n < 1000000000000) {
            if (n < 100000000) {
                if (n < 10000) copy1(n);
                else {
                    copy1(n / 10000);
                    copy2(n % 10000);
                }
            } else {
                copy1(n / 100000000);
                copy2(n / 10000 % 10000);
                copy2(n % 10000);
            }
        } else {
            copy1(n / 1000000000000);
            copy2(n / 100000000 % 10000);
            copy2(n / 10000 % 10000);
            copy2(n % 10000);
        }
    }
}  

template<> class Formatter<itype::u16> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u16 n) const {
        stream.reload(8);
        internal::Formatu16(stream, n);
    }
};
template<> class Formatter<itype::i16> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i16 n) const {
        stream.reload(8);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu16(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u32> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u32 n) const {
        stream.reload(16);
        internal::Formatu32(stream, n);
    }
};
template<> class Formatter<itype::i32> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i32 n) const {
        stream.reload(16);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu32(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u64> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u64 n) const {
        stream.reload(32);
        internal::Formatu64(stream, n);
    }
};
template<> class Formatter<itype::i64> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i64 n) const {
        stream.reload(32);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu64(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u128> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u128 n) const {
        stream.reload(64);
        internal::Formatu128(stream, n);
    }
};
template<> class Formatter<itype::i128> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i128 n) const {
        stream.reload(64);
        *stream.current() = '-';
        stream.skip(n < 0);
        internal::Formatu128(stream, n < 0 ? -n : n);
    }
};
template<> class Formatter<itype::u8dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u8dig n) const {
        stream.reload(8);
        internal::Formatu8dig(stream, n);
    }
};
template<> class Formatter<itype::i8dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i8dig n) const {
        stream.reload(9);
        *stream.current() = '-';
        stream.skip(n.val < 0);
        internal::Formatu8dig(stream, itype::u8dig{ static_cast<itype::u32>(n.val < 0 ? -n.val : n.val) });
    }
};
template<> class Formatter<itype::u16dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::u16dig n) const {
        stream.reload(16);
        internal::Formatu16dig(stream, n);
    }
};
template<> class Formatter<itype::i16dig> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, itype::i16dig n) const {
        stream.reload(17);
        *stream.current() = '-';
        stream.skip(n.val < 0);
        internal::Formatu16dig(stream, itype::u16dig{ static_cast<itype::u64>(n.val < 0 ? -n.val : n.val) });
    }
};
template<> class Formatter<ctype::c8> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, ctype::c8 c) const {
        stream.reload(1);
        *stream.current() = c;
        stream.skip(1);
    }
};
template<> class Formatter<const ctype::c8*> {
public:
    template<class Stream> constexpr void operator()(Stream& stream, const ctype::c8* s) const {
        itype::u32 len = std::strlen(s);
        itype::u32 avail = stream.avail();
        if (avail >= len) [[likely]] {
            std::memcpy(stream.current(), s, len);
            stream.skip(len);
        } else {
            std::memcpy(stream.current(), s, avail);
            len -= avail;
            s += avail;
            stream.skip(avail);
            while (len != 0) {
                stream.reload();
                avail = stream.avail();
                const itype::u32 tmp = len < avail ? len : avail;
                std::memcpy(stream.current(), s, tmp);
                len -= tmp;
                s += tmp;
                stream.skip(tmp);
            }
        }
    }
};

class BasicReader {
    constexpr static itype::u32 Bufsize = 1 << 18;
    itype::i32 fd = 0;
    ctype::c8 buf[Bufsize + 1];
    ctype::c8 *cur = buf, *eof = buf;
public:
    BasicReader() {}
    BasicReader(itype::i32 filehandle) : fd(filehandle) {}
    BasicReader(const BasicReader& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
        cur = buf + (rhs.cur - rhs.buf);
        eof = buf + (rhs.cur - rhs.eof);
    }
    BasicReader& operator=(const BasicReader& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
        cur = buf + (rhs.cur - rhs.buf);
        eof = buf + (rhs.cur - rhs.eof);
        return *this;
    }
    void reload() {
        if (eof == buf + Bufsize || eof == cur || [&] {
                auto p = cur;
                while (*p >= '!') ++p;
                return p;
            }() == eof) [[likely]] {
            itype::u32 rem = eof - cur;
            std::memmove(buf, cur, rem);
            *(eof = buf + rem + read(fd, buf + rem, Bufsize - rem)) = '\0';
            cur = buf;
        }
    }
    void reload(itype::u32 len) {
        if (avail() < len) [[unlikely]]
            reload();
    }
    itype::u32 avail() const { return eof - cur; }
    const ctype::c8* current() const { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class MmapReader {
    const itype::i32 fh;
    ctype::c8* buf;
    ctype::c8 *cur, *eof;
public:
    MmapReader() : fh(0) {
#ifdef _WIN32
        write(1, "gsh::MmapReader / gsh::MmapReader is not available for Windows.\n", 64);
        std::exit(1);
#else
        struct stat st;
        fstat(0, &st);
        buf = reinterpret_cast<ctype::c8*>(mmap(nullptr, st.st_size + 64, PROT_READ, MAP_PRIVATE, 0, 0));
        cur = buf;
        eof = buf + st.st_size;
#endif
    }
    void reload() const {}
    void reload(itype::u32) const {}
    itype::u32 avail() const { return eof - cur; }
    const ctype::c8* current() const { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class StaticStrReader {
    const ctype::c8* cur;
public:
    constexpr StaticStrReader() {}
    constexpr StaticStrReader(const ctype::c8* c) : cur(c) {}
    constexpr void reload() const {}
    constexpr void reload(itype::u32) const {}
    constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
    constexpr const ctype::c8* current() { return cur; }
    constexpr void skip(itype::u32 n) { cur += n; }
};

class BasicWriter {
    constexpr static itype::u32 Bufsize = 1 << 18;
    itype::i32 fd = 1;
    ctype::c8 buf[Bufsize];
    ctype::c8 *cur = buf, *eof = buf + Bufsize;
public:
    BasicWriter() {}
    BasicWriter(itype::i32 filehandle) : fd(filehandle) {}
    BasicWriter(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
    }
    BasicWriter& operator=(const BasicWriter& rhs) {
        fd = rhs.fd;
        std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
        cur = buf + (rhs.cur - rhs.buf);
        return *this;
    }
    void reload() {
        [[maybe_unused]] itype::i32 tmp = write(fd, buf, cur - buf);
        cur = buf;
    }
    void reload(itype::u32 len) {
        if (eof - cur < len) [[unlikely]]
            reload();
    }
    itype::u32 avail() const { return eof - cur; }
    ctype::c8* current() { return cur; }
    void skip(itype::u32 n) { cur += n; }
};
class StaticStrWriter {
    ctype::c8* cur;
public:
    constexpr StaticStrWriter() {}
    constexpr StaticStrWriter(ctype::c8* c) : cur(c) {}
    constexpr void reload() const {}
    constexpr void reload(itype::u32) const {}
    constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
    constexpr ctype::c8* current() { return cur; }
    constexpr void skip(itype::u32 n) { cur += n; }
};

}  

namespace gsh {

class Exception {
    char str[512];
    char* cur = str;
    void write(const char* x) {
        for (int i = 0; i != 512; ++i, ++cur) {
            if (x[i] == '\0') break;
            *cur = x[i];
        }
    }
    void write(long long x) {
        if (x == 0) *(cur++) = '0';
        else {
            if (x < 0) {
                *(cur++) = '-';
                x = -x;
            }
            char buf[20];
            int i = 0;
            while (x != 0) buf[i++] = x % 10 + '0', x /= 10;
            while (i--) *(cur++) = buf[i];
        }
    }
    template<class T, class... Args> void generate_message(T x, Args... args) {
        write(x);
        if constexpr (sizeof...(Args) > 0) generate_message(args...);
    }
public:
    Exception() noexcept { *cur = '\0'; }
    Exception(const Exception& x) noexcept {
        for (int i = 0; i != 512; ++i) str[i] = x.str[i];
        cur = x.cur;
    }
    explicit Exception(const char* what_arg) noexcept {
        for (int i = 0; i != 512; ++i, ++cur) {
            *cur = what_arg[i];
            if (what_arg[i] == '\0') break;
        }
    }
    template<class... Args> explicit Exception(Args... args) noexcept {
        generate_message(args...);
        *cur = '\0';
    }
    Exception& operator=(const Exception& x) noexcept {
        for (int i = 0; i != 512; ++i) str[i] = x.str[i];
        cur = x.cur;
        return *this;
    }
    const char* what() const noexcept { return str; }
};

}  

#include <type_traits>        
#include <limits>             
#include <bit>                
    
  

namespace gsh {

namespace internal {

    
    template<class T> constexpr T calc_gcd(T x, T y) {
        if (x == 0 || y == 0) return x | y;
        const itype::i32 n = std::countr_zero(x);
        const itype::i32 m = std::countr_zero(y);
        const itype::i32 l = n < m ? n : m;
        x >>= n;
        y >>= m;
        while (x != y) {
            const T a = y - x, b = x - y;
            const itype::i32 m = std::countr_zero(a), n = std::countr_zero(b);
            GSH_INTERNAL_ASSUME(m == n);
            const T s = y < x ? b : a;
            const T t = x < y ? x : y;
            x = s >> m;
            y = t;
        }
        return x << l;
    }

    template<class T> concept IsStaticModint = !requires(T x, typename T::value_type m) { x.set(m); };
    template<class T, itype::u32 id> class ModintBase {
    protected:
        static inline T mint{};
    };
    template<IsStaticModint T, itype::u32 id> class ModintBase<T, id> {
    protected:
        constexpr static T mint{};
    };

    template<class T, itype::u32 id = 0> class ModintImpl : public ModintBase<T, id> {
        typename T::value_type val_{};
        constexpr static auto& mint() noexcept { return ModintBase<T, id>::mint; }
        constexpr static ModintImpl construct(typename T::value_type x) noexcept {
            ModintImpl res;
            res.val_ = x;
            return res;
        }
    public:
        using value_type = typename T::value_type;
        constexpr static bool is_static_mod = IsStaticModint<T>;
        constexpr ModintImpl() noexcept {}
        template<class U> constexpr ModintImpl(U x) noexcept { operator=(x); }
        constexpr explicit operator value_type() const noexcept { return val(); }
        constexpr static void set_mod(value_type x) { mint().set(x); }
        constexpr value_type val() const noexcept { return mint().val(val_); }
        constexpr static value_type mod() noexcept { return mint().mod(); }
        template<class U> constexpr ModintImpl& operator=(U x) noexcept {
            val_ = mint().build(x);
            return *this;
        }
        constexpr static ModintImpl raw(value_type x) noexcept { return construct(mint().raw(x)); }
        constexpr ModintImpl inv() const noexcept { return construct(mint().inv(val_)); }
        constexpr ModintImpl pow(itype::u64 e) const noexcept { return construct(mint().pow(val_, e)); }
        constexpr ModintImpl operator+() const noexcept { return *this; }
        constexpr ModintImpl operator-() const noexcept { return construct(mint().neg(val_)); }
        constexpr ModintImpl& operator++() noexcept {
            val_ = mint().inc(val_);
            return *this;
        }
        constexpr ModintImpl& operator--() noexcept {
            val_ = mint().dec(val_);
            return *this;
        }
        constexpr ModintImpl operator++(int) noexcept {
            ModintImpl copy = *this;
            val_ = mint().inc(val_);
            return copy;
        }
        constexpr ModintImpl operator--(int) noexcept {
            ModintImpl copy = *this;
            val_ = mint().dec(val_);
            return copy;
        }
        constexpr ModintImpl& operator+=(ModintImpl x) noexcept {
            val_ = mint().add(val_, x.val_);
            return *this;
        }
        constexpr ModintImpl& operator-=(ModintImpl x) noexcept {
            val_ = mint().sub(val_, x.val_);
            return *this;
        }
        constexpr ModintImpl& operator*=(ModintImpl x) noexcept {
            val_ = mint().mul(val_, x.val_);
            return *this;
        }
        constexpr ModintImpl& operator/=(ModintImpl x) {
            val_ = mint().div(val_, x.val_);
            return *this;
        }
        friend constexpr ModintImpl operator+(ModintImpl l, ModintImpl r) noexcept { return construct(mint().add(l.val_, r.val_)); }
        friend constexpr ModintImpl operator-(ModintImpl l, ModintImpl r) noexcept { return construct(mint().sub(l.val_, r.val_)); }
        friend constexpr ModintImpl operator*(ModintImpl l, ModintImpl r) noexcept { return construct(mint().mul(l.val_, r.val_)); }
        friend constexpr ModintImpl operator/(ModintImpl l, ModintImpl r) { return construct(mint().div(l.val_, r.val_)); }
        friend constexpr bool operator==(ModintImpl l, ModintImpl r) noexcept { return mint().same(l.val_, r.val_); }
        friend constexpr bool operator!=(ModintImpl l, ModintImpl r) noexcept { return !mint().same(l.val_, r.val_); }
    };

    template<class D, class T> class ModintInterface {
        constexpr D& derived() noexcept { return *static_cast<D*>(this); }
        constexpr const D& derived() const noexcept { return *static_cast<const D*>(this); }
    public:
        using value_type = T;
        constexpr value_type val(value_type x) const noexcept { return x; }
        constexpr value_type build(itype::u32 x) const noexcept { return x % derived().mod(); }
        constexpr value_type build(itype::u64 x) const noexcept { return x % derived().mod(); }
        template<class U> constexpr value_type build(U x) const noexcept {
            static_assert(std::is_integral_v<U>, "gsh::internal::ModintInterface::build<U> / Only integer types can be assigned.");
            if constexpr (std::is_unsigned_v<U>) {
                if constexpr (std::is_same_v<U, itype::u128>) return derived().raw(static_cast<value_type>(x % derived().mod()));
                else if constexpr (std::is_same_v<U, unsigned long long> || std::is_same_v<U, unsigned long>) return derived().build(static_cast<itype::u64>(x));
                else return derived().build(static_cast<itype::u32>(x));
            } else {
                if (x < 0) {
                    if constexpr (std::is_same_v<U, itype::i128>) return derived().neg(derived().raw(static_cast<value_type>(-x % derived().mod())));
                    else if constexpr (std::is_same_v<U, long long> || std::is_same_v<U, long>) return derived().neg(derived().build(static_cast<itype::u64>(-x)));
                    else return derived().neg(derived().build(static_cast<itype::u32>(-x)));
                } else {
                    if constexpr (std::is_same_v<U, itype::i128>) return derived().raw(static_cast<value_type>(x % derived().mod()));
                    else if constexpr (std::is_same_v<U, long long> || std::is_same_v<U, long>) return derived().build(static_cast<itype::u64>(x));
                    else return derived().build(static_cast<itype::u32>(x));
                }
            }
        }
        constexpr value_type raw(value_type x) const noexcept { return x; }
        constexpr value_type zero() const noexcept { return derived().raw(0); }
        constexpr value_type one() const noexcept { return derived().raw(1); }
        constexpr value_type neg(value_type x) const noexcept { return x == 0 ? 0 : derived().mod() - x; }
        constexpr value_type inc(value_type x) const noexcept { return x == derived().mod() - 1 ? 0 : x + 1; }
        constexpr value_type dec(value_type x) const noexcept { return x == 0 ? derived().mod() - 1 : x - 1; }
        constexpr value_type add(value_type x, value_type y) const noexcept { return derived().mod() - x > y ? x + y : y - (derived().mod() - x); }
        constexpr value_type sub(value_type x, value_type y) const noexcept { return x >= y ? x - y : derived().mod() - (y - x); }
        constexpr value_type fma(value_type x, value_type y, value_type z) const noexcept { return derived().add(derived().mul(x, y), z); }
        constexpr value_type div(value_type x, value_type y) const noexcept {
            const value_type iv = derived().inv(y);
            if (derived().same(iv, derived().zero())) [[unlikely]]
                throw gsh::Exception("gsh::internal::ModintInterface::div / Cannot calculate inverse.");
            return derived().mul(x, iv);
        }
        constexpr bool same(value_type x, value_type y) const noexcept { return x == y; }
        constexpr value_type pow(value_type x, itype::u64 e) const noexcept {
            value_type res = derived().one();
            while (e) {
                value_type tmp = derived().mul(x, x);
                if (e & 1) res = derived().mul(res, x);
                x = tmp;
                e >>= 1;
            }
            return res;
        }
        constexpr value_type pow(value_type x, itype::u32 e) const noexcept {
            value_type res = derived().one();
            while (e) {
                value_type tmp = derived().mul(x, x);
                if (e & 1) res = derived().mul(res, x);
                x = tmp;
                e >>= 1;
            }
            return res;
        }
        constexpr value_type inv(value_type t) const noexcept {
            auto a = 1, b = 0, x = derived().val(t), y = derived().mod();
            while (true) {
                if (x <= 1) {
                    if (x == 0) [[unlikely]]
                        return derived().zero();
                    else return derived().build(a);
                }
                b += a * (y / x);
                y %= x;
                if (y <= 1) {
                    if (y == 0) [[unlikely]]
                        return derived().zero();
                    else return derived().build(derived().mod() - b);
                }
                a += b * (x / y);
                x %= y;
            }
        }
    };

    template<itype::u32 mod_> class StaticModint32Impl : public ModintInterface<StaticModint32Impl<mod_>, itype::u32> {
    public:
        constexpr StaticModint32Impl() noexcept {}
        constexpr itype::u32 mod() const noexcept { return mod_; }
        constexpr itype::u32 mul(itype::u32 x, itype::u32 y) const noexcept { return static_cast<itype::u64>(x) * y % mod_; }
    };

    template<itype::u64 mod_> class StaticModint64Impl : public ModintInterface<StaticModint64Impl<mod_>, itype::u64> {
    public:
        constexpr StaticModint64Impl() noexcept {}
        constexpr itype::u64 mod() const noexcept { return mod_; }
        constexpr itype::u64 mul(itype::u64 x, itype::u64 y) const noexcept {
            constexpr itype::u128 M_ = std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
            if constexpr (mod_ < (1ull << 63)) {
                const itype::u64 a = (((M_ * x) >> 64) * y) >> 64;
                const itype::u64 b = x * y;
                const itype::u64 c = a * mod_;
                const itype::u64 d = b - c;
                const bool e = d < mod_;
                const itype::u64 f = d - mod_;
                return e ? d : f;
            } else {
                const itype::u64 a = (((M_ * x) >> 64) * y) >> 64;
                const itype::u128 b = static_cast<itype::u128>(x) * y;
                const itype::u128 c = static_cast<itype::u128>(a) * mod_;
                const itype::u128 d = b - c;
                const bool e = d < mod_;
                const itype::u128 f = d - mod_;
                return e ? d : f;
            }
        }
    };

    class DynamicModint32Impl : public ModintInterface<DynamicModint32Impl, itype::u32> {
        itype::u32 mod_ = 0;
        itype::u64 M_ = 0;
    public:
        constexpr DynamicModint32Impl() noexcept {}
        constexpr void set(itype::u32 n) {
            if (n <= 1) [[unlikely]]
                throw Exception("gsh::internal::DynamicModint32Impl::set / Mod must be at least 2.");
            mod_ = n;
            M_ = std::numeric_limits<itype::u64>::max() / mod_ + std::has_single_bit(mod_);
        }
        constexpr itype::u32 mod() const noexcept { return mod_; }
        constexpr itype::u32 mul(itype::u32 x, itype::u32 y) const noexcept {
            const itype::u64 a = static_cast<itype::u64>(x) * y;
            const itype::u64 b = (static_cast<itype::u128>(M_) * a) >> 64;
            const itype::u64 c = a - b * mod_;
            return c < mod_ ? c : c - mod_;
        }
    };

    class DynamicModint64Impl : public ModintInterface<DynamicModint64Impl, itype::u64> {
        itype::u64 mod_ = 0;
        itype::u128 M_ = 0;
    public:
        constexpr DynamicModint64Impl() noexcept {}
        constexpr void set(itype::u64 n) {
            if (n <= 1) [[unlikely]]
                throw Exception("gsh::internal::DynamicModint64Impl::set / Mod must be at least 2.");
            mod_ = n;
            M_ = std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
        }
        constexpr itype::u64 mod() const noexcept { return mod_; }
        constexpr itype::u64 mul(itype::u64 x, itype::u64 y) const noexcept {
            const itype::u64 a = (((M_ * x) >> 64) * y) >> 64;
            const itype::u64 b = x * y;
            const itype::u64 c = a * mod_;
            const itype::u64 d = b - c;
            const bool e = d < mod_;
            const itype::u64 f = d - mod_;
            return e ? d : f;
        }
    };

    class MontgomeryModint64Impl : public ModintInterface<MontgomeryModint64Impl, itype::u64> {
        itype::u64 mod_ = 0, R2 = 0, ninv = 0;
        constexpr itype::u64 reduce(const itype::u128 t) const noexcept {
            const itype::u64 a = t, b = t >> 64;
            const itype::u64 c = (static_cast<itype::u128>(a * ninv) * mod_) >> 64;
            return b + c + (a != 0);
        }
    public:
        constexpr MontgomeryModint64Impl() noexcept {}
        constexpr void set(itype::u64 n) {
            if (n <= 1) [[unlikely]]
                throw Exception("gsh::internal::MontgomeryModint64Impl::set / Mod must be at least 2.");
            if (n % 2 == 0) [[unlikely]]
                throw Exception("gsh::internal::MontgomeryModint64Impl::set / It is not allowed to set the modulo to an even number.");
            mod_ = n;
            R2 = -static_cast<itype::u128>(mod_) % mod_;
            ninv = mod_;
            for (itype::u32 i = 0; i != 5; ++i) ninv *= 2 - mod_ * ninv;
            ninv = -ninv;
        }
        constexpr itype::u64 val(itype::u64 x) const noexcept {
            const itype::u64 res = static_cast<itype::u64>((static_cast<itype::u128>(x * ninv) * mod_) >> 64) + (x != 0);
            return res == mod_ ? 0 : res;
        }
        constexpr itype::u64 mod() const noexcept { return mod_; }
        constexpr itype::u64 build(itype::u32 x) const noexcept { return reduce(static_cast<itype::u128>(x % mod_) * R2); }
        constexpr itype::u64 build(itype::u64 x) const noexcept { return reduce(static_cast<itype::u128>(x % mod_) * R2); }
        template<class U> constexpr itype::u64 build(U x) const noexcept { return ModintInterface::build(x); }
        constexpr itype::u64 raw(itype::u64 x) const noexcept { return reduce(static_cast<itype::u128>(x) * R2); }
        constexpr itype::u64 neg(itype::u64 x) const noexcept {
            const itype::u64 tmp = 2 * mod_ - x;
            return x == 0 ? 0 : tmp;
        }
        constexpr itype::u64 inc(itype::u64 x) const noexcept { return x + 1 == 2 * mod_ ? 0 : x + 1; }
        constexpr itype::u64 dec(itype::u64 x) const noexcept { return x == 0 ? 2 * mod_ - 1 : x - 1; }
        constexpr itype::u64 add(itype::u64 x, itype::u64 y) const noexcept { return x + y >= 2 * mod_ ? x + y - 2 * mod_ : x + y; }
        constexpr itype::u64 sub(itype::u64 x, itype::u64 y) const noexcept { return x - y < 2 * mod_ ? x - y : 2 * mod_ + (x - y); }
        constexpr itype::u64 mul(itype::u64 x, itype::u64 y) const noexcept { return reduce(static_cast<itype::u128>(x) * y); }
        constexpr itype::u64 fma(itype::u64 x, itype::u64 y, itype::u64 z) const noexcept {
            const itype::u128 t = static_cast<itype::u128>(x) * y;
            const itype::u64 a = t, b = t >> 64;
            const itype::u64 c = (static_cast<itype::u128>(a * ninv) * mod_) >> 64;
            const itype::u64 res = b + c + (a != 0) + z;
            return res < 2 * mod_ ? res : res - 2 * mod_;
        }
        constexpr bool same(itype::u64 x, itype::u64 y) const noexcept { return x + mod_ == y || x == y; }
    };

}  

template<itype::u32 mod_ = 998244353> using StaticModint32 = internal::ModintImpl<internal::StaticModint32Impl<mod_>>;
template<itype::u64 mod_ = 998244353> using StaticModint64 = internal::ModintImpl<internal::StaticModint64Impl<mod_>>;
template<itype::u64 mod_ = 998244353> using StaticModint = std::conditional_t<(mod_ <= 0xffffffff), StaticModint32<mod_>, StaticModint64<mod_>>;
template<itype::u32 id = 0> using DynamicModint32 = internal::ModintImpl<internal::DynamicModint32Impl, id>;
template<itype::u32 id = 0> using DynamicModint64 = internal::ModintImpl<internal::DynamicModint64Impl, id>;
template<itype::u32 id = 0> using MontgomeryModint64 = internal::ModintImpl<internal::MontgomeryModint64Impl, id>;

}  




#include <ctime>   
#include <limits>  
#include <cstdio>  

namespace gsh {

class ClockTimer {
    std::clock_t start_time;
public:
    ClockTimer() { start_time = std::clock(); }
    void restart() { start_time = std::clock(); }
    std::clock_t elapsed() const { return (std::clock() - start_time) * 1000 / CLOCKS_PER_SEC; }
    void print() const { std::printf("gsh::ClockTimer::print / %llu ms\n", static_cast<unsigned long long>(elapsed())); }
};

}  



#if false
#include <fcntl.h>
gsh::BasicReader r(open("in.txt", O_RDONLY));
gsh::BasicWriter w(open("out.txt", O_WRONLY | O_TRUNC));
#else
gsh::BasicReader r;
gsh::BasicWriter w;
#endif
void Main() {
    using namespace std;
    using namespace gsh;
    using namespace gsh::itype;
    using namespace gsh::ftype;
    using namespace gsh::ctype;
    internal::DynamicModint64Impl mint;
    mint.set((1ull << 62) - 1);
    auto a = mint.build(2u), b = mint.build(3u), c = mint.build(5u);
    ClockTimer t;
    [&]() __attribute__((noinline)) {
        for (u32 i = 0; i != 100000000; ++i) {
            a = mint.add(mint.mul(a, a), b);
            b = mint.add(mint.mul(b, b), c);
            c = mint.add(mint.mul(c, c), a);
            
            
            
        }
    }
    ();
    t.print();
    Formatter<u64>{}(w, mint.val(a));
    Formatter<c8>{}(w, '\n');
    Formatter<u64>{}(w, mint.val(b));
    Formatter<c8>{}(w, '\n');
    Formatter<u64>{}(w, mint.val(c));
    Formatter<c8>{}(w, '\n');
}
int main() {
#ifdef ONLINE_JUDGE
    Main();
    w.reload();
#else
    try {
        Main();
        w.reload();
    } catch (gsh::Exception& e) {
        gsh::Formatter<const gsh::ctype::c8*>{}(w, "gsh::Exception was throwed: ");
        gsh::Formatter<const gsh::ctype::c8*>{}(w, e.what());
        gsh::Formatter<gsh::ctype::c8>{}(w, '\n');
        w.reload();
    }
#endif
}

