#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;
};

class ParticleSystem {
private:
    std::vector<Particle> particles;
    sf::VertexArray vertices;
    float particleSize = 2.0f;

public:
    ParticleSystem() {
        // why sfml... why dont you have squares
        vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    }

    void setScale(float scale) {
        particleSize = 2.0f * scale;
    }

    // move the particles
    void emit(sf::Vector2f pos, int count = 2) {
        for (int i = 0; i < count; ++i) {
            // slight random spread for cool effect
            float vx = static_cast<float>((std::rand() % 40) - 20);
            float vy = -static_cast<float>((std::rand() % 50) + 20);
            particles.push_back({ pos, {vx, vy}, 1.0f });
        }
    }

    void update(float dt) {
        vertices.clear();
        for (auto it = particles.begin(); it != particles.end();) {
            it->lifetime -= dt;
            // destroy particles that are over their lifetime / move the rest
            if (it->lifetime <= 0) {
                it = particles.erase(it);
            } else {
                it->position += it->velocity * dt;
                addParticleVertices(*it);
                ++it;
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (vertices.getVertexCount() > 0) {
            window.draw(vertices);
        }
    }

private:
    void addParticleVertices(const Particle& p) {
        unsigned char alpha = static_cast<unsigned char>(255 * p.lifetime);
        sf::Color color(150, 0, 255, alpha);

        // vertex construction via designated initializers
        sf::Vertex v1{.position = p.position, .color = color};
        sf::Vertex v2{.position = {p.position.x + particleSize, p.position.y}, .color = color};
        sf::Vertex v3{.position = {p.position.x + particleSize, p.position.y + particleSize}, .color = color};
        sf::Vertex v4{.position = {p.position.x, p.position.y + particleSize}, .color = color};

        // making squares from triangles :)
        vertices.append(v1); vertices.append(v2); vertices.append(v3);
        vertices.append(v1); vertices.append(v3); vertices.append(v4);
    }
};
