#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "renderer/Renderer.hpp"
#include "framework/EntityManager.hpp"
#include "framework/MeshManager.hpp"
#include "framework/Entity.hpp"
#include "Globals.hpp"

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

void Draw(EntityManager& entityManager) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

#if FALSE
	ImGui::ShowDemoWindow();
#else
	static float f = 0.0f;
	static int counter = 0;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("Entity Editor"); // Create a window                          
	ImGui::Text("This is some useful text."); 

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
		3); // Entries to show minimum

	// Ability to manipulate name, model, texture, position, rotation
	// TextEdit. InputText. 
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

		// Drop down menu for model and texture selection
		static int modelIndex = 0;
		static const std::vector<std::string_view>& meshNames = MeshManager::GetNames();
		if (!meshNames.empty()) {
			ImGui::Combo(
				"model",
				&modelIndex,
				MeshManager::GetNames().data()->data(),
				MeshManager::GetNames().size());
			e->SetMesh(MeshManager::GetMesh(meshNames[modelIndex]));
		} else {
			ImGui::Combo("model", &modelIndex, "EMPTY", 1);
		}

		// Drop down for texture
		static int textureIndex = 0;
		static const char* previewName = "EMPTY";
		if (!g_textures.empty()) {
			textureIndex = e->m_textureId;
			previewName = g_textures[textureIndex].m_name;
			if (ImGui::BeginCombo("textures", previewName)) {
				for (int i = 0; i < g_textures.size(); ++i) {
					const bool isSelected = (textureIndex == i);
					if (ImGui::Selectable(g_textures[i].m_name, isSelected)) {
						textureIndex = i;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			e->m_textureId = textureIndex;
		} else {
			previewName = "EMPTY";
		}

	}
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);

	if (ImGui::Button("Button")) { // Widget returns true when interacted with
		counter++;
	}

	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
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
