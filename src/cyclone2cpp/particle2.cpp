#include <assert.h>
#include <cyclone2/particle2.h>

using namespace cyclone2;

void Particle2::integrate(real duration)
{
    // We don't integrate things with zero mass.
    if (inverseMass <= 0.0f) return;

    assert(duration > 0.0);

    // Update linear position.
    position.addScaledVector(velocity, duration);

    // Work out the acceleration from the force
    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    // Update linear velocity from the acceleration.
    velocity.addScaledVector(resultingAcc, duration);

    // Impose drag.
    velocity *= real_pow(damping, duration);

    // Clear the forces.
    clearAccumulator();

}

void Particle2::setMass(const real mass)
{
    assert(mass != 0);
    Particle2::inverseMass = ((real)1.0)/mass;
}

real Particle2::getMass() const
{
    if (inverseMass == 0) 
    {
        return REAL_MAX;
    }
    else
    {
        return ((real)1.0)/inverseMass;
    }
}

void Particle2::setInverseMass(const real inverseMass)
{
    Particle2::inverseMass = inverseMass;
}

real Particle2::getInverseMass() const
{
    return inverseMass; // original doesnt have the Particle2:: clarification ???
}

bool Particle2::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void Particle2::setDamping(const real damping)
{
    Particle2::damping = damping;
}

real Particle2::getDamping() const
{
    return damping;
}

void Particle2::setPosition(const Vector3 &position)
{
    Particle2::position = position;
}

void Particle2::setPosition(const real x, const real y, const real z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void Particle2::getPosition(Vector3 *position) const
{
    *position = Particle2::position;
}

Vector3 Particle2::getPosition() const
{
    return position;
}

void Particle2::setVelocity(const Vector3 &velocity)
{
    Particle2::velocity = velocity;
}

void Particle2::setVelocity(const real x, const real y, const real z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Particle2::getVelocity(Vector3 *velocity) const
{
    *velocity = Particle2::velocity;
}

Vector3 Particle2::getVelocity() const
{
    return velocity;
}

void Particle2::setAcceleration(const Vector3& acceleration)
{
    Particle2::acceleration = acceleration;
}

void Particle2::setAcceleration( const real x, const real y, const real z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void Particle2::getAcceleration(Vector3 *acceleration) const
{
    *acceleration = Particle2::acceleration;
}

Vector3 Particle2::getAcceleration() const
{
    return acceleration;
}

void Particle2::clearAccumulator()
{
    forceAccum.clear();
}

void Particle2::addForce(const Vector3& force)
{
    forceAccum += force;
}



