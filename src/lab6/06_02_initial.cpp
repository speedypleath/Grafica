/* INDEXARE
Elemente de noutate:
   - folosirea indecsilor: elemente asociate (matrice, buffer)
   - cele 4 functii de desenare (glDrawArrays, glDrawElements, glDrawElementsBaseVertex, glDrawArraysInstanced) */
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
#include "SOIL.h"

using namespace std;

//////////////////////////////////////

GLuint
VaoId,
VboId,
EboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
texture,
viewLocation,
projLocation;


float width = 80.f, height = 60.f;
glm::mat4
myMatrix, 
resizeMatrix=glm::ortho(-width, width, -height, height), 
scaleMatrix=glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 0.5, 1.0)),
translateMatrix=glm::translate(glm::mat4(1.0f), glm::vec3(30.f, 30.f, 0.0)),
projection,
view;

void CreateVBO(void)
{
    // coordonatele varfurilor
    static const GLfloat vf_pos[] =
    {
    -5.0f, -5.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,      0.0f, 0.0f,
     5.0f,  -5.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,      2.0f, 0.0f,
     5.0f,  5.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,      2.0f, 2.0f,
    -5.0f,  5.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,      0.0f, 2.0f,
    };
 

    // indici pentru trasarea unui triunghi
    static const GLuint vf_ind[] =
    {
    0, 1, 2, 2, 3, 0
    };
 
    // se creeaza un buffer nou pentru varfuri
    glGenBuffers(1, &VboId);
    // buffer pentru indici
    glGenBuffers(1, &EboId);
    // se creeaza / se leaga un VAO (Vertex Array Object)
    glGenVertexArrays(1, &VaoId);
 
    // legare VAO
    glBindVertexArray(VaoId);

    // buffer-ul va contine atat coordonatele varfurilor, cat si datele de culoare
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // punctele sunt "copiate" in bufferul curent
    glBufferData(GL_ARRAY_BUFFER, sizeof(vf_pos), vf_pos, GL_STATIC_DRAW);

    // buffer-ul pentru indici
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vf_ind), vf_ind, GL_STATIC_DRAW);

    // se activeaza lucrul cu atribute; atributul 0 = pozitie
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

    // se activeaza lucrul cu atribute; atributul 1 = culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
 
    // se activeaza lucrul cu atribute; atributul 2 = coordonate de texturare
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}
void DestroyVBO(void)
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &EboId);
    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
 
}

void LoadTexture(void)
{

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height;
    unsigned char* image = SOIL_load_image("../../res/text_smiley_face.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void CreateShaders(void)
{
    ProgramId = LoadShaders("../../res/06_02_Shader.vert", "../../res/06_02_Shader.frag");
    glUseProgram(ProgramId);
}

void CreateShadersTexture(void)
{
    ProgramId = LoadShaders("../../res/06_04_Shader.vert", "../../res/06_04_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
}
void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Creare VBO
    CreateVBO();
    CreateShaders();
    // Transmitere variabile uniforme
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    // Desenare
    myMatrix = resizeMatrix;
    myMatrix = myMatrix * translateMatrix * scaleMatrix;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));

    LoadTexture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    CreateShadersTexture();
    // se schimba pozitia observatorului
    glm::vec3 Obs = glm::vec3(0, 0, 100);

    // pozitia punctului de referinta
    glm::vec3 PctRef = glm::vec3(0, 0, -1.0f);

    // verticala din planul de vizualizare 

    glm::vec3 Vert = glm::vec3(0, 1.0f, 0.0f);


    view = glm::lookAt(Obs, PctRef, Vert);

    projection = glm::perspective(45.0f, GLfloat(width) / GLfloat(height), 0.0f, 1000.0f);

    myMatrix = glm::mat4(1.0f);
    myMatrix = myMatrix * scaleMatrix * translateMatrix;
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    viewLocation = glGetUniformLocation(ProgramId, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    projLocation = glGetUniformLocation(ProgramId, "projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));

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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Utilizarea indexarii varfurilor");
    glewInit();
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);

    glutMainLoop();


}

