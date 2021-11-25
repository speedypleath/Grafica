/*
- Folosirea indexarii pentru a trasa separata fetele si muchiile unui obiect 3D (cub)
- Rolul testului de adancime
*/
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
using namespace std;

// identificatori 
GLuint
VaoId,
VboId,
EboId,
ProgramId,
viewLocation,
projLocation,
codColLocation;
float PI=3.141592;
// variabile
int codCol;

// elemente pentru matricea de vizualizare
float Obsx = 0.0, Obsy = 0.0, Obsz = -300.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;

// elemente pentru matricea de proiectie
float width = 800, height = 600, xwmin = -200.f, xwmax = 200, ywmin = -200, ywmax = 200, znear = 100, zfar = -100, fov = 30;

// vectori
glm::vec3 Obs, PctRef, Vert;

// matrice utilizate
glm::mat4 view, projection;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		Vx += 0.1;
		break;
	case 'r':
		Vx -= 0.1;
		break;
	case '+':
		Obsz += 10;
		break;
	case '-':
		Obsz -= 10;
		break;
	}
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Obsx -= 20;
		break;
	case GLUT_KEY_RIGHT:
		Obsx += 20;
		break;
	case GLUT_KEY_UP:
		Obsy += 20;
		break;
	case GLUT_KEY_DOWN:
		Obsy -= 20;
		break;
	}
}
void CreateVBO(void)
{
	// indicii pentru varfuri
	GLuint Indices[397];
    GLfloat Vertices[700];
    int k=0;
    for(int i=0; i<99; i++){
        float theta = 2 * PI * i / 99;
        Vertices[k++] = cos(theta) * 100.0f;
        Vertices[k++] = sin(theta) * 100.0f;
        Vertices[k++] = 50.0f;
        Vertices[k++] = 1.0f;
		Vertices[k++] = 0.0f;
        Vertices[k++] = 1.0f;
        Vertices[k++] = 0.0f;
    }
	Vertices[k++] = 0.0f;
	Vertices[k++] = 0.0f;
	Vertices[k++] = -50.0f;
	Vertices[k++] = 1.0f;
	Vertices[k++] = 1.0f;
	Vertices[k++] = 0.0f;
	Vertices[k++] = 0.0f;
	for(int i=0; i<99; i++){
		Indices[i] = i; // baza
	}
	Indices[99] = 0;
	k = 100;
	for(int i=0; i<98; i++){
		Indices[k++] = 99;
		Indices[k++] = i;
		Indices[k++] = i+1;
	}
	Indices[k++] = 99;
	Indices[k++] = 98;
	Indices[k++] = 0;
	// generare VAO/buffere
	glGenBuffers(1, &VboId); // atribute
	glGenBuffers(1, &EboId); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // "copiere" in bufferul curent
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW); // "copiere" indici in bufferul curent

	// se activeaza lucrul cu atribute; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);
}
void CreateShaders(void)
{
	ProgramId = LoadShaders("../../res/08_01_Shader.vert", "../../res/08_01_Shader.frag");
	glUseProgram(ProgramId);
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}
void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului

	// Creare VBO+shader
	CreateVBO();
	CreateShaders();

	// Locatii ptr shader
	viewLocation = glGetUniformLocation(ProgramId, "viewShader");
	projLocation = glGetUniformLocation(ProgramId, "projectionShader");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // decomentati acest rand daca este cazul 
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	// matricea de vizualizare
	Obs = glm::vec3(Obsx, Obsy, Obsz); // pozitia observatorului	
	Refx = Obsx; Refy = Obsy;
	PctRef = glm::vec3(Refx, Refy, 800.0f); // pozitia punctului de referinta
	Vert = glm::vec3(Vx, 1.0f, 0.0f); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie, pot fi testate si alte variante
	projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
	// projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, 80.f, -80.f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// Fetele
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_POLYGON, 100, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 297, GL_UNSIGNED_INT, (GLvoid*)(100 * sizeof(GLuint)));
	//Muchiile
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glLineWidth(5.0f);
	glDrawElements(GL_LINE_LOOP, 100, GL_UNSIGNED_INT, 0);

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Desenarea unui cub folosind testul de adancime");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

