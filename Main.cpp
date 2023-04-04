// main.cpp
#include <iostream>
#include <vector>
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 normalized() const {
        float len = length();
        return Vector3(x / len, y / len, z / len);
    }
};

class Particle {
public:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float mass;

    Particle(const Vector3& position, float mass) : position(position), mass(mass) {
        velocity = Vector3();
        acceleration = Vector3();
    }

    void applyForce(const Vector3& force) {
        Vector3 f = force / mass;
        acceleration += f;
    }

    void update(float dt) {
        velocity += acceleration * dt;
        position += velocity * dt;
        acceleration = Vector3();
    }
};

class World {
public:
    std::vector<Particle> particles;
    Vector3 gravity;

    World(const Vector3& gravity) : gravity(gravity) {}

    void addParticle(const Particle& particle) {
        particles.push_back(particle);
    }

    void update(float dt) {
        for (auto& p : particles) {
            p.applyForce(gravity * p.mass);
            p.update(dt);
        }
    }

    bool rayIntersectsParticle(const Vector3& rayOrigin, const Vector3& rayDirection, const Particle& particle, float radius) {
        Vector3 originToParticle = particle.position - rayOrigin;
        float t = originToParticle.dot(rayDirection);
        Vector3 closestPoint = rayOrigin + rayDirection * t;
        Vector3 closestToParticle = particle.position - closestPoint;

        return closestToParticle.length() <= radius;
    }
};

int main() {
    Vector3 gravity(0, -9.81, 0);
    World world(gravity);

    Particle p1(Vector3(0, 10, 0), 1);
    Particle p2(Vector3(5, 15, 0), 2);

    world.addParticle(p1);
    world.addParticle(p2);

    float dt = 0.01f;
    int steps = 100;

    Vector3 rayOrigin(0, 0, 0);
    Vector3 rayDirection(1, 1, 0);
    rayDirection = rayDirection.normalized();

    for (int i = 0; i < steps; i++) {
        world.update(dt);

        std::cout << "Step " << i + 1 << ":\n";
        for (const auto& p : world.particles) {
std::cout << "Position: (" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")\n";
if (world.rayIntersectsParticle(rayOrigin, rayDirection, p, 1.0f)) {
std::cout << "Ray intersects with particle at position (" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")\n";
}
}
}

return 0;
}
