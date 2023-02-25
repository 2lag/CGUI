#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

#define size 666

//mouse detection
void mouse_button_callback(GLFWwindow* window, int btn, int action, int mods) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
        system("cls");
        printf("left click @ { x: %0.f, y: %0.f }", xpos, ypos);
    } else if (btn == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		system("cls");
		printf("rite click @ { x: %0.f, y: %0.f }", xpos, ypos);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(size, size, "CGUI", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // load in png icon
    int imgW, imgH, imgCh;
	unsigned char* data = stbi_load("cgui.png", &imgW, &imgH, &imgCh, 0);
    GLFWimage icon;
    icon.width = imgW;
    icon.height = imgH;
    icon.pixels = data;
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(data); //free img data
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Register mouse button callback function */
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.1f);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
		glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.4f, 0.4f, 0.9f);
        glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}