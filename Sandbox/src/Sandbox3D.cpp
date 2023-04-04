#include "Sandbox3D.h"


template<typename Fn>
class Timer {

public:
	Timer(const char* name, Fn&& func) : m_Name(name), m_Func(func), m_Stopped(false) {

		m_StartTimepoint = std::chrono::high_resolution_clock::now();

	}

	~Timer() {

		if (!m_Stopped)
			Stop();
	}

	void Stop() {
	
		auto endTimepoint = std::chrono::high_resolution_clock::now();
	
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();

		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) / 1000.0f;
		m_Func({ m_Name, duration });
	}
	
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

float Sandbox3D::deltaTime = 0.016f;


Sandbox3D::Sandbox3D() 
	: Layer("Sandbox3D"), m_CameraController() {

}

void Sandbox3D::OnAttach() {

	//Construct And Buffer Texture To Memory
	m_Texture = Vi::Texture2D::Create("assets/textures/Cat03.jpg", 0);
	m_TransparentTexture = Vi::Texture2D::Create("assets/textures/RGBA.png", 0);

}

void Sandbox3D::OnDetach() {

}

void Sandbox3D::OnUpdate(Vi::Timestep ts) {

	PROFILE_SCOPE("Sandbox3D::OnUpdate");

	// Update 
	Sandbox3D::deltaTime = ts;										//temp
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	// Render 

	// Set Background Color And Clear It 
	{
		PROFILE_SCOPE("Renderer Prep");
		Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vi::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Vi::Renderer3D::BeginScene(m_CameraController.GetCamera());
		Vi::Renderer3D::DrawCube(m_Cube1Position, { 0.0f,0.0f,0.0f }, { 0.5f,0.5f,0.5f }, m_CubeColor );
		Vi::Renderer3D::DrawCube(m_Cube3Position, { 0.0f,0.0f,0.0f }, { 0.5f,0.5f,0.5f }, m_Texture, { 0.0f,0.0f,1.0f,1.0f });
		Vi::Renderer3D::DrawPlane(m_Cube2Position, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_GroundColor);
		Vi::Renderer3D::DrawPlane(m_Cube4Position, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_TransparentTexture, { 0.0f,1.0f,0.0f,1.0f });
	}
	
	


	Vi::Renderer3D::EndScene();


	fullScreen = Vi::Application::Get().GetWindow().IsFullScreen();		// temp 
}

void Sandbox3D::OnImGuiRender() {

	auto& window = Vi::Application::Get().GetWindow();
	ImGui::Begin("Settings");														 // temp
	ImGui::ColorEdit4("ColorShader", (float*)&m_CubeColor);
	ImGui::ColorEdit4("GroundShader", (float*)&m_GroundColor);
	ImGui::End();																   // temp

	float tempPosition[3];

	{
		// m_Cube1Position
		tempPosition[0] = m_Cube1Position.x;
		tempPosition[1] = m_Cube1Position.y;
		tempPosition[2] = m_Cube1Position.z;

		ImGui::DragFloat3("Cube1Position", tempPosition, 0.01f);
		m_Cube1Position.x = tempPosition[0];
		m_Cube1Position.y = tempPosition[1];
		m_Cube1Position.z = tempPosition[2];

		// m_Cube2Position
		tempPosition[0] = m_Cube2Position.x;
		tempPosition[1] = m_Cube2Position.y;
		tempPosition[2] = m_Cube2Position.z;

		ImGui::DragFloat3("Cube2Position", tempPosition, 0.01f);
		m_Cube2Position.x = tempPosition[0];
		m_Cube2Position.y = tempPosition[1];
		m_Cube2Position.z = tempPosition[2];

		// m_Cube3Position
		tempPosition[0] = m_Cube3Position.x;
		tempPosition[1] = m_Cube3Position.y;
		tempPosition[2] = m_Cube3Position.z;

		ImGui::DragFloat3("Cube3Position", tempPosition, 0.01f);
		m_Cube3Position.x = tempPosition[0];
		m_Cube3Position.y = tempPosition[1];
		m_Cube3Position.z = tempPosition[2];

		// m_Cube4Position
		tempPosition[0] = m_Cube4Position.x;
		tempPosition[1] = m_Cube4Position.y;
		tempPosition[2] = m_Cube4Position.z;

		ImGui::DragFloat3("Cube4Position", tempPosition, 0.01f);
		m_Cube4Position.x = tempPosition[0];
		m_Cube4Position.y = tempPosition[1];
		m_Cube4Position.z = tempPosition[2];
	}

	ImGui::Begin("Stats");														   // temp 
	ImGui::Text("Fps  %.0fFps", 1 / Sandbox3D::deltaTime);
	ImGui::Text("ts  %.3fms", Sandbox3D::deltaTime);
	ImGui::End();

	ImGui::Begin("Profiler");

	for (auto& result : m_ProfileResults) {

		char label[50];
		strcpy(label, "%.3fms  ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	ImGui::End();

	ImGui::Begin("GraphicsSetting");
	ImGui::Checkbox("Vsync", &vSync);
	window.SetVSync(vSync);
	bool fullscreen = window.IsFullScreen();
	ImGui::Checkbox("FullScreen", &fullScreen);
	if (fullScreen != fullscreen) {
		if (fullScreen) {
			window.SetFullScreen(true);
		}
		else {
			window.SetFullScreen(false);
		}
	}
	const char* items[] = {
		"800x600","1024x768","1128x634","1152x864","1280x600",
		"1280x720","1280x768","1280x960","1280x1024","1360x768",
		"1366x768","1440x900","1600x900","1600x1200","1680x1050",
		"1760x990","1920x1080","1920x1200", "2560x1440" };
	static const char* current_item = NULL;
	int temporary = -1;
	if (ImGui::BeginCombo("##Resolution", current_item))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				temporary = n;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();

			}
		}
		ImGui::EndCombo();
		switch (temporary)
		{
		case 0:
			window.SetResolution(800, 600);
			break;
		case 1:
			window.SetResolution(1024, 768);
			break;
		case 2:
			window.SetResolution(1128, 634);
			break;
		case 3:
			window.SetResolution(1152, 900);
			break;
		case 4:
			window.SetResolution(1280, 600);
			break;
		case 5:
			window.SetResolution(1280, 720);
			break;
		case 6:
			window.SetResolution(1280, 786);
			break;
		case 7:
			window.SetResolution(1280, 960);
			break;
		case 8:
			window.SetResolution(1280, 1024);
			break;
		case 9:
			window.SetResolution(1360, 768);
			break;
		case 10:
			window.SetResolution(1366, 768);
			break;
		case 11:
			window.SetResolution(1440, 900);
			break;
		case 12:
			window.SetResolution(1600, 900);
			break;
		case 13:
			window.SetResolution(1600, 1200);
			break;
		case 14:
			window.SetResolution(1680, 1050);
			break;
		case 15:
			window.SetResolution(1760, 990);
			break;
		case 16:
			window.SetResolution(1920, 1080);
			break;
		case 17:
			window.SetResolution(1920, 1200);
			break;
		case 18:
			window.SetResolution(2560, 1440);
			break;
		default:
			break;
		}
	}


	ImGui::End();
}

void Sandbox3D::OnEvent(Vi::Event& e) {

	m_CameraController.OnEvent(e);
}
