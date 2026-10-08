#include "sim/simulation.h"
#include <cmath>
#include <utility>

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

void removeCollisions(std::vector<Body>& bodies) {
    std::vector<bool> hit(bodies.size(), false);
    bool anyHit = false;

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies.size(); ++j) {
            const double dist = (bodies[j].position - bodies[i].position).length();
            if (dist < bodies[i].radius + bodies[j].radius) {
                hit[i] = true;
                hit[j] = true;
                anyHit = true;
            }
        }
    }

    if (!anyHit) return;

    std::vector<Body> survivingBodies;
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        if (!hit[i]) {
            survivingBodies.push_back(bodies[i]);
        }
    }
    bodies = std::move(survivingBodies);

    computeAccelerations(bodies);
}

std::vector<Vec2> predictPath(std::vector<Body> bodies, const Body& candidate,
                              double dt, int steps) {
    bodies.push_back(candidate);
    computeAccelerations(bodies);

    std::vector<Vec2> path;
    path.push_back(candidate.position);

    for (int s = 0; s < steps; ++s) {
        step(bodies, dt);

        const Body& self = bodies.back();
        path.push_back(self.position);

        for (std::size_t i = 0; i + 1 < bodies.size(); ++i) {
            const double dist = (bodies[i].position - self.position).length();
            if (dist < bodies[i].radius + self.radius) {
                return path;   // predicted impact
            }
        }
    }

    return path;
}