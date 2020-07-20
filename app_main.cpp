#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <string.h>
#include <cmath>
#include <math.h>
#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

class Square {
	float x;
	float y; 
	float s;
public:
	float r;
	float g;
	float b;
	bool pressed;
	bool xPlayer;
	bool oPlayer;
	Square() : x(0), y(0), s(0.05) {}

	//main constructor for the squares
	Square(float x, float y, float r, float g, float b) : s(0.15), pressed(false), xPlayer(false), oPlayer(false) {
		this->x = x * .30;
		this->y = y * .30;
		this->r = r;
		this->g = g;
		this->b = b;
	}
 
	void draw() {

		if (xPlayer) { //if r = 0 then it draws the x on the square
			glColor3f(0, 0, 1);

			glBegin(GL_LINES);
			glVertex2f(x, y);
			glVertex2f(x + s, y - s);
			glVertex2f(x + s, y);
			glVertex2f(x, y - s);
			glEnd();

		}
		else if (oPlayer) { //if b = 0, it draws the o
			float xCenter = (x + (x + s)) / 2;
			float yCenter = (y + (y - s)) / 2;

			glColor3f(1, 0, 0);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i <= 290; i++) {
				float angle = 2 * M_PI * i / 290;
				float x = xCenter + s * .5* cos(angle);
				float y = yCenter + s * .5* sin(angle);
				glVertex2f(x, y);
			}
			glEnd();
		}

		//square
		 glColor3f(r, g, b);

		glBegin(GL_POLYGON);

		glVertex2f(x, y);
		glVertex2f(x, y - s);
		glVertex2f(x + s, y - s);
		glVertex2f(x + s, y);

		glEnd();
	}

	bool contains(float mx, float my) {
		return (mx >= x) && (mx <= (x + s)) && (my <= y) && (my >= (y - s));
	}
};

// Stores the width and height of the window
int width = 500, height = 500;

vector< vector<Square*> > grid;

Square PVP, PVE;

bool turn;
bool PvE;
bool PvP;
bool menu;
bool p1W;
bool p2W;
bool gameDone;

int moves;

void matchCheck() { // checks if there is win condition in the columns
	for (int i = 0; i < grid.size(); i++) {
		if (grid[i][0]->pressed && grid[i][1]->pressed && grid[i][2]->pressed) {
			if (grid[i][0]->xPlayer && grid[i][1]->xPlayer && grid[i][2]->xPlayer) {
				p1W = true;
				gameDone = true;
				cout << "Player 1 Wins!" << endl;
			}
	
			else if (grid[i][0]->oPlayer && grid[i][1]->oPlayer && grid[i][2]->oPlayer) {
				p2W = true;
				gameDone = true;
				cout << "Player 2 Wins!" << endl;
			}
		}
	}
	// checks the rows to see if there is a winner
	for (int i = 0; i < grid.size(); i++) {
		if (grid[0][i]->pressed && grid[1][i]->pressed && grid[2][i]->pressed) {
			if (grid[0][i]->xPlayer && grid[1][i]->xPlayer && grid[2][i]->xPlayer) {
				p1W = true;
				gameDone = true;
				cout << "Player 1 Wins!" << endl;
			}

			else if (grid[0][i]->oPlayer && grid[1][i]->oPlayer && grid[2][i]->oPlayer) {
				p2W = true;
				gameDone = true;
				cout << "Player 2 Wins!" << endl;
			}
		}
	}

	//checks the diagonal way
	if ((grid[0][0]->xPlayer && grid[1][1]->xPlayer && grid[2][2]->xPlayer) || (grid[0][2]->xPlayer && grid[1][1]->xPlayer && grid[2][0]->xPlayer)) {
		p1W = true;
		gameDone = true;
		cout << "Player 1 Wins!" << endl;
	}

	else if ((grid[0][0]->oPlayer && grid[1][1]->oPlayer && grid[2][2]->oPlayer) || (grid[0][2]->oPlayer && grid[1][1]->oPlayer && grid[2][0]->oPlayer)) {
		p2W = true;
		gameDone = true;
		cout << "Player 2 Wins!" << endl;

	}

	//if there are no more space and no winner, game is tied
	if (moves == 9 || (PvE && moves == 5)) {
		cout << "It's a tie!" << endl;
		gameDone = true;
	}
}

