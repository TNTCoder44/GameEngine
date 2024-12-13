#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

inline constexpr int WINDOW_WIDTH = 1200;
inline constexpr int WINDOW_HEIGHT = 1000;

inline bool firstMouse = true;

inline float deltaTime = 0.0f;
inline float lastFrame = 0.0f;

inline float mixValue = 0.2f;
