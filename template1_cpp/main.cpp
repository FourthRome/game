#include "common.h"
#include "Image.h"
#include "Tile.h"
#include "Level.h"
#include "Player.h"


#include <GLFW/glfw3.h>
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int startX{}, startY{}, posExitX{}, posExitY{};
bool flag_stop{}, flag_fall{}, flag_exit{}, flag_final_exit{};
int curX = 0;
int curY = WINDOW_HEIGHT - TILE_HEIGHT;
double EPSILON = 0.00001;

//map<TileType, Image*> images; // clear
//void addImage(TileType type, const std::string& path) {	images[type] = Image(path); }

struct InputState
{
	bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
	GLfloat lastX = 400, lastY = 300; //исходное положение мыши
	bool firstMouse = true;
	bool captureMouse = true;  // Мышка захвачена нашим приложением или нет?
	bool capturedMouseJustNow = false;
} static Input;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_1:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case GLFW_KEY_2:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		if (action == GLFW_PRESS)
			Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
			Input.keys[key] = false;
	}
}

void processPlayerMovement(Player& player, std::string& chars)
{
	if (Input.keys[GLFW_KEY_W])
		player.ProcessInput(MovementDir::UP, chars, flag_stop, flag_fall, flag_exit, flag_final_exit);
	else if (Input.keys[GLFW_KEY_S])
		player.ProcessInput(MovementDir::DOWN, chars, flag_stop, flag_fall, flag_exit, flag_final_exit);
	if (Input.keys[GLFW_KEY_A])
		player.ProcessInput(MovementDir::LEFT, chars, flag_stop, flag_fall, flag_exit, flag_final_exit);
	else if (Input.keys[GLFW_KEY_D])
		player.ProcessInput(MovementDir::RIGHT, chars, flag_stop, flag_fall, flag_exit, flag_final_exit);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		Input.captureMouse = !Input.captureMouse;

	if (Input.captureMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Input.capturedMouseJustNow = true;
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (Input.firstMouse)
	{
		Input.lastX = float(xpos);
		Input.lastY = float(ypos);
		Input.firstMouse = false;
	}

	GLfloat xoffset = float(xpos) - Input.lastX;
	GLfloat yoffset = Input.lastY - float(ypos);

	Input.lastX = float(xpos);
	Input.lastY = float(ypos);
}

void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	// ...
}

int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "Controls: " << std::endl;
	std::cout << "press right mouse button to capture/release mouse cursor  " << std::endl;
	std::cout << "W, A, S, D - movement  " << std::endl;
	std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void drawTile(const int curX, const int curY, Image& screenBuffer, Image& picture) {
	for (int i = 0; i < picture.Width(); ++i)
		for (int j = 0; j < picture.Height(); ++j)
			screenBuffer.PutPixel(curX + i, curY + j, picture.GetPixel(i, j));

}

void openFiles(const std::string& path, std::string& chars) {
	std::ifstream room_file(path);
	char c;
	if (room_file.is_open()) {
		while (room_file.get(c)) {
			chars.push_back(c);
		}
		std::cout << chars.size() << std::endl;
	}
	else { std::cout << "ERROR::COULDN'T OPEN FILE: " << path << std::endl; }
}

void drawRoom(std::string chars, Image& screenBuffer, Image& floor, Image& wall, Image& man, Image& thorn, Image& exit, Image& finalExit) {

	curX = 0;
	curY = WINDOW_HEIGHT - TILE_HEIGHT;

	startX = 0;
	startY = posExitY;

	for (int i = 0; i < chars.size(); i++) {

		switch (chars[i]) {
			case ' ':
				break;
			case '.':
				drawTile(curX, curY, screenBuffer, floor);
				break;
			case '#':
				drawTile(curX, curY, screenBuffer, wall);
				break;
			case '@':
				//chars[i] = '.';
				startX = curX;
				startY = curY;
				break;
			case 'T':
				drawTile(curX, curY, screenBuffer, thorn);
				break;
			case 'x':
				posExitX = 0;
				posExitY = curY;
				drawTile(curX, curY, screenBuffer, exit);
				break;
			case 'Q':
				drawTile(curX, curY, screenBuffer, finalExit);
				break;
			default: break;
		}
		if (chars[i] == '\n') {
			curX = 0;
			curY -= wall.Height();
		}
		else {
			curX += wall.Width();
			
		}
		std::cout << chars[i];
	}
}

