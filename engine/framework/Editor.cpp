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

	// ImGui::ShowDemoWindow();

	static float f = 0.0f;
	static int counter = 0;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("Entity Editor"); // Create a window                          
	ImGui::Text("This is some useful text."); 

	static int item_current = 0;
	ImGui::ListBox(
		"Entities",
		&item_current,
		[](void* data, int i, const char** output) -> bool {
			Entity** entities = static_cast<Entity**>(data);
			Entity* currentEntity = entities[i];
			*output = currentEntity->GetName().c_str();
			return true;
		},
		entityManager.GetEntities().data(),
		entityManager.GetEntities().size(),
		3); // Entries to show minimum

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);

	if (ImGui::Button("Button")) { // Widget returns true when interacted with
		counter++;
	}

	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	ImGui::End(); // End of Entity Editor



	ImGui::Render();
}

void Destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

} // namespace Editor
} // namespace FG24
