#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <glut.h>
#define _CRT_SECURE_NO_WARNING

using namespace std;

struct snake
{
	int x;
	int y;
};

struct snake* wall = (struct snake*)malloc(100*sizeof(struct snake));
int t = 200;
int N = 30;
int M = 20;
int Size = 20;
int Wight = Size*N;
int Height = Size*M;
int area = 0;
int num = 2;
int k = 0;
int p = 0;

struct snake* s = (struct snake*)malloc(100*sizeof(struct snake));

void Snake()
{
	glColor3f(0.2, 0.5, 0.4);
	glRectf(s[0].x*Size + 2, s[0].y*Size + 2, (s[0].x + 0.8)*Size, (s[0].y + 0.8)*Size);
	glColor3f(0.2, 0.5, 0.1);
	for (int i = 1; i < num; i++)
	{
		glRectf(s[i].x*Size + 2, s[i].y*Size + 2, (s[i].x + 0.8)*Size, (s[i].y + 0.8)*Size);
	}
}

void zero()
{
	for (int i = 0; i < 100; i++)
	{
		s[i].x = 0;
		s[i].y = 0;
	}
}


void areaa()
{
	if (area == 0)
		s[0].y = s[0].y + 1;
	if (area == 1)
		s[0].x = s[0].x - 1;
	if (area == 2)
		s[0].x = s[0].x + 1;
	if (area == 3)
		s[0].y = s[0].y - 1;
}

void change()
{
	for (int i = num; i > 0; i--)
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}
}

void Board()
{
	glColor3f(0.0, 0.7, 0.0);
	glBegin(GL_LINES);
	for (int i = 1; i < Wight; i += Size)
	{
		glVertex2f(i, 0);
		glVertex2f(i, Height);
	}
	for (int j = 1; j < Height; j += Size)
	{
		glVertex2f(0, j);
		glVertex2f(Wight, j);
	}
	glEnd();
}

int check1(int x, int y)
{
	for (int i = 0; i < num; i++)
	{
		if (s[i].x == x && s[i].y == y)
		{
			return 0;
		}
	}
	return 1;
}

int check3(int x, int y)
{
	for (int i = 0; i <= p; i++)
	{
		if (wall[i].x == x && wall[i].y == y)
		{
			return 0;
		}
	}
	return 1;
}

class eat
{
public:
	int x, y;
	void New()
	{
		while (1)
		{
			x = rand() % N;
			y = rand() % M;
			if (check1(x, y) == 1 && check3(x, y) == 1)
				break;
		}
	}
	void Apple()
	{
		glColor3f(1.0, 0.6, 0.0);
		glRectf(x*Size + 2, y*Size + 2, (x + 1)*Size - 2, (y + 1)*Size - 2);
	}
};

eat m[15];

int check(int x, int y)
{
	for (int i = 0; i < 15; i++)
	{
		if (m[i].x == x && m[i].y == y)
		{
			return 0;
		}
	}
	return 1;
}

class antieat
{
public: 
	int x, y;
	void New()
	{
		while (1)
		{
			x = rand() % N;
			y = rand() % M;
			if (check(x, y) == 1 && check1(x, y) == 1 && check3(x, y) == 1)
				break;
		}
	}
	void AntiApple()
	{
		glColor3f(1.0, 0.9, 0.0);
		glRectf(x*Size + 2, y*Size + 2, (x + 1)*Size - 2, (y + 1)*Size - 2);
	}
};

antieat n[2];

void kek()
{
	int o, h;

	for (int i = 0; i <= p; i++)
	{
		if ((s[0].x == wall[i].x) && (s[0].y == wall[i].y))
		{
			
			num = 2;
			o = 10;
			h = 10;
			s[0].x = o;
			s[0].y = h;
			s[1].x = o;
			s[1].y = o + 1;
			area = 0;
			printf("\a");
			Sleep(2000);
			t = 200;
			return;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if ((s[0].x == n[i].x) && (s[0].y == n[i].y))
		{
			if (num <= 2)
			{
				num = 2;
				o = 10;
				h = 10;
				s[0].x = o;
				s[0].y = h;
				s[1].x = o;
				s[1].y = o + 1;
				area = 0;
				printf("\a");
				Sleep(2000);
				n[i].New();
				return;
			}
			else
			{
				num--;
				n[i].New();
			}
		}
	}
}

void Tick()
{
	int o, h;

	if (num >= 99)
	{
		s = (struct snake*)realloc(s, (num+1)*sizeof(struct snake));
	}

	change();

	areaa();

	kek();

	if (s[0].x >= N)
	{
		s[0].x = 0;
		//Tick();
		Snake();
		Sleep(t);
	}
	if (s[0].x < 0)
	{
		s[0].x = N;
		Tick();
		Snake();
		Sleep(t);
	}
	if (s[0].y >= M)
	{
		s[0].y = 0;
		//Tick();
		Snake();
		Sleep(t);
	}
	if (s[0].y < 0)
	{
		s[0].y = M;
		Tick();
		Snake();
		Sleep(t);
	}

	for (int j = 0; j < 15; j++)
	{
		if ((s[0].x == m[j].x) && (s[0].y == m[j].y))
		{
			num++;
			m[j].New();
		}
	}

	for (int i = 1; i < num; i++)
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			num = 2;
			o = 10;
			h = 10;
			s[0].x = o;
			s[0].y = h;
			s[1].x = o;
			s[1].y = o + 1;
			area = 0;
			t = 200;
			Sleep(2000);
		}
	}
}

