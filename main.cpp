#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include "tekstura.h"

//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective


/*******************************  ustawianie parametrów itp.  ***************************************/

//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX=0.0;
double kameraZ = 20.0;
double kameraD = -60;
double kameraPredkosc;
double kameraKat = -180;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;
double z2;
double x2;


double rotation = 0;

//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy

glm::vec3 lightPos(-45.0f, 11.0f, 20.0f);

GLuint objectColor_id = 0;
GLuint lightColor_id = 0;
GLuint lightPos_id = 0;
GLuint viewPos_id = 0;


/******************************* zmienne  ***************************************/


//shaders
GLuint programID = 0;
GLuint lamp_ID = 0;

//zmienne rysowania do scian
unsigned int lightVAO;
unsigned int  cubeVAO[5], VBO, VBO2, VBO3, VBO4, VBO5;
unsigned int podVAO, VBOpod;

//zmienne do labiryntu
int szer, wys; //wymiary labiryntu
char p; //do pomijania 
char** sciany;

//zmienne skarbow
int liczba_skarbow = 0;
int* skarby;
int k;

int n_v_1, n_el_1, n_v_2, n_el_2, n_v_3, n_el_3;
float* vertices_1, * vertices_2, * vertices_3;
float* normals_1, * normals_2, * normals_3;
GLuint* elements_1, * elements_2, * elements_3;

unsigned int VBO_skarb[3], VAO_skarb[3], VBO_normals[3], ebo_skarb[3];



/*******************************  wierzcholki scian  ***************************************/

float t = 2.5f; //długosc sciany
float w = 4.0f; //wysokosc sciany

float s1[] = {
	//A
	1.0f, 0.0f, -t,		 0.0f, 0.0f, -1.0f,
	-1.0f, 0.0f, -t,		 0.0f, 0.0f, -1.0f,
	-1.0f, w, -t,		 0.0f, 0.0f, -1.0f,
	1.0f, w, -t,		 0.0f, 0.0f, -1.0f,
	//B
	1.0f, 0.0f, -1.0f,		 0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, -1.0f,		 0.0f, 0.0f, 1.0f,
	-1.0f, w, -1.0f,		 0.0f, 0.0f, 1.0f,
	1.0f, w, -1.0f,		 0.0f, 0.0f, 1.0f,
	//C
	-1.0f, 0.0f, -t,		 -1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, w, -1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, w, -t,		 -1.0f, 0.0f, 0.0f,
	//D
	1.0f, 0.0f, -t,		 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, w, -1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, w, -t,		 1.0f, 0.0f, 0.0f,
	//E
	1.0f, w, -t,		 0.0f, 1.0f, 0.0f,
	-1.0f, w, -t,		 0.0f, 1.0f, 0.0f,
	-1.0f, w, -1.0f,		 0.0f, 1.0f, 0.0f,
	1.0f, w, -1.0f,		 0.0f, 1.0f, 0.0f,
	//F
	1.0f, 0.0f, -t,		 0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, -t,		 0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		 0.0f, -1.0f, 0.0f,
};

float s2[] = {
	//A 
	1.0f, 0.0f, t,		 0.0f, 0.0f, -1.0f,
	-1.0f, 0.0f, t,		 0.0f, 0.0f, -1.0f,
	-1.0f, w, t,		 0.0f, 0.0f, -1.0f,
	1.0f, w, t,		 0.0f, 0.0f, -1.0f,
	//B
	1.0f, 0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,
	-1.0f, w, 1.0f,		 0.0f, 0.0f, 1.0f,
	1.0f, w, 1.0f,		 0.0f, 0.0f, 1.0f,
	//C
	-1.0f, 0.0f, t,		 -1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, w, 1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, w, t,		 -1.0f, 0.0f, 0.0f,
	//D
	1.0f, 0.0f, t,		 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, w, 1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, w, t,		 1.0f, 0.0f, 0.0f,
	//E
	1.0f, w, t,		 0.0f, 1.0f, 0.0f,
	-1.0f, w, t,		 0.0f, 1.0f, 0.0f,
	-1.0f, w, 1.0f,		 0.0f, 1.0f, 0.0f,
	1.0f, w, 1.0f,		 0.0f, 1.0f, 0.0f,
	//F
	1.0f, 0.0f, t,		 0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, t,		 0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, 1.0f,		 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		 0.0f, -1.0f, 0.0f,
};