//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color to blue
	glClearColor(0.0, 0.0, 1.0, 1.0);

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw 
	if (menu) {
		PVP.draw();
		PVE.draw();
	
	glColor3d(1.0, 1.0, 1.0);
	}
	else {
		//Grid of the squares 
		if (turn && !gameDone) {
			glColor3f(1.0, 1.0, 1.0);
		}
		else if (!turn && !gameDone) {
			glColor3f(1.0, 1.0, 1.0);
		}
		for (int i = 0; i < grid.size(); i++) {
			for (int j = 0; j < grid[i].size(); j++) {
				grid[i][j]->draw();
			}
		}
	}

	// We have been drawing everything to the back buffer
	// Swap the buffers to see the result of what we drew
	glFlush();
	glutSwapBuffers();
}

//-------------------------------------------------------
// A function to convert window coordinates to scene
// We use it when a mouse event is handled
// Arguments: 	
//	x, y - the coordinates to be updated
//-------------------------------------------------------
void windowToScene(float& x, float& y) {
	x = (2.0f * (x / float(width))) - 1.0f;
	y = 1.0f - (2.0f * (y / float(height)));
}

//-------------------------------------------------------
// A function to handle window resizing
// Called every time the window is resized
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appReshapeFunc(int w, int h) {
	// Window size has changed
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = -1.0;
	const double appXmax = 1.0;
	const double appYmin = -1.0;
	const double appYmax = 1.0;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disered ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = (appXmax + appXmin) / 2;
		winXmin = center - (center - appXmin)*scale;
		winXmax = center + (appXmax - center)*scale;
		winYmin = appYmin;
		winYmax = appYmax;
	}
	else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = (appYmax + appYmin) / 2;
		winYmin = center - (center - appYmin)*scale;
		winYmax = center + (appYmax - center)*scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}



//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appMouseFunc(int b, int s, int x, int y) {
	
	float mx = (float)x;
	float my = (float)y;

	windowToScene(mx, my);

	if (!gameDone) {
		if (s == 0) {
			if (menu) {
				if (PVP.contains(mx, my) && !PVP.pressed) {
					PvP = true;
					menu = false;
				}
				
				else if (PVE.contains(mx, my) && !PVE.pressed) {
					PvE = true;
					menu = false;
				}
			}
			else {
				//the next two for-loops iterates through the grid
				for (int i = 0; i < grid.size(); i++) {
					for (int j = 0; j < grid[i].size(); j++) { //checks if the mouse clicks over a square that hasnt been checked
						if (grid[i][j]->contains(mx, my) && !grid[i][j]->pressed) {
							if (turn) {
								grid[i][j]->xPlayer = true;
								grid[i][j]->pressed = true;
								//if PvE, it will fill in the next available square
								if (PvE) { //checks if the game has ended with the previous move first
									matchCheck();
									if (!gameDone) {
										for (int k = 0; k < grid.size(); k++) {
											bool found = false;
											for (int l = 0; l < grid[k].size(); l++) {
												if (!grid[k][l]->pressed && !grid[k][l]->xPlayer) {
													found = true;
													grid[k][l]->oPlayer = true;
													grid[k][l]->pressed = true;
													break;
												}
											}
											if (found) {
												break;
											}
										}
									}
								}
								else if (PvP) {
									turn = false;
								}
							}
							else {
								if (PvP) {
									grid[i][j]->oPlayer = true;
									grid[i][j]->pressed = true;
									turn = true;
								}
							}							
							moves++;
							matchCheck();
						}
					}
				}
			}
		}
	}
	// Redraw the scene by calling appDrawScene above
	glutPostRedisplay();
	
}

//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments: 	
//	x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {

	// Redraw the scene
	glutPostRedisplay();
}

//-------------------------------------------------------
// A function to handle keyboard events
// Called every time a key is pressed on the keyboard
// Arguments: 	
//	key  - the key that was pressed
//	x, y - coordinates of the mouse when key is pressed
//-------------------------------------------------------
void appKeyboardFunc(unsigned char key, int x, int y) {

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	//initial variables 
	turn = true;
	menu = true;
	PvP = false;
	PvE = false;
	gameDone = false;

	//menu squares
	PVP = Square(-2, -1, 0, 1, 0);
	PVE = Square(-1, -1, 1, 0, 0);
	cout << "Choose green to go against another player or choose red to play against a computer" << endl;

	// Initialize the grid
	grid.resize(3);
	for (int i = 0; i < 3; i++) {
		grid[i].resize(3);
		for (int j = 0; j < 3; j++) {
			grid[i][j] = new Square(i, j, 1, 1, 1);
		}
	}

	// Setup window position, size, and title
	glutInitWindowPosition(20, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Mini Project 3: Tic Tac Toe");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);


	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing th window
	glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboad events
	glutKeyboardFunc(appKeyboardFunc);

	// Start the main loop
	glutMainLoop();
}
