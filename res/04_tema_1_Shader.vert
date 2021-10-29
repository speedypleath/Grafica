 
 #version 400


in vec4 in_Position;
in vec4 in_Color;

out vec4 gl_Position; 
out vec4 ex_Color;
uniform mat4 resizeMatrix;
uniform mat4 matrScale;
uniform mat4 matrTransl;


void main(void)
{
    gl_Position = (resizeMatrix * matrTransl * matrScale) * in_Position;
    ex_Color = in_Color;
} 
 