float s3[] = {
 -t, 0.0f, 1.0f,			 -1.0f, 0.0f, 0.0f,
 -t, 0.0f, -1.0f,		 -1.0f, 0.0f, 0.0f,
 -t, w, -1.0f,		 -1.0f, 0.0f, 0.0f,
 -t, w, 1.0f,			 -1.0f, 0.0f, 0.0f,

 -1.0f, 0.0f, 1.0f,			 1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, -1.0f,		 1.0f, 0.0f, 0.0f,
 -1.0f, w, -1.0f,		 1.0f, 0.0f, 0.0f,
 -1.0f, w, 1.0f,			 1.0f, 0.0f, 0.0f,

 -t, 0.0f, -1.0f,		 0.0f, 0.0f, -1.0f,
 -1.0f, 0.0f, -1.0f,		 0.0f, 0.0f, -1.0f,
 -1.0f, w, -1.0f,		 0.0f, 0.0f, -1.0f,
 -t, w, -1.0f,		 0.0f, 0.0f, -1.0f,

 -t, 0.0f, 1.0f,			 0.0f, 0.0f, 1.0f,
 -1.0f, 0.0f, 1.0f,			 0.0f, 0.0f, 1.0f,
 -1.0f, w, 1.0f,			 0.0f, 0.0f, 1.0f,
 -t, w, 1.0f,			 0.0f, 0.0f, 1.0f,

 -t, w, 1.0f,			 0.0f, 1.0f, 0.0f,
 -t, w, -1.0f,		 0.0f, 1.0f, 0.0f,
 -1.0f, w, -1.0f,		 0.0f, 1.0f, 0.0f,
 -1.0f, w, 1.0f,			 0.0f, 1.0f, 0.0f,

 -t, 0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
 -t, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,
 -1.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,
 -1.0f, 0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
};

float s4[] = {
 t, 0.0f, 1.0f,			 -1.0f, 0.0f, 0.0f,
 t, 0.0f, -1.0f,			 -1.0f, 0.0f, 0.0f,
 t, w, -1.0f,			 -1.0f, 0.0f, 0.0f,
 t, w, 1.0f,			 -1.0f, 0.0f, 0.0f,

 1.0f, 0.0f, 1.0f,			 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, -1.0f,			 1.0f, 0.0f, 0.0f,
 1.0f, w, -1.0f,			 1.0f, 0.0f, 0.0f,
 1.0f, w, 1.0f,			 1.0f, 0.0f, 0.0f,

 t, 0.0f, -1.0f,			 0.0f, 0.0f, -1.0f,
 1.0f, 0.0f, -1.0f,			 0.0f, 0.0f, -1.0f,
 1.0f, w, -1.0f,			 0.0f, 0.0f, -1.0f,
 t, w, -1.0f,			 0.0f, 0.0f, -1.0f,

 t, 0.0f, 1.0f,			 0.0f, 0.0f, 1.0f,
 1.0f, 0.0f, 1.0f,			 0.0f, 0.0f, 1.0f,
 1.0f, w, 1.0f,			 0.0f, 0.0f, 1.0f,
 t, w, 1.0f,			 0.0f, 0.0f, 1.0f,

 t, w, 1.0f,			 0.0f, 1.0f, 0.0f,
 t, w, -1.0f,			 0.0f, 1.0f, 0.0f,
 1.0f, w, -1.0f,			 0.0f, 1.0f, 0.0f,
 1.0f, w, 1.0f,			 0.0f, 1.0f, 0.0f,

 t, 0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
 t, 0.0f, -1.0f,			0.0f, -1.0f, 0.0f,
 1.0f, 0.0f, -1.0f,			0.0f, -1.0f, 0.0f,
 1.0f, 0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
};

