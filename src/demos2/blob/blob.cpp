/*
 * The Blob demo.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

#include <cyclone2/cyclone2.h>
#include "../ogl_headers.h"
#include "../app2.h"
#include "../timing2.h"

#include <stdio.h>
#include <cassert>

#define BLOB_COUNT 5
#define PLATFORM_COUNT 10
#define BLOB_RADIUS 0.4f


/**
 * Platforms are two dimensional: lines on which the
 * particles can rest. Platforms are also contact generators for the physics.
 */
class Platform : public cyclone2::ParticleContactGenerator
{
public:
    cyclone2::Vector3 start;
    cyclone2::Vector3 end;

    /**
     * Holds a pointer to the particles we're checking for collisions with.
     */
    cyclone2::Particle2 *particles;

    virtual unsigned addContact(
        cyclone2::ParticleContact *contact,
        unsigned limit
        ) const;
};

unsigned Platform::addContact(cyclone2::ParticleContact *contact,
                              unsigned limit) const
{
    const static cyclone2::real restitution = 0.0f;

    unsigned used = 0;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        if (used >= limit) break;

        // Check for penetration
        cyclone2::Vector3 toParticle = particles[i].getPosition() - start;
        cyclone2::Vector3 lineDirection = end - start;
        cyclone2::real projected = toParticle * lineDirection;
        cyclone2::real platformSqLength = lineDirection.squareMagnitude();
        if (projected <= 0)
        {
            // The blob is nearest to the start point
            if (toParticle.squareMagnitude() < BLOB_RADIUS*BLOB_RADIUS)
            {
                // We have a collision
                contact->contactNormal = toParticle.unit();
                contact->contactNormal.z = 0;
                contact->restitution = restitution;
                contact->particle[0] = particles + i;
                contact->particle[1] = 0;
                contact->penetration = BLOB_RADIUS - toParticle.magnitude();
                used ++;
                contact ++;
            }

        }
        else if (projected >= platformSqLength)
        {
            // The blob is nearest to the end point
            toParticle = particles[i].getPosition() - end;
            if (toParticle.squareMagnitude() < BLOB_RADIUS*BLOB_RADIUS)
            {
                // We have a collision
                contact->contactNormal = toParticle.unit();
                contact->contactNormal.z = 0;
                contact->restitution = restitution;
                contact->particle[0] = particles + i;
                contact->particle[1] = 0;
                contact->penetration = BLOB_RADIUS - toParticle.magnitude();
                used ++;
                contact ++;
            }
        }
        else
        {
            // the blob is nearest to the middle.
            cyclone2::real distanceToPlatform =
                toParticle.squareMagnitude() -
                projected*projected / platformSqLength;
            if (distanceToPlatform < BLOB_RADIUS*BLOB_RADIUS)
            {
                // We have a collision
                cyclone2::Vector3 closestPoint =
                    start + lineDirection*(projected/platformSqLength);

                contact->contactNormal = (particles[i].getPosition()-closestPoint).unit();
                contact->contactNormal.z = 0;
                contact->restitution = restitution;
                contact->particle[0] = particles + i;
                contact->particle[1] = 0;
                contact->penetration = BLOB_RADIUS - real_sqrt(distanceToPlatform);
                used ++;
                contact ++;
            }
        }
    }
    return used;
}

/**
 * A force generator for proximal attraction.
 */
class BlobForceGenerator : public cyclone2::ParticleForceGenerator
{
public:
    /**
    * Holds a pointer to the particles we might be attracting.
    */
    cyclone2::Particle2 *particles;

    /**
     * The maximum force used to push the particles apart.
     */
    cyclone2::real maxReplusion;

    /**
     * The maximum force used to pull particles together.
     */
    cyclone2::real maxAttraction;

    /**
     * The separation between particles where there is no force.
     */
    cyclone2::real minNaturalDistance, maxNaturalDistance;

    /**
     * The force with which to float the head particle, if it is
     * joined to others.
     */
    cyclone2::real floatHead;

    /**
     * The maximum number of particles in the blob before the head
     * is floated at maximum force.
     */
    unsigned maxFloat;

    /**
     * The separation between particles after which they 'break' apart and
     * there is no force.
     */
    cyclone2::real maxDistance;

    virtual void updateForce(
        cyclone2::Particle2 *particle,
        cyclone2::real duration
        );
};

