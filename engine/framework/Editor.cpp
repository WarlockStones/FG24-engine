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
#include "framework/Lighting.hpp"
#include "framework/Light.hpp"
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
		// Go through index in names. But when assigning use the GLuint ID
		const auto& texNames = Texture::GetNames();
		previewName = Texture::GetName(e->m_textureId).data();
		if (ImGui::BeginCombo("albedo texture", previewName)) {
			// 0 is OpenGL for None or invalid texture
			for (std::size_t i = 0; i < texNames.size(); ++i) { 
				const bool isSelected = (previewName == texNames[i]);
				// Create a new selectable widget
				if (ImGui::Selectable(texNames[i].data(), isSelected)) {
					e->m_textureId = Texture::GetId(texNames[i]);
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("remove albedo")) {
		    e->m_textureId = 0;
		}

		previewName = Texture::GetName(e->m_textureSpecularId).data();
		if (ImGui::BeginCombo("specular texture", previewName)) {
			for (std::size_t i = 0; i < texNames.size(); ++i) {
				const bool isSelected = (previewName == texNames[i]);
				if (ImGui::Selectable(texNames[i].data(), isSelected)) {
					e->m_textureSpecularId = Texture::GetId(texNames[i]);
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("remove specular")) {
		    e->m_textureSpecularId = 0;
		}

		ImGui::Checkbox("Draw as wireframe", &e->m_drawAsWireframe);
	}
}

void LightEditor() {
	const auto& lights = Lighting::GetLights();
	static std::size_t lightSelectedIndex = 0;

	if (ImGui::Button("Create light")) {
		Lighting::CreateLight(
			glm::vec3(0,0,0),
			LightType::Point,
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec3(2.0f, 0.01f, 0.001f));
		lightSelectedIndex = lights.size() - 1;
	}
	
	if (ImGui::BeginListBox("Lights")) {
		for (std::size_t i = 0; i < lights.size(); ++i) {
			const bool isSelected = (lightSelectedIndex == i);
			static char buf[32];
			std::sprintf(buf, "Light %ld", i);
			if (ImGui::Selectable(buf, isSelected)) {
				lightSelectedIndex = i;
			}
			  
		}
		ImGui::EndListBox();
	}

	if(!lights.empty()) {
		auto* l = lights[lightSelectedIndex];

		if (ImGui::Button("Delete selected light")) {
			Lighting::DeleteLight(l);
		} else {
			int& type = l->m_type;
			const char* typeNames[3] = { "Point", "Spot", "Directional" };
			const char* typeName = (type >= 0 && type < 3) ? typeNames[type] : "Unknown";
			ImGui::SliderInt("type", &type, 0, 2, typeName);
			ImGui::Checkbox("enabled", &l->m_enabled);

			static float pos[3]{};
			pos[0] = l->m_position[0];
			pos[1] = l->m_position[1];
			pos[2] = l->m_position[2];
			ImGui::InputFloat3("position", pos);
			l->m_position = glm::vec3(pos[0], pos[1], pos[2]);

			static float diff[4]{};
			diff[0] = l->m_diffuse[0];
			diff[1] = l->m_diffuse[1];
			diff[2] = l->m_diffuse[2];
			diff[3] = l->m_diffuse[3];
			ImGui::ColorEdit4("diffuse", (float*)&diff, ImGuiColorEditFlags_Float);
			l->m_diffuse = glm::vec4(diff[0], diff[1], diff[2], diff[3]);

			static float spec[4]{};
			spec[0] = l->m_specular[0];
			spec[1] = l->m_specular[1];
			spec[2] = l->m_specular[2];
			spec[3] = l->m_specular[3];
			ImGui::ColorEdit4("specular", (float*)&spec, ImGuiColorEditFlags_Float);
			l->m_specular = glm::vec4(spec[0], spec[1], spec[2], spec[3]);

			ImGui::Text("attenuation");
			ImGui::SliderFloat("constant",  &l->m_attenuation.x, 0.1, 4);
			ImGui::SliderFloat("linear",    &l->m_attenuation.y, 0.01, 0.1);
			ImGui::SliderFloat("quadratic", &l->m_attenuation.z, 0.001, 0.01);

			if (type == LightType::Spot) {
				static float rot[3]{};
				rot[0] = l->m_rotation[0];
				rot[1] = l->m_rotation[1];
				rot[2] = l->m_rotation[2];
				ImGui::InputFloat3("rotation", rot);
				l->m_rotation = glm::vec3(rot[0], rot[1], rot[2]);
			}
		}
	}
}

static void CameraEditor() {
	// ListBox of Active cameras ids.
	static std::size_t cameraSelectedIndex = 0; // Auto select top of list
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
	ImGui::Text("Selected camera: %ld", cameraSelectedIndex);
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
	float ambient[4]{};
	ambient[0] = Lighting::ambient.r;
	ambient[1] = Lighting::ambient.g;
	ambient[2] = Lighting::ambient.b;
	ambient[3] = Lighting::ambient.a;
	ImGui::ColorEdit4("Ambient light", (float*)&ambient, ImGuiColorEditFlags_Float);
	Lighting::ambient = glm::vec4(ambient[0], ambient[1], ambient[2], ambient[3]);

	ImGui::Checkbox("Draw lights as wireframe", &g_drawLightsAsWireframe);
	if (ImGui::Button("Switch texture MipMap settings")) {
		static bool flipFlop = false;
		Texture::UpdateMipMapSettings(flipFlop);
		flipFlop = !flipFlop;
	}

	using code = LevelSaver::ErrorCode;
    static auto ec = code::Ok;
	static bool saveWasPressed = false;
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
		if (ecLoad == code::Ok) {
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
		if (ImGui::BeginTabItem("Lights")) {
			LightEditor();
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
