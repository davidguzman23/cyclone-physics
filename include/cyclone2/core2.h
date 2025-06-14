#include <math.h>
#ifndef CYCLONE_CORE_H
#define CYCLONE_CORE_H
#include "precision2.h"

namespace cyclone2 {

    class Vector3 
    {
        public:
            
            real x;

            real y;

            real z;

        private: 

            real pad;

        public:

            Vector3() : x(0), y(0), z(0) {}

            Vector3( const real x, const real y, const real z)
                : x(x), y(y), z(z) {}

            const static Vector3 GRAVITY;
            const static Vector3 HIGH_GRAVITY;
            const static Vector3 UP;
            const static Vector3 RIGHT;
            const static Vector3 OUT_OF_SCREEN;
            const static Vector3 X;
            const static Vector3 Y;
            const static Vector3 Z; 

            real operator[](unsigned i) const
            {
                if ( i == 0 ) return x;
                if ( i == 1 ) return y;
                return z;
            }

            real& operator[](unsigned i) 
            {
                if ( i == 0) return x;
                if ( i == 1) return y;
                return z;
            }

            /* Vector Addition Set, & should give us a copy instead of the whole object */
            void operator+=(const Vector3& v)
            {
                x += v.x;
                y += v.y;
                z += v.z;
            }

            /* Vector Addition */
            Vector3 operator+(const Vector3& v) const
            {
                return Vector3(x+v.x, y+v.y, z+v.z);
            }

            /* Vector Subtraction Set */
            void operator-=(const Vector3& v)
            {
                x -= v.x;
                y -= v.y;
                z -= v.z;
            }

            /* Vector Subtraction */
            Vector3 operator-(const Vector3& v) const
            {
                return Vector3(x-v.x, y-v.y, z-v.z);
            }

            /* Scalar Multiplication Set */
            void operator*=(const real value)
            {
                x *= value;
                y *= value;
                z *= value;
            }

            /* Scalar Multiplication, const ensures this vector's values are not changed */
            Vector3 operator*(const real value) const 
            {
                return Vector3(x*value, y*value, z*value);
            }

            /* Component Product Set */
            void componentProductUpdate(const Vector3 &v)
            {
                x *= v.x;
                y *= v.y;
                z *= v.z;
            }

            Vector3 componentProduct(const Vector3 &v) const
            {
                return Vector3(x*v.x, y*v.y, z*v.z);
            }

            /**
            * Calculates and returns the vector product of this vector
            * with the given vector.
            */
            Vector3 vectorProduct(const Vector3& v) const
            {
                return Vector3( y * v.z - z * v.y,
                                z * v.x - x * v.z,
                                x * v.y - y * v.x );
            }

            /**
            * Updates this vector to be the vector product of its current
            * value and the given vector.
            */
            void operator%=(const Vector3& v) 
            {
                *this = vectorProduct(v);
            }

            /**
            * Calculates and returns the vector product of this vector
            * with the given vector.
            */
            Vector3 operator%(const Vector3& v) const
            {
                return Vector3( y * v.z - z * v.y,
                                z * v.x - x * v.z,
                                x * v.y - y * v.x );
            }

            /**
            * Calculates and returns the scalar product of this vector
            * with the given vector.
            */
            real scalarProduct(const Vector3 &v) const
            {
                return x*v.x + y*v.y + z*v.z;
            }

            /**
            * Calculates and returns the scalar product of this vector
            * with the given vector.
            */
            real operator*(const Vector3 &v) const
            {
                return x*v.x + y*v.y + z*v.z;
            }

            /**
            * Adds the given vector to this, scaled by the given amount.
            */
            void addScaledVector(const Vector3& v, real scale)
            {
                x += v.x * scale;
                y += v.y * scale;
                z += v.z * scale;

            }

            /** Gets the magnitude of this vector. */
            real magnitude() const 
            {
                return real_sqrt(x*x+y*y+z*z);
            }

            /** Gets the squared magnitude of this vector. */
            real squareMagnitude() const
            {
                return x*x+y*y+z*z;
            }

            /** Limits the size of the vector to the given maximum. */
            void trim(real size)
            {
                if (squareMagnitude() > size*size)
                {
                    normalize();
                    x *= size;
                    y *= size;
                    z *= size;
                }
            }

            /* this is derefenrenced because this returns a pointer */
            void normalize()
            {
                real l = magnitude();
                if (l > 0)
                {
                    (*this)*=((real)1)/l;
                }
            }

            /** Returns the normalised version of a vector. */
            Vector3 unit() const
            {
                Vector3 result = *this;
                result.normalize();
                return result;
            }

            /** Checks if the two vectors have identical components. */
            bool operator==(const Vector3& other) const
            {
                return x == other.x && 
                    y == other.y && 
                    z == other.z;
            }

            /** Checks if the two vectors have non-identical components. */
            bool operator!=(const Vector3& other) const
            {
                return !(*this == other);
            }

            /**
            * Checks if this vector is component-by-component less than
            * the other.
            *
            * @note This does not behave like a single-value comparison:
            * !(a < b) does not imply (b >= a).
            */
            bool operator<(const Vector3& other) const
            {
                return x < other.x && y < other.y && z < other.z;
            }


            /**
            * Checks if this vector is component-by-component less than
            * the other.
            *
            * @note This does not behave like a single-value comparison:
            * !(a < b) does not imply (b >= a).
            */
            bool operator>(const Vector3& other) const
            {
                return x > other.x && y > other.y && z > other.z;
            }


            /**
            * Checks if this vector is component-by-component less than
            * the other.
            *
            * @note This does not behave like a single-value comparison:
            * !(a <= b) does not imply (b > a).
            */
            bool operator<=(const Vector3& other) const
            {
                return x <= other.x && y <= other.y && z <= other.z;   
            }

            /**
            * Checks if this vector is component-by-component less than
            * the other.
            *
            * @note This does not behave like a single-value comparison:
            * !(a <= b) does not imply (b > a).
            */
            bool operator>=(const Vector3& other) const
            {
                return x >= other.x && y >= other.y && z >= other.z;   
            }

            /** Zero all the components of the vector. */
            void clear()
            {
                x = y = z = 0;
            }
            
            /** Flips all the components of the vector. */
            void invert()
            {
                x = -x;
                y = -y;
                z = -z;
            }  
    };
}

#endif // CYCLONE_CORE_H