void BlobForceGenerator::updateForce(cyclone2::Particle2 *particle,
                                      cyclone2::real duration)
{
    unsigned joinCount = 0;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        // Don't attract yourself
        if (particles + i == particle) continue;

        // Work out the separation distance
        cyclone2::Vector3 separation =
            particles[i].getPosition() - particle->getPosition();
        separation.z = 0.0f;
        cyclone2::real distance = separation.magnitude();

        if (distance < minNaturalDistance)
        {
            // Use a repulsion force.
            distance = 1.0f - distance / minNaturalDistance;
            particle->addForce(
                separation.unit() * (1.0f - distance) * maxReplusion * -1.0f
                );
            joinCount++;
        }
        else if (distance > maxNaturalDistance && distance < maxDistance)
        {
            // Use an attraction force.
            distance =
                (distance - maxNaturalDistance) /
                (maxDistance - maxNaturalDistance);
            particle->addForce(
                separation.unit() * distance * maxAttraction
                );
            joinCount++;
        }
    }

    // If the particle is the head, and we've got a join count, then float it.
    if (particle == particles && joinCount > 0 && maxFloat > 0)
    {
        cyclone2::real force = cyclone2::real(joinCount / maxFloat) * floatHead;
        if (force > floatHead) force = floatHead;
        particle->addForce(cyclone2::Vector3(0, force, 0));
    }

}

/**
 * The main demo class definition.
 */
class BlobDemo : public Application2
{
    cyclone2::Particle2 *blobs;

    Platform *platforms;

    cyclone2::ParticleWorld world;

    BlobForceGenerator blobForceGenerator;

    /* The control for the x-axis. */
    float xAxis;

    /* The control for the y-axis. */
    float yAxis;

    void reset();

public:
    /** Creates a new demo object. */
    BlobDemo();
    virtual ~BlobDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);

};

// Method definitions
BlobDemo::BlobDemo()
:
xAxis(0), yAxis(0),
world(PLATFORM_COUNT+BLOB_COUNT, PLATFORM_COUNT)
{
    // Create the blob storage
    blobs = new cyclone2::Particle2[BLOB_COUNT];
    cyclone2::Random r;

    // Create the force generator
    blobForceGenerator.particles = blobs;
    blobForceGenerator.maxAttraction = 20.0f;
    blobForceGenerator.maxReplusion = 10.0f;
    blobForceGenerator.minNaturalDistance = BLOB_RADIUS*0.75f;
    blobForceGenerator.maxNaturalDistance = BLOB_RADIUS*1.5f;
    blobForceGenerator.maxDistance = BLOB_RADIUS * 2.5f;
    blobForceGenerator.maxFloat = 2;
    blobForceGenerator.floatHead = 8.0f;

    // Create the platforms
    platforms = new Platform[PLATFORM_COUNT];
    for (unsigned i = 0; i < PLATFORM_COUNT; i++)
    {
        platforms[i].start = cyclone2::Vector3(
            cyclone2::real(i%2)*10.0f - 5.0f,
            cyclone2::real(i)*4.0f + ((i%2)?0.0f:2.0f),
            0);
        platforms[i].start.x += r.randomBinomial(2.0f);
        platforms[i].start.y += r.randomBinomial(2.0f);

        platforms[i].end = cyclone2::Vector3(
            cyclone2::real(i%2)*10.0f + 5.0f,
            cyclone2::real(i)*4.0f + ((i%2)?2.0f:0.0f),
            0);
        platforms[i].end.x += r.randomBinomial(2.0f);
        platforms[i].end.y += r.randomBinomial(2.0f);

        // Make sure the platform knows which particles it
        // should collide with.
        platforms[i].particles = blobs;
        world.getContactGenerators().push_back(platforms + i);
    }

    // Create the blobs.
    Platform *p = platforms + (PLATFORM_COUNT-2);
    cyclone2::real fraction = (cyclone2::real)1.0 / BLOB_COUNT;
    cyclone2::Vector3 delta = p->end - p->start;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        unsigned me = (i+BLOB_COUNT/2) % BLOB_COUNT;
        blobs[i].setPosition(
            p->start + delta * (cyclone2::real(me)*0.8f*fraction+0.1f) +
            cyclone2::Vector3(0, 1.0f+r.randomReal(), 0));

        blobs[i].setVelocity(0,0,0);
        blobs[i].setDamping(0.2f);
        blobs[i].setAcceleration(cyclone2::Vector3::GRAVITY * 0.4f);
        blobs[i].setMass(1.0f);
        blobs[i].clearAccumulator();

        world.getParticles().push_back(blobs + i);
        world.getForceRegistry().add(blobs + i, &blobForceGenerator);
    }
}

