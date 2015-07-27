
#ifndef COLOQUINTE_GP_COMMON
#define COLOQUINTE_GP_COMMON

#include <cstdint>
#include <algorithm>

namespace coloquinte{

using float_t    = float;
using int_t      = std::int32_t;
using index_t    = std::uint32_t;
using capacity_t = std::int64_t;
using mask_t     = std::uint32_t;

using ext_object = std::uint64_t;

enum PlacementType{
    Optimist  = 0,
    Pessimist = 1
};

enum Movability{
    XMovable   = 1     ,
    YMovable   = 1 << 1,
    XFlippable = 1 << 2,
    YFlippable = 1 << 3,
    SoftMacro  = 1 << 4
};

template<typename T>
struct point{
    T x, y;
    point(){}
    point(T x, T y): x(x), y(y){}

    template<typename S>
    operator point<S>() const{
        return point<S>(static_cast<S>(x), static_cast<S>(y));
    }

    void operator+=(point<T> const o){
        x += o.x;
        y += o.y;
    }
};

template<typename T>
point<T> operator+(point<T> const a, point<T> const b){
    return point<T>(a.x+b.x, a.y+b.y);
}
template<typename T>
point<T> operator-(point<T> const a, point<T> const b){
    return point<T>(a.x-b.x, a.y-b.y);
}
template<typename T>
point<T> operator*(T lambda, point<T> const p){
    return point<T>(lambda * p.x, lambda * p.y);
}
template<typename T>
point<T> operator*(point<T> const a, point<T> const b){
    return point<T>(a.x*b.x, a.y*b.y);
}

template<typename T>
struct box{
    T x_min, x_max, y_min, y_max;
    box(){}
    box(T x_mn, T x_mx, T y_mn, T y_mx) : x_min(x_mn), x_max(x_mx), y_min(y_mn), y_max(y_mx){}
    box(point<T> mn, point<T> mx) : x_min(mn.x), x_max(mx.x), y_min(mn.y), y_max(mx.y){}

    bool in(box<T> const o) const{
        return x_max   <= o.x_max
            && y_max   <= o.y_max
            && x_min   >= o.x_min
            && y_min   >= o.y_min;
    }
    bool intersects(box<T> const o) const{
        return x_min   < o.x_max
            && y_min   < o.y_max
            && o.x_min < x_max
            && o.y_min < y_max;
    }
    box<T> intersection(box<T> const o) const{
        return box<T>(
            std::max(x_min, o.x_min),
            std::min(x_max, o.x_max),
            std::max(y_min, o.y_min),
            std::min(y_max, o.y_max)
        );
    }
    box<T> bounding_box(box<T> const o) const{
        return box<T>(
            std::min(x_min, o.x_min),
            std::max(x_max, o.x_max),
            std::min(y_min, o.y_min),
            std::max(y_max, o.y_max)
        );
    }
    point<T> dimensions() const{
        return point<T>(x_max-x_min, y_max-y_min);
    }
    bool empty() const{
        return dimensions().x <= 0 or dimensions().y <= 0;
    }

    template<typename S>
    operator box<S>() const{
        return box<S>(static_cast<S>(x_min), static_cast<S>(x_max), static_cast<S>(y_min), static_cast<S>(y_max));
    }
};

using orientation_t = point<bool>;

} // Namespace coloquinte

#endif

