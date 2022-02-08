// Survolarea unei sfere.
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
TRUNK_VaoId,
CROWN_VaoId,
TRUNK_VboId,
TRUNK_EboId,
CROWN_VboId,
CROWN_EboId,
ProgramId,
viewLocation,
projLocation,
codColLocation;

float const PI = 3.141592f;
// Elemente pentru reprezentarea suprafetei trunchiului
// (1) intervalele pentru parametrii considerati (u si v)
float const TRUNK_U_MIN = -PI, TRUNK_U_MAX = PI, TRUNK_V_MIN = - 60 * PI, TRUNK_V_MAX = 20 * PI;
// (2) numarul de paralele/meridiane, de fapt numarul de valori ptr parametri
int const TRUNK_NR_PARR = 10, TRUNK_NR_MERID = 20;
// (3) pasul cu care vom incrementa u, respectiv v
float TRUNK_step_u = (TRUNK_U_MAX - TRUNK_U_MIN) / TRUNK_NR_PARR, TRUNK_step_v = (TRUNK_V_MAX - TRUNK_V_MIN) / TRUNK_NR_MERID;

// Elemente pentru reprezentarea suprafetei coroanei
// (1) intervalele pentru parametrii considerati (u si v)
float const CROWN_U_MIN = -PI / 2, CROWN_U_MAX = PI / 2, CROWN_V_MIN = 0, CROWN_V_MAX = 2 * PI;
// (2) numarul de paralele/meridiane, de fapt numarul de valori ptr parametri
int const CROWN_NR_PARR = 10, CROWN_NR_MERID = 20;
// (3) pasul cu care vom incrementa u, respectiv v
float CROWN_step_u = (CROWN_U_MAX - CROWN_U_MIN) / CROWN_NR_PARR, CROWN_step_v = (CROWN_V_MAX - CROWN_V_MIN) / CROWN_NR_MERID;

// alte variabile
int codCol;
float radius = 50;
int _index, index_aux;

// variabile pentru matricea de vizualizare
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = 0.0f, beta = 0.0f, dist = 300.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0f, Vy = 0.0f, Vz = -1.0f;
float incr_alpha1 = 0.01f, incr_alpha2 = 0.01f;

// variabile pentru matricea de proiectie
float width = 800, height = 600, znear = 1, fov = 30;

// pentru fereastra de vizualizare 
GLint winWidth = 1000, winHeight = 600;

// vectori
glm::vec3 Obs, PctRef, Vert;

