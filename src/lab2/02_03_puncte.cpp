#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"

GLuint
  VaoId,
  VboId,
  ColorBufferId,
  ProgramId;

void CreateVBO(void)
{
  // varfurile 
  GLint Vertices[] = {
    20, 20, 0, 0,
    21, 21, 0, 0,
    22, 22, 0, 0,
  };

  // culorile, ca atribute ale varfurilor
  GLint Colors[] = {
    0, 0, 255, 0,
  };
 

  // se creeaza un buffer nou
  glGenBuffers(1, &VboId);
  // este setat ca buffer curent
  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  // varfurile sunt "copiate" in bufferul curent
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  
  // se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  // se activeaza lucrul cu atribute; atributul 0 = pozitie
  glEnableVertexAttribArray(0);
  // 
  glVertexAttribPointer(0, 4, GL_INT, GL_FALSE, 0, 0);
 
  // un nou buffer, pentru culoare
  glGenBuffers(1, &ColorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  // atributul 1 =  culoare
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_INT, GL_FALSE, 0, 0);
  
 }
 void DestroyVBO(void)
{
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &ColorBufferId);
  glDeleteBuffers(1, &VboId);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
  ProgramId=LoadShaders("../../res/02_01_Shader.vert", "../../res/02_01_Shader.frag");
  glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
  glDeleteProgram(ProgramId);
}
 
void init (void)  // initializare fereastra de vizualizare
{
	glClearColor (1.0, 1.0, 1.0, 0.0); // precizeaza culoarea de fond a ferestrei de vizualizare
  CreateVBO();
  CreateShaders();
  glMatrixMode (GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D (0.0, 1200.0, 0.0, 700.0); // su
}


void desen (void) // procedura desenare  
{
    glClear(GL_COLOR_BUFFER_BIT);


    glPointSize(25.0);
    glEnable(GL_POINT_SMOOTH);   
    glDrawArrays(GL_POINTS, 0, 16);

    
    glDisable(GL_POINT_SMOOTH);
    glFlush ( );
	// glColor3f (0.0, 0.0, 1.0); // culoarea punctelor: albastru
	// {
	// 	 glPointSize (1.0);
	// 	glBegin (GL_POINTS); // reprezinta puncte
	// 	glVertex2i (20, 20);
	// 	glVertex2i (21, 21);
	// 	glVertex2i (22, 22);
	// 	glVertex2i (23, 23);
	// 	glVertex2i (24, 24);
	// 	glVertex2i (27, 27);
	// 	glVertex2i (100, 100);
	// 	glEnd ( );
		
	// }
 
    
// 	glColor3d (0.0, 0.05, 0.05);
// 	// glPointSize (6.0);
// 	glBegin (GL_POINTS);
// 	   glVertex2i (100, 400);
// 	   glColor3f (1.0, 0.0, 0.5);
// 	   glVertex2i (300, 500);
//     glEnd ( );
	

//    glColor3f (1.0, 0.0, 0.0); // culoarea liniei: rosu
//        // reprezinta o linie franta
//      //  glLineWidth (2.0);
// 	  //  glEnable (GL_LINE_STIPPLE);
// 	 //  glLineStipple (1, 0x1EED);
// 	   glBegin (GL_LINE_STRIP); 
//        glVertex2i (0,100);
// 	   glVertex2i (400, 500);
//    glEnd ( );
//   // glDisable (GL_LINE_STIPPLE);

//     glColor3f (0.5, 0.0, 1.0);  
// 	//	   glLineWidth (6.0);
//        glBegin (GL_LINES); // reprezinta o reuniune de segmente
//        glVertex2i (400,400);
// 	   glVertex2i (600, 500);
// 	   glVertex2i (700, 520);
// 	   glVertex2i (655, 690);
//    glEnd ( );

 
//    glFlush ( ); // proceseaza procedurile OpenGL cat mai rapid
}

void Cleanup(void)
{
  DestroyShaders();
  DestroyVBO();
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv); // initializare GLUT
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // se utilizeaza un singur buffer | modul de colorare RedGreenBlue (= default)
	glutInitWindowPosition (100, 100); // pozitia initiala a ferestrei de vizualizare (in coordonate ecran)
	glutInitWindowSize (600, 350); // dimensiunile ferestrei 
	glutCreateWindow ("Puncte & Segmente"); // creeaza fereastra, indicand numele ferestrei de vizualizare - apare in partea superioara
  glewInit();
	init (); // executa procedura de initializare
	glClear (GL_COLOR_BUFFER_BIT); // reprezentare si colorare fereastra de vizualizare
	glutDisplayFunc (desen); // procedura desen este invocata ori de cate ori este nevoie
  glutCloseFunc(Cleanup);
	glutMainLoop ( ); // ultima instructiune a programului, asteapta (eventuale) noi date de intrare
	return 0; 
}