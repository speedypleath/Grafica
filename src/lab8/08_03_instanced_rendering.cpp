// Functii de desenare in Open GL. Randare instantiata 
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"

using namespace std;

#define INSTANCE_COUNT 40
const GLfloat PI = 3.141592;

// identificatori 
GLuint
ColorBufferId,
ProgramId;
Shader *shader;
glm::mat4 view, projection;
void CreateVBO(void)
{
	GLuint VaoId,
VBPos,
VBCol,
VBModelMat,
EboId;
	// Varfurile 
	GLfloat Vertices[] =
	{
		// punctele din planul z=-50   coordonate                   		
		-50.0f,  -50.0f, -50.0f, 1.0f,
		50.0f,  -50.0f,  -50.0f, 1.0f,
		50.0f,  50.0f,  -50.0f, 1.0f,
		-50.0f,  50.0f, -50.0f, 1.0f,
		// punctele din planul z=+50  coordonate                   		
		-50.0f,  -50.0f, 50.0f, 1.0f,
		50.0f,  -50.0f,  50.0f, 1.0f,
		50.0f,  50.0f,  50.0f, 1.0f,
		-50.0f,  50.0f, 50.0f, 1.0f,
	};

	// Culorile instantelor
	glm::vec4 Colors[INSTANCE_COUNT];
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		Colors[n][0] = 0.35f + 0.30f * (sinf(a + 2.0f) + 1.0f);
		Colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
		Colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
		Colors[n][3] = 1.0f;
	}

	// Matricele instantelor
	glm::mat4 MatModel[INSTANCE_COUNT];
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		MatModel[n] = glm::translate(glm::mat4(1.0f), glm::vec3(80 * n * sin(10.f * n * 180 / PI), 80 * n * cos(10.f * n * 180 / PI), 0.0)) * glm::rotate(glm::mat4(1.0f), n * PI / 8, glm::vec3(n, 2 * n * n, n / 3));
	}

	// indicii pentru varfuri
	GLubyte Indices[] =
	{
	  1, 2, 0,   0, 2, 3,  //  Fata "de jos"
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7, //  Fata "de sus"
	  0, 1, 2, 3,  // Contur fata de jos
	  4, 5, 6, 7,  // Contur fata de sus
	  0, 4,
	  1, 5,
	  2, 6,
	  3, 7
	};


	// generare buffere
	glGenVertexArrays(1, &VaoId);
	glGenBuffers(1, &VBPos);
	glGenBuffers(1, &VBCol);
	glGenBuffers(1, &VBModelMat);
	glGenBuffers(1, &EboId);

	// legarea VAO 
	glBindVertexArray(VaoId);

	// 0: Pozitie
	glBindBuffer(GL_ARRAY_BUFFER, VBPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	// 1: Culoare
	glBindBuffer(GL_ARRAY_BUFFER, VBCol); // legare buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);  // rata cu care are loc distribuirea culorilor per instanta

	// 2..5 (2+i): Matrice de pozitie
	glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MatModel), MatModel, GL_STATIC_DRAW);
	for (int i = 0; i < 4; i++) // Pentru fiecare coloana
	{
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i,              // Location
			4, GL_FLOAT, GL_FALSE,                // vec4
			sizeof(glm::mat4),                    // Stride
			(void*)(sizeof(glm::vec4) * i));      // Start offset
		glVertexAttribDivisor(2 + i, 1);
	}

	// Indicii 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glDeleteBuffers(1, &VBPos);
// 	glDeleteBuffers(1, &VBCol);
// 	glDeleteBuffers(1, &VBModelMat);
// 	glDeleteBuffers(1, &EboId);
// 	glBindVertexArray(0);
// 	glDeleteVertexArrays(1, &VaoId);
}
void CreateShaders(void)
{
	ProgramId = LoadShaders("../../res/08_03_Shader.vert", "../../res/08_03_Shader.frag");
	glUseProgram(ProgramId);
	shader = new Shader("../../res/08_03_Shader.vert", "../../res/08_03_Shader.frag");
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}
void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
	CreateVBO();
	CreateShaders();
}
void RenderFunction(void)
{
	GLuint viewLocation,
projLocation,
codColLocation,
codCol;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // decomentati acest rand daca este cazul 
	// glBindVertexArray(VaoId);
	// glBindBuffer(GL_ARRAY_BUFFER, VBPos);
	// glBindBuffer(GL_ARRAY_BUFFER, VBCol);
	// glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	// matricea de vizualizare
	view = glm::lookAt(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection =  glm::infinitePerspective(3.141592f / 2.0f, GLfloat(800.0f) / GLfloat(600.0f), 1.0f);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	viewLocation = glGetUniformLocation(ProgramId, "viewMatrix");
	projLocation = glGetUniformLocation(ProgramId, "projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// Fetele
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0, INSTANCE_COUNT);
	// Muchiile
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glLineWidth(2.5);
	glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(36), INSTANCE_COUNT);
	glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(40), INSTANCE_COUNT);
	glDrawElementsInstanced(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(44), INSTANCE_COUNT);

    // shader->setInt("codCol", codCol);
    // shader->setMat4("viewMatrix", view);
    // shader->setMat4("projectionMatrix", projection);
    // codCol = 0;
    // glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, INSTANCE_COUNT);

    // codCol = 1;
    // shader->setInt("codCol", codCol);
    // glLineWidth(2.5f);
    // glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(36), INSTANCE_COUNT);
    // glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(40), INSTANCE_COUNT);
    // glDrawElementsInstanced(GL_LINES, 8, GL_UNSIGNED_INT, (void*)(44), INSTANCE_COUNT);


	glutSwapBuffers();
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("<<Instanced rendering>>");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}