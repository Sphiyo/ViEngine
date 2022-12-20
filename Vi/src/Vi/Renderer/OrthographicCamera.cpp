#include "Vipch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

// temp
/*
#include "Vi/Application.h"
#include <GLFW/glfw3.h>
*/
namespace Vi {



	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top ,-1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	// temp
	/*void OrthographicCamera::SetAspectRatio(unsigned int width, unsigned int height)
	{

		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		
		float lheight = (float)height;
		float lwidth = (float)width;
		float aspectratioh = lheight / lwidth;
		float aspectratiow = lwidth / lheight;
		
	
		std::cout << aspectratioh << " = aspectratioh " << "\n";
		std::cout << aspectratiow << " = aspectratiow " << "\n";


		m_ProjectionMatrix = glm::ortho(-lwidth * aspectratioh / 1000, lwidth * aspectratioh / 1000, -lheight * aspectratiow / 1000, lheight * aspectratiow / 1000, -1.0f, 1.0f);
	}*/

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0,0,1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}