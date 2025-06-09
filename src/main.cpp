#include "main.h"

bool SetupGLFW()
{
	if (!glfwInit())
	{
		std::cout << "GLFW cant be loaded" << std::endl;
		return false;
	}
	window = glfwCreateWindow(screen_width, screen_height, "Cube", NULL, NULL);
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
void Draw()
{
	for (Ball* ball : balls)
	{
		ball->Draw();
	}
}
void MoveToBorders()
{
	for (Ball* ball : balls)
	{
		if(ball->position.x < -screen_width)
			ball->position.x = -screen_width;
		if(ball->position.x > screen_width)
			ball->position.x = screen_width;
		if(ball->position.y < -screen_height)
			ball->position.y = -screen_height;
		if(ball->position.y > screen_height)
			ball->position.y = screen_height;
	}
}
void Update(float dt)
{
	for (Ball* ball : balls)
	{
		ball->Update(dt);
	}
	MoveToBorders();
	Collision();
}
void Collision()
{
	for (Ball* a : balls)
	{
		for (Ball* b : balls)
		{
			if (a != b)
			{
				if (DetectCollision(a, b))
				{
					SolveCollision(a, b);
				}
			}
		}
	}
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

	// Creating balls
	Model* model = new Model(shader);

	// Set delta time
	float last_time = (float)glfwGetTime();
	float dt = 0.f;

	// Set timers for FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	//glfwSwapInterval(0);

	// Setup for spawn ball
	float time_for_spawn_ball = 0.f;
	float count = 0.0;

	// Program loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Calculate delta time
		float dt = (float)glfwGetTime() - last_time;
		last_time = (float)glfwGetTime();

		// FPS
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string newTitle = "balls: " + std::to_string(count) + " | " + FPS + " FPS";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
		}

		// Ball spawner
		time_for_spawn_ball += dt;
		if (spawn_ball_ratio <= time_for_spawn_ball)
		{
			balls.push_back(new Ball(glm::vec2(-340.0 + count, 0.0), model));
			balls.push_back(new Ball(glm::vec2(-340.0 + count + 10.f, 0.0), model));
			balls.push_back(new Ball(glm::vec2(-340.0 + count + 20.f, 0.0), model));
			balls.push_back(new Ball(glm::vec2(-340.0 + count + 30.f, 0.0), model));
			count += 4.f;
			time_for_spawn_ball = 0.f;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		Update(dt);
		Draw();

		glfwSwapBuffers(window);
	}

	// Ending program
	glfwTerminate();
	return 0;
}