#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>
#include <fstream>

#define numPoints 130830
using namespace std;

double points[numPoints][3];

GLint winWidth = 500, winHeight = 500;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;
static GLdouble viewer[] = { 0.0, 3.0, 0.0 }; /* initial viewer location */
static GLdouble center[] = { 0.0, 0.0, 0.0 };
static GLdouble updirc[] = { 0.0, 0.0, 1.0 };
void readDate(){
	ifstream file("heart.txt");
	for(int i=0;i<numPoints;++i){
		file>>points[i][0]>>points[i][1]>>points[i][2];
	}
	file.close();
}
void heart(){
	glColor3ub(255,0,0);
	glBegin(GL_POINTS);{
		for(int i=0;i<numPoints;++i){
			glVertex3dv(points[i]);
		}
	}glEnd();
}
void display(void){ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* Update viewer position in model view matrix */
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], center[0], center[1], center[2], updirc[0], updirc[1], updirc[2]); 
	/* rotate cube */
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0); 
	heart();
	glFlush();
	glutSwapBuffers();
}
double x_0, y_0;
bool change;
void mouse(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON&&state == GLUT_DOWN){
		change = true;
	}
	if (btn == GLUT_LEFT_BUTTON&&state == GLUT_UP){
		change = false;
	}
}
void mouseMove(int x, int y){
	if (!change)	return;
	double mod, dis0, dis1;
	dis0 = sqrt(viewer[0] * viewer[0] + viewer[1] * viewer[1] + viewer[2] * viewer[2]);
	double x0 = viewer[1] * updirc[2] - viewer[2] * updirc[1];
	double y0 = viewer[2] * updirc[0] - viewer[0] * updirc[2];
	double z0 = viewer[0] * updirc[1] - viewer[1] * updirc[0];
	if (x < x_0){
		mod = sqrt(x0*x0 + y0*y0 + z0*z0);
		viewer[0] -= x0 / mod / 5.0;
		viewer[1] -= y0 / mod / 5.0;
		viewer[2] -= z0 / mod / 5.0;
	}
	if (x > x_0){
		mod = sqrt(x0*x0 + y0*y0 + z0*z0);
		viewer[0] += x0 / mod / 5.0;
		viewer[1] += y0 / mod / 5.0;
		viewer[2] += z0 / mod / 5.0;
	}
	if (y < y_0){
		mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		viewer[0] -= updirc[0] / mod / 5.0;
		viewer[1] -= updirc[1] / mod / 5.0;
		viewer[2] -= updirc[2] / mod / 5.0;
		updirc[0] = viewer[2] * y0 - viewer[1] * z0;
		updirc[1] = viewer[0] * z0 - viewer[2] * x0;
		updirc[2] = viewer[1] * x0 - viewer[0] * y0;
		mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		updirc[0] /= mod;
		updirc[1] /= mod;
		updirc[2] /= mod;
	}
	if (y > y_0){
		mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		viewer[0] += updirc[0] / mod / 5.0;
		viewer[1] += updirc[1] / mod / 5.0;
		viewer[2] += updirc[2] / mod / 5.0;
		updirc[0] = viewer[2] * y0 - viewer[1] * z0;
		updirc[1] = viewer[0] * z0 - viewer[2] * x0;
		updirc[2] = viewer[1] * x0 - viewer[0] * y0;
		mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		updirc[0] /= mod;
		updirc[1] /= mod;
		updirc[2] /= mod;
	}
	dis1 = sqrt(viewer[0] * viewer[0] + viewer[1] * viewer[1] + viewer[2] * viewer[2]);
	viewer[0] = viewer[0] * dis0 / dis1;
	viewer[1] = viewer[1] * dis0 / dis1;
	viewer[2] = viewer[2] * dis0 / dis1;
	x_0 = x;
	y_0 = y;
	display();
}
void keys(unsigned char key, int x, int y) { 
	/* Use x, X, y, Y, z, and Z keys to move viewer */ 
	if (key == 'x') 
		viewer[0] -= 1.0;
	if (key == 'X') 
		viewer[0] += 1.0;
	if (key == 'y')
		viewer[1] -= 1.0;
	if (key == 'Y') 
		viewer[1] += 1.0; 
	if (key == 'z') 
		viewer[2] -= 1.0;
	if (key == 'Z') 
		viewer[2] += 1.0; 
	if (key == '+'){
		viewer[0] *= 0.95;
		viewer[1] *= 0.95;
		viewer[2] *= 0.95;
	}
	if (key == '-'){
		viewer[0] /= 0.95;
		viewer[1] /= 0.95;
		viewer[2] /= 0.95;
	}
	display(); 
}
void specialkeys(int key, int x, int y){
	double x0 = viewer[1] * updirc[2] - viewer[2] * updirc[1];
	double y0 = viewer[2] * updirc[0] - viewer[0] * updirc[2];
	double z0 = viewer[0] * updirc[1] - viewer[1] * updirc[0];
	double dis0, dis1;
	dis0 = sqrt(viewer[0] * viewer[0] + viewer[1] * viewer[1] + viewer[2] * viewer[2]);
	if (key == GLUT_KEY_UP){
		double mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		viewer[0] += updirc[0] / mod / 5.0;
		viewer[1] += updirc[1] / mod / 5.0;
		viewer[2] += updirc[2] / mod / 5.0;
		updirc[0] = viewer[2] * y0 - viewer[1] * z0;
		updirc[1] = viewer[0] * z0 - viewer[2] * x0;
		updirc[2] = viewer[1] * x0 - viewer[0] * y0;
		mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		updirc[0] /= mod;
		updirc[1] /= mod;
		updirc[2] /= mod;
	}
	if (key == GLUT_KEY_DOWN){
		double mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		viewer[0] -= updirc[0] / mod / 5.0;
		viewer[1] -= updirc[1] / mod / 5.0;
		viewer[2] -= updirc[2] / mod / 5.0;
		updirc[0] = viewer[2] * y0 - viewer[1] * z0;
		updirc[1] = viewer[0] * z0 - viewer[2] * x0;
		updirc[2] = viewer[1] * x0 - viewer[0] * y0;
		mod = sqrt(updirc[0] * updirc[0] + updirc[1] * updirc[1] + updirc[2] * updirc[2]);
		updirc[0] /= mod;
		updirc[1] /= mod;
		updirc[2] /= mod;
	}
	if (key == GLUT_KEY_LEFT){
		double mod = sqrt(x0*x0 + y0*y0 + z0*z0);
		viewer[0] += x0 / mod / 5.0;
		viewer[1] += y0 / mod / 5.0;
		viewer[2] += z0 / mod / 5.0;
	}
	if (key == GLUT_KEY_RIGHT){
		double mod = sqrt(x0*x0 + y0*y0 + z0*z0);
		viewer[0] -= x0 / mod / 5.0;
		viewer[1] -= y0 / mod / 5.0;
		viewer[2] -= z0 / mod / 5.0;
	}
	dis1 = sqrt(viewer[0] * viewer[0] + viewer[1] * viewer[1] + viewer[2] * viewer[2]);
	viewer[0] = viewer[0] * dis0 / dis1;
	viewer[1] = viewer[1] * dis0 / dis1;
	viewer[2] = viewer[2] * dis0 / dis1;
	display();
}
void myReshape(int w, int h) {
	glViewport(0, 0, w, h);
	/* Use a perspective view */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	winWidth = w;
	winHeight = h;
	if (winWidth <= winHeight)
		glFrustum(-1.0, 1.0, -1.0 * (GLfloat)winHeight / (GLfloat)winWidth, 1.0* (GLfloat)winHeight / (GLfloat)winWidth, 1.0, 5.0);
	else
		glFrustum(-1.0 * (GLfloat)winWidth / (GLfloat)winHeight, 1.0* (GLfloat)winWidth / (GLfloat)winHeight, -1.0, 1.0, 1.0, 5.0);
	/* Or we can use gluPerspective */
	/* gluPerspective(45.0, w/h, -10.0, 10.0); */ 
	glMatrixMode(GL_MODELVIEW); 
}
int main(int argc, char **argv) {
	readDate();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Heart");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keys); 
	glutSpecialFunc(specialkeys);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop(); 
	return 0;
}
