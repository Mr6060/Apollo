module;

#include "raylib.h"
#include "raymath.h"

export module SpaceCamera;

import std;
import IUpdate;

export class SpaceCamera final : public IUpdate {
private:
    float _rotationSpeed{0.2f};
    float _zoomSpeed{60.0f};
    float _smoothZoomSpeed{5.0f};
    float _zoom{20.0f};
    float _maxZoom{10.0f};
    float _minZoom{500.0f};
    const Vector3* _target;
    float _targetZoom{_zoom};
    Camera3D _camera = {{0.0f, 10.0f, 20.0f}, *_target, {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE};

public:
    float Yaw{};
    float Pitch{0.6f};

    // Constructor
    explicit SpaceCamera(const Vector3& target) noexcept : _target{&target}
    {
    }

    // Destructor
    ~SpaceCamera() noexcept override = default;

    // Copy Constructor
    SpaceCamera(const SpaceCamera&) noexcept = default;

    // Copy Assignment Operator
    SpaceCamera& operator=(const SpaceCamera&) noexcept = default;

    // Move Constructor
    SpaceCamera(SpaceCamera&&) noexcept = default;

    // Move Assignment Operator
    SpaceCamera& operator=(SpaceCamera&&) noexcept = default;

    void Begin() const noexcept
    {
        BeginMode3D(_camera);
    }

    void End() const noexcept
    {
        EndMode3D();
    }

    void Update(const float ms) noexcept override
    {
        // Check for mouse input for yaw and pitch rotation
        auto [x, y]{GetMouseDelta()};
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Yaw += x * _rotationSpeed * ms;
            Pitch += y * _rotationSpeed * ms;

            // Optionally clamp pitch to avoid flipping
            constexpr float pitchLimit = PI / 2.0f - 0.1f;
            if (Pitch > pitchLimit)
                Pitch = pitchLimit;
            if (Pitch < 0.0f)
                Pitch = 0.0f; // Ensure we can't go below the horizontal plane
        }

        // Zoom control via mouse wheel
        const float zoomDelta = GetMouseWheelMove() * _zoomSpeed * ms;
        _targetZoom -= zoomDelta;

        // Clamp target zoom to the allowed range
        if (_targetZoom < _maxZoom)
            _targetZoom = _maxZoom;
        if (_targetZoom > _minZoom)
            _targetZoom = _minZoom;

        // Smooth zoom interpolation
        _zoom = Lerp(_zoom, _targetZoom, _smoothZoomSpeed * ms);

        // Recalculate the camera's position in spherical coordinates
        _camera.position.x = _target->x + _zoom * std::cos(Yaw) * std::cos(Pitch);
        _camera.position.y = _target->y + _zoom * std::sin(Pitch);
        _camera.position.z = _target->z + _zoom * std::sin(Yaw) * std::cos(Pitch);

        // Keep the camera pointed at the target
        _camera.target = *_target;
    }
};