#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "renderer/Renderer.hpp"
#include "framework/EntityManager.hpp"
#include "framework/MeshManager.hpp"
#include "framework/Entity.hpp"
#include "framework/CameraManager.hpp"
#include "framework/Camera.hpp"
#include "framework/LevelSaver.hpp"
#include "Globals.hpp"
#include <cstdio>

namespace FG24 {
namespace Editor {
void Init(const Renderer& renderer) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(renderer.m_window, renderer.m_context);
	ImGui_ImplOpenGL3_Init("#version 330");
}

static void EntityEditor(EntityManager& entityManager) {
	static int entityIndex = 0;
	ImGui::Text("Selected entity index = %d", entityIndex);
	ImGui::ListBox(
		"Entities",
		&entityIndex,
		[](void* data, int i, const char** output) -> bool {
			Entity** entities = static_cast<Entity**>(data);
			Entity* currentEntity = entities[i];
			*output = currentEntity->GetName().c_str();
			return true;
		},
		entityManager.GetEntities().data(),
		entityManager.GetEntities().size(),
		3); // Minimum entries to show

	if (entityManager.GetEntities().size() > 0) {
		Entity* e = entityManager.GetEntities()[entityIndex];

		// Populate new buffer containing name
		char buf[32] = "";
		buf[0] = '\0';
		std::string name = e->GetName();
		for (std::size_t i = 0; i < name.length() && i < 32; ++i) {
			if (name[i] == '\0') {
				break;
			}
			buf[i] = name[i];
		}
		ImGui::InputText("name", buf, IM_ARRAYSIZE(buf));
		e->SetName(buf);
		
		static float pos[3]{};
		pos[0] = e->m_transform.GetLocation().x;
		pos[1] = e->m_transform.GetLocation().y;
		pos[2] = e->m_transform.GetLocation().z;
		ImGui::InputFloat3("position", pos);
		e->m_transform.SetLocation(glm::vec3(pos[0], pos[1], pos[2]));

		// BUG: Setting Y to certain values causes odd issues
		static float rot[3]{};
		glm::vec v = e->m_transform.GetRotationEuler();
		rot[0] = v.x;
		rot[1] = v.y;
		rot[2] = v.z;
		ImGui::InputFloat3("rotation", rot);
		e->m_transform.SetRotation(glm::vec3(rot[0], rot[1], rot[2]));

		// Mesh selection
		static const char* previewName = "EMPTY";
		static const std::vector<std::string_view>& meshNames = MeshManager::GetNames();
		if (!meshNames.empty()) {
			previewName = e->GetMesh().m_name.data();
			if (ImGui::BeginCombo("mesh", previewName)) {
			  for (std::size_t i = 0; i < meshNames.size(); ++i) {
					const bool isSelected = (previewName == meshNames[i]);
					// Create new selectable as part of the combo. With label and bIsSelected
					if (ImGui::Selectable(meshNames[i].data(), isSelected)) {
						previewName = meshNames[i].data();
						e->SetMesh(MeshManager::GetMesh(previewName)); // Mesh name is used as ID
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		} else {
			ImGui::Combo("model", 0, "EMPTY", 1);
		}

		// Texture selection
		std::uint32_t textureIndex = e->m_textureId;
		previewName = Texture::GetName(textureIndex).data();
		if (ImGui::BeginCombo("textures", previewName)) {
			// 0 is OpenGL for None or invalid texture
			for (std::size_t i = 0; i < Texture::GetNames().size() + 1; ++i) { 
				const bool isSelected = (textureIndex == i);
				// Create a new selectable widget
				if (ImGui::Selectable(Texture::GetName(i).data(), isSelected)) {
					textureIndex = i;
					e->m_textureId = textureIndex;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("Draw as wireframe", &e->m_drawAsWireframe);
	}
}

static void CameraEditor() {
	// ListBox of Active cameras ids.
	static int cameraSelectedIndex = 0; // Auto select top of list
	if (ImGui::BeginListBox("Cameras")) {
		for (std::size_t i = 0; i < CameraManager::GetNumCameras(); ++i) {
			const bool isSelected = (cameraSelectedIndex == i);
			std::uint32_t* activeIds = CameraManager::GetIdArray();
			std::uint32_t camId = activeIds[i];
			static char buf[32];
			std::sprintf(buf, "Camera %d", camId);
			if (ImGui::Selectable(buf, isSelected)) {
				cameraSelectedIndex = i;
			}
		}
		ImGui::EndListBox();
	}
	ImGui::Text("Selected camera: %d", cameraSelectedIndex);
	if (CameraManager::GetNumCameras() > 0) {
		auto camId = CameraManager::GetIdArray()[cameraSelectedIndex];
		Camera* cam = CameraManager::GetCamera(camId);
		if (cam) {
			if (ImGui::Button("Control camera")) {
				CameraManager::SetActiveCamera(camId);
			}
			// Display camera things
			static float pos[3]{};
			pos[0] = cam->GetPosition().x;
			pos[1] = cam->GetPosition().y;
			pos[2] = cam->GetPosition().z;
			ImGui::InputFloat3("position", pos);
			cam->SetPosition(pos[0], pos[1], pos[2]);

			static float yaw;
			yaw = cam->GetYaw();
			ImGui::InputFloat("yaw", &yaw);
			cam->SetYaw(yaw);

			static float pitch;
			pitch = cam->GetPitch();
			ImGui::InputFloat("pitch", &pitch);
			cam->SetPitch(pitch);

			// FOV
			ImGui::SliderFloat("fov", &cam->m_fov, 5, 120);

			// Projection type
			if (cam->projection == ProjectionType::Perspective) {
				if (ImGui::Button("Switch to orthographic view")) {
					cam->projection = ProjectionType::Orthographic;
				}
			} else {
				if (ImGui::Button("Switch to perspective view")) {
					cam->projection = ProjectionType::Perspective;
				}
			}
		}
	}
}

void SceneEditor(EntityManager& entityManager) {
	using code = LevelSaver::ErrorCode;
    static auto ec = code::Ok;
	static bool saveWasPressed = false;
	ImGui::Checkbox("Draw lights as wireframe", &g_drawLightsAsWireframe);
	if (ImGui::Button("Save entities")) {
		ec = LevelSaver::SaveEntities(entityManager);
		saveWasPressed = true;
	}
	if (saveWasPressed) {
	    if (ec == code::Ok) {
			ImGui::Text("Level save created successfully");
		} else if (ec == code::NoFile) {
			ImGui::Text("Error: Level save failed! File path was invalid");
		} else if (ec == code::Fail) {
		    ImGui::Text("Error: Level was not able to be saved");
		}
	}
	static auto ecLoad = code::Ok;
	static bool loadWasPressed = false;
	if (ImGui::Button("Load entities")) {
		ecLoad = LevelSaver::LoadEntities(entityManager);
	}
	if (loadWasPressed) {
		if (ec == code::Ok) {
			ImGui::Text("Entities loaded successfully");
		} else {
			ImGui::Text("Failed to load entities");
		}
	}
}

void Draw(EntityManager& entityManager) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

#if false
	ImGui::ShowDemoWindow();
#else
	ImGui::Begin("Editor"); // Create a window                          
	ImGui::Text("Press F1 to detach mouse cursor."); 

	if (ImGui::BeginTabBar("Editor")) {
		if (ImGui::BeginTabItem("Entities")) {
			EntityEditor(entityManager);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Cameras")) {
			CameraEditor();
			ImGui::EndTabItem();
		} 
		if (ImGui::BeginTabItem("Scene")) {
			SceneEditor(entityManager);
			ImGui::EndTabItem();
		}

	ImGui::EndTabBar();
	} // BeginTabBar


	ImGui::End(); // End of Entity Editor
#endif

	ImGui::Render();
}

void Destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

} // namespace Editor
} // namespace FG24
