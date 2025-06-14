#ifndef CYCLONE_PFGEN_H
#define CYCLONE_PFGEN_H

#include "core2.h"
#include "particle2.h"
#include <vector>

namespace cyclone2 {

    /**
     * A force generator can be asked to add a force to one or more
     * particles.
     */
    class ParticleForceGenerator
    {
    public:

        /**
         * Overload this in implementations of the interface to calculate
         * and update the force applied to the given particle. 
         * (= 0 means its a pure virtual function, meaning its base class does not define it but those who inherit the base class have to.)
         */

        virtual void updateForce(Particle2 *particle, real duration) = 0;
    };

    /**
     * A force generator that applies a gravitational force. One instance
     * can be used for multiple particles.
     */
    class ParticleGravity : public ParticleForceGenerator
    {
        /** Holds the acceleration due to gravity. */
        Vector3 gravity;

    public:

        /** Creates the generator with the given acceleration. */
        ParticleGravity(const Vector3 &gravity);

        /** Applies the gravitational force to the given particle. */
        virtual void updateForce(Particle2 *particle, real duration);
    };

    /**
     * A force generator that applies a drag force. One instance
     * can be used for multiple particles.
     */
    class ParticleDrag : public ParticleForceGenerator
    {

        /** Holds the velocity drag coeffificent. */
        real k1;

        /** Holds the velocity squared drag coeffificent. */
        real k2;

    public:
        
        /** Creates the generator with the given coefficients. */
        ParticleDrag(real k1, real k2);

        /** Applies the drag force to the given particle. */
        virtual void updateForce(Particle2 *particle, real duration);

    };

    /**
     * A force generator that applies a spring force, where one 
     * end is attached to a fixed point in space.
     */
    class ParticleAnchoredSpring : public ParticleForceGenerator
    {
    protected:
        /** The location of the anchored end of the spring. */
        Vector3 *anchor;

        /** Holds the spring constant */
        real springConstant;

        /** Holds the rest length of the spring */
        real restLength;

    public:
        ParticleAnchoredSpring();

        /** Creates a new spring with the given parameters */
        ParticleAnchoredSpring(Vector3 *anchor,
                               real springConstant,
                               real restlength);
        
        /** Retrieve the anchor point. */
        const Vector3* getAnchor() const { return anchor; }

        /** Set the spring's properties. */
        void init(Vector3 *anchor,
                 real springConstant,
                 real restLength);

        /** Applies the spring force to the given particle. */
        virtual void updateForce(Particle2 *particle, real duration);
    };

    /**
    * A force generator that applies a bungee force, where
    * one end is attached to a fixed point in space.
    */
    class ParticleAnchoredBungee : public ParticleAnchoredSpring
    {
    public:
        /** Applies the spring force to the given particle. */
        virtual void updateForce(Particle2 *particle, real duration);
    };

    /** 
     * A force generator that fakes a stiff spring force, and where 
     * one end is attached to a fixed point in space 
     * */
    class ParticleFakeSpring : public ParticleForceGenerator
    {
        /** The location of the anchored end of the spring. */
        Vector3 *anchor;
        
        /** Holds the spring constant. */
        real springConstant;

        /** Holds the damping on the oscillation of the spring.*/
        real damping;

    public:

        /** Creates a new spring with the given parameters */
        ParticleFakeSpring(Vector3 *anchor, real springConstant, 
            real damping);

        /** Applies the spring force to the given particle. */
        virtual void updateForce(Particle2 *particle, real duration);
    };

    /**
     * A force generator that applies a Spring force.
     */
    class ParticleSpring : public ParticleForceGenerator
    {
        /** The particle at the other end of the spring. */
        Particle2 *other;

        /** Holds the spring constant. */
        real springConstant;

        /** Holds the rest length of the spring. */
        real restLength;

    public:

        /** Creates a new spring with the given parameters. */
        ParticleSpring(Particle2 *other, 
            real springConstant, real restLength);

        /** Applies the spring force to the given particle */
        virtual void updateForce(Particle2 *particle, real duration);

    };

    /**
     * A force generator that applies a spring force only
     * when extended.
     */
    class ParticleBungee : public ParticleForceGenerator
    {
        /** The particle at the other end of the spring. */
        Particle2 *other;
        
        /** Holds the sprint constant. */
        real springConstant;

        /** Holds the length of the bungee at the point it begins to generate a force. */
        real restLength;

    public:

        /** Creates a new bungee with the given parameters. */
        ParticleBungee(Particle2 *other, 
            real springConstant, real restLength);

        /** Applies the spring force to the given particle. */
        virtual void updateForce(Particle2 *particle, real duration);

    };
    
    /**
     *  A force generator that applies a buoyancy force for a plane of liquid
     *  parallel to XZ plane.
     */
    class ParticleBuoyancy : public ParticleForceGenerator
    {

        /** 
         * The maximum submersion depth of the object before it generates 
         * its maximum buoyancy force.
         */
        real maxDepth;

        /**
         * The volume of the object.
         */
        real volume;

        /**
         * The height of the water plane above y = 0. The plane will be parallel to the XZ plane. 
         */
        real waterHeight;

        /**
         * The density of the liquid. Pure water has a density of 1000 kg per cubic meter.
         */
        real liquidDensity;

    public:

        /** Creates a new buoyancy force with the given parameters */
        ParticleBuoyancy(real maxDepth, real volume, real waterHeight, 
            real liquidDensity = 1000.0f);

        /** Applies the buoyancy force to the given particle */
        virtual void updateForce(Particle2 *particle, real duration);


    };

    /**
     * Holds all the force generators and the particles they apply to.
     */
    class ParticleForceRegistry
    {
    protected:
        
        /**
         * Keeps track of one force generator and the particle it
         * applies to.
         */
        struct ParticleForceRegistration
        {
            Particle2 *particle;
            ParticleForceGenerator *fg;
        };

        /**
         * Holds the list of registrations.
         */
        typedef std::vector<ParticleForceRegistration> Registry;
        Registry registrations;

    public:

        /**
         * Registers the given force generator to apply to the
         * given particle.
         */
        void add(Particle2* particle, ParticleForceGenerator *fg);

        /**
         * Removes the given registered pair from the registry.
         * If the pair is not registered, this method will have
         * no effect.
         */
        void remove(Particle2* particle, ParticleForceGenerator *fg );

        /**
         * Clears all registrations from the registry. This will
         * not delete the particles or the force generators
         * themselves, just the records of their connection.
         */
        void clear();

        /**
         * Calls all the force generators to update the forces of
         * their corresponding particles.
         */
        void updateForces(real duration);
    };
}

#endif //CYCLONE_PFGEN_H