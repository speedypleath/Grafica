#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "loadShaders.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

GLuint
  VaoId,
  VboId,
  ColorBufferId,
  ProgramId,
  myMatrixLocation,
  matrScaleLocation,
  matrTranslLocation,
  resizeMatrixLocation,
  codColLocation;
 
glm::mat4 myMatrix, resizeMatrix, matrTransl, matrTransl2, matrScale, matrRot; 


int codCol;
float PI=3.141592;
int width=400, height=300;

void CreateVBO(void)
{
  // varfurile 
  GLfloat Vertices[] = {
    // varfuri pentru colturi
    0.0f, 0.0f, 0.0f, 1.0f,
    800.0f, 0.0f, 0.0f, 1.0f,
    800.0f, 600.0f, 0.0f, 1.0f,
    0.0f, 600.0f, 0.0f, 1.0f,
	// varfuri pentru D
    50.0f,  50.0f, 0.0f, 1.0f,
    300.0f, 50.0f, 0.0f, 1.0f,
	300.0f,  300.0f, 0.0f, 1.0f,
	50.0f,  300.0f, 0.0f, 1.0f,
    // varfuri pentru P1
    500.0f,  50.0f, 0.0f, 1.0f,
    700.0f, 100.0f, 0.0f, 1.0f,
	500.0f,  150.0f, 0.0f, 1.0f,
    // varfuri pentru P2
    500.0f,  500.0f, 0.0f, 1.0f,
    700.0f, 300.0f, 0.0f, 1.0f,
	500.0f,  400.0f, 0.0f, 1.0f,
	400.0f,  300.0f, 0.0f, 1.0f,
  };
  // culorile varfurilor din colturi
  GLfloat Colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.0f, 0.5f, 1.0f,
  };
 

  // se creeaza un buffer nou
  glGenBuffers(1, &VboId);
  // este setat ca buffer curent
  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  // punctele sunt "copiate" in bufferul curent
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  
  // se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  // se activeaza lucrul cu atribute; atributul 0 = pozitie
  glEnableVertexAttribArray(0);
  // 
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
 
  // un nou buffer, pentru culoare
  glGenBuffers(1, &ColorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  // atributul 1 =  culoare
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
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
  ProgramId=LoadShaders("../../res/04_tema_0_Shader.vert", "../../res/04_tema_Shader.frag");
  glUseProgram(ProgramId);
}

void CreateShaders1(void)
{
  ProgramId=LoadShaders("../../res/04_tema_1_Shader.vert", "../../res/04_tema_Shader.frag");
  glUseProgram(ProgramId);
}
 
void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
    myMatrix = glm::mat4(1.0f);
	  resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f/width, 1.f/height, 1.0));
    matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0));
	  matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-400.f, -300.f, 0.0));
    matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(150.f, -130.f, 0.0));
    matrRot = glm::rotate(glm::mat4(1.0f), PI/8, glm::vec3(0.0, 0.0, 1.0));


    myMatrix = resizeMatrix * matrTransl;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
    CreateVBO();
}
void RenderFunction(void)
{
  	CreateShaders();
    glClear(GL_COLOR_BUFFER_BIT);

    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);

    codColLocation = glGetUniformLocation(ProgramId, "codCol"); 
    codCol = 0;
    glUniform1i(codColLocation, codCol);
    glDrawArrays(GL_POLYGON, 0, 4);
    
    codCol = 1;
    glUniform1i(codColLocation, codCol);
    glDrawArrays(GL_POLYGON, 4, 4); // D

    codCol = 2;
    glUniform1i(codColLocation, codCol);
    glDrawArrays(GL_POLYGON, 8, 3); // P1
    glDrawArrays(GL_TRIANGLE_FAN, 11, 4); // P2

    // ex 3
    myMatrix = resizeMatrix * matrTransl * matrRot * matrTransl2; 
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
    codCol = 1;
    glUniform1i(codColLocation, codCol);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 3);
    glDrawArrays(GL_TRIANGLE_FAN, 11, 4);
    myMatrix = resizeMatrix * matrTransl * matrScale; 
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
    codCol = 2;
    glUniform1i(codColLocation, codCol);
    glDrawArrays(GL_POLYGON, 4, 4);
    // ex 4
    CreateShaders1();
    resizeMatrixLocation = glGetUniformLocation(ProgramId, "resizeMatrix");
    matrScaleLocation = glGetUniformLocation(ProgramId, "matrScale");
    matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");

    glUniformMatrix4fv(resizeMatrixLocation, 1, GL_TRUE, &resizeMatrix[0][0]);
    glUniformMatrix4fv(matrScaleLocation, 1, GL_TRUE, &matrScale[0][0]);
    glUniformMatrix4fv(matrTranslLocation, 1, GL_TRUE, &matrTransl[0][0]);

    codColLocation = glGetUniformLocation(ProgramId, "codCol"); 
    codCol = 0;
    glUniform1i(codColLocation, codCol);

    glDrawArrays(GL_POLYGON, 4, 4);


    glutSwapBuffers();
    glFlush ( );
}
void Cleanup(void)
{
  DestroyShaders();
  DestroyVBO();
}

int main(int argc, char* argv[])
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowPosition (100,100); 
  glutInitWindowSize(800,600); 
  glutCreateWindow("Laborator 4"); 
  glewInit(); 
  Initialize( );
  glutDisplayFunc(RenderFunction);
  glutCloseFunc(Cleanup);
  glutMainLoop();
  return 0;
  
}

