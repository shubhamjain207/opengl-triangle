#include "Header.h"


void frame_buffer_resize(GLFWwindow* window, int W, int H) {

	glViewport(0,0,W,H);



}

Vertex vertices[] = {

	glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)

};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {

	0, 1, 2

};

unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);


void updateInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	
	}

}

bool loadShaders(GLuint &program) {
	bool loadSuccess = true;

	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string source = "";  

	std::ifstream infile;

	//vertex
	infile.open("vertex_core.glsl");
	
	if (infile.is_open()) {
		while (std::getline(infile, temp)) {
			source += temp + "\n";
		}
	}
	else {
		loadSuccess = false;
		std::cout << "Error" << std::endl;

	}
	
	infile.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = source.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "12::Load Shader";
		std::cout << infoLog << std::endl;
	}

	temp = "";
	source = "";

	

	//fragment
	infile.open("fragment_core.glsl");

	if (infile.is_open()) {
		while (std::getline(infile, temp)) {
			source += temp + "\n";
		}

		
	}
	else {
		std::cout << "Error" << std::endl;
		loadSuccess = false;
		
	}

	infile.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSource  = source.c_str();
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error::Load ShaderFrag";
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}
	


	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetShaderiv(program, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Error::1212Link Shader";
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;

}


int main() {
		

	glfwInit();

	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;

	//major==4.minor==4 4.4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);




	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window 1", NULL,NULL);

	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0,0,frameBufferWidth, frameBufferHeight);

	glfwMakeContextCurrent(window);
	 
	glfwSetFramebufferSizeCallback(window, frame_buffer_resize);
	
	glewExperimental = GL_TRUE;

	if (glewInit() !=GLEW_OK) {
		std::cout << "Error" << std::endl;

		glfwTerminate();
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);





	GLuint core_program;
	if (!loadShaders(core_program)) {
		glfwTerminate();
	}


	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	//texcoord
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {
	
		glfwPollEvents();

		updateInput(window);

		glClearColor(0.f,0.f,0.f,1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(core_program);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(window);
		glFlush();



	
	}

	

	glfwTerminate();

	return 0;
}