#include "main.h"

bool SetupGLFW()
{
	if (!glfwInit())
	{
		std::cout << "GLFW cant be loaded" << std::endl;
		return false;
	}
	window = glfwCreateWindow(640, 480, "Cube", NULL, NULL);
	if (!window)
	{
		std::cout << "window is NULLPTR" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return false;
	}
	return true;
}
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
	std::vector<unsigned int> modules;
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	int success;
	glGetShaderiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << std::endl;
	}

	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	return shader;
}
unsigned int make_module(const std::string& filepath, unsigned int module_type)
{
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line))
	{
		bufferedLines << line << "\n";
	}
	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
	}
	return shaderModule;
}
int main()
{
	if (!SetupGLFW())
		return -1;

	// Setup View
	glClearColor(0.16f, 0.20f, 0.20f, 1.0f);
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);

	// Create Shaders
	shader = make_shader(
		"../src/shaders/vertex.txt",
		"../src/shaders/fragment.txt"
	);

	// Set delta time
	float last_time = (float)glfwGetTime();
	float dt = 0.f;

	// Program loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float dt = (float)glfwGetTime() - last_time;
		last_time = (float)glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	// Ending program
	glfwTerminate();
	return 0;
}