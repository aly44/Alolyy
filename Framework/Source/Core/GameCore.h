#pragma once

namespace fw {

class FWCore;

    class GameCore
    {
    public:
        GameCore(FWCore& framework);
        virtual ~GameCore();

        virtual void update(float deltaTime) = 0;
        virtual void draw() = 0;

        virtual void onKeyPressed(int key) = 0;
        virtual void onKeyReleased(int key) = 0;
        virtual void onMouseMove(float x, float y) = 0;
        virtual void onMouseButtonDown(int mouseId, float x, float y) = 0;

        FWCore& getFramework() { return m_framework; }

    protected:
        FWCore& m_framework;
    };

} // namespace fw
