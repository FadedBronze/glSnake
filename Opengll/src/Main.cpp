#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Window.h"
#include "FlyCamera.h"
#include "Color.h"

const int gridX = 10;
const int gridY = 10;

inline void setGrid(Color grid[], int x, int y, Color color) { grid[x + y * gridX] = color; }

void clearGrid(Color grid[], int size) {
    for (int i = 0; i < size; i++) {
        grid[i] = Color{ 0.0f, 0.0f, 0.0f, 0.0f };
    }
}

enum GameState {
    First,
    Dead,
    Starting,
    Playing,
    Paused
};

enum SnakeDir {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Position {
    int x;
    int y;
};

template <typename T>
T pop(T arr[], int size) {
    return deleteElement(arr, size, 0);
}

template <typename T>
T deleteElement(T arr[], int size, int index) {

    T ele = arr[index];

    for (int i = index; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }

    return ele;
}

void renderSnakeGrid(Color grid[], Position snake[], int snakeLength, Position apple) {
    setGrid(grid, apple.x, apple.y, Color{ 0.8f, 0.3f, 0.2f, 1.0f });

    for (int i = 0; i < snakeLength; i++) {
        Position& snakeChunk = snake[i];

        setGrid(grid, int(snakeChunk.x), int(snakeChunk.y), Color{ 0.2f - (0.1f * i / snakeLength), 0.3f - (0.1f * i / snakeLength), 0.8f - (0.1f * i / snakeLength), 1.0f });
    }
}

bool posOnSnake(int snakeLength, Position snake[], Position pos) {

    for (int i = 0; i < snakeLength; i++) {
        Position& snakeChunk = snake[i];

        if (pos.x == snakeChunk.x && pos.y == snakeChunk.y) {
            return true;
        }
    }

    return false;
}

void LogPositions(Position positions[], int length) {
    std::cout << "[" << std::endl;

    for (int i = 0; i < length; i++) {
        std::cout << "x: " << positions[i].x << ", y:" << positions[i].y << std::endl;
    }

    std::cout << "]" << std::endl;
}


void LogMatrix(Color positions[]) {
    std::cout << "[" << std::endl;

    for (int i = 0; i < gridX; i++) {
        for (int j = 0; j < gridY; j++) {
            if (positions[i + gridX * j].a != 0) {
                std::cout << " ";
            }
            else {
                std::cout << "o";
            }
        }

        std::cout << std::endl;
    }

    std::cout << "]" << std::endl;
}


void LogInts(int integers[], int length) {
    std::cout << "[" << std::endl;

    for (int i = 0; i < length; i++) {
        std::cout << integers[i] << std::endl;
    }

    std::cout << "]" << std::endl;
}

int compare(const void* a, const void* b) {
    return (*((int*)a) - *((int*)b));
}

Position getValidAppleSpawn(Position snake[], int snakeLength) {
    Position spawns[gridX * gridY];

    for (int i = 0; i < gridX; i++)
    {
        for (int j = 0; j < gridY; j++)
        {
            spawns[i + j * gridX] = Position{ i, j };
        }
    }

    int deleteIndecies[gridX * gridY];

    for (int i = 0; i < snakeLength; i++) {
        auto& snakeChunk = snake[i];
        
        deleteIndecies[i] = snakeChunk.x + snakeChunk.y * gridX;
    }

    qsort(&deleteIndecies, snakeLength, sizeof(int), compare);

    for (int i = snakeLength - 1; i >= 0; i--) {
        deleteElement(spawns, gridX * gridY, deleteIndecies[i]);
    }

    int spawnsLength = ((gridX * gridY) - snakeLength);

    Position spawn = spawns[rand() % spawnsLength];

    return spawn;
}

void snake(Color grid[], Window& window, float deltaTime) {
    static GameState gameState;
    static float timer;
    static Position snake[gridX * gridY];
    static Position apple;
    static Position snakeHead;
    static SnakeDir snakeDir;
    static int snakeLength;
    static float snakeSpeed;
    timer += deltaTime;

    switch (gameState)
    {
    case First:
        window.onKeyDown = [=](int key) {

            if (key == GLFW_KEY_UP) {
                snakeDir = SnakeDir::UP;
            }
            if (key == GLFW_KEY_DOWN) {
                snakeDir = SnakeDir::DOWN;
            }
            if (key == GLFW_KEY_LEFT) {
                snakeDir = SnakeDir::LEFT;
            }
            if (key == GLFW_KEY_RIGHT) {
                snakeDir = SnakeDir::RIGHT;
            }
            if (key == GLFW_KEY_ENTER) {
                gameState = GameState::Starting;
            }
            if (key == GLFW_KEY_P) {

                if (gameState == GameState::Playing) {
                    gameState = GameState::Paused;
                }
            }
            if (key == GLFW_KEY_R) {
                if (gameState == GameState::Paused) {
                    gameState = GameState::Playing;
                }
            }
        };
        break;
    case Starting:
        timer = 0;
        apple = { 5, 5 };
        snakeHead = { 4, 2 };
        snakeDir = SnakeDir::RIGHT;
        snakeSpeed = 4;

        while (snakeLength != 0) {
            pop(snake, snakeLength);
            snakeLength--;
        }

        snakeLength = 1;
        snake[0] = { 4, 2 };

        gameState = GameState::Playing;
        break;
    case Dead:
        renderSnakeGrid(grid, snake, snakeLength, apple);
        break;
    case Playing:
        if (timer < 1 / snakeSpeed) return;

        timer = 0;

        clearGrid(grid, gridX * gridY);

        switch (snakeDir)
        {
        case LEFT:
            snakeHead.x += 1;
            break;
        case RIGHT:
            snakeHead.x -= 1;
            break;
        case UP:
            snakeHead.y += 1;
            break;
        case DOWN:
            snakeHead.y -= 1;
            break;
        }

        if (snakeHead.x > gridX - 1 || snakeHead.x < 0 || snakeHead.y > gridY - 1 || snakeHead.y < 0) {
            gameState = GameState::Dead;
            break;
        }

        if (snakeHead.x == apple.x && snakeHead.y == apple.y) {
            apple = getValidAppleSpawn(snake, snakeLength);
            snakeLength++;
        }
        else {
            pop(snake, snakeLength);
        }

        if (posOnSnake(snakeLength, snake, snakeHead)) {
            gameState = GameState::Dead;
            break;
        }

        snake[snakeLength - 1] = Position{ snakeHead.x, snakeHead.y };

        renderSnakeGrid(grid, snake, snakeLength, apple);
        break;
    case Paused:
        renderSnakeGrid(grid, snake, snakeLength, apple);
        break;
    }
}

int main(void)
{
    Window window(1600, 1200);

    Color grid[gridX * gridY]{};

    {
        Shader shader_program("res/shaders/Simple.shader");
        Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), shader_program, 90.0f, 90.0f, 0.0f);
        FlyCamera flyCamera(window, camera, 0.05);

