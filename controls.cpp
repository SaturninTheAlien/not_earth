#include "controls.h"
#include "math.h"

#include <glm/gtc/matrix_transform.hpp>

void computeMatricesFromInputs(GLFWwindow* window, glm::vec3&position, glm::mat4& P, glm::mat4& V, float speed, float mouseSpeed){

	// Get mouse position
	double xpos=0, ypos=0;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1280/2, 800/2);

	// Compute new orientation
	static float horizontalAngle = 0;
	static float verticalAngle = 0;
	horizontalAngle += mouseSpeed * float(1280/2 - xpos );
	verticalAngle   += mouseSpeed * float( 800/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * speed;
	}

	float FoV = 0;

	P = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	V = glm::lookAt(
        position,           // Camera is here
        position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
}