//slupek
float s0[] = {
	//a
	1.0f, 0.0f, -1.0f,		 0.0f, 0.0f, -1.0f,
	-1.0f, 0.0f, -1.0f,		 0.0f, 0.0f, -1.0f,
	-1.0f, w, -1.0f,		 0.0f, 0.0f, -1.0f,
	1.0f, w, -1.0f,		 0.0f, 0.0f, -1.0f,
	//b
	1.0f, 0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,
	-1.0f, w, 1.0f,		 0.0f, 0.0f, 1.0f,
	1.0f, w, 1.0f,		 0.0f, 0.0f, 1.0f,
	//c
	-1.0f, 0.0f, -1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, w, 1.0f,		 -1.0f, 0.0f, 0.0f,
	-1.0f, w, -1.0f,		 -1.0f, 0.0f, 0.0f,
	//d
	1.0f, 0.0f, -1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, w, 1.0f,		 1.0f, 0.0f, 0.0f,
	1.0f, w, -1.0f,		 1.0f, 0.0f, 0.0f,
	//e
	1.0f, w, -1.0f,		 0.0f, 1.0f, 0.0f,
	-1.0f, w, -1.0f,		 0.0f, 1.0f, 0.0f,
	-1.0f, w, 1.0f,		 0.0f, 1.0f, 0.0f,
	1.0f, w, 1.0f,		 0.0f, 1.0f, 0.0f,
	//f
	1.0f, 0.0f, -1.0f,		 0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		 0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, 1.0f,		 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		 0.0f, -1.0f, 0.0f,
};

float pod[24];




/*******************************  funkcje ***************************************/

/*###############################################################*/
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
	}

}
/******************************************/
void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;
	////swiatlo
	case '6':
		lightPos[0] ++ ;
		break;
	case '4':
		lightPos[0] --;
		break;
	case '9':
		lightPos[1] ++;
		break;
	case '3':
		lightPos[1] --;
		break;
	case '8':
		lightPos[2] ++;
		break;
	case '2':
		lightPos[2] --;
		break;
	
	///////przyblizanie
	case '+': 
		kameraD += 5;
		break;
	case '-':
		kameraD += -5;
		break;

	////////przesuwanie 
	case 'd':
		z2 += -1;
		break;
	case 'a':
		z2 += +1;
		break;
	case 'w':
		x2 += -1;
		break;
	case 's':
		x2 += +1;
		break;
	}
	
	
}


