#pragma once
#include <GLFW/glfw3.h>

#define canvaSz 500 /* More consistent sizing ratio with normalized values for opengl coords */

struct mod {
public:
	static GLfloat n(double pos) { return pos / canvaSz * 2 - 1; } /* Function to normalize current mouse position to -1 -> 1 range */
	static GLfloat dn(double pos) { return (pos + 1) / 2 * canvaSz; } /* Function to denormalize current mouse pos to 0-500 range */
	static GLfloat nC(int val) { return (double)val / 255; } /* Func to normalize RGB vals */
};