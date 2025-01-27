#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "renderer/Renderer.hpp"
#include "framework/EntityManager.hpp"
#include "framework/Entity.hpp"

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

		// BUG: Setting Y to certain values caues odd issues
		static float rot[3]{};
		glm::vec v = e->m_transform.GetRotationEuler();
		rot[0] = v.x;
		rot[1] = v.y;
		rot[2] = v.z;
		ImGui::InputFloat3("rotation", rot);
		e->m_transform.SetRotation(glm::vec3(rot[0], rot[1], rot[2]));

		// Drop down menu for model and texture selection
		// TODO: Get list of Mesh names from MeshManager. MeshManager should
		// probably have that as a container or if it is not possible to get one
		// from the map. Don't want to generate it every frame
		// Send selected mesh to entity as set mesh
		static int modelIndex = 0;
		const char* items[] = { "Default", "Monkey", "Flag pole"};
		ImGui::Combo("model", &modelIndex, items, IM_ARRAYSIZE(items));

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
