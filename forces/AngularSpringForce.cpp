#include "SpringForce.h"
#include "AngularSpringForce.h"

#if defined(__CYGWIN__) || defined(WIN32)

#include <GL/glut.h>

#else
#include <GLUT/glut.h>
#endif

#include <assert.h>

AngularSpringForce::AngularSpringForce(Particle *p1, Particle *p2, Particle *p3, float dist, float ks, float kd) :
        AngularSpringForce({p1, p2, p3}, dist, ks, kd) {}

AngularSpringForce::AngularSpringForce(std::vector<Particle *> particles, float dist, float ks, float kd) : dist(dist),
                                                                                                            ks(ks),
                                                                                                            kd(kd) {
    setTarget(particles);
}


void AngularSpringForce::setTarget(std::vector<Particle *> particles) {
    assert(particles.size() == 3);
    this->particles = particles;
}

void AngularSpringForce::apply() {
    Vec3f l1 = particles[0]->position - particles[1]->position;
    Vec3f l2 = particles[1]->position - particles[2]->position;
    double cosAngle = (l1 * l2) / (norm(l1) * norm(l2));
    //deal with rounding errors
    if (cosAngle > 1.0) cosAngle = 1.0;
    if (cosAngle < -1.0) cosAngle = -1.0;
    double angle = acos(cosAngle);
    Vec3f l = particles[0]->position - particles[2]->position;
    Vec3f ld = particles[0]->velocity - particles[2]->velocity;

    // Compute spring force
    Vec3f result = -(ks * ( norm(l) - sin(dist/2)*norm(l1) ) + kd * ((l * ld) / norm(l)) ) * (l / norm(l));

    particles[0]->force += result;
    particles[2]->force -= result;
}

void AngularSpringForce::draw() {
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(particles[0]->position[0], particles[0]->position[1], particles[0]->position[2]);
        glVertex3f(particles[1]->position[0], particles[1]->position[1], particles[1]->position[2]);
        glVertex3f(particles[0]->position[0], particles[0]->position[1], particles[0]->position[2]);
        glVertex3f(particles[2]->position[0], particles[2]->position[1], particles[2]->position[2]);
    glEnd();
}

map<int, map<int, float>> AngularSpringForce::jx() {
    map<int, map<int, float>> values = map<int, map<int, float>>();
    return values;
}