// matrice utilizate
glm::mat4 view, projection;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		dist -= 5.0;
		break;
	case '+':
		dist += 5.0;
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
		beta -= 0.01f;
		break;
	case GLUT_KEY_RIGHT:
		beta += 0.01f;
		break;
	case GLUT_KEY_UP:
		alpha += incr_alpha1;
		if (abs(alpha - PI / 2) < 0.05)
		{
			incr_alpha1 = 0.f;
		}
		else
		{
			incr_alpha1 = 0.01f;
		}
		break;
	case GLUT_KEY_DOWN:
		alpha -= incr_alpha2;
		if (abs(alpha + PI / 2) < 0.05)
		{
			incr_alpha2 = 0.f;
		}
		else
		{
			incr_alpha2 = 0.01f;
		}
		break;
	}
}
void CreateVBO(void)
{
	// varfurile 
	// (4) Matricele pentru varfuri, culori, indici
	glm::vec4 TRUNK_Vertices[(TRUNK_NR_PARR + 1) * TRUNK_NR_MERID];
	glm::vec3 TRUNK_Colors[(TRUNK_NR_PARR + 1) * TRUNK_NR_MERID];
	GLushort TRUNK_Indices[2 * (TRUNK_NR_PARR + 1) * TRUNK_NR_MERID + 4 * (TRUNK_NR_PARR + 1) * TRUNK_NR_MERID];
	for (int merid = 0; merid < TRUNK_NR_MERID; merid++)
	{
		for (int parr = 0; parr < TRUNK_NR_PARR + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			float u = TRUNK_U_MIN + parr * TRUNK_step_u; // valori pentru u si v
			float v = TRUNK_V_MIN + merid * TRUNK_step_v;
			float x_vf = radius * cosf(u); // coordonatele varfului corespunzator lui (u,v)
			float y_vf = radius * sinf(u);
			float z_vf = v;
			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			_index = merid * (TRUNK_NR_PARR + 1) + parr;
			TRUNK_Vertices[_index] = glm::vec4(x_vf, y_vf, z_vf, 1.0);
			TRUNK_Colors[_index] = glm::vec3(0.6f, 0.3f, 0.0f);
			TRUNK_Indices[_index] = _index;
			// indice ptr acelasi varf la parcurgerea paralelelor
			index_aux = parr * (TRUNK_NR_MERID)+merid;
			TRUNK_Indices[(TRUNK_NR_PARR + 1) * TRUNK_NR_MERID + index_aux] = _index;
			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (TRUNK_NR_PARR + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				int AUX = 2 * (TRUNK_NR_PARR + 1) * TRUNK_NR_MERID;
				int index1 = _index; // varful v considerat
				int index2 = _index + (TRUNK_NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = _index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == TRUNK_NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = index2 % (TRUNK_NR_PARR + 1);
					index3 = index3 % (TRUNK_NR_PARR + 1);
				}
				TRUNK_Indices[AUX + 4 * _index] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				TRUNK_Indices[AUX + 4 * _index + 1] = index2;
				TRUNK_Indices[AUX + 4 * _index + 2] = index3;
				TRUNK_Indices[AUX + 4 * _index + 3] = index4;
			}
		}
	};

	// (4) Matricele pentru varfuri, culori, indici
	glm::vec4 CROWN_Vertices[(CROWN_NR_PARR + 1) * CROWN_NR_MERID];
	glm::vec3 CROWN_Colors[(CROWN_NR_PARR + 1) * CROWN_NR_MERID];
	GLushort CROWN_Indices[2 * (CROWN_NR_PARR + 1) * CROWN_NR_MERID + 4 * (CROWN_NR_PARR + 1) * CROWN_NR_MERID];
	for (int merid = 0; merid < CROWN_NR_MERID; merid++)
	{
		for (int parr = 0; parr < CROWN_NR_PARR + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			float u = CROWN_U_MIN + parr * CROWN_step_u; // valori pentru u si v
			float v = CROWN_V_MIN + merid * CROWN_step_v;
			float x_vf = 2 * radius * cosf(u) * cosf(v); // coordonatele varfului corespunzator lui (u,v)
			float y_vf = 2 * radius * cosf(u) * sinf(v);
			float z_vf = 100 + 2 * radius * sinf(u);
			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			_index = merid * (CROWN_NR_PARR + 1) + parr;
			CROWN_Vertices[_index] = glm::vec4(x_vf, y_vf, z_vf, 1.0);
			CROWN_Colors[_index] = glm::vec3(0.0f, 0.4f, 0.0f);
			CROWN_Indices[_index] = _index;
			// indice ptr acelasi varf la parcurgerea paralelelor
			index_aux = parr * (CROWN_NR_MERID)+merid;
			CROWN_Indices[(CROWN_NR_PARR + 1) * CROWN_NR_MERID + index_aux] = _index;
			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (CROWN_NR_PARR + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				int AUX = 2 * (CROWN_NR_PARR + 1) * CROWN_NR_MERID;
				int index1 = _index; // varful v considerat
				int index2 = _index + (CROWN_NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = _index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == CROWN_NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = index2 % (CROWN_NR_PARR + 1);
					index3 = index3 % (CROWN_NR_PARR + 1);
				}
				CROWN_Indices[AUX + 4 * _index] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				CROWN_Indices[AUX + 4 * _index + 1] = index2;
				CROWN_Indices[AUX + 4 * _index + 2] = index3;
				CROWN_Indices[AUX + 4 * _index + 3] = index4;
			}
		}
	};
    
    //vertex setup
    glGenVertexArrays(1, &TRUNK_VaoId);
    glBindVertexArray(TRUNK_VaoId);

	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glEnableVertexAttribArray(1); // atributul 1 = culoare

    // generare buffere
	glGenBuffers(1, &TRUNK_VboId); // atribute
	glGenBuffers(1, &TRUNK_EboId); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, TRUNK_VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TRUNK_EboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TRUNK_Vertices) + sizeof(TRUNK_Colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TRUNK_Vertices), TRUNK_Vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(TRUNK_Vertices), sizeof(TRUNK_Colors), TRUNK_Colors);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TRUNK_Indices), TRUNK_Indices, GL_STATIC_DRAW);

	// atributele; 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)sizeof(TRUNK_Vertices));

    //vertex setup
    glGenVertexArrays(1, &CROWN_VaoId);
    glBindVertexArray(CROWN_VaoId);

	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glEnableVertexAttribArray(1); // atributul 1 = culoare

    // generare buffere
	glGenBuffers(1, &CROWN_VboId); // atribute
	glGenBuffers(1, &CROWN_EboId); // indici

    // legare+"incarcare" buffer
    glBindBuffer(GL_ARRAY_BUFFER, CROWN_VboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CROWN_EboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CROWN_Vertices) + sizeof(CROWN_Colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CROWN_Vertices), CROWN_Vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(CROWN_Vertices), sizeof(CROWN_Colors), CROWN_Colors);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CROWN_Indices), CROWN_Indices, GL_STATIC_DRAW);

    // atributele; 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)sizeof(CROWN_Vertices));
}

