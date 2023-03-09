#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "mod.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

/* 
    maybe restart and do classes for everything
    would make everything much cleaner and organized
*/

/* Click detection */
void mouse_button_callback(GLFWwindow* window, int btn, int action, int mods) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        system("cls");
        printf("left  @ { x: %0.0f, y: %0.0f }", xpos, ypos);
    } else if (btn == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        system("cls");
        printf("right @ { x: %0.0f, y: %0.0f }", xpos, ypos);
    }
}

/* Move detection */
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    system("cls");
    printf("move  @ { x: %0.0f, y: %0.0f }", xpos, ypos);
}

struct RGB { int R, G, B; };
void quad(int x, int y, int w, int h, RGB in, bool outline) {
    if (outline)
        quad(x - 2, y - 2, w + 4, h + 4, { 225, 102, 102 }, false);

    glBegin(GL_QUADS);
    glColor3f(mod::nC(in.R), mod::nC(in.G), mod::nC(in.B));
    glVertex2f(mod::n(x), mod::n(y));
    glVertex2f(mod::n(x + w), mod::n(y));
    glVertex2f(mod::n(x + w), mod::n(y + h));
    glVertex2f(mod::n(x), mod::n(y + h));
    glEnd();
}

int main(void)
{
    GLFWwindow* window; // Required
    if (!glfwInit())
        return -1; // Required

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); /* Disable resize */

    /* Create win & ogl context */
    window = glfwCreateWindow(canvaSz, canvaSz, "CGUI", NULL, NULL);
    
    if (!window) {
        glfwTerminate(); // Required
        return -1; // Required
    }

    /* Load png icon from current .exe dir */
    int imgW, imgH, imgCh;
	unsigned char* data = stbi_load("cgui.png", &imgW, &imgH, &imgCh, 0);
    GLFWimage icon = { imgW, imgH, data };
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(data);
    
    glfwMakeContextCurrent(window); // Required

    /* Register mouse movement & button callbacks */
    glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // Required

        /* Background */
        quad(2, 2, canvaSz - 4, canvaSz - 4, { 24, 24, 24 }, true);

        /* Start defining button here 
        swap this to button function
        quad(0, 0, 50, 25, { 61, 61, 61 }, true);
        */

        glfwSwapBuffers(window); // Required
        glfwPollEvents(); // Required
    }
	glfwDestroyWindow(window); // Required
    glfwTerminate(); // Required
    return 0; // Required
}