#ifndef UTAHA_DATATYPE_H_
#define UTAHA_DATATYPE_H_

namespace utaha {

template<class T>
struct Vector2 {
    T x;
    T y;
};

template<class T>
struct Vector3 {
    T x;
    T y;
    T z;
};

template<class T>
struct Bound {
    Vector2<T> _1;
    Vector2<T> _2;
};

typedef Vector2<int>    Int2;
typedef Vector2<float>  Float2;
typedef Vector2<double> Double2;

typedef Vector3<int>    Int3;
typedef Vector3<float>  Float3;
typedef Vector3<double> Double3;

struct Texture2DBinder {
    int tex_id;
    Float2 _1;
    Float2 _2;
    Float2 _3;
    Float2 _4;
};


} // namespace utaha

#endif // UTAHA_DATATYPE_H_