void drawLevel(Level& level, Image& screenBuffer) {
	for (int curX = 0; curX < WINDOW_WIDTH; curX += TILE_WIDTH) {
		for (int curY = 0; curY < WINDOW_HEIGHT; curY += TILE_HEIGHT) {
			
			auto curType = level.GetTile(curX, curY).GetType();
            if (curType == TileType::MAN)
            {
                startX = curX;
                startY = curY;
            }
			//Image curImage = map[curType];
			drawTile(curX, curY, screenBuffer, *(Tile::images[curType]));
		}
	}
		
}

void restoreBackGround(Image& screenBuffer, Image& picture, Player& man) {

	int numY = (int) trunc(man.GetOldCoords().y / TILE_HEIGHT); // here should trunc all brackets
	int numX = (int) trunc(man.GetOldCoords().x / TILE_WIDTH); // here should trunc all brackets
	if (man.GetOldCoords().y - trunc(man.GetOldCoords().y) < EPSILON && man.GetOldCoords().y % TILE_HEIGHT == 0 &&
		man.GetOldCoords().x - trunc(man.GetOldCoords().x) < EPSILON && man.GetOldCoords().x % TILE_WIDTH == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
	}
	else if (man.GetOldCoords().y - trunc(man.GetOldCoords().y) < EPSILON && man.GetOldCoords().y % TILE_HEIGHT == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
	}
	else if (man.GetOldCoords().x - trunc(man.GetOldCoords().x) < EPSILON && man.GetOldCoords().x % TILE_WIDTH == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile(numX * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
	}
	else {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile(numX * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
	}
}

int main(int argc, char** argv) {
	if (!glfwInit())
		return -1;
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, OnKeyboardPressed);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback(window, OnMouseScroll);
	if (initGL() != 0)
		return -1;
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();



	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);// 4 - number of channels
	Image gameOver("..resources/gameOver.jpg");
	Image gameWin("..resources/win.jpg");
    /*Image floor("resources\\floor.png");
	Image wall("resources\\wall.png");
	Image man("resources\\man.png");
	Image thorn("resources\\thorn.png");
	Image exit("resources\\exit.png");
	Image finalExit("resources\\finalExit.jpg");
	*/

	Tile::addImage(TileType::FLOOR, "resources\\floor.png");
	Tile::addImage(TileType::WALL, "resources\\wall.png");
	Tile::addImage(TileType::MAN, "resources\\man.png");
	Tile::addImage(TileType::THORN, "resources\\thorn.png");
	Tile::addImage(TileType::EXIT, "resources\\exit.png");
	Tile::addImage(TileType::FINAL_EXIT, "resources\\finalExit.jpg");


	// std::string chars1 = "", chars2 = "", chars3 = "", chars4 = "";
	std::string arrOfTypes = "";
	Level level1("resources\\room1.txt");
	//Level level2("resources\\room2.txt");
	//Level level3("resources\\room3.txt");
	//Level level4("resources\\room4.txt");
	openFiles("resources\\common_plan.txt", arrOfTypes);
	
	int indexOfTypes{};
	std::string charsCur;
	switch (arrOfTypes[indexOfTypes++]) {
		case 'A':
			drawLevel(level1, screenBuffer);
			//drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
			break;
		case 'B':
			//charsCur = chars2;
			//drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
			drawLevel(level1, screenBuffer);
			break;
		case 'C':
			//charsCur = chars3;
			//drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
			drawLevel(level1, screenBuffer);
			break;
		case 'D':
			//charsCur = chars4;
			//drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
			drawLevel(level1, screenBuffer);
			break;
		}

	Point starting_pos{ .x = startX, .y = startY };
	std::cout << "pos  = " << startX << " " << startY << std::endl;
	Player player{ starting_pos };
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);	GL_CHECK_ERRORS;

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		processPlayerMovement(player, charsCur);
		restoreBackGround(screenBuffer, *(Tile::images[TileType::FLOOR]), player);
		player.DrawOfPlayer(screenBuffer, *(Tile::images[TileType::MAN]));
		 

		if (flag_fall) {
			curX = 0;
			curY = 0;
			drawTile(curX, curY, screenBuffer, gameOver);
		}

		//if (flag_exit) {
		//	flag_stop = false;
		//	flag_fall = false;
		//	flag_exit = false;
		//	flag_final_exit = false;
		//	curX = 0;
		//	curY = 0;
		//	switch (arrOfTypes[indexOfTypes++]) {
		//		case 'A':
		//			std::cout << "A" << std::endl;
		//			charsCur = chars1;
		//			drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
		//			break;
		//		case 'B':
		//			std::cout << "B" << std::endl;
		//			charsCur = chars2;
		//			drawRoom(chars2, screenBuffer, floor, wall, man, thorn, exit, finalExit);
		//			for (int i = 0; i < chars2.size(); i++)
		//				std::cout << chars2[i];
		//			break;
		//		case 'C':
		//			charsCur = chars3;
		//			drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
		//			break;
		//		case 'D':
		//			charsCur = chars4;
		//			drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit);
		//			break;
		//	}
		//	//starting_pos{ .x = startX, .y = startY };
		//	//startX = player.GetCoords().x;
		//	startX = 0;
		//	startY = player.GetCoords().y;
		//	std::cout << "pos  = " << startX << " " << startY << std::endl;
		//	//player.PutStartCoords(startX, startY);// { starting_pos };
		//	player.PutCoords(0, startY);
		//}
	
		if (flag_final_exit) {
			curX = 0;
			curY = 0;
			drawTile(curX, curY, screenBuffer, gameWin);
		}




		glClearColor(0.0f, 1.0f, 1.0f, 1.0f); GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include "shader_s.h"
