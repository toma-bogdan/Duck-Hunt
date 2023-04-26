#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/duck.h"
#include "lab_m1/Tema1/transform2D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
   
    //Screen margins
    xMax = (float)resolution.x;
    yMax = (float)resolution.y;

    moveDuckX = rand() % ((int)xMax - 300) + 150;
    moveDuckY = 50;
    
    angularStep = 0;
    duckSpeed = 3.5;
    duckAngle = RADIANS(rand() % 130 + 30);
    swingForward = false;

    //Calculates the position of the duck's head depending on the angle
    headOffsetX = (175.f) * cos(duckAngle) - (175.f / 4) * sin(duckAngle);
    headOffsetY = (175.f) * sin(duckAngle) + (175.f / 4) * cos(duckAngle);

    //Calculates the position of the duck's nose depending on the angle
    noseOffsetX = (175.f) * cos(duckAngle) - (175.f / 6) * sin(duckAngle);
    noseOffsetY = (175.f) * sin(duckAngle) + (175.f / 6) * cos(duckAngle);

    //Calculates the position of the duck's wing depending on the angle
    wingOffsetX = (175.f / 4) * cos(duckAngle) - (175.f / 4) * sin(duckAngle);
    wingOffsetY = (175.f / 4) * sin(duckAngle) + (175.f / 4) * cos(duckAngle);

    //Calculates the center of the duck depending on the angle
    cX = (175.f / 2) * cos(duckAngle) - (175.f / 4) * sin(duckAngle);
    cY = (175.f / 2) * sin(duckAngle) + (175.f / 4) * cos(duckAngle);


    glm::vec3 corner = glm::vec3(0, 0, 0);    

    Mesh* hitbox = duck::CreateRectangle("hitbox", corner, hitboxSize, glm::vec3(0.7, 0.7, 1), false);
    AddMeshToList(hitbox);

    Mesh* bullet = duck::CreateRectangle("bullet", corner, 30, glm::vec3(0, 1, 0), true);
    AddMeshToList(bullet);
    
    Mesh* grass = duck::CreateRectangle("grass", corner, xMax, glm::vec3(0, 1, 0), true);
    AddMeshToList(grass);

    Mesh* wireframe = duck::CreateRectangle("wireframe", corner, 275, glm::vec3(0, 0, 1));
    AddMeshToList(wireframe);

    Mesh* scorePoint = duck::CreateRectangle("scorePoint", corner, 69, glm::vec3(0, 0, 1), true);
    AddMeshToList(scorePoint);

    Mesh* body = duck::CreateTriangle("body", corner, 175, glm::vec3(0.35, 0.16, 0.16), true);
    AddMeshToList(body);

    Mesh* wing = duck::CreateTriangle("wing", corner, 90, glm::vec3(0.35, 0.16, 0.16), true);
    AddMeshToList(wing);

    Mesh* second_wing = duck::CreateTriangle("second_wing", corner, 90, glm::vec3(0.35, 0.16, 0.16), true);
    AddMeshToList(second_wing);

    Mesh* nose = duck::CreateTriangle("nose", corner, 70, glm::vec3(1, 1, 0), true);
    AddMeshToList(nose);

    Mesh* circle = duck::CreateCircle("circle", corner, 40, glm::vec3(0, 0.5, 0), true);
    AddMeshToList(circle);

    Mesh* lives = duck::CreateCircle("lives", corner, 20, glm::vec3(1, 0, 0), true);
    AddMeshToList(lives);
}

