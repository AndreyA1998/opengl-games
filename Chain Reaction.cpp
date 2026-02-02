#include <stdio.h>
#include <iostream>
#include <glut.h>
#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <mmsystem.h>
#include "gamey.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

#define _CRT_SECURE_NO_WARNING

struct point
{
	int q;
	int pl;
	double angle;
	double move;
	int bump;
};

//mode controlling variables:
int t = 200;
int u = 0;
int Weight = 800;
int Height = 800;
int mass = 8;
char* arr = (char*)malloc(mass*sizeof(char));
int mode = 1;
int pl = 1;
point** sp = (point**)malloc(8*sizeof(point*));
bool butallow = true;
bool wasbump = 0;
int first = 0;
int second = 0;
int summold = 0;

//huinya for bot
int* proute = (int*)malloc(sizeof(int));
int* pevr = (int*)malloc(sizeof(int));


//functions:
void glEnter2D(void) 
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, Weight, 0, Height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void glLeave2D(void) 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void glWrite(float x, float y, int *font, char text[256], int kls) {
	int i;
	glRasterPos2f(x, y);
	for (i = 0; i<kls; i++)
		glutBitmapCharacter(font, (int)text[i]);
}

/*void show()
{
	switch (u)
	{
	case 1:
	{printf("1"); break;}
	case 2:
	{printf("2"); break;}
	case 3:
	{printf("3"); break;}
	}
}*/

void windsize(int weight, int height)
{
	glViewport(0, 0, weight, height);
	Weight = weight;
	Height = height;
}

void screenchange()
{
	if (pl == 1)
		glClearColor(0.0, 0.3, 0.0, 0.0);
	else
		glClearColor(0.3, 0.0, 0.3, 0.0);
}

void clean()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			sp[i][j].q = 0;
			sp[i][j].pl = 0;
			sp[i][j].angle = 0;
			sp[i][j].move = 0.016;
			sp[i][j].bump = 0;
		}
}

