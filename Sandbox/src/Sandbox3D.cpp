#include "Sandbox3D.h"


float Sandbox3D::deltaTime = 0.016f;


Sandbox3D::Sandbox3D() 
	: Layer("Sandbox3D"), m_CameraController() {

}

void Sandbox3D::OnAttach() {

	// Square VertexArray 
	m_CubeVA = Vi::VertexArray::Create();

	// Square VertexBuffer 
	float cubeVertices[5 * 4 * 2] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f
	};
	Vi::Ref<Vi::VertexBuffer> cubeVB;
	cubeVB.reset(Vi::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)));
	cubeVB->SetLayout({
		{ Vi::ShaderDataType::Float3, "a_Position" }
		});
	m_CubeVA->AddVertexBuffer(cubeVB);

	// Cube IndexBuffer 
	uint32_t cubeIndices[6 * 6] = {
		0,1,2,2,3,0,
		0,4,7,7,3,0,
		4,5,6,6,7,4,
		5,1,2,2,6,5,
		3,2,6,6,7,3,
		0,1,5,5,4,0
	};
	Vi::Ref<Vi::IndexBuffer> cubeIB;
	cubeIB.reset(Vi::IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t)));
	m_CubeVA->SetIndexBuffer(cubeIB);


	// Create And Compile flat Shader 
	m_FlatColorShader = Vi::Shader::Create("assets/shaders/FlatColor.glsl");

}

void Sandbox3D::OnDetach() {

}

void Sandbox3D::OnUpdate(Vi::Timestep ts) {

	// Update 
	Sandbox3D::deltaTime = ts;										//temp
	m_CameraController.OnUpdate(ts);

	// Render 

	// Set Background Color And Clear It 
	Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Vi::RenderCommand::Clear();

	Vi::Renderer::BeginScene(m_CameraController.GetCamera());


	//Bind FlatColor Shader And Set Uniform Color 
	std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Vi::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_ShaderColor);



	// Draw Cube With RGBA Color 
	Vi::Renderer::Submit(m_FlatColorShader, m_CubeVA);

	Vi::Renderer::EndScene();
	fullScreen = Vi::Application::Get().GetWindow().IsFullScreen();		// temp 

}

void Sandbox3D::OnImGuiRender() {

	auto& window = Vi::Application::Get().GetWindow();
	ImGui::Begin("Settings");														 // temp
	ImGui::ColorEdit4("ColorShader", (float*)&m_ShaderColor);
	ImGui::ColorEdit4("GroundShader", (float*)&m_GroundColor);
	ImGui::End();																   // temp




	ImGui::Begin("Stats");														   // temp 
	ImGui::Text("Fps  %.0fFps", 1 / Sandbox3D::deltaTime);
	ImGui::Text("ts  %.3fms", Sandbox3D::deltaTime);
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
