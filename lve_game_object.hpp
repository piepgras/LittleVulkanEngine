#pragma once

#include "lve_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace lve {

    struct TransformComponent {
        glm::vec3 translation{};  // (position offset)
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        //Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
        //Uses Tait-Bryan angles with axis order Y(1), X(2), Z(3).. for now.
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    /* OLD 
            glm::mat4 mat4() {
            auto transform = glm::translate(glm::mat4{ 1.f }, translation);

            transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f });
            transform = glm::rotate(transform, rotation.y, { 1.f, 0.f, 0.f });
            transform = glm::rotate(transform, rotation.y, { 0.f, 0.f, 1.f });

            transform = glm::scale(transform, scale);

            return transform;
        }
    };
    */

    struct RigidBody2d {
        glm::vec2 velocity;
        float mass{ 1.0f };
    };

    class LveGameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, LveGameObject>;

        LveGameObject() {

        }

        static LveGameObject createGameObject() {
            static id_t currentId = 0;
            return LveGameObject{ currentId++ };
        }

        LveGameObject(const LveGameObject&) = delete;
        LveGameObject& operator=(const LveGameObject&) = delete;
        LveGameObject(LveGameObject&&) = default;
        LveGameObject& operator=(LveGameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<LveModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};
        RigidBody2d rigidBody2d{};

    private:
        LveGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}