        camera.position = glm::vec3(4, 5, -4);

        float positions[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        Texture texture("res/textures/banana.jpg");
        texture.Bind();
        shader_program.SetUniform1i("u_Texture", 0);

        Renderer renderer;

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (unsigned int i = 0; i < gridX; i++)
        {
            for (unsigned int j = 0; j < gridY; j++)
            {
                grid[i + j * gridX] = Color {
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f
                };
            }
        }

        window.Update([&camera, &shader_program, &renderer, &vb, &flyCamera, &grid, &window](float deltaTime) {
            renderer.Clear();
            camera.render();
            flyCamera.fly(window, camera);

            shader_program.SetUniform1i("u_UseTexture", false);

            for (unsigned int i = 0; i < gridX; i++)
            {
                for (unsigned int j = 0; j < gridY; j++)
                {

                    i % 2 == j % 2 ? shader_program.SetUniform4f("u_Color", 0.6, 0.8, 0.6, 1.0) : shader_program.SetUniform4f("u_Color", 0.2, 0.6, 0.2, 1.0);

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(1.0f * i, 0.375f, 1.0f * j));
                    model = glm::scale(model, glm::vec3(1.0f, 0.25f, 1.0f));
                    //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                    shader_program.SetUniformMat4("u_Model", model);
                    renderer.Draw3D(vb, 36, shader_program);
                }
            }

            snake(grid, window, deltaTime);

            for (unsigned int i = 0; i < gridX; i++)
            {
                for (unsigned int j = 0; j < gridY; j++)
                {
                    Color color = grid[i + j * gridX];

                    if (color.a == 0) continue;

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(1.0f * i, 1.0f, 1.0f * j));

                    shader_program.SetUniform4f( "u_Color", color.r, color.g, color.b, color.a );

                    shader_program.SetUniformMat4("u_Model", model);
                    renderer.Draw3D(vb, 36, shader_program);
                }
            }
        });
    }

    return 0;
}
