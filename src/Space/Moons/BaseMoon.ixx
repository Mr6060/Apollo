module;

#include "raylib.h"

export module BaseMoon;

import std;
import BaseSpace;

export class BaseMoon : public BaseSpace {
private:
    Vector3 _orbitPathAxis{1.0f, 0.0f, 0.0f};
    Vector3 _orbitPathCenter{0.0f, 1.0f, 0.0f};

public:
    float Radius;
    Color PlanetColor;
    float OrbitRadius;
    float OrbitSpeed;
    float OrbitAngle;
    Vector3 Position{};

    // Constructor
    BaseMoon(const float radius, const Color& planetColor, const float orbitRadius, const float orbitSpeed,
               const float orbitAngle) noexcept
        : Radius{radius}, PlanetColor{planetColor}, OrbitRadius{orbitRadius}, OrbitSpeed{orbitSpeed},
          OrbitAngle{orbitAngle}
    {
        Position.y = 1.0f;
    }

    // Destructor
    ~BaseMoon() noexcept override = default;

    // Copy Constructor
    BaseMoon(const BaseMoon&) noexcept = default;

    // Copy Assignment Operator
    BaseMoon& operator=(const BaseMoon&) noexcept = default;

    // Move Constructor
    BaseMoon(BaseMoon&&) noexcept = default;

    // Move Assignment Operator
    BaseMoon& operator=(BaseMoon&&) noexcept = default;

    void SetOrbitPathCenter(const Vector3 orbit_center)
    {
        _orbitPathCenter = orbit_center;
    }

    void Update(const float ms) noexcept override
    {
        OrbitAngle += OrbitSpeed * ms;

        Position.x = _orbitPathCenter.x + OrbitRadius * std::cosf(OrbitAngle);
        Position.z = _orbitPathCenter.z + OrbitRadius * std::sinf(OrbitAngle);
    }

    void Draw() noexcept override
    {
        // Draw planet
        DrawSphere(Position, Radius, PlanetColor);

        // Draw the orbit path
        DrawCircle3D(_orbitPathCenter, OrbitRadius, _orbitPathAxis, 90.0f, LIGHTGRAY);
    }
};