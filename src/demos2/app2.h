#include <cstdlib>
#include <cyclone2/cyclone2.h>

class Application2
{
protected:
    int height;
    int width;

public:
    virtual const char* getTitle();

    virtual void initGraphics();

    virtual void setView();

    virtual void deinit();

    virtual void display();

    virtual void update();

    virtual void key(unsigned char key);

    virtual void resize(int width, int height);

    virtual void mouse(int button, int state, int x, int y);

    virtual void mouseDrag(int x, int y);

    void renderText(float x, float y, const char*text, void* font=NULL);
};

/**
 * This application adds additional functionality used in the mass-aggregate demos.
 */
class MassAggregateApplication2 : public Application2
{
protected:
    cyclone2::ParticleWorld world;
    cyclone2::Particle2 *particleArray;
    cyclone2::GroundContacts groundContactGenerator;

public:
    MassAggregateApplication2(unsigned int particleCount);
    virtual ~MassAggregateApplication2();

    /** Update the particle positions. */
    virtual void update();

    /** Sets up the graphic rendering. */
    virtual void initGraphics();

    /** Display the particles. */
    virtual void display();
};

/**
 * This application adds additional functionality used in many of the
 * demos. This includes the ability to track contacts (for rigid bodies)
 * and move the camera around.
 */
class RigidBodyApplication2 : public Application2
{
protected:
    /** Holds the maximum number of contacts. */
    const static unsigned maxContacts = 256;

    /** Holds the array of contacts. */
    cyclone2::Contact contacts[maxContacts];

    /** Holds the collision data structure for collision detection. */
    cyclone2::CollisionData cData;

    /** Holds the contact resolver. */
    cyclone2::ContactResolver resolver;

    /** Holds the camera angle. */
    float theta;

    /** Holds the camera elevation. */
    float phi;

    /** Holds the position of the mouse at the last frame of a drag. */
    int last_x, last_y;

    /** True if the contacts should be rendered. */
    bool renderDebugInfo;

    /** True if the simulation is paused. */
    bool pauseSimulation;

    /** Pauses the simulation after the next frame automatically */
    bool autoPauseSimulation;

    /** Processes the contact generation code. */
    virtual void generateContacts() = 0;

    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(cyclone2::real duration) = 0;

    /**
     * Finishes drawing the frame, adding debugging information
     * as needed.
     */
    void drawDebug();

    /** Resets the simulation. */
    virtual void reset() = 0;

public:
    /**
     * Creates a new application object.
     */
    RigidBodyApplication2();

    /** Display the application. */
    virtual void display();

    /** Update the objects. */
    virtual void update();

    /** Handle a mouse click. */
    virtual void mouse(int button, int state, int x, int y);

    /** Handle a mouse drag */
    virtual void mouseDrag(int x, int y);

    /** Handles a key press. */
    virtual void key(unsigned char key);
};

