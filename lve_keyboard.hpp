#pragma once

#include "lve_game_object.hpp"
#include "lve_window.hpp"

namespace lve {
	class KeyboardMovementController {
		
		public:
			struct keyMapping {
				int moveForward = GLFW_KEY_W;
				int moveLeft = GLFW_KEY_A;
				int moveBackward = GLFW_KEY_S;
				int moveRight = GLFW_KEY_D;

				int moveUp = GLFW_KEY_E;
				int moveDown = GLFW_KEY_Q;

				int rotateLeft = GLFW_KEY_R;
				int rotateRight = GLFW_KEY_T;

				int lookUp = GLFW_KEY_UP;
				int lookLeft = GLFW_KEY_LEFT;
				int lookDown = GLFW_KEY_DOWN;
				int lookRight = GLFW_KEY_RIGHT;

			};

			void moveInPlaneXZ(GLFWwindow* window,
							   float dt,
							   LveGameObject& gameObject);

			keyMapping keys{};
			float moveSpeed{ 15.f };
			float lookSpeed{ 1.5f };
	};
}