//#include "stb_image.h"
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//int main()
//{   
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
//    if (window == NULL){
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // glad: загрузка всех указателей на OpenGL-функции
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // Компилирование нашей шейдерной программы
//    Shader ourShader("../4.1.texture.vs", "../4.1.texture.fs");
//
//    // Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
//    float vertices1[] = {
//         // координаты        // цвета            // текстурные координаты
//         -0.8f, -0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // верхняя правая вершина
//         -0.8f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // нижняя правая вершина
//         -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // нижняя левая вершина
//         -1.0f, -0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // верхняя левая вершина 
//    };
//    float vertices2[] = {
//        // координаты        // цвета            // текстурные координаты
//        -0.6f, -0.6f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // верхняя правая вершина
//        -0.6f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // нижняя правая вершина
//        -0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // нижняя левая вершина
//        -0.8f, -0.6f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // верхняя левая вершина 
//    };
//    unsigned int indices[] = {
//        0, 1, 3, // первый треугольник
//        1, 2, 3  // второй треугольник
//    };
//    unsigned int VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
//    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Координатные атрибуты
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//	
//    // Цветовые атрибуты
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//	
//    // Атрибуты текстурных координат
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//
//    // Загрузка и создание текстуры
//    unsigned int texture;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture); // все последующие GL_TEXTURE_2D-операции теперь будут влиять на данный текстурный объект
//	
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT (стандартный метод наложения)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	
//    // Загрузка изображения, создание текстуры и генерирование мипмап-уровней
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load("resources\\man_min.png", &width, &height, &nrChannels, 0);
//    if (data) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    } else  { std::cout << "Failed to load texture" << std::endl;}
//    stbi_image_free(data);
//
//
//    // Цикл рендеринга
//    while (!glfwWindowShouldClose(window))
//    {
//        // Обработка ввода
//        processInput(window);
//
//        // Рендеринг
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Связывание текстуры
//        glBindTexture(GL_TEXTURE_2D, texture);
//
//        // Рендеринг ящика
//        ourShader.use();
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    glfwTerminate();
//    return 0;
//}

//// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
//void processInput(GLFWwindow* window){
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
//void framebuffer_size_callback(GLFWwindow* window, int width, int height){
//    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
//    // Обратите внимание, что высота и ширина будут значительно больше, чем указано, на Retina-дисплеях
//    glViewport(0, 0, width, height);
//}
