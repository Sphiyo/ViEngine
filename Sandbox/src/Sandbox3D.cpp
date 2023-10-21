#include "Sandbox3D.h"
#include "Vi/Debug/Timer.h"
#include <glm/gtx/quaternion.hpp>

float Sandbox3D::deltaTime = 0.016f;

//////////////////////////////////

// Function to convert a PhysX quaternion to an OpenGL quaternion
glm::quat convertPhysXToOpenGLQuaternion(const physx::PxQuat& physxQuaternion) {
	// Convert the PhysX quaternion to a glm::quat (OpenGL's coordinate system)
	return glm::quat(physxQuaternion.w, physxQuaternion.x, physxQuaternion.y, physxQuaternion.z);
}

// Function to convert an OpenGL quaternion to Euler angles (in degrees)
glm::vec3 quaternionToEulerAngles(const glm::quat& quaternion) {
	glm::mat4 rotationMatrix = glm::toMat4(quaternion);
	return glm::eulerAngles(quaternion) * glm::degrees(1.0f);
}



///////////////////////////////////


Sandbox3D::Sandbox3D() :
	Layer("Sandbox3D") {

	VI_PROFILE_FUNCTION();
	Vi::CameraController3D::Get().GetCamera().SetWorldLocation(0.0f, 4.0f, 8.0f);
}

void Sandbox3D::OnAttach() {

	VI_PROFILE_FUNCTION();

	//Construct And Buffer Texture To Memory
	m_Texture = Vi::Texture2D::Create("assets/textures/Cat03.jpg", 0);
	m_TransparentTexture = Vi::Texture2D::Create("assets/textures/RGBA.png", 0);
	m_CheckerTexture = Vi::Texture2D::Create("assets/textures/Checkerboard.png", 0);

}

void Sandbox3D::OnDetach() {
	VI_PROFILE_FUNCTION();
}

void Sandbox3D::OnUpdate(Vi::Timestep ts) {
	deltaTime = ts.GetMilliSeconds();										//temp
	VI_PROFILE_FUNCTION();
	VI_CORE_WARN("Sandbox3D.cpp: OnUpdate(): Timestep is {} : FPS = {}", deltaTime, 1000 / deltaTime);
	//PROFILE_SCOPE("Sandbox3D::OnUpdate");
	// Update 
										//temp
	{
		VI_PROFILE_SCOPE("CameraController::OnUpdate");
		Vi::CameraController3D::Get().OnUpdate(ts);
	}
	// Render 

	// Set Background Color And Clear It 
	///////////////////////////{
	///////////////////////////	VI_PROFILE_SCOPE("Renderer Prep");
	///////////////////////////	Vi::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	///////////////////////////	Vi::RenderCommand::Clear();
	///////////////////////////}

	Vi::Renderer3D::BeginScene(Vi::CameraController3D::Get().GetCamera());

	{
		VI_PROFILE_SCOPE("Renderer Draw");
	#if 0
		Vi::Renderer3D::DrawPlane(m_GroundLocation, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,100.0f }, m_GroundColor);
		Vi::Renderer3D::DrawPlane(m_GroundLocation, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,100.0f }, m_TransparentTexture, { 1.0f,0.0f,0.0f,1.0f });

		int rows = m_GridCount[0];
		int columns = m_GridCount[1];
		float offsetX = m_GridOffset[0];
		float offsetY = m_GridOffset[1];

		for (int x = 0; x < rows; x++) {

			for (int y = 0; y < columns; y++) {

				//Vi::Renderer3D::DrawCube({ m_CubeGridLocation.x + y * offsetY ,m_CubeGridLocation.y + 0.5f,m_CubeGridLocation.z + x * offsetX }, { 0.0f, 0.0f, 0.0f }, { 1.0f,3.0f,1.0f }, m_CubeColor);
			}
		}
		Vi::Renderer3D::DrawCube({ 0.0f  , -0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, m_CubeColor);
		Vi::Renderer3D::DrawPlane({ 0.0f  , 0.8f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f }, m_CubeColor);
	#endif
	}
	auto dir = Vi::CameraController3D::Get().GetCamera().GetForwardVector();
	glm::quat quatstat = glm::quat();
	VI_INFO("RIGIDDYNAMIC count {}", Vi::Physics::VPhysicsRigidActor::GetRigidDynamic().size());

	int i = 0;
	for (auto& actor : Vi::Physics::VPhysicsRigidActor::GetRigidDynamic()) {

		auto tr = actor->getGlobalPose();

		physx::PxQuat rot = tr.q;

		// Convert the PhysX quaternion to an OpenGL quaternion
		glm::quat openglQuaternion = convertPhysXToOpenGLQuaternion(rot);

		// Convert the OpenGL quaternion to Euler angles (in degrees)
		glm::vec3 eulerAngles = quaternionToEulerAngles(openglQuaternion);


		glm::quat quat;
		quat.x = openglQuaternion.x;
		quat.y = openglQuaternion.y;
		quat.z = openglQuaternion.z;
		quat.w = openglQuaternion.w;


		Vi::Renderer3D::DrawCube({ tr.p.x , tr.p.y, tr.p.z }, quat, { 2.0f,2.0f,2.0f }, { (float)i/3.0f,0.0f,1.0f,1.0f });
		
		i++;
	}
	for (auto& actor : Vi::Physics::VPhysicsRigidActor::GetRigidDynamic()) {

		auto tr = actor->getGlobalPose();

		physx::PxQuat rot = tr.q;

		// Convert the PhysX quaternion to an OpenGL quaternion
		glm::quat openglQuaternion = convertPhysXToOpenGLQuaternion(rot);

		// Convert the OpenGL quaternion to Euler angles (in degrees)
		glm::vec3 eulerAngles = quaternionToEulerAngles(openglQuaternion);


		glm::quat quat;
		quat.x = openglQuaternion.x;
		quat.y = openglQuaternion.y;
		quat.z = openglQuaternion.z;
		quat.w = openglQuaternion.w;


		Vi::Renderer3D::DrawPlane({ 0.0f,0.0f,0.0f }, glm::quat(), {100.0f,100.0f,100.0f}, {1.0f,0.0f,1.0f,1.0f});

		
	}
	//Vi::Renderer3D::DrawCube(Vi::CameraController3D::Get().GetCamera().GetWorldLocation(), dir, { 0.0f , 10.0f, 0.0f }, quatstat, { 2.0f,2.0f,2.0f }, {1.0f,0.0f,1.0f,1.0f });

	Vi::Renderer3D::EndScene();

	fullScreen = Vi::Application::Get().GetWindow().IsFullScreen();		// temp 
}

