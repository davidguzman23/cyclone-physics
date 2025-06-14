#ifndef CYCLONE_PARTICLE_H
#define CYCLONE_PARTICLE_H

#include "core2.h"

namespace cyclone2 {

    class Particle2
    {
        public:

        protected:

            real damping;

            real inverseMass;

            Vector3 position;
            Vector3 velocity;
            Vector3 acceleration;

            Vector3 forceAccum;

        public:

            void integrate(real duration);

            /* Mass should not be zero. Invalidates internal particle data. Should not be called before integrating! */
            void setMass(const real mass);
            real getMass() const;

            /* Mass can be zero, to account for objects with infinite mass. 
            Invalidates internal particle data. Should not be called before integrating! */
            void setInverseMass(const real inverseMass);
            real getInverseMass() const;

            bool hasFiniteMass() const;

            void setDamping( const real damping);
            real getDamping() const;

            void setPosition(const Vector3 &position);
            void setPosition(const real x, const real y, const real z);
            void getPosition(Vector3 *position) const;
            Vector3 getPosition() const;

            void setVelocity(const Vector3 &velocity);
            void setVelocity(const real x, const real y, const real z);

            /* returned velocities are returned in the local world space */
            void getVelocity(Vector3 *velocity) const;
            Vector3 getVelocity() const;

            void setAcceleration(const Vector3 &acceleration);
            void setAcceleration(const real x, const real y, const real z);

            void getAcceleration(Vector3 *acceleration) const;
            Vector3 getAcceleration() const;

            void clearAccumulator();
            void addForce(const Vector3 &force);
    };

}

#endif // CYCLONE_BODY_H
