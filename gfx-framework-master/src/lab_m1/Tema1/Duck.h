#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace duck
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
}
