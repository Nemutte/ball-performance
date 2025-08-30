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

void Input(GLFWwindow* window, Camera* camera)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->w = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->a = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->s = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->d = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera->space_bar = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera->shift = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
		camera->w = false;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
		camera->a = false;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
		camera->s = false;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		camera->d = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		camera->space_bar = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		camera->shift = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		CLOSE = true;
	}

	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	glm::vec2 mouse_pos = { (float)mouse_x, (float)mouse_y };
	glm::vec2 center = { screen_width / 2.0, screen_height / 2.0 };
	glm::vec2 delta = mouse_pos - center;
	// Zmieniamy yaw i pitch na podstawie ruchu myszy
	cameraAngle_alfa += delta.x * mouseSensitivity;
	cameraAngle_beta += delta.y * mouseSensitivity;
	// Ograniczamy pitch, ¿eby kamera siê nie "przewróci³a"
	const float pitchLimit = M_PI / 2.0f - 0.01f;
	if (cameraAngle_beta > pitchLimit) cameraAngle_beta = pitchLimit;
	if (cameraAngle_beta < -pitchLimit) cameraAngle_beta = -pitchLimit;

	camera->Rotate(cameraAngle_alfa, cameraAngle_beta);
	glfwSetCursorPos(window, screen_width / 2.0, screen_height / 2.0);

	glfwPollEvents();
}
void Draw2d()
{
	for (Circle* circle : circles)
	{
		circle->Draw();
	}
}
void Draw3d()
{
	glUseProgram(shader3d);
	unsigned int body_location = glGetUniformLocation(shader3d, "model");
	unsigned int body_view_location = glGetUniformLocation(shader3d, "view");
	unsigned int body_proj_location = glGetUniformLocation(shader3d, "projection");

	glm::mat4 body_model = glm::mat4(1.0f);
	//glUniformMatrix4fv(body_location, 1, GL_FALSE, glm::value_ptr(body_model));
	glUniformMatrix4fv(body_view_location, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(body_proj_location, 1, GL_FALSE, glm::value_ptr(projection));

	for (Ball* ball : balls)
	{
		body_model = glm::translate(glm::mat4(1.0f), ball->collision_body->position);
		glUniformMatrix4fv(body_location, 1, GL_FALSE, glm::value_ptr(body_model));
		ball->Draw();
	}
	body_model = glm::translate(glm::mat4(1.0f), testB.position);
	glUniformMatrix4fv(body_location, 1, GL_FALSE, glm::value_ptr(body_model));
	testB.Draw();
}
void MoveToBorders()
{
	for (Circle* circle : circles)
	{
		if(circle->position.x < -screen_width / 2.0f + 5.0f)
			circle->position.x = -screen_width / 2.0f + 5.0f;
		if(circle->position.x > screen_width / 2.0f - 5.0f)
			circle->position.x = screen_width / 2.0f - 5.0f;
		if(circle->position.y < -screen_height / 2.0f + 5.0f)
			circle->position.y = -screen_height / 2.0f + 5.0f;
		if(circle->position.y > screen_height / 2.0f - 5.0f)
			circle->position.y = screen_height / 2.0f - 5.0f;
	}
}
void Update2d(float dt)
{
	for (Circle* circle : circles)
	{
		circle->Update(dt);
	}
	Collision();
	MoveToBorders();
}
void Update3d(float dt)
{
	for (Ball* ball : balls)
	{
		//ball->position+=glm::vec3(0.01, 0, 0);
	}
}
void Collision()
{
	for (Circle* a : circles)
	{
		for (Circle* b : circles)
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
int StartSimulation2d()
{
	if (!SetupGLFW())
		return -1;

	// Setup View
	glClearColor(0.16f, 0.20f, 0.20f, 1.0f);
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);

	// Create Shaders
	shader2d = make_shader(
		"../src/shaders/vertex.txt",
		"../src/shaders/fragment.txt"
	);

	// Creating model for balls
	Model* model = new Model(shader2d);

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
			float move = ((int)count % 300);
			circles.push_back(new Circle(glm::vec2(-300.0 + move, 200.0), model));
			circles.push_back(new Circle(glm::vec2(-300.0 + move + 10.f, 200.0), model));
			circles.push_back(new Circle(glm::vec2(-300.0 + move + 20.f, 200.0), model));
			circles.push_back(new Circle(glm::vec2(-300.0 + move + 30.f, 200.0), model));
			count += 4.f;
			time_for_spawn_ball = 0.f;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		Update2d(dt);
		Draw2d();

		glfwSwapBuffers(window);
	}

	// Ending program
	glfwTerminate();
	return 0;
}
int StartSimulation3d()
{
	if (!SetupGLFW())
		return -1;

	// Setup View
	glClearColor(0.16f, 0.20f, 0.20f, 1.0f);
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);

	// Set static cursor position
	glfwSetCursorPos(window, screen_width / 2.0, screen_height / 2.0);

	// Create Shaders
	shader3d = hib::make_shader();

	//Create Balls
	balls.push_back(new Ball(glm::vec3(0.0, 0.0, 0.0), 1.0));

	// Creating model for balls
	for (Ball* ball : balls)
	{
		ball->collision_body->CreateDrawableModel();
	}
	testB.CreateDrawableModel();
	// Setup Camera and perspective
	Camera* camera = new Camera(glm::vec3(0.0, 1.0, 1.0));
	projection = glm::perspective(45.0f, (float)screen_width / screen_height, 0.1f, 30.0f);

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
	while (!glfwWindowShouldClose(window) && !CLOSE)
	{
		glfwPollEvents();
		// Calculate delta time
		float dt = (float)glfwGetTime() - last_time;
		last_time = (float)glfwGetTime();

		Input(window, camera);
		camera->Move(dt);
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
			float move = ((int)count % 300);
			//count += 4.f;
			time_for_spawn_ball = 0.f;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setup camera
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		view = glm::lookAt(camera->position, camera->GetCameraTarget(), up);

		Update3d(dt);
		Draw3d();

		//printf("camera: x = %f, y = %f, z = %f\n", camera->position.x, camera->position.y, camera->position.z);

		glfwSwapBuffers(window);
	}

	// Ending program
	glfwTerminate();
	return 0;
}
int main()
{
	return StartSimulation3d();
}