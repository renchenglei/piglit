/* [config]
 * expect_result: pass
 * glsl_version: 1.20
 * [end config]
 *
 * From page 35 (page 41 of the PDF) of the GLSL 1.20 spec:
 *
 *     "The equality operators and assignment operator are only allowed if the
 *     two operands are same size and type....Both array operands must be
 *     explicitly sized. When using the equality operators ... two arrays are
 *     equal if and only if all the elements are element-wise equal.
 */
#version 120

uniform vec4 a[2];
uniform vec4 b[2];

void main()
{
  gl_Position = vec4(a == b);
}
