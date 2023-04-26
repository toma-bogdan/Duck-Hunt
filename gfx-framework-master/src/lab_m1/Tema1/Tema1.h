#pragma once

#include "components/simple_scene.h"

namespace m1 {
    class Tema1 : public gfxc::SimpleScene {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderBodyPart(Mesh* bodyPart, GLfloat offsetX,GLfloat offsetY, GLfloat radians,GLfloat deltaTimeSeconds);
        void BounceDuck();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat3 modelMatrix;
        glm::ivec2 resolution;
        int nrOfLives = 3, nrOfBullets = 3, score = 0;
        float moveDuckX, moveDuckY;
        float angularStep; // rotation angle for wings
        float duckAngle;
        float duckSpeed;
        float cX, cY; // center of the duck
        float headOffsetX, headOffsetY;
        float noseOffsetX, noseOffsetY;
        float wingOffsetX, wingOffsetY;
        float xMax, yMax; // screen resolution
        float hitboxSize = 220; // size for the duck's hitbox
        float escapeTime = 0; // time measuring for how long a duck stays on the screen
        bool isHit = false; // shows if a duck is hit
        bool swingForward; // used to determine whether the wings rotate fw or backwords
    };
}
