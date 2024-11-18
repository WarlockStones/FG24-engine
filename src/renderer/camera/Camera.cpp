#include "./Camera.hpp"
#include "../../Globals.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace FG24 {
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov,
               float movementSpeed, float mouseSensitivity) {
  this->position = position;
  this->front = front;
  this->up = up;
  this->fov = fov;
  this->movementSpeed = movementSpeed;
  this->mouseSensitivity = mouseSensitivity;

  UpdateCameraVectors(); // Set initial camera values
}

static bool printed = false; // Used in Camera::UpdateCameraVectors
void Camera::UpdateCameraVectors() {
  // std::cout<<".......................\n";
  // std::cout<<"Camera Yaw: "<<yaw<<'\n'<<"Camera Pitch: "<<pitch<<'\n';
  // Super smart math trigonometry to calculate pitch and yaw based on mouse
  glm::vec3 direction; // Temporary value to calculate the camera front
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);

  // ---- Debug Printing ----
  // Print Camera Location and Direction once every "printTimer"-second
  // int printTimer = static_cast<int>(SDL_GetTicks() * 0.001f); // Seconds
  // passed int printCD = 2;

  // if (printTimer % printCD == 0) {
  //   if (!printed) {
  //     std::cout << "Camera pos = " << position.x << '|' << position.y << '|'
  //               << position.z << '\n';
  //     std::cout << "Camera Front = " << front.x << '|' << front.y << '|'
  //               << front.z << '\n';
  //     printed = true;
  //   }
  // } else {
  //   printed = false;
  // }
}

// Process camera inputs. Move and Look
void Camera::ProcessInput() {
  SDL_Event e{};
  while (SDL_PollEvent(&e)) {
    std::cout << "Found input of type: "<<e.type<< '\n';
    switch (e.type) {
    case SDL_QUIT:
      std::cout << "SDL_QUIT input recieved. Exiting game loop...\n";
      g_isRunning = false;
      break;
    case SDL_KEYDOWN: // ----- KEYBOARD INPUTS
      switch (e.key.keysym.sym) {
      case SDLK_ESCAPE:
        std::cout << "SDL_QUIT input recieved. Exiting game loop...\n";
        g_isRunning = false;
        break;
      case SDLK_UP:
        // textureMix += 0.05;
        continue;
      case SDLK_DOWN:
        // textureMix -= 0.05;
        continue;
      // ---- Camera movement ----
      case SDLK_w:
        yvel = 1;
        continue;
      case SDLK_d:
        xvel = 1;
        continue;
      case SDLK_s:
        yvel = -1;
        continue;
      case SDLK_a:
        xvel = -1;
        continue;
      // Swap sync mode
      case SDLK_1:
        SDL_GL_SetSwapInterval(0); // Immediate updates. No vsync
        continue;
      case SDLK_2:
        SDL_GL_SetSwapInterval(1); // vsync
        continue;
      case SDLK_3:
        SDL_GL_SetSwapInterval(-1); // Adaptive vsync
        continue;
      default:
	continue;
      }
    // Check for key up to zero the x and y velocities. But we must also be
    // careful not to zero the velocities when we shouldn't
    case SDL_KEYUP:
      switch (e.key.keysym.sym) {
      case SDLK_w:
        // Check and make sure we are moving to the forward. If we are then zero
        // the velocity. If down is still pressed we don't touch the velocity
        if (yvel > 0) // down key puts yvel at -1.
          yvel = 0;
        continue;
      case SDLK_d:
        if (xvel > 0)
          xvel = 0;
        continue;
      case SDLK_s:
        if (yvel < 0)
          yvel = 0;
        continue;
      case SDLK_a:
        if (xvel < 0)
          xvel = 0;
        continue;
      default: // <-- OBS!! ALWAYS end with a default: They can be empty.
	continue; 
	/* ALWAYS have a default at the bottom of your switch-case!  */
        /* Else it will just fall over down to SDL_MOUSEWHEEL...     */
        /* Even though SDL_KEYUP is not the same as SDL_MOUSEHEEL... */
      }
    case SDL_MOUSEWHEEL:
      fov -= e.wheel.preciseY; // Mouse wheel to zoom camera
      if (fov < 1.0f)
        fov = 1.0f;
      if (fov > 45.0f)
        fov = 45.0f;
      continue;
    case SDL_MOUSEMOTION:
      mouseMotion = true;
      // FirstMouseCheck. might be necessary if going from cursor to locked cam
      yaw += (e.motion.xrel * mouseSensitivity);
      pitch += (e.motion.yrel * mouseSensitivity * -1); // -1 to reverse up

      // Lock pitch. For a more common FPS camera
      if (pitch > 89.0f)
        pitch = 89.0f;
      if (pitch < -89.0f)
        pitch = -89.0f;

      // If necessary find a smart solution to keep it normalized between 0-360
      // I don't think it will matter but in case issues arise this can be done.
      // if (yaw > 360.0f)
      // 	yaw = 0;
      // if (yaw < -360.0f)
      // 	yaw = 0;
      continue;
    default:
      continue;
    } // -- e.type switch end
  }   // -- SDL_PollEvent loop end

  // Only do math caluclations once a frame if we have read new mouse inputs
  if (mouseMotion) {
    mouseMotion = false;
    UpdateCameraVectors();
  }

  // Camera movement WASD, etc.
  // Move y. Forward and Backward
  position += (movementSpeed * front) * yvel * static_cast<float>(g_deltaTime);
  // Move x. Right and Left. Using cross product to get "Right Vector"
  position += (glm::normalize(glm::cross(front, up)) * movementSpeed) * xvel *
              static_cast<float>(g_deltaTime);
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

// PrintFPS. Mostly for the end user and light debugging
static int frames = 0;
static double startTime = 0;
static bool first = true;
static int fps = 0;
void Camera::PrintFPS() {
  int timePassed = SDL_GetTicks() / 1000; // In seconds
  if (first) {
    frames = 0;
    startTime = timePassed;
    first = false;
  }
  frames++;
  // Update FPS count every 0.25 seconds for readability
  if (timePassed - startTime > 0.25 && frames > 10) {
    // fps = static_cast<double>(frames / (timePassed - startTime));
    fps = frames / (timePassed - startTime);
    startTime = timePassed;
    frames = 0;
  }
  std::cout << "FPS: " << fps << '\n';
  // std::cout << "deltaTime: "<<deltaTime<<'\n';
}

}