void check2()
{
	if ((first > 0) && (second > 0))
		return;

	if ((first == 0) && (wasbump == true))
		if (MessageBox(NULL, L"Do you want to Restart the game?", L"Player1 LOST!", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			mode = 1;
			second = 0;
			first = 0;
			pl = 1;
			wasbump = false;
			clean();
			return;
		}
		else
			exit(0);

	if ((second == 0) && (wasbump == true))
		if (MessageBox(NULL, L"Do you want to Restart the game?", L"Player2 LOST!", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			mode = 1;
			second = 0;
			first = 0;
			pl = 1;
			wasbump = false;
			clean();
			return;
		}
		else
			exit(0);
}

int bump(int x1, int y1)
{
	switch (sp[x1][y1].q)
	{
	case 0:
	{
		return 1;
		break;
	}break;
	case 1:
	{
		return 1;

	}
	case 2:
	{
		if (((x1 == 0) && (y1 == 0)) || ((x1 == 0) && (y1 == 7)) || ((x1 == 7) && (y1 == 0)) || ((x1 == 7) && (y1 == 7)))
		{
			sp[x1][y1].bump = 1;
			return 0;
		}
		else
			return 1;
		break;
	}
	case 3:
	{
		if ((x1 == 0) || (y1 == 0) || (y1 == 7) || (x1 == 7))
		{
			sp[x1][y1].bump = 1;
			return 0;
		}
		else
			return 1;
		break;
	}
	case 4:
	{
		sp[x1][y1].bump = 1;
		return 0;
		break;
	}
	}
}

void build1(int i, int j)
{
	glPointSize(40);
	glBegin(GL_POINTS);
	glVertex2f((((float)i + 1) / 10 + 0.05)*Weight, Height - (((float)j + 1) / 10 + 0.05)*Height);
	glEnd();
}

void build2(int i, int j)
{
	glPointSize(40);
	glBegin(GL_POINTS);
	glVertex2f((((float)i + 1) / 10 + 0.05 + 0.016*cos(sp[i][j].angle))*Weight, Height - (((float)j + 1) / 10 + 0.05 + 0.016*sin(sp[i][j].angle))*Height);
	glVertex2f((((float)i + 1) / 10 + 0.05 + 0.016*cos(sp[i][j].angle + 3.141592654))*Weight, Height - (((float)j + 1) / 10 + 0.05 + 0.016*sin(sp[i][j].angle + 3.141592654))*Height);
	glEnd();
	if (sp[i][j].angle >= 2 * 3.141592654)
		sp[i][j].angle -= 2 * 3.141592654;
	else
		sp[i][j].angle += 2 * 3.141592654 / 60;
}

void build3(int i, int j)
{
	glPointSize(40);
	glBegin(GL_POINTS);
	glVertex2f((((float)i + 1) / 10 + 0.05 + 0.016*sin(sp[i][j].angle + 2 * 3.141592654 / 3))*Weight, Height - (((float)j + 1) / 10 + 0.05 + 0.016*cos(sp[i][j].angle + 2 * 3.141592654 / 3))*Height);
	glVertex2f((((float)i + 1) / 10 + 0.05 + 0.016*sin(sp[i][j].angle + 4 * 3.141592654 / 3))*Weight, Height - (((float)j + 1) / 10 + 0.05 + 0.016*cos(sp[i][j].angle + 4 * 3.141592654 / 3))*Height);
	glVertex2f((((float)i + 1) / 10 + 0.05 + 0.016*sin(sp[i][j].angle))*Weight, Height - (((float)j + 1) / 10 + 0.05 + 0.016*cos(sp[i][j].angle))*Height);
	glEnd();
	if (sp[i][j].angle >= 2 * 3.141592654)
		sp[i][j].angle -= 2 * 3.141592654;
	else
		sp[i][j].angle += 2 * 3.141592654 / 60;
}

int check(int x1, int y1)
{
	if ((x1 < 0) || (x1 >= 8) || (y1 < 0) || (y1 >= 8))
		return -1;

	if (((sp[x1][y1].pl != pl) && (sp[x1][y1].pl != 0)) || (sp[x1][y1].q > 4))
		return -1;

	switch (sp[x1][y1].q)
	{
	case 0:
	{
		return 0;
		break;
	}
	case 1:
	{
		if (((x1 == 0) && (y1 == 0)) || ((x1 == 0) && (y1 == 7)) || ((x1 == 7) && (y1 == 0)) || ((x1 == 7) && (y1 == 7)))
		{
			sp[x1][y1].bump = 1;
			return 1;
		}
		else
			return 0;
		break;
	}
	case 2:
	{
		if ((x1 == 0) || (y1 == 0) || (y1 == 7) || (x1 == 7))
		{
			sp[x1][y1].bump = 1;
			return 1;
		}
		else
			return 0;
		break;
	}
	case 3:
	{
		sp[x1][y1].bump = 1;
		return 1;
		break;
	}
	}
}

void buildspheregroup()
{
	int f = 1;
	int summ = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (sp[i][j].pl == 1)
			{
				glColor3f(1.0, 0.0, 0.0);
			}
			else
			{
				glColor3f(0.0, 1.0, 0.0);
			}
			if (sp[i][j].bump == 0)
			{
				switch (sp[i][j].q)
				{
				case 1:
				{
					build1(i, j);
					break;
				}
				case 2:
				{
					build2(i, j);
					break;
				}
				case 3:
				{
					build3(i, j);
					break;
				}
				}
				summ += 1;
			}
			else
			{
				glPointSize(40);
				glBegin(GL_POINTS);
				if (sp[i][j].move <= 0.1)
				{
					if (i <=6)
						glVertex2f((((float)i + 1) / 10 + 0.05 + sp[i][j].move)*Weight, Height - (((float)j + 1) / 10 + 0.05)*Height);
					if (i >= 1)
						glVertex2f((((float)i + 1) / 10 + 0.05 - sp[i][j].move)*Weight, Height - (((float)j + 1) / 10 + 0.05)*Height);
					if (j <= 6)
						glVertex2f((((float)i + 1) / 10 + 0.05)*Weight, Height - (((float)j + 1) / 10 + 0.05 + sp[i][j].move)*Height);
					if (j >= 1)
						glVertex2f((((float)i + 1) / 10 + 0.05)*Weight, Height - (((float)j + 1) / 10 + 0.05 - sp[i][j].move)*Height);
					
					switch (sp[i][j].q)
					{
						case 5:
						{
							build1(i, j);
							break;
						}
						case 6:
						{
							build2(i, j);
							break;
						}
						case 7:
						{
							build3(i, j);
							break;
						}
					}

					glEnd();
					sp[i][j].move += 0.1 / 20;
				}
				else 
				{ 
					PlaySound(_T("1.wav"), NULL, SND_FILENAME | SND_ASYNC);
					sp[i][j].bump = 0;
					if (i <= 6)
					{
						if (sp[i][j].pl != sp[i + 1][j].pl)
							if (sp[i][j].pl == 1)
							{
								first = first + sp[i + 1][j].q;
								second = second - sp[i + 1][j].q;
							}
							else
							{
								second = second + sp[i + 1][j].q;
								first = first - sp[i + 1][j].q;
							}
						sp[i + 1][j].q++;
						sp[i + 1][j].pl = sp[i][j].pl;
						f = bump(i+1, j);
					}
					if (i >= 1)
					{
						if (sp[i][j].pl != sp[i - 1][j].pl)
							if (sp[i][j].pl == 1)
							{
								first = first + sp[i - 1][j].q;
								second = second - sp[i - 1][j].q;
							}
							else
							{
								second = second + sp[i - 1][j].q;
								first = first - sp[i - 1][j].q;
							}
						sp[i - 1][j].q++;
						sp[i - 1][j].pl = sp[i][j].pl;
						f = bump(i - 1, j);
					}
					if (j <= 6)
					{
						if (sp[i][j].pl != sp[i][j + 1].pl)
							if (sp[i][j].pl == 1)
							{
								first = first + sp[i][j + 1].q;
								second = second - sp[i][j + 1].q;
							}
							else
							{
								second = second + sp[i][j + 1].q;
								first = first - sp[i][j + 1].q;
							}
						sp[i][j + 1].q++;
						sp[i][j + 1].pl = sp[i][j].pl;
						f = bump(i, j+1);
					}
					if (j >= 1)
					{
						if (sp[i][j].pl != sp[i][j - 1].pl)
							if (sp[i][j].pl == 1)
							{
								first = first + sp[i][j - 1].q;
								second = second - sp[i][j - 1].q;
							}
							else
							{
								second = second + sp[i][j - 1].q;
								first = first - sp[i][j - 1].q;
							}
						sp[i][j - 1].q++;
						sp[i][j - 1].pl = sp[i][j].pl;
						f = bump(i, j-1);
					}
					if (f == 1)
					{
						butallow = true;
					}

					sp[i][j].move = 0.016;
					switch (sp[i][j].q)
					{
						case 5:
						{
							sp[i][j].q = 1;
							break;
						}
						default:
						{
							sp[i][j].q = 0;
							sp[i][j].pl = 0;
						}
					}
				}
			}
		}
	}
	if (butallow == true && summ == 64 && summold == 64)
	{
		int x1, y1;
		check2();
		if (u != 1)
		{
			switch (u)
			{
			case 2:
			{
				if (pl == 2)
				{
					for (int i = 0; i < 8; i++)
						for (int j = 0; j < 8; j++)
						{
							char hui = (sp[i][j].pl == 1) ? 1 : -1; //hui
							a0[i][j] = sp[i][j].q * hui;
						}
					char code = stupid();
					x1 = code / 8;
					y1 = code % 8;
				}
				break;
			}
			case 3:
			{
				if (pl == 2)
				{
					for (int i = 0; i < 8; i++)
						for (int j = 0; j < 8; j++)
						{
							char hui = (sp[i][j].pl == 1) ? 1 : -1; //hui
							a0[i][j] = sp[i][j].q * hui;
						}
					*(pevr) = 228000;
					think(proute, pevr, -1000000, 1);
					int route = *(proute);
					x1 = route / 10;
					y1 = route % 10;
				}				
				break;
			}
			}
			int l = check(x1, y1);
			if ((l == 0) || (l == 1))
			{
				PlaySound(_T("2.wav"), NULL, SND_FILENAME | SND_ASYNC);
				if (l == 1)
				{
					butallow = false;
					wasbump = true;
				}

				sp[x1][y1].q++;
				sp[x1][y1].pl = pl;
				sp[x1][y1].angle = 0;
				if (pl == 1)
				{
					pl = 2;
					first++;
				}
				else
				{
					pl = 1;
					second++;
				}
				printf("\n%d\n%d\n", first, second);
			}
		}

	}
	summold = summ;
}

