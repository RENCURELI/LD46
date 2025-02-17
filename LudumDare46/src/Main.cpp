#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>
#include <vector>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Player.h"
#include "Ball.h"


#define SPEED 20;


static float Normalize(float min, float max, float val)
{
	float normalized = (val - min) / (max - min);
	
	return normalized;
}

Renderer renderer;

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1280, 720, "LudumDare46", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	Player player1(-1175.0f, 0.0f, 50.0f, 200.0f, 0.0f, 150.f, 230.f);
	Player player2(1175.0f, 0.0f, 50.0f, 200.0f, 0.0f, 150.f, 230.f);

	Ball ball(0.0f, 0.0f, 25.0f, 1, 1, 10.0f, 10);
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR during GLEW Initialization" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		const float positionsA[] =
		{
			Normalize(0.0f, 1280.0f, (player1.x - player1.width / 2)), Normalize(0.0f, 720.0f, (player1.y - player1.height / 2)),
			Normalize(0.0f, 1280.0f, (player1.x + player1.width / 2)), Normalize(0.0f, 720.0f, (player1.y - player1.height / 2)),
			Normalize(0.0f, 1280.0f, (player1.x + player1.width / 2)), Normalize(0.0f, 720.0f, (player1.y + player1.height / 2)),
			Normalize(0.0f, 1280.0f, (player1.x - player1.width / 2)), Normalize(0.0f, 720.0f, (player1.y + player1.height / 2))
		};

		const float positionsB[] =
		{
			Normalize(0.0f, 1280.0f, (player2.x - player2.width / 2)), Normalize(0.0f, 720.0f, (player2.y - player2.height / 2)),
			Normalize(0.0f, 1280.0f, (player2.x + player2.width / 2)), Normalize(0.0f, 720.0f, (player2.y - player2.height / 2)),
			Normalize(0.0f, 1280.0f, (player2.x + player2.width / 2)), Normalize(0.0f, 720.0f, (player2.y + player2.height / 2)),
			Normalize(0.0f, 1280.0f, (player2.x - player2.width / 2)), Normalize(0.0f, 720.0f, (player2.y + player2.height / 2))
		};

		const float positionsC[] =
		{
			Normalize(0.0f, 1280.0f, (ball.x - ball.radius / 2)), Normalize(0.0f, 720.0f, (ball.y - ball.radius / 2)),
			Normalize(0.0f, 1280.0f, (ball.x + ball.radius / 2)), Normalize(0.0f, 720.0f, (ball.y - ball.radius / 2)),
			Normalize(0.0f, 1280.0f, (ball.x + ball.radius / 2)), Normalize(0.0f, 720.0f, (ball.y + ball.radius / 2)),
			Normalize(0.0f, 1280.0f, (ball.x - ball.radius / 2)), Normalize(0.0f, 720.0f, (ball.y + ball.radius / 2))
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		
		VertexBuffer vb(positionsA, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/Shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", Normalize(0.0f, 255.0f, player1.r), Normalize(0.0f, 255.0f, player1.g), Normalize(0.0f, 255.0f, player1.b), 1.0f);


		shader.UnBind();
		vb.UnBind();
		
		while (!glfwWindowShouldClose(window))
		{

			renderer.Clear();

			//Move Player 1
			shader.Bind();
			{
				if (glfwGetKey(window, 87) && player1.fYOffset < 1 - Normalize(0, player1.height, player1.height / 10))
				{
					player1.PlayerMove(Normalize(0, 720, player1.y++) * 20);
				}
				else if (glfwGetKey(window, 83) && player1.fYOffset > -1 - Normalize(0, player1.height, player1.height / 10))
				{
					player1.PlayerMove(Normalize(0, 720, player1.y--) * 20);
				}
				std::vector<float> fNewData(sizeof(positionsA) / sizeof(*positionsA));
				memcpy(&fNewData[0], positionsA, sizeof(positionsA));

				for (int iVertex = 0; iVertex < sizeof(positionsA) / sizeof(*positionsA); iVertex += 2)
				{
					fNewData[iVertex] += player1.fXOffset;
					fNewData[iVertex + 1] += player1.fYOffset;
				}

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionsA), &fNewData[0]);
				va.AddBuffer(vb, layout);
				renderer.Draw(va, ib, shader);
			}

			
			//Move Player2
			shader.Bind();
			{
				if (glfwGetKey(window, 73) && player2.fYOffset < 1 - Normalize(0, player2.height, player2.height / 10))
				{
					player2.PlayerMove(Normalize(0, 720, player2.y++) * 20);
				}
				else if (glfwGetKey(window, 75) && player2.fYOffset > -1 - Normalize(0, player2.height, player2.height / 10))
				{
					player2.PlayerMove(Normalize(0, 720, player2.y--) * 20);
				}
				std::vector<float> fNewData(sizeof(positionsB) / sizeof(*positionsB));
				memcpy(&fNewData[0], positionsB, sizeof(positionsB));

				for (int iVertex = 0; iVertex < sizeof(positionsB) / sizeof(*positionsB); iVertex += 2)
				{
					fNewData[iVertex] += player2.fXOffset;
					fNewData[iVertex + 1] += player2.fYOffset;
				}

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionsB), &fNewData[0]);
				va.AddBuffer(vb, layout);
				renderer.Draw(va, ib, shader);
			}

			//Ball movement
			shader.Bind();
			{
				ball.fYOffset = Normalize(0, 720, ball.y += 1 * ball.multY) * ball.speed;
				ball.fXOffset = Normalize(0, 720, ball.x += 1 * ball.multX) * ball.speed;

				if (ball.fYOffset > 1 - Normalize(0, 720, ball.radius))
					ball.multY *= -1;
				else if (ball.fYOffset < -1 - Normalize(0, 720, ball.radius))
					ball.multY *= -1;

				if (ball.fXOffset > 1 - Normalize(0, 1280, ball.radius) && ball.lives > 0)
				{
					ball.multX *= -1;
					ball.lives--;
					std::cout << ball.lives << " LIVES REMAINING" << std::endl;
				}
				else if (ball.fXOffset < -1 - Normalize(0, 1280, ball.radius) && ball.lives > 0)
				{
					ball.multX *= -1;
					ball.lives--;
					std::cout << ball.lives << " LIVES REMAINING" << std::endl;
				}

				if (ball.fXOffset > Normalize(0, 1280, player2.x) - Normalize(0, 720, player2.width / 2) && ball.fYOffset < player2.fYOffset + Normalize(0, 720, player2.height) && ball.fYOffset > player2.fYOffset - Normalize(0, 720, player2.height))
				{
					ball.multX *= -1;
				}

				if (ball.fXOffset < Normalize(0, 1280, player1.x) + Normalize(0, 720, player1.width / 2) && ball.fYOffset < player1.fYOffset + Normalize(0, 720, player1.height) && ball.fYOffset > player1.fYOffset - Normalize(0, 720, player1.height))
				{
					ball.multX *= -1;
				}

				if ((ball.fXOffset < -1 || ball.fXOffset >= 1) && ball.lives <= 0)
				{
					std::cout << "BALL IS DEAD" << std::endl;
					ball.fYOffset = 0;
					ball.fXOffset = 0;
					ball.x = 0.0f;
					ball.y = 0.0f;
					break;
				}
				
				std::vector<float> fNewData(sizeof(positionsC) / sizeof(*positionsC));
				memcpy(&fNewData[0], positionsC, sizeof(positionsC));

				for (int iVertex = 0; iVertex < sizeof(positionsC) / sizeof(*positionsC); iVertex += 2)
				{
					fNewData[iVertex] += ball.fXOffset;
					fNewData[iVertex + 1] += ball.fYOffset;
				}

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionsC), &fNewData[0]);
				va.AddBuffer(vb, layout);
				renderer.Draw(va, ib, shader);
			}
			
			glDisableVertexAttribArray(0);
			glUseProgram(0);


			glfwSwapBuffers(window);

			glfwPollEvents();

			
		}

	}
	glfwTerminate();
	std::cin.get();
	
	return 0;
}
