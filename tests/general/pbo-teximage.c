/*
 * Copyright © 2009 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

/** @file pbo-teximage.c
 *
 * Tests that using a PBO as the unpack buffer for glTexImage works correctly.
 */

#include "piglit-util-gl.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;

	config.window_visual = PIGLIT_GL_VISUAL_RGB | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

enum piglit_result
piglit_display(void)
{
	GLboolean pass = GL_TRUE;
	static float red[] = {1.0, 0.0, 0.0, 0.0};
	static float green[] = {0.0, 1.0, 0.0, 0.0};
	static float blue[]  = {0.0, 0.0, 1.0, 0.0};
	float *pixels;
	GLuint pbo, tex;

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glGenBuffersARB(1, &pbo);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, pbo);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER, 4 * 4 * sizeof(float),
			NULL, GL_STREAM_DRAW_ARB);
	pixels = glMapBufferARB(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY_ARB);

	memcpy(pixels + 0,  red, sizeof(red));
	memcpy(pixels + 4,  green, sizeof(green));
	memcpy(pixels + 8,  blue, sizeof(blue));
	memcpy(pixels + 12, red, sizeof(red));

	glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0,
		     GL_RGBA,
		     2, 2, 0,
		     GL_RGBA, GL_FLOAT, 0);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, 0);
	glDeleteBuffersARB(1, &pbo);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0, 0.0); glVertex2f(10, 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(20, 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(20, 20);
	glTexCoord2f(0.0, 1.0); glVertex2f(10, 20);
	glEnd();

	glDeleteTextures(1, &tex);

	pass &= piglit_probe_pixel_rgb(12, 12, red);
	pass &= piglit_probe_pixel_rgb(18, 12, green);
	pass &= piglit_probe_pixel_rgb(12, 18, blue);
	pass &= piglit_probe_pixel_rgb(18, 18, red);

	piglit_present_results();

	return pass ? PIGLIT_PASS : PIGLIT_FAIL;
}


static void reshape(int width, int height)
{
	piglit_width = width;
	piglit_height = height;

	piglit_ortho_projection(width, height, GL_FALSE);
}

void
piglit_init(int argc, char **argv)
{
	reshape(piglit_width, piglit_height);
	piglit_require_extension("GL_ARB_pixel_buffer_object");
}