void players()
{
	if (pl == 1)
		glWrite((Weight - 8 * (float)sizeof("Player1:")) / 2, 0.95*Height, (int*)GLUT_BITMAP_9_BY_15, "Player1", sizeof("Player1"));
	else
		glWrite((Weight - 8 * (float)sizeof("Player2:")) / 2, 0.95*Height, (int*)GLUT_BITMAP_9_BY_15, "Player2", sizeof("Player2"));
}

void display1()
{
	if (mode == 1)
	{
		t = 200;
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glClearColor(0.0, 0.0, 0.3, 0.0);

		glColor3f(0.4, 0.5, 0.1);
		glRectf(-0.8f, 0.2f, -0.4f, -0.2f);
		glColor3f(0.0, 0.2, 0.6);
		glRectf(-0.2f, 0.2f, 0.2f, -0.2f);
		glColor3f(0.3, 0.8, 0.6);
		glRectf(0.4f, 0.2f, 0.8f, -0.2f);

		glColor3ub(145, 30, 66);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.8f, 0.0f); //верхняя вершина
		glVertex3f(-0.4f, 0.4f, 0.0f); //левая вершина
		glVertex3f(0.4f, 0.4f, 0.0f); //правая вершина
		glEnd();

		glEnter2D();
		glColor3f(1, 0.5, (float)(rand() % 100) / 100);
		for (int i = 0; i < mass; i++)
			arr[i] = (char)(rand() % 100);

		glWrite((Weight - 8 * (float)sizeof(arr)) / 2, 0.8* Height, (int*)GLUT_BITMAP_9_BY_15, arr, sizeof(arr));
		glColor3f(0.5, 0.5, 0.5);
		glWrite((Weight - 8 * (float)sizeof("Chose your level:")) / 2, 0.65*Height, (int*)GLUT_BITMAP_9_BY_15, "Chose your level:", sizeof("Chose your level:"));
		glColor3f(1, 0.0, 0.0);
		glWrite(0.2*Weight, 0.5* Height, (int*)GLUT_BITMAP_HELVETICA_18, "1", 1);
		glColor3f(1, 0.0, 0.0);
		glWrite(0.5*Weight, 0.5* Height, (int*)GLUT_BITMAP_HELVETICA_18, "2", 1);
		glColor3f(1, 0.0, 0.0);
		glWrite(0.8*Weight, 0.5* Height, (int*)GLUT_BITMAP_HELVETICA_18, "3", 1);
		glColor3f(0.5, 0.5, 0.5);
		glLeave2D();
	}
	if (mode == 2)
	{
		t = 25;
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3f(1, 0.3, 0.3);
		glRectf(-0.8, -0.8, 0.8, 0.8);

		glColor3f(0.0, 0.0, 0.0);
		
		gluOrtho2D(0, Weight, 0, Height);
		for (int i = 0.1*Weight; i < 0.9*Weight; i += 0.1*Weight)
		{
			glBegin(GL_LINES);
			glVertex2f((float)i, 0.1*Height);
			glVertex2f((float)i, 0.9*Height);
			glEnd();
		}
		glBegin(GL_LINES);
		for (int j = 0.1*Height; j < 0.9*Height; j += 0.1*Height)
		{
			glVertex2f(0.1*Weight, (float)j);
			glVertex2f(0.9*Weight, (float)j);
		}
		glEnd();
		screenchange();
		buildspheregroup();
		glColor3f(1.0, 0.5, 1.0);
		players();
	}
	if (mode == 0)
	{
		exit(0);
	}
	glutSwapBuffers();
}