void Tema1::FrameStart()
{
    // Change the background (win, lose, still playing)
    if (score >= 16) {
        glClearColor(0, 1, 0, 1);
    }
    else if (nrOfLives == 0) {
        glClearColor(1, 0, 0, 1);
    }
    else {
        glClearColor(0.7, 0.7, 1, 1);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    // Adjust the position of the duck and count the time it has been on the screen
    moveDuckX += cos(duckAngle) * duckSpeed;
    moveDuckY += sin(duckAngle) * duckSpeed;
    escapeTime += deltaTimeSeconds;

    // Adjust rotation for wings
    if (!swingForward) {
        angularStep += duckSpeed/100;
    }
    else {
        angularStep -= duckSpeed/100;
    }
    if (angularStep > 0.7) {
        swingForward = true;
    }
    if (angularStep < -0.2) {
        swingForward = false;
    }

    //  If the duck hasn't escaped and is not hit, it can bounce from the margins 
    if (escapeTime < 5 && isHit == false) {
        BounceDuck();
    }
    else { // If the duck has been hit or it is her escapeTime we wait till it exit the screen and spawn another one
        if (isHit == true) {
            duckAngle = RADIANS(270);

            if (moveDuckY < -50) {
                moveDuckX = rand() % ((int)xMax - 300) + 150;
                moveDuckY = 50;
                duckAngle = RADIANS(rand() % 130 + 30);
                escapeTime = 0;
                nrOfBullets = 3;
                score++;
                isHit = false;
                duckSpeed += 0.2;
            }
        }
        else {
            duckAngle = RADIANS(90);
        }
        
        if (moveDuckY > yMax + 100) {
            moveDuckX = rand() % ((int)xMax - 300) + 150;
            moveDuckY = 50;
            duckAngle = RADIANS(rand() % 130 + 30);
            escapeTime = 0;
            nrOfLives--;
            nrOfBullets = 3;
            duckSpeed += 0.2;
        }
    }
    // If there are no bullets left the duck escapes
    if (nrOfBullets == 0 && isHit == false) {
        escapeTime = 5;
    }

    //Updates the body offsets depending on the angle
    headOffsetX = (175.f) * cos(duckAngle) - (175.f / 4) * sin(duckAngle);
    headOffsetY = (175.f) * sin(duckAngle) + (175.f / 4) * cos(duckAngle);
    noseOffsetX = (175.f) * cos(duckAngle) - (175.f / 6) * sin(duckAngle);
    noseOffsetY = (175.f) * sin(duckAngle) + (175.f / 6) * cos(duckAngle);
    wingOffsetX = (175.f / 4) * cos(duckAngle) - (175.f / 4) * sin(duckAngle);
    wingOffsetY = (175.f / 4) * sin(duckAngle) + (175.f / 4) * cos(duckAngle);
    cX = (175.f / 2) * cos(duckAngle) - (175.f / 4) * sin(duckAngle);
    cY = (175.f / 2) * sin(duckAngle) + (175.f / 4) * cos(duckAngle);
    resolution = window->GetResolution();
    
    if (score < 16 && nrOfLives > 0) {

        //Render grass, bullets, lives and score
        RenderMesh2D(meshes["grass"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(0, 200 - yMax));
        RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(xMax - 300, yMax - 130));

        for (int i = 1; i <= score; i++) {
            RenderMesh2D(meshes["scorePoint"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(xMax - 300 + 17.2 * i, yMax - 130) * transform2D::Rotate(RADIANS(90)));
        }

        for (int i = 1; i <= nrOfLives; i++) {
            RenderMesh2D(meshes["lives"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(xMax - 300 + i * 50, yMax - 30));
        }
        for (int i = 1; i <= nrOfBullets; i++) {
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(xMax - 100 + i * 25, yMax - 45) * transform2D::Rotate(RADIANS(90)));
        }

        // Renders the duck
        RenderBodyPart(meshes["circle"], headOffsetX, headOffsetY, 0, deltaTimeSeconds);
        RenderBodyPart(meshes["wing"], cX, cY, RADIANS(75), deltaTimeSeconds);
        RenderBodyPart(meshes["second_wing"], wingOffsetX, wingOffsetY, RADIANS(280), deltaTimeSeconds);
        RenderBodyPart(meshes["nose"], noseOffsetX, noseOffsetY, RADIANS(330), deltaTimeSeconds);
        RenderBodyPart(meshes["body"], 0, 0, 0, deltaTimeSeconds);
    }
    else { // If the game finished only render the score
        RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(xMax/2.90, yMax/2));

        for (int i = 1; i <= score; i++) {
            RenderMesh2D(meshes["scorePoint"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(xMax/2.90 + 17.2 * i, yMax/2) * transform2D::Rotate(RADIANS(90)));
        }

    }
    
}

/* Bounces the duck if it hits the margins */
void Tema1::BounceDuck() {
    //For max Y
    if (moveDuckY + abs(cY) >= yMax) {
        // if the duck is moving from left to right
        if (cX >= 0 && cY > 0) {
            duckAngle = 2 * M_PI - duckAngle;
        }
        // if the duck is moving from right to left
        if (cX < 0 && cY >= 0) {
            duckAngle = 2 * M_PI - duckAngle;
        }
    }
    // For min Y
    if (moveDuckY + cY <= 40) {
        // if the duck is moving from left to right
        if (cX > 0 && cY <= 0) {
            duckAngle = 2 * M_PI - duckAngle;
        }
        // if the duck is moving from right to left
        if (cX <= 0 && cY < 0) {
            duckAngle = 2 * M_PI - duckAngle;
        }
    }

    //From max X
    if (moveDuckX + abs(cX) >= xMax) {
        // if the duck is moving from left to right
        if (cX >= 0 && cY > 0) {
            duckAngle = M_PI - duckAngle;
        }
        // if the duck is moving from right to left
        if (cX > 0 && cY <= 0) {
            duckAngle = M_PI - duckAngle;
        }
    }
    // From min X
    if (moveDuckX + cX <= 40) {
        // if the duck is moving from left to right
        if (cX < 0 && cY >= 0) {
            duckAngle = M_PI - duckAngle;
        }
        // if the duck is moving from right to left
        if (cX <= 0 && cY < 0) {
            duckAngle = M_PI - duckAngle;
        }
    }
}

/* Renders a body part of the duck */
void Tema1::RenderBodyPart(
    Mesh* bodyPart,
    float offsetX,
    float offsetY,
    float radians,
    float deltaTimeSeconds)
{
    modelMatrix = transform2D::Translate(moveDuckX, moveDuckY);
    modelMatrix *= transform2D::Translate(offsetX, offsetY);
    modelMatrix *= transform2D::Rotate(radians + duckAngle);

    if (!strcmp(bodyPart->GetMeshID(), "wing"))
    {
        modelMatrix *= transform2D::Rotate(angularStep);
    }
    if (!strcmp(bodyPart->GetMeshID(), "second_wing"))
    {
        modelMatrix *= transform2D::Rotate(-angularStep);
    }
    RenderMesh2D(bodyPart, shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Rotates the mouse coordinates to the unrotated hitbox and verifies if the point is inside 
    float mY = yMax - (float)mouseY * 720 / resolution.y;
    float rotatedMouseX = (float)mouseX * 1280 / resolution.x - moveDuckX;
    float rotatedMouseY = mY - moveDuckY;
    float temp1 = rotatedMouseX * cos(-duckAngle) - rotatedMouseY * sin(-duckAngle);
    float temp2 = rotatedMouseX * sin(-duckAngle) + rotatedMouseY * cos(-duckAngle);
    rotatedMouseX = temp1 + moveDuckX;
    rotatedMouseY = temp2 + moveDuckY;

    nrOfBullets--;
    if (nrOfBullets >= 0 && rotatedMouseX >= moveDuckX && rotatedMouseX <= (hitboxSize + moveDuckX) && rotatedMouseY > moveDuckY && rotatedMouseY < (hitboxSize/2 + moveDuckY)) {
        isHit = true;
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
