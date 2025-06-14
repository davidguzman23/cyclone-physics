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