void Sandbox3D::OnFixedUpdate(Vi::Timestep ts)
{

}

void Sandbox3D::OnEvent(Vi::Event& e) {

	Vi::CameraController3D::Get().OnEvent(e);
	//m_CameraController2D.OnEvent(e);
	Vi::EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<Vi::KeyPressedEvent>(VI_BINDEVENT(Sandbox3D::OnKeyPressed));
}

bool Sandbox3D::OnKeyPressed(Vi::KeyPressedEvent& e) {

	if (e.GetKeyCode() == VI_KEY_SPACE) {

		m_CubeCountPx++;
		//Vi::Physics::VRigidDynamic Actor;
		physx::PxVec3 Position = physx::PxVec3(m_CubeCountPx * 2.0f, 10.0f, 0.0f);
		physx::PxVec3 Rotation = physx::PxVec3(40.f, 45.f, 0.0f);
		//Vi::Physics::VPhysicsMaterial Mat;
		Vi::Physics::VPhysicsRigidActor::Get().CreateRigidDynamic(Position, Rotation, physx::PxBoxGeometry(1.0f, 1.0f, 1.0f)/*, Mat*/);
		//m_CubeCount++;
	}


	return false;
}


void Sandbox3D::OnImGuiRender() {

	VI_PROFILE_FUNCTION();

	auto& window = Vi::Application::Get().GetWindow();
	ImGui::Begin("Settings");														 // temp
	ImGui::ColorEdit4("ColorShader", (float*)&m_CubeColor);
	ImGui::ColorEdit4("GroundShader", (float*)&m_GroundColor);
	ImGui::DragInt2("Row Count", (int*)&m_GridCount, 1.0f, 0, 1000);
	ImGui::DragFloat2("Row Offset", (float*)&m_GridOffset, 0.1f, 0.0f, 1000.0f, "%4.2f");
	ImGui::End();																   // temp

	{
		ImGui::DragFloat3("Cube1Position", glm::value_ptr(m_Cube1Position), 0.01f);
		ImGui::DragFloat3("m_GroundLocation", glm::value_ptr(m_GroundLocation), 0.01f);
		ImGui::DragFloat3("m_CubeGridLocation", glm::value_ptr(m_CubeGridLocation), 0.01f);
		ImGui::DragFloat3("Cube4Position", glm::value_ptr(m_Cube4Position), 0.01f); 
	}

	ImGui::Begin("Stats");														   // temp 
	ImGui::Text("Fps  %.0fFps", 1000 / Sandbox3D::deltaTime);
	ImGui::Text("ts  %.3fms", Sandbox3D::deltaTime);
	ImGui::End();

//	ImGui::Begin("Profiler");
//
//	// remove delete to do to fix
//	for (auto& result : ProfileResults::m_ProfileResults) {
//
//		char label[50];
//		strcpy_s(label, "%.3fms  ");
//		strcat_s(label, result.Name);
//		ImGui::Text(label, result.Time);
//	}
//	ProfileResults::Get().clear();
//
//	ImGui::End();

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