#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <math.h>

/**
 * Cette classe représente un vecteur 2D au sens mathématique.
 * Elle a donc les méthodes usuelles de manipulation de vecteurs.
 */
template <typename T>
struct Vector2
{
    T x;
    T y;

    static Vector2<T> create(T const & norm, T const & angle) {
        return Vector2<T>(norm * cos(angle), norm * sin(angle));
    }

    static Vector2<T> createDegree(T const & norm, T const & angle) {
        T a = angle * M_PI / 180;
        return Vector2<T>(norm * cos(a), norm * sin(a));
    }

    Vector2(T x = 0, T y = 0) : x(x), y(y) {}
    Vector2(Vector2 const & v) : x(v.x), y(v.y) {}

    Vector2 operator = (Vector2<T> const & v);

    T operator * (Vector2<T> const & v) const {
        return x * v.x + y * v.y;
    }

    Vector2<T> operator * (T const & t) const {
        return Vector2<T>(t * x, t * y);
    }

    Vector2<T> operator / (T const & t) const {
        return Vector2<T>(x / t, y / t);
    }

    Vector2<T> operator + (Vector2<T> const & v) const {
        return Vector2<T>(x + v.x, y + v.y);
    }

    Vector2<T> operator - (Vector2<T> const & v) const {
        return Vector2<T>(x - v.x, y - v.y);
    }

    bool operator == (Vector2<T> const & v) const {
        return x == v.x && y == v.y;
    }

    bool operator != (Vector2<T> const & v) const {
        return x != v.x || y != v.y;
    }

    Vector2<T> & operator += (Vector2<T> const & v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2<T> & operator -= (Vector2<T> const & v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2<T> & operator /= (T const & t) {
        x /= t;
        y /= t;
        return *this;
    }

    Vector2<T> & operator *= (T const & t) {
        x *= t;
        y *= t;
        return *this;
    }

    Vector2<T> cpy() {
        return Vector2<T>(x, y);
    }

    /** @brief la norme du vecteur */
    T norm() const {
        return sqrt(x * x + y * y);
    }

    /** @brief normalise le vecteur. */
    Vector2<T> & normalize() {
        T n = norm();
        x /= n;
        y /= n;
        return *this;
    }

    /** @return l'angle du vecteur (par rapport à l'axe des x) */
    T angle() {
        return atan2(y, x);
    }

    /** @return l'angle en degrée */
    T angleDegree() {
        return angle() * 180 / M_PI;
    }

    /** @brief tourne le vecteur d'un angle (en radian) */
    Vector2<T> & rotate(T const & angle) {
        T n = norm(),
          a  = this->angle();
        x = n * cos(a + angle);
        y = n * sin(a + angle);
        return *this;
    }

    /** @brief tourne le vecteur d'un angle (en degree) */
    Vector2<T> & rotateDegree(T const & angle) {
        T n = norm(),
          a  = this->angle(),
          angDeg = angle * M_PI / 180;
        x = n * cos(a + angDeg);
        y = n * sin(a + angDeg);
        return *this;
    }

    Vector2<T> & set(T const & x, T const & y)
    {
        this->x = x;
        this->y = y;
        return *this;
    }
};

template <typename T>
Vector2<T> operator * (T const & coef, Vector2<T> const & v)
{
    return Vector2<T>(coef * v.x, coef * v.y);
}

#endif // VECTOR2_HPP