void bindTrunk()
{
    glBindVertexArray(TRUNK_VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, TRUNK_VboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TRUNK_EboId);
}

void bindCrown()
{
    glBindVertexArray(CROWN_VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, CROWN_VboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CROWN_EboId);
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &TRUNK_VboId);
	glDeleteBuffers(1, &TRUNK_EboId);
    glDeleteBuffers(1, &CROWN_VboId);
	glDeleteBuffers(1, &CROWN_EboId);
}
void CreateShaders(void)
{
	ProgramId = LoadShaders("../../res/09_03_Shader.vert", "../../res/09_03_Shader.frag");
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
void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
	width = winWidth / 10, height = winHeight / 10;
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//pozitia observatorului
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);

	// reperul de vizualizare
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie 
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

    bindTrunk();

	// (5) desenarea punctelor/muchiilor/fetelor

	// desenarea punctelor
	// glPointSize(3.0);
	// glDrawArrays(GL_POINTS, 0, NR_PARR);

	// desenarea fetelor
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (TRUNK_NR_PARR + 1) * TRUNK_NR_MERID; patr++)
	{
		if ((patr + 1) % (TRUNK_NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*)((2 * (TRUNK_NR_PARR + 1) * (TRUNK_NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	// // desenarea muchiilor - meridiane/paralele
	// codCol = 1;
	// glUniform1i(codColLocation, codCol);
	// glLineWidth(2.0);
	// glDrawElements(GL_LINE_STRIP, (TRUNK_NR_PARR + 1) * (TRUNK_NR_MERID), GL_UNSIGNED_SHORT, 0); // meridiane
	// for (int parr = 1; parr < TRUNK_NR_PARR; parr++) // fiecare cerc paralel separat
	// {
	// 	glDrawElements(GL_LINE_LOOP, (TRUNK_NR_MERID), GL_UNSIGNED_SHORT, (GLvoid*)(((TRUNK_NR_PARR + 1) * (TRUNK_NR_MERID)+parr * TRUNK_NR_MERID) * sizeof(GLushort)));
	// }

    bindCrown();

	// (5) desenarea punctelor/muchiilor/fetelor

	// desenarea punctelor
	// glPointSize(3.0);
	// glDrawArrays(GL_POINTS, 0, NR_PARR);

	// desenarea fetelor
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (CROWN_NR_PARR + 1) * CROWN_NR_MERID; patr++)
	{
		if ((patr + 1) % (CROWN_NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*)((2 * (CROWN_NR_PARR + 1) * (CROWN_NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	// // desenarea muchiilor - meridiane/paralele
	// codCol = 1;
	// glUniform1i(codColLocation, codCol);
	// glLineWidth(2.0);
	// glDrawElements(GL_LINE_STRIP, (CROWN_NR_PARR + 1) * (CROWN_NR_MERID), GL_UNSIGNED_SHORT, 0); // meridiane
	// for (int parr = 1; parr < CROWN_NR_PARR; parr++) // fiecare cerc paralel separat
	// {
	// 	glDrawElements(GL_LINE_LOOP, (CROWN_NR_MERID), GL_UNSIGNED_SHORT, (GLvoid*)(((CROWN_NR_PARR + 1) * (CROWN_NR_MERID)+parr * CROWN_NR_MERID) * sizeof(GLushort)));
	// }

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
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Desenarea si survolarea unei sfere");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshapeFcn);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

