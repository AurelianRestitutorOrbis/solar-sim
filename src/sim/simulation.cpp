#include "sim/simulation.h"
#include <cmath>

void computeAccelerations(std::vector<Body>& bodies) {
    for (Body& body : bodies) {
        body.acceleration = {0.0, 0.0};
    }

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t j = 0; j < bodies.size(); ++j) {
            if (i == j) continue;

            Vec2 r = bodies[j].position - bodies[i].position;
            double dist = r.length();
            if (dist == 0.0) continue; // Avoid division by zero
            bodies[i].acceleration += (G * bodies[j].mass / (dist * dist * dist)) * r;
        }
    }
}

void step(std::vector<Body>& bodies, double dt) {

    for (Body& body : bodies) {
        body.velocity += body.acceleration * (0.5 * dt);
        body.position += body.velocity * dt;
    }

    computeAccelerations(bodies);

    for (Body& body : bodies) {
        body.velocity += body.acceleration * (0.5 * dt);
    }
}
