#include "Duck.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* duck::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    if (!name.compare("wireframe") || !name.compare("scorePoint")) {
        vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length, length / 4, 0), color),
            VertexFormat(corner + glm::vec3(0, length / 4, 0), color)
        };
    }
    else {
        vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length, length / 2, 0), color),
            VertexFormat(corner + glm::vec3(0, length / 2, 0), color)
        };
    }

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* duck::CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, length/2, 0), color),
        VertexFormat(corner + glm::vec3(length,length/4,0),color),
    };
    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = {
        0,1,2
    };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }

    triangle->InitFromData(vertices, indices);

    return triangle;
}

Mesh* duck::CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill)
{
    int steps = 72;
    float angle = M_PI * 2.f / steps;
    float prevX = center[0];
    float prevY = center[1] - radius;


    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(prevX,prevY,0),color)
    };
    std::vector<unsigned int> indices = {
        0,1,2
    };

    for (int i = 0; i <= steps; i++) {
        float newX = radius * sin(angle * i);
        float newY = - radius * cos(angle * i);

        vertices.push_back(VertexFormat(glm::vec3(newX, newY, 0),color));
        if (i > 2) {
            indices.push_back(0);
            indices.push_back(i - 1);
            indices.push_back(i);
        }
        prevX = newX;
        prevY = newY;
    }
    Mesh* circle = new Mesh(name);

    circle->InitFromData(vertices, indices);

    return circle;
}
