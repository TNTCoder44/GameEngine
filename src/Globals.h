#pragma once

inline constexpr int WINDOW_WIDTH = 1000;
inline constexpr int WINDOW_HEIGHT = 800;

inline float yaw = -90.0f, pitch = 0.0f;

inline bool firstMouse = true;

inline float deltaTime = 0.0f;
inline float lastFrame = 0.0f;

inline glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
inline glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
inline glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