void BlobDemo::reset()
{
    cyclone2::Random r;
    Platform *p = platforms + (PLATFORM_COUNT-2);
    cyclone2::real fraction = (cyclone2::real)1.0 / BLOB_COUNT;
    cyclone2::Vector3 delta = p->end - p->start;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        unsigned me = (i+BLOB_COUNT/2) % BLOB_COUNT;
        blobs[i].setPosition(
            p->start + delta * (cyclone2::real(me)*0.8f*fraction+0.1f) +
            cyclone2::Vector3(0, 1.0f+r.randomReal(), 0));
        blobs[i].setVelocity(0,0,0);
        blobs[i].clearAccumulator();
    }
}

BlobDemo::~BlobDemo()
{
    delete blobs;
}

void BlobDemo::display()
{
    cyclone2::Vector3 pos = blobs[0].getPosition();

    // Clear the view port and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, 6.0,  pos.x, pos.y, 0.0,  0.0, 1.0, 0.0);

    glColor3f(0,0,0);


    glBegin(GL_LINES);
    glColor3f(0,0,1);
    for (unsigned i = 0; i < PLATFORM_COUNT; i++)
    {
        const cyclone2::Vector3 &p0 = platforms[i].start;
        const cyclone2::Vector3 &p1 = platforms[i].end;
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
    }
    glEnd();

    glColor3f(1,0,0);
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        const cyclone2::Vector3 &p = blobs[i].getPosition();
        glPushMatrix();
        glTranslatef(p.x, p.y, p.z);
        glutSolidSphere(BLOB_RADIUS, 12, 12);
        glPopMatrix();
    }

    cyclone2::Vector3 p = blobs[0].getPosition();
    cyclone2::Vector3 v = blobs[0].getVelocity() * 0.05f;
    v.trim(BLOB_RADIUS*0.5f);
    p = p + v;
    glPushMatrix();
    glTranslatef(p.x-BLOB_RADIUS*0.2f, p.y, BLOB_RADIUS);
    glColor3f(1,1,1);
    glutSolidSphere(BLOB_RADIUS*0.2f, 8, 8);
    glTranslatef(0,0,BLOB_RADIUS*0.2f);
    glColor3f(0,0,0);
    glutSolidSphere(BLOB_RADIUS*0.1f, 8, 8);
    glTranslatef(BLOB_RADIUS*0.4f, 0, -BLOB_RADIUS*0.2f);
    glColor3f(1,1,1);
    glutSolidSphere(BLOB_RADIUS*0.2f, 8, 8);
    glTranslatef(0,0,BLOB_RADIUS*0.2f);
    glColor3f(0,0,0);
    glutSolidSphere(BLOB_RADIUS*0.1f, 8, 8);
    glPopMatrix();
}

void BlobDemo::update()
{
    // Clear accumulators
    world.startFrame();

    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    // Recenter the axes
    xAxis *= pow(0.1f, duration);
    yAxis *= pow(0.1f, duration);

    // Move the controlled blob
    blobs[0].addForce(cyclone2::Vector3(xAxis, yAxis, 0)*10.0f);

    // Run the simulation
    world.runPhysics(duration);

    // Bring all the particles back to 2d
    cyclone2::Vector3 position;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        blobs[i].getPosition(&position);
        position.z = 0.0f;
        blobs[i].setPosition(position);
    }

    Application2::update();
}

const char* BlobDemo::getTitle()
{
    return "Cyclone > Blob Demo";
}

void BlobDemo::key(unsigned char key)
{
    switch(key)
    {
    case 'w': case 'W':
        yAxis = 1.0;
        break;
    case 's': case 'S':
        yAxis = -1.0;
        break;
    case 'a': case 'A':
        xAxis = -1.0f;
        break;
    case 'd': case 'D':
        xAxis = 1.0f;
        break;
    case 'r': case 'R':
        reset();
        break;
    }
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application2* getApplication()
{
    return new BlobDemo();
}