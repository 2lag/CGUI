#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

#define size 500 /* More consistent sizing ratio with normalized values for opengl coords */

/* Function to normalize current mouse X-position to 0-1 range */
double normalizeX(double xpos) { return xpos / size; }
double normalizeY(double ypos) { return ypos / size; }

/* Function to denormalize curr mouse pos to 0-500 range */
double denormalizeX(double xpos) { return xpos * size; }
double denormalizeY(double ypos) { return ypos * size; }

/* Mouse detection */
void mouse_button_callback(GLFWwindow* window, int btn, int action, int mods) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
        system("cls");
        printf("left click @ { x: %0.f, y: %0.f }", xpos, ypos);
		printf("\nnormalized @ { x: %0.2f, y: %0.2f }", normalizeX(xpos), normalizeY(ypos));
    } else if (btn == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		system("cls");
		printf("rite click @ { x: %0.f, y: %0.f }", xpos, ypos);
		printf("\nnormalized @ { x: %0.2f, y: %0.2f }", normalizeX(xpos), normalizeY(ypos));
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Disable resize */
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(size, size, "CGUI", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Load png icon from current .exe dir */
    int imgW, imgH, imgCh;
	unsigned char* data = stbi_load("cgui.png", &imgW, &imgH, &imgCh, 0);
    GLFWimage icon;
    icon.width = imgW;
    icon.height = imgH;
    icon.pixels = data;
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(data);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Register mouse button callback function */
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Background */
        glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.1f);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
		glEnd();


        /* Start defining button here */
        glBegin(GL_QUADS);
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(-0.96f, 0.96f);
        glVertex2f(-0.64f, 0.96f);
        glVertex2f(-0.64f, 0.79f);
        glVertex2f(-0.96f, 0.79f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.4f, 0.4f, 0.9f);
        glVertex2f(-0.95f, 0.95f);
        glVertex2f(-0.65f, 0.95f);
        glVertex2f(-0.65f, 0.8f);
        glVertex2f(-0.95f, 0.8f);
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