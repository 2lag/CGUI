#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "mod.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

/* Click detection */
void mouse_button_callback(GLFWwindow* window, int btn, int action, int mods) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
        system("cls");
        printf("left click @ { x: %0.0f, y: %0.0f }", xpos, ypos);
		printf("\nnormalized @ { x: %0.2f, y: %0.2f }", mod::n(xpos), mod::n(ypos));
    } else if (btn == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		system("cls");
		printf("rite click @ { x: %0.0f, y: %0.0f }", xpos, ypos);
		printf("\nnormalized @ { x: %0.2f, y: %0.2f }", mod::n(xpos), mod::n(ypos));
    }
}

/* Move detection */
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    system("cls");
    printf("mouse move @ { x:  %0.0f, y: %0.0f }", xpos, ypos);
    printf("\nnormalized @ { x: %0.2f, y: %0.2f }", mod::n(xpos), mod::n(ypos));
}

struct RGB { int R, G, B; };
void quad(int x, int y, int w, int h, RGB in, bool outline) {
    if (outline) {
        quad(x - 2, y - 2, w + 4, h + 4, { 225, 102, 102 }, false);
    }
    glBegin(GL_QUADS);
    glColor3f(mod::nC(in.R), mod::nC(in.G), mod::nC(in.B));

    /*
    FIX THESE BROKEN ASS VALUES ??? UNLESS IS N() ISSUE BUT THOUGHT I FIXED 
    maybe remove the .5f's since changed the func defs? unsure hmm
    */
    glVertex2f(mod::n(x) + 0.5f, mod::n(y) + 0.5f);
    glVertex2f(mod::n(x + w) + 0.5f, mod::n(y) + 0.5f);
    glVertex2f(mod::n(x + w) + 0.5f, mod::n(y + h) + 0.5f);
    glVertex2f(mod::n(x) + 0.5f, mod::n(y + h) + 0.5f);
    glEnd();
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Disable resize */
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(canvaSz, canvaSz, "CGUI", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Load png icon from current .exe dir */
    int imgW, imgH, imgCh;
	unsigned char* data = stbi_load("cgui.png", &imgW, &imgH, &imgCh, 0);
    GLFWimage icon = { imgW, imgH, data };
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(data);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Register mouse movement & button callbacks */
    glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Background */
        glBegin(GL_QUADS);
        glColor3f(mod::nC(24), mod::nC(24), mod::nC(24));
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
		glEnd();

        /* Start defining button here */
        quad(0, 0, 50, 25, { 61, 61, 61 }, true);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}