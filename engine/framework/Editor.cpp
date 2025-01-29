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
	ImGui::Begin("Entity Editor"); // Create a window                          
	ImGui::Text("Press F1 to detach mouse cursor."); 

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

		// Drop down menu for model and texture selection
		// TODO: Fix so that it works with OpenGL that id number is correct!
		// texture id 0 is invalid id meaning texture could not be created in opengl
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
	}

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
