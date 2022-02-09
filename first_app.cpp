#include "first_app.hpp"

#include "lve_keyboard.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>

namespace lve {

    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{
            lveDevice,
            lveRenderer.getSwapChainRenderPass() };
        LveCamera camera{};
        camera.setViewTarget(glm::vec3(-1.f, -2., 2.f),
                             glm::vec3(0.f, 0.f, 25.f));

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!lveWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();

            float frameTime =
                std::chrono::duration<float,
                std::chrono::seconds::period>(newTime - currentTime).count();

            currentTime = newTime;

            cameraController.moveInPlaneXZ(
                lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation,
                viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(
                glm::radians(50.f),
                aspect, 0.1f, 100.f);

            if (auto commandBuffer = lveRenderer.beginFrame()) {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(
                    commandBuffer,
                    gameObjects,
                    camera);
                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    // temporary helper function, creates a 1x1x1 cube centered at offset

    void FirstApp::loadGameObjects() {
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "colored_cube.obj");
        const int numCubes = 1000;
        float scale = 1;
        float position = 0;

        for (int i = 0; i <= numCubes; ++i) {

            auto cube = LveGameObject::createGameObject();
            cube.model = lveModel;
            cube.transform.translation = { position, -position, -position };
            cube.transform.scale = { scale, scale, scale };
            scale = scale + 0.1;
            position = position + 5.5;

            gameObjects.push_back(std::move(cube));
        }
    }
}