void spfunc(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		if (MessageBox(NULL, L"Do you want to Exit game?", L"Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			printf("\a");
			Sleep(1000);
			mode = 0;
			display1();
		}
	}
	if ((key == 9) && (mode != 1))
		if (MessageBox(NULL, L"Do you want to Restart the game?", L"Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			mode = 1;
			clean();
			first = 0;
			second = 0;
			pl = 1;
			display1();
		}
}

void mouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if ((x >= 0.1*Weight) && (x <= 0.3*Weight) && (y >= 0.4*Height) && (y <= 0.6*Height) && (mode == 1))
		{
			u = 1;
			mode = 2;
			screenchange();
			display1();
			goto label;
		}
	}
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if ((x >= 0.4*Weight) && (x <= 0.6*Weight) && (y >= 0.4*Height) && (y <= 0.6*Height) && (mode == 1))
		{
			u = 2;
			mode = 2;
			screenchange();
			display1();
			goto label;
		}
	}
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if ((x >= 0.7*Weight) && (x <= 0.9*Weight) && (y >= 0.4*Height) && (y <= 0.6*Height) && (mode == 1))
		{
			u = 3;
			mode = 2;
			screenchange();
			display1();
			goto label;
		}
	}

	int x1 = 0;
	int y1 = 0;

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && (mode == 2) && (butallow == true))
	{
		if (((x - 0.1*Weight) / (Weight / 10) >= 0) && ((x - 0.1*Weight) / (Weight / 10) < 9))
			x1 = (int)(x - 0.1*Weight) / (Weight / 10);
		else
			x1 = -1;

		if (((y - 0.1*Height) / (Height / 10) >= 0) && ((y - 0.1*Height) / (Height / 10) < 9))
			y1 = (int)(y - 0.1*Height) / (Height / 10);
		else
			y1 = -1;



		printf("%d", x1);
		printf("%d", y1);

		if ((pl == 2) && (u != 1))
			goto label;

		/*if ((u == 2) && (pl == 2))
		{
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
				{
					char hui = (sp[i][j].pl == 1) ? 1 : -1; //hui
					a[i][j] = sp[i][j].q * hui;
				}
			char code = stupid();
			x1 = code / 8;
			y1 = code % 8;
		}*/
		
		int l = check(x1, y1);
		if ((l == 0) || (l == 1))
		{
			PlaySound(_T("2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (l == 1)
			{
				butallow = false;
				wasbump = true;
			}

			sp[x1][y1].q++;
			sp[x1][y1].pl = pl;
			sp[x1][y1].angle = 0;
			if (pl == 1)
			{
				pl = 2;
				first++;
			}
			else
			{
				pl = 1;
				second++;
			}
			printf("\n%d\n%d\n", first, second);
			display1();
		}
	}
	label:;
}

void timer(int = 0)
{
	display1();
	//show();
	glutTimerFunc(t, timer, 0);
}


int main(int argc, char **argv)
{
	for (int i = 0; i < 8; i++)
	{
		sp[i] = (point*)malloc(8*sizeof(point));
	}
	clean();

	q = (queue1*)malloc(sizeof(queue1)); //nevkusniy
	create(q);

	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(Weight, Height);
	glutInitWindowPosition(400, 200);
	glutCreateWindow("Chain Reaction");
	glClearColor(0.0, 0.0, 0.3, 0.0);
	glutDisplayFunc(display1);
	glutTimerFunc(t, timer, 0);
	glutMouseFunc(mouse);
	glutKeyboardFunc((spfunc));
	glutReshapeFunc(windsize);
	glutMainLoop();

	free(proute);
	free(pevr);
	return 0;
}
