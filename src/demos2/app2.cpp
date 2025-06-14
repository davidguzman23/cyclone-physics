#include <cstring>
#include "ogl_headers.h"
#include "app2.h"
#include "timing2.h"

void Application2::initGraphics()
{
    glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    setView();
}

void Application2::setView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width/(double)height, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void Application2::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(639, 319);
    glEnd();
}

const char* Application2::getTitle()
{
    return "Cyclone Demo";
}

void Application2::deinit()
{
}

void Application2::update()
{
    glutPostRedisplay();
}

void Application2::key(unsigned char key)
{
}

void Application2::resize(int width, int height)
{
    // Avoid the divide by zero.
    if (height <= 0) height = 1;
    
    // Set the internal variables and update the view
    Application2::width = width;
    Application2::height = height;
    glViewport(0, 0, width, height);
    setView();
}

void Application2::mouse(int button, int state, int x, int y)
{
}

void Application2::mouseDrag(int x, int y)
{
}

// The following methods aren't intended to be overloaded
void Application2::renderText(float x, float y, const char *text, void *font)
{
    glDisable(GL_DEPTH_TEST);

    // Temporarily set up the view in orthographic projection.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

    // Move to modelview mode.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Ensure we have a font
    if (font == NULL)
    {
        font = GLUT_BITMAP_HELVETICA_10;
    }

    // Loop through characters displaying them.
    size_t len = strlen(text);

    glRasterPos2f(x, y);
    for (const char *letter = text; letter < text+len; letter++)
    {
        // If we meet a newline, then move down by the line-height
        // TODO: Make the line-height a function of the font
        if (*letter == '\n')
        {
            y -= 12.0f;
            glRasterPos2f(x, y);
        }
        glutBitmapCharacter(font, *letter);
    }

    // Pop the matrices to return to how we were before.
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);

}


MassAggregateApplication2::MassAggregateApplication2(unsigned int particleCount)
: 
world(particleCount*10)
{
    particleArray = new cyclone2::Particle2[particleCount];
    for (unsigned i = 0; i < particleCount; i++)
    {
        world.getParticles().push_back(particleArray + i);
    }

    groundContactGenerator.init(&world.getParticles());
    world.getContactGenerators().push_back(&groundContactGenerator);
}

MassAggregateApplication2::~MassAggregateApplication2()
{
    delete[] particleArray;
}

void MassAggregateApplication2::initGraphics()
{
    // Call the superclass
    Application2::initGraphics();
}

void MassAggregateApplication2::display()
{
    // Clear the view port and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 3.5, 8.0,  0.0, 3.5, 0.0,  0.0, 1.0, 0.0);

    glColor3f(0,0,0);

    cyclone2::ParticleWorld::Particles2 &particles = world.getParticles(); // wtf where is world defined
    for (cyclone2::ParticleWorld::Particles2::iterator p = particles.begin();
        p != particles.end();
        p++)
    {
        cyclone2::Particle2 *particle = *p;
        const cyclone2::Vector3 &pos = particle->getPosition();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glutSolidSphere(0.1f, 20, 10);
        glPopMatrix();

    }

}

void MassAggregateApplication2::update()
{
    // clear accumulators
    world.startFrame();

    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    // Run the simulation
    world.runPhysics(duration);

    Application2::update();
}

