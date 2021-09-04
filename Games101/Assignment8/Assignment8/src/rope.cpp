#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

        Vector2D springstep = (end - start) / (num_nodes - 1);
        for (int i = 0; i < num_nodes; ++i)
        {
            Vector2D position;
            if (i == 0)
                position = start;
            else if (i == num_nodes - 1)
                position = end;
            else
                position = start + springstep * i;
            masses.push_back(new Mass(position, node_mass, false));

            if (i > 0)
                springs.push_back(new Spring(masses[i - 1], masses[i], k));
        }

        // Comment-in this part when you implement the constructor
        for (auto &i : pinned_nodes) {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D d = s->m2->position - s->m1->position;
            Vector2D f = s->k * d.unit() * (d.norm() - s->rest_length);
            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity * m->mass;

                // TODO (Part 2): Add global damping
                float kd  = 0.1;
                m->forces += -kd * m->velocity;

                Vector2D a = m->forces / m->mass;

                // explicit method
                // m->position += m->velocity * delta_t;
                // m->velocity += a * delta_t;

                // semi-implicit method
                m->velocity += a * delta_t;
                m->position += m->velocity * delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D d = s->m2->position - s->m1->position;
            Vector2D f = s->k * d.unit() * (d.norm() - s->rest_length);
            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                m->forces += gravity * m->mass;
                Vector2D a = m->forces / m->mass;
                // TODO (Part 3.1): Set the new position of the rope mass
                // m->position = m->position + (m->position - m->last_position) + a * delta_t * delta_t;
                // m->last_position = temp_position;
                
                // TODO (Part 4): Add global Verlet damping
                float damping_factor = 0.00005;
                m->position = m->position + (1.0 - damping_factor) * (m->position - m->last_position) + a * delta_t * delta_t;
                m->last_position = temp_position;
            }
            m->forces = Vector2D(0, 0);
        }
    }
}