void walls()
{
	for (int i = 0; i < p; i++)
	{
		glColor3f(1.0, 0.9, 0.8);
		glRectf(wall[i].x*Size + 1, wall[i].y*Size + 1, (wall[i].x + 1)*Size - 1, (wall[i].y + 1)*Size - 1);
	}
}

void art()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Board();
	walls();
	Snake();
	for (int j = 0; j < 15; j++)
	{
		m[j].Apple();
	}
	for (int j = 0; j < 2; j++)
	{
		n[j].AntiApple();
	}
	//glFlush();
	glutSwapBuffers();
}

void run()
{
	if (k == 1)
		k = 0;
	else
		Tick();
	art();
}

void timer(int = 0)
{
	run();
	glutTimerFunc(t, timer, 0);
}

void Key(int key, int a, int b)
{
	switch (key)
	{
		case 101: 
			if (area != 3)
			{
				if (area == 0)
				{
					if (t > 100)
						t = t - 10;
				}
				else
					area = 0;
			}
			else 
				if (t < 400) 
					t = t + 10;
			k = 1;
			Tick();
			break;
		case 102: 
			if (area != 1)
			{
				if (area == 2)
				{
					if (t > 100)
						t = t - 10;
				}
				else
					area = 2;
			}
			else
				 if(t < 400)
					t=t+10;
			k = 1;
			Tick();
			break;
		case 100: 
			if (area != 2)
			{
				if (area == 1)
				{
					if (t > 100)
						t = t - 10;
				}
				else
					area = 1;
			}
			else
				if (t < 400)
					t=t+10;
			k = 1;
			Tick();
			break;
		case 103: 
			if (area != 0)
			{
				if (area == 3)
				{
					if (t > 100)
						t = t - 10;
				}
				else
					area = 3;
			}
			else
				if (t < 400)
					t=t+10;
			k = 1;
			Tick();
			break;
	}
	Sleep(t);
}

int main(int argv, char** argc)
{
	zero();
	srand(time(0));
	char c;
	FILE* fm = fopen("input1.txt", "r+");
	if (fm != NULL)
	{
		while (fscanf(fm, "%d %d ", &wall[p].x, &wall[p].y) != EOF)
		{
			p++;
			if (p >= 100)
			{
				wall = (struct snake*)realloc(wall, (p + 1) * sizeof(struct snake));
			}
		}
		fclose(fm);
	}
	printf("Random possesions of flowers - \"R\", read from File - other buttons: ");
	scanf_s("%c", &c);
	if (c == 'R')
	{
		FILE*fn = fopen("input.txt", "w+");
		for (int j = 0; j < 15; j++)
		{
			m[j].New();
			fprintf(fn, "%d %d ", m[j].x, m[j].y);
		}
		for (int i = 0; i < 2; i++)
		{
			n[i].New();
			fprintf(fn, "%d %d ",n[i].x, n[i].y);
		}
		fclose(fn);
	}
	else
	{
		FILE*fn = fopen("input.txt", "r+");
		if (fn == NULL)
		{
			printf("No file");
			return 0;
		}
		for (int i = 0; i < 15; i++)
		{
			fscanf(fn, "%d ", &m[i].x);
			fscanf(fn, "%d ", &m[i].y);
		}
		for (int i = 0; i < 2; i++)
		{
			fscanf(fn, "%d ", &n[i].x);
			fscanf(fn, "%d ", &n[i].y);
		}
		fclose(fn);
	}
	s[0].x = 10;
	s[0].y = 10;
	s[1].x = 10;
	s[1].y = 11;
	glutInit(&argv, argc);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(Wight+1, Height+1);
	glutInitWindowPosition(100, 200);
	glClearColor(0.2, 0.8, 0.3, 1.0);
	glutCreateWindow("SNAKE");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Wight, 0, Height);
	glutDisplayFunc(art);
	glutSpecialFunc(Key);
	glutTimerFunc(t, timer, 0);
	glutMainLoop();
	return 0;
}