/*###############################################################*/
void rysuj(void)
{


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID); //uzyj wskazanego shadera	
	


	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobranie lokalizacji zmiennej 'uniform' "MV" w programie

	
	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(0, -1, kameraD ));
	MV = glm::translate(MV, glm::vec3(z2, 0, 0));
	MV = glm::translate(MV, glm::vec3(0, x2, 0));

	MV = glm::rotate(MV, (float)glm::radians(kameraZ + 10), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX + 30), glm::vec3(0, 1, 0));

	glm::mat4 MVP = P * MV;
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	/************************  parametry oswietlenia  *****************************/
	
	glUniform3f(lightColor_id, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPos_id,lightPos[0], lightPos[1], lightPos[2]);

	/************************   rysowanie labiryntu   *****************************/
	k = 0;

	//podloga
	glBindVertexArray(podVAO);
	glUniform3f(objectColor_id, 0.5f, 0.5f, 0.6f);
	glDrawArrays(GL_QUADS, 0, 4);

	for (int i = 0; i < wys; i++) {
		for (int j = 0; j < szer; j++) {
			
			char scn = sciany[i][j];
					
			if (scn == 'X') {
				//rysowanie scian
				glUniform3f(objectColor_id, 0.75f, 0.6f, 0.7f);
				glBindVertexArray(cubeVAO[0]);
				glDrawArrays(GL_QUADS, 0, 24); //slupek

				if (i + 1 < wys && sciany[i + 1][j] == 'X') {
					glBindVertexArray(cubeVAO[4]);
					glDrawArrays(GL_QUADS, 0, 24); 
				}
				if (i - 1 >= 0 && sciany[i - 1][j] == 'X') {
					glBindVertexArray(cubeVAO[3]);
					glDrawArrays(GL_QUADS, 0, 24); 
				}
				if (j + 1 < szer && sciany[i][j + 1] == 'X') {
					glBindVertexArray(cubeVAO[2]);
					glDrawArrays(GL_QUADS, 0, 24); 
					
				}
				if (j - 1 >= 0 && sciany[i][j - 1] == 'X') {
					glBindVertexArray(cubeVAO[1]);
					glDrawArrays(GL_QUADS, 0, 24);
					
				}
				
			}
			else if (scn == ' ') {
				                
			}
			else if (scn == 'o') {
				//rysowanie skarbow
				switch (skarby[k])
				{
					
				case 1:
					glBindVertexArray(VAO_skarb[0]);
					MV = glm::translate(MV, glm::vec3(0, 0.5*w, 0));
					MVP = P * MV;
					glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

					glUniform3f(objectColor_id, 0.4f, 0.5f, 0.75f);
					glDrawElements(GL_TRIANGLES, n_el_1, GL_UNSIGNED_INT, 0);

					MV = glm::translate(MV, glm::vec3(0, -0.5*w, 0));
					MVP = P * MV;
					glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
					break;
				case 2:
					glBindVertexArray(VAO_skarb[1]);
					MV = glm::translate(MV, glm::vec3(0, 0.5 * w, 0));
					MVP = P * MV;
					glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

					glUniform3f(objectColor_id, 0.5f, 0.75f, 0.4f);
					glDrawElements(GL_TRIANGLES, n_el_2, GL_UNSIGNED_INT, 0);

					MV = glm::translate(MV, glm::vec3(0, -0.5 * w, 0));
					MVP = P * MV;
					glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
					break;
				case 3:
					glBindVertexArray(VAO_skarb[2]);
					MV = glm::translate(MV, glm::vec3(0, 0.5 * w, 0));
					MVP = P * MV;
					glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

					glUniform3f(objectColor_id, 0.75f, 0.75f, 0.4f);
					glDrawElements(GL_TRIANGLES, n_el_3, GL_UNSIGNED_INT, 0);

					MV = glm::translate(MV, glm::vec3(0, -0.5 * w, 0));
					MVP = P * MV;
					glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
					break;

					
				}
				k++;

			}
			else {
				
			}
			//przesuniecie do nastepnej pozycji
			MV = glm::translate(MV, glm::vec3(0, 0, t*2));
			MVP = P * MV;
			glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

		}
		MV = glm::translate(MV, glm::vec3(t*2, 0, -t*szer*2));
		MVP = P * MV;
		glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	};

	/******************* rysowanie zrodla swiatla  ************************/


	glUseProgram(lamp_ID);
	GLuint MVPlamp_id = glGetUniformLocation(lamp_ID, "MVP");
	MV = glm::translate(MV, lightPos);
	MV = glm::scale(MV, glm::vec3(0.1f, 0.1f, 0.1f));
	MV = glm::rotate(MV, (float)glm::radians(-60.0), glm::vec3(0, 1, 0));
	 MVP = P * MV;
	glUniformMatrix4fv(MVPlamp_id, 1, GL_FALSE, &(MVP[0][0]));
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_QUADS, 0, 6);



	glFlush();
	glutSwapBuffers();

}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/

GLfloat ad = 0.0;

