#include "first_app.hpp"

#include "lve_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
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
        //camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));
        camera.setViewTarget(glm::vec3(-1.f, -2., 2.f),
                             glm::vec3(0.f, 0.f, 25.f));

        while (!lveWindow.shouldClose()) {
            glfwPollEvents();

            float aspect = lveRenderer.getAspectRatio();
   // Orthographic projection:
   // camera.setOrthographicProjection(-1, 1, -1 / aspect, 1 / aspect, -1, 1);
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
    std::unique_ptr<LveModel> createCubeModel(
        LveDevice& device,
        glm::vec3 offset) {
        std::vector<LveModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<LveModel>(device, vertices);
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice,
                                                            { .0f, .0f, .0f });
        
        const int numCubes = 1;

        for (int i = 0; i <= numCubes; ++i) {
            float random = 0.1f;
            //playing around 
            //float random = static_cast <float> (rand()) / 
            //  (static_cast <float> (RAND_MAX / 1.2));

            auto cube = LveGameObject::createGameObject();
            cube.model = lveModel;
            cube.transform.translation = { .0f, .0f, 25.f };
            cube.transform.scale = { random, random, random };

            gameObjects.push_back(std::move(cube));
        }

       /* Single Cubes
        auto cube1 = LveGameObject::createGameObject();
        cube1.model = lveModel;
        cube1.transform.translation = { .0f, .3f, .5f };
        cube1.transform.scale = { .2f, .7f, .5f };

       gameObjects.push_back(std::move(cube1));
       */
    }

}