void timer(int value) {

	
	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 7");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
	glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	//glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)

	
	glEnable(GL_DEPTH_TEST);

	srand((unsigned)time(NULL));


	/*******************************  wczytanie labiryntu  ***************************************/
	
	std::ifstream file("lab22x22.txt");
	//std::ifstream file("lab13x15.txt");
	//std::ifstream file("lab8x6.txt");
	if (file.fail())
	{
		printf("Cannot open this file or is not in this directory ! \n");
		system("pause");
		exit(-4);
	}
	if (file.is_open())
	{
		file >> std::noskipws; //nie ignoruj białych znaków

		file >> szer; //szerokość labiryntu
		file >> p;
		file >> wys; //wysokość
		file >> p;

		sciany = new char* [wys];

		for (int i = 0; i < wys; i++) {

			sciany[i] = new char[szer];

			if (sciany[i] == NULL) {
				printf("Nie mozna zaalokowac pamieci\n");
				exit(1);
			}

			for (int j = 0; j < szer; j++) {
				sciany[i][j] = file.get();
				if (sciany[i][j] == 'o') {
					liczba_skarbow++;
				}
			}
			file >> p;
		}
	}
	//wypisanie
	printf("wysokosc: %d\nszerokosc: %d\n", wys, szer);
	printf("liczba skarbow: %d\n", liczba_skarbow);
	for (int i = 0; i < wys; i++) {

		for (int j = 0; j < szer; j++) {
			printf("%c", sciany[i][j]);

		}
		printf("\n");
	}


	
	///wierzcholki podlogi
	float pod[24] = {
		-t,										0.0f,	-t,									 0.0f, 1.0f, 0.0f,
		-t,										0.0f,	(t + 1) * 2 + (szer - 2) * (2 * t),	 0.0f, 1.0f, 0.0f,
		(t + 1) * 2 + (wys - 2) * (2 * t),		0.0f,   (t + 1) * 2 + (szer - 2) * (2 * t),	 0.0f, 1.0f, 0.0f,
		(t + 1) * 2 + (wys - 2) * (2 * t),		0.0f,	-t,									 0.0f, 1.0f, 0.0f, };


	//zmienne do przesuwania
	 z2 = -3 * wys;
	 kameraD = kameraD - szer * 3;
	 x2 = wys;

	/********************************  losowanie skarbów  ****************************************/
	skarby = new int[liczba_skarbow];
	for (int i = 0; i < liczba_skarbow; i++) {
		skarby[i] = 1 + (rand() % 3); //losowa liczba od 1 do 3
	};
	//for (int i = 0; i < liczba_skarbow; i++) {
	//	printf("%d ", skarby[i]);
	//}

	/*******************************  wczytywanie skarbów  ***************************************/
	/////////skarb1
	std::ifstream file1("icosa.txt"); //niebieski
	if (file1.fail())
	{
		printf("Cannot open this file or is not in this directory ! \n");
		system("pause");
		exit(-4);
	}

	if (file1.is_open())
	{
		
		file1 >> n_v_1; //liczba wierzcholkow
		file1 >> n_el_1; //liczba elementów
		n_v_1 = n_v_1 * 3;  //rzeczywista liczba elementów
		
		vertices_1 = (float*)calloc(n_v_1, sizeof(float));

		elements_1 = (GLuint*)calloc(n_el_1, sizeof(int));


		for (int i = 0; i < n_v_1; i++)
			file1 >> vertices_1[i];

		for (int i = 0; i < n_el_1; i++)
			file1 >> elements_1[i];


		normals_1 = vertices_1;

	}
	
	
	/////////skarb2
	std::ifstream file2("icosa_k.txt"); //zielony
	if (file2.fail())
	{
		printf("Cannot open this file or is not in this directory ! \n");
		system("pause");
		exit(-4);
	}

	if (file2.is_open())
	{

		file2 >> n_v_2; 
		file2 >> n_el_2; 
		n_v_2 = n_v_2 * 3; 
	
		
		vertices_2 = (float*)calloc(n_v_2, sizeof(float));

		elements_2 = (GLuint*)calloc(n_el_2, sizeof(int));


		for (int i = 0; i < n_v_2; i++)
			file2 >> vertices_2[i];

		for (int i = 0; i < n_el_2; i++)
			file2 >> elements_2[i];

	
		normals_2 = vertices_2;

	}


	/////////skarb3
	std::ifstream file3("octa.txt"); //zolty
	if (file3.fail())
	{
		printf("Cannot open this file or is not in this directory ! \n");
		system("pause");
		exit(-4);
	}

	if (file3.is_open())
	{

		file3 >> n_v_3; 
		file3 >> n_el_3; 
		n_v_3 = n_v_3 * 3;  
	
		vertices_3 = (float*)calloc(n_v_3, sizeof(float));

		elements_3 = (GLuint*)calloc(n_el_3, sizeof(int));


		for (int i = 0; i < n_v_3; i++)
			file3 >> vertices_3[i];

		for (int i = 0; i < n_el_3; i++)
			file3 >> elements_3[i];

		normals_3 = vertices_3;

	}


	/*******************************  programy  ***************************************/
	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");
 
	glUseProgram(programID);

	objectColor_id = glGetUniformLocation(programID, "objectColor");
	lightColor_id = glGetUniformLocation(programID, "lightColor");
    lightPos_id = glGetUniformLocation(programID, "lightPos");



	/*******************************  bufory scian  ***************************************/
	glGenVertexArrays(5, cubeVAO);


	glBindVertexArray(cubeVAO[0]);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s0), s0, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s1), s1, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s2), s2, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s3), s3, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s4), s4, GL_STATIC_DRAW);

	glGenBuffers(1, &VBOpod);
	glBindBuffer(GL_ARRAY_BUFFER, VBOpod);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pod), pod, GL_STATIC_DRAW);

	
	//////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(cubeVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(cubeVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(cubeVAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(cubeVAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(cubeVAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(podVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOpod);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	/*******************************  bufory skarbów  ***************************************/
	
	glGenVertexArrays(3, VAO_skarb);
	/////////////////skarb 1
	glGenBuffers(1, &VBO_skarb[0]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skarb[0]);
	glBufferData(GL_ARRAY_BUFFER, n_v_1 * sizeof(float), vertices_1, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_normals[0]); //normalne
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals[0]);
	glBufferData(GL_ARRAY_BUFFER, n_v_1 * sizeof(float), normals_1, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo_skarb[0]); //elementy
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skarb[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_el_1 * sizeof(int), elements_1, GL_STATIC_DRAW);

	

	glBindVertexArray(VAO_skarb[0]);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skarb[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skarb[0]);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	////////////////////skarb 2
	glGenBuffers(1, &VBO_skarb[1]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skarb[1]);
	glBufferData(GL_ARRAY_BUFFER, n_v_2 * sizeof(float), vertices_2, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_normals[1]); //normalne
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals[1]);
	glBufferData(GL_ARRAY_BUFFER, n_v_2 * sizeof(float), normals_2, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo_skarb[1]); //elementy
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skarb[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_el_2 * sizeof(int), elements_2, GL_STATIC_DRAW);

	

	glBindVertexArray(VAO_skarb[1]);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skarb[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skarb[1]);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	////////////////////skarb 3
	glGenBuffers(1, &VBO_skarb[2]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skarb[2]);
	glBufferData(GL_ARRAY_BUFFER, n_v_3 * sizeof(float), vertices_3, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_normals[2]); //normalne
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals[2]);
	glBufferData(GL_ARRAY_BUFFER, n_v_3 * sizeof(float), normals_3, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo_skarb[2]); //elementy
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skarb[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_el_3 * sizeof(int), elements_3, GL_STATIC_DRAW);

	

	glBindVertexArray(VAO_skarb[2]);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skarb[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skarb[2]);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);



	/*******************************  bufory swiatla  ***************************************/
	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	lamp_ID = loadShaders("lamp_vshader.glsl", "lamp_fshader.glsl");
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/**************************************  start  *********************************************/

	glutMainLoop();					

	/*******************************  zwalnianie pamieci  ***************************************/

	glDeleteVertexArrays(2, cubeVAO);
	glDeleteVertexArrays(2, VAO_skarb);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &podVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &VBOpod);
	glDeleteBuffers(2, VBO_skarb);

	return(0);

}

