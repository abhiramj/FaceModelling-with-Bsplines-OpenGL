
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <algorithm>
//#include <vector>
#include "glm.h"
#define TOLERANCE 0.00001f
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifndef min
float min(float a,float b){
	return a < b ? a : b;
}
#endif
#ifndef round
 float round(float r) {
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
#endif
using namespace std;

//Constants

const int window_width = 600, window_height = 600;
const char * window_title = "Abhiram Jagarlapudi(29015299)";
const float view_width = 5, view_height = 5;
const float PI = 3.1415923565;
const float grid_unit =0.5f;
const float point_size = 10.0f, line_width = 1.0f;
const int uOrder=4;
const int vOrder=4;
const int noKnots=17;
const int uStride=3;
const int vStride=39;
// Offset from control point to display point
const float zDispl=0.1f;
char fname[]="cm.hw3";

const float  animation_step = .5;
const int TIMERMSECS = 20;
//const float faceRad=1.5f;
//const float faceHeight = 3.0f;

// Function definitions
void graphics_init();
void reshape(int , int );
void display();
void keyboard(unsigned char, int, int);
void processSpecialKeys(int , int , int );
void set_cam();
void draw_grid();
void load_face();

// Setting the alpha values while loading the texture.
void load_tex(char* name, int texno, int mode, float alpha);
void mouse(int button, int state, int x, int y );
void motion( int x, int y);
void screen_to_point(int x,int y,float d[3]);
void assign_random();
int color_code_pick(int x,int y);
float frand();
void load_control_points(char*, bool);
void save_control_points(char*);
void load_tex_control_points();
void output();
void make_smile();
void animate(int t);
void load_strap();
// Vars
GLfloat control_pts[169][3];
GLfloat point_color[169][3];
GLfloat texture_control_pts[169][3];
/* counter clockwise */
GLfloat edgePt[5][2] = {{0,0}, {13,0}, {13, 13}, {0, 13}, {0, 0}};
/* clockwise */ 
   GLfloat curvePt[4][2] = /* clockwise */ 
      {{5.1, 3.55}, {5.1, 3.8}, {5.7, 3.8}, {5.7, 3.55}};
   GLfloat curveKnots[8] = 
      {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
   GLfloat pwlPt[4][2] = /* clockwise */ 
      {{5.7, 3.55}, {5.45, 3.4}, {5.1, 3.55}};
    GLfloat curvePt2[4][2] = /* clockwise */ 
	{{6.4, 3.8}, {6.4, 4.1}, {7.0, 4.1}, {7.0, 3.8}};
	GLfloat pwlPt2[4][2] = /* clockwise */ 
      {{7.0, 3.8}, {6.75, 3.7}, {6.4, 3.8}};


//GLfloat hair[4][3]={{control_pts[94][0],control_pts[94][1],control_pts[94][2]},{control_pts[81][0],control_pts[81][1],control_pts[81][2]},{control_pts[68][0],control_pts[68][1],control_pts[68][2]},{control_pts[42][0],control_pts[42][1],control_pts[42][2]}};
int rotX=0;
int rotY=0;
int rotZ=0;
GLMmodel* pmodel1 = NULL;
GLMmodel* spec=NULL;
GLMmodel* scalp=NULL;
bool drawObj=false;
bool gridDrawn=false;
GLUnurbs* myface= NULL;
GLUquadric* head = NULL; 
bool drawNurbs = false;
float knots[17]={0,0,0,0,1,2,3,4,5,6,7,8,9,10,10,10,10};
int img_width,img_height;
static GLuint texid[4];
float previous_mouse_point[3];
int current_point_i=-1;
bool zPointMoved=false;
bool isPointInit=false;
bool animation_enabled=false;
GLfloat strap[15][3];
GLfloat strap_orig[15][3];
GLfloat strap_final[15][3];
GLfloat smile_pts[10][3]={{0.490933,-0.6728,4},{0,-1,4},{1,-1,4},{0.5,-1.14667,4},{-0.093333,-1.3933,4},{0.933333,-

1.39333,4},{0.5,-1.76667,4},{-0.173333,-1.74667,4},{0.5,-2.26667,4},{0.950745,-1.76,4}};
GLfloat nonsmile_pts[10][3];
float u=0;
bool getHair=false;
bool drawSpec=false;
bool animateSpec=false;
float v=0;

GLfloat hairBezPoints[15][3];

GLfloat specPoint0_orig[3]={0.5,-1.5,-2.1} ;
GLfloat specPoint0_final[3];
GLfloat specPoint0[3]={0.5,-1.5,-2.1} ;



void graphics_init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4,4,-4,4,-4,4);
	glPointSize(point_size);
	glLineWidth(line_width);
	assign_random();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	// Antialiasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MAP1_VERTEX_3);
	glShadeModel(GL_SMOOTH);


	//Light

	GLfloat light_position[] = { 1, 1, 1, 0 };
	 GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[]={0.2,0.2,0.2,0.2};
	GLfloat light_direction[]={0,0,0};

   
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light_direction);
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	

	// Nurbs surface 
	myface= gluNewNurbsRenderer();
	head = gluNewQuadric();
	gluQuadricTexture( head, GL_TRUE );
	gluQuadricNormals(head,GL_SMOOTH);
	gluQuadricDrawStyle(head,GL_FILL);
	gluNurbsProperty(myface, GLU_SAMPLING_TOLERANCE, 5.0);

	
	glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3,0.0,1.0,3,3,0.0,1.0,9,5,&strap[0][0]);
	glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);

}

void reshape(int width, int height){
	int unit = min(width/view_width,height/view_height);
	int wp =(width-view_width*unit)/2, hp = (height-view_height*unit)/2;
	glViewport(wp,hp,view_width*unit,view_height*unit);
	glutPostRedisplay();

}
void load_strap(){
float theta=0;
float r=0.10;
float z=0.0;
int p=0;
for (;z<2.5;z+=0.5){
	for (theta=0.0;theta<360;theta+=130){
		if (z<=1){
	strap[p][1]=-z+r*cos(theta*(M_PI/180.0));
	strap[p][2]=-z+r*sin(theta*(M_PI/180.0));
	strap_orig[p][1]=-z+r*cos(theta*(M_PI/180.0));
	strap_orig[p][2]=-z+r*sin(theta*(M_PI/180.0));
		}
		else {
	strap[p][1]=(z-2)+r*cos(theta*(M_PI/180.0));
	strap[p][2]=(z-2)+r*sin(theta*(M_PI/180.0));
	strap_orig[p][1]=(z-2)+r*cos(theta*(M_PI/180.0));
	strap_orig[p][2]=(z-2)+r*sin(theta*(M_PI/180.0));
		}
	strap[p][0]=z;
	strap_orig[p][0]=z;
	p++;
	}

}
}

void assign_random(){
for (int i=0;i<169;i++){
		point_color[i][0]=rand()%255;
		point_color[i][1]=rand()%255;
		point_color[i][2]=rand()%255;
	}

}

void load_control_points(char* fname,bool isFile){
	FILE *fp=NULL;
	if (isFile){
	fp=fopen(fname,"r");
	}
	// Control points
	int p=0;

	if (fp==NULL && !isPointInit){
	for (float j=-6*grid_unit;j<=6*grid_unit;j+=grid_unit){
		for (float i=-6*grid_unit;i<=6*grid_unit;i+=grid_unit){
			control_pts[p][0]=i;
			control_pts[p][1]=j;
			control_pts[p][2]=0;
			p++;
		}
	}
	isPointInit=true;
	}
	else {
		float x[3];
		do {
		int y=fscanf(fp,"%f	%f	%f",&x[0],&x[1],&x[2]);
		if (y!=3){
				cout<<"Faulty file. Press l to reload "<<y<<endl;
				isPointInit=false;
				break;
		}
		control_pts[p][0]=x[0];
		control_pts[p][1]=x[1];
		control_pts[p][2]=x[2];
		p++;
		} while (p<169);
	
	}
	if (fp!=NULL)
	fclose(fp);
}

void save_control_points(char* fname){
	FILE* fp=fopen(fname,"w");
	for (int i=0;i<169;i++){
		fprintf(fp,"%f	%f	%f\n",control_pts[i][0],control_pts[i][1],control_pts[i][2]);
	}
	fclose(fp);
}

void load_tex_control_points(){
	int p=0;
	
for (float j=0;j<=12;j+=1){
		for (float i=0;i<=12;i+=1){
			texture_control_pts[p][0]=j/12;
			texture_control_pts[p][1]=i/12;
			texture_control_pts[p][2]=0;
			p++;
		}
	}
}


void keyboard(unsigned char key, int x, int y){
	switch(key){
	
	case 'r':
		rotX=0;
		rotY=0;
		rotZ=0;
		graphics_init();
		glutPostRedisplay();
		
		break;

	case 't':
		rotY=0;
		rotX=-90;
	break;
	case 'f':
		rotX=0;
		rotY=0;
	break;
	case 'm':
		rotX=-45;
		rotY=-30;
		break;
	case 'c':
		gridDrawn=!gridDrawn;
		break;
	case 'n':
		drawNurbs=!drawNurbs;
		
		break;
	case 'l':
		load_control_points(fname,true);
		glColor3f(0.0f,1.0f,0.0f);
		draw_grid();
		glutPostRedisplay();
		nonsmile_pts[0][0]=control_pts[72][0];
		nonsmile_pts[0][1]=control_pts[72][1];
		

		nonsmile_pts[1][0]=control_pts[58][0];
		nonsmile_pts[1][1]=control_pts[58][1];
		
		
		nonsmile_pts[2][0]=control_pts[60][0];
		nonsmile_pts[2][1]=control_pts[60][1];
		
		
		nonsmile_pts[3][0]=control_pts[59][0];
		nonsmile_pts[3][1]=control_pts[59][1];
		
		
		nonsmile_pts[4][0]=control_pts[45][0];
		nonsmile_pts[4][1]=control_pts[45][1];
		
		
		nonsmile_pts[5][0]=control_pts[47][0];
		nonsmile_pts[5][1]=control_pts[47][1];
		
		
		nonsmile_pts[6][0]=control_pts[46][0];
		nonsmile_pts[6][1]=control_pts[46][1];
		
		
		nonsmile_pts[7][0]=control_pts[32][0];
		nonsmile_pts[7][1]=control_pts[32][1];
		
		
		nonsmile_pts[8][0]=control_pts[33][0];
		nonsmile_pts[8][1]=control_pts[33][1];
		
		
		nonsmile_pts[9][0]=control_pts[34][0];
		nonsmile_pts[9][1]=control_pts[34][1];
		
		
		break;
	case 's':
		save_control_points(fname);
		break;
	case 27:
		exit(0);
		break;
	case '1':
		drawObj=!drawObj;
		break;
	case ' ':
		animation_enabled=true;
		break;

	case 'h': 
		getHair=(!getHair);
		break;

	case 'p':
		drawSpec=(!drawSpec);
		break;
	}
}

void processSpecialKeys(int key, int x, int y){
	int k=glutGetModifiers();
	switch (key){

	case GLUT_KEY_UP:
		
			rotX-=5;
			break;
	
	case GLUT_KEY_LEFT:
		
		rotY-=5;
		break;

	case GLUT_KEY_RIGHT:
		
			rotY+=5;
			break;


	case GLUT_KEY_DOWN:
		
			rotX+=5;
			break;

	
	}
}

void mouse(int button, int state, int x, int y ){

		if(state == GLUT_DOWN){
		screen_to_point(x,y,previous_mouse_point);
		cout <<"X is :"<<x<<"  Y is :"<<y<<endl;
		current_point_i = color_code_pick(x,y);
		if (current_point_i<169){
		cout<<control_pts[current_point_i][0]<<endl;
		cout<<control_pts[current_point_i][1]<<endl;
		}
		int mod=glutGetModifiers();
		 if (current_point_i!=-1){
			switch (mod){
			case GLUT_ACTIVE_SHIFT:
				zPointMoved=true;
				break;
			}
		
	}else{
		current_point_i = -1;
	}
}
		if (state==GLUT_UP){
		zPointMoved=false;
		specPoint0_final[0]=specPoint0[0];
		specPoint0_final[1]=specPoint0[1];
		specPoint0_final[2]=specPoint0[2];


		for(int i=0;i<15;i++){
			for(int j=0;j<3;j++){
			strap_final[i][j]=strap[i][j];
			}
		}

		animateSpec=true;
		}
}

void motion( int x, int y){
	float current_mouse_point[3];
	screen_to_point(x,y,current_mouse_point);
	float dx = current_mouse_point[0] - previous_mouse_point[0];
	float dy = current_mouse_point[1] - previous_mouse_point[1];
	//float dz = current_mouse_point[2] - previous_mouse_point[2];
		if(current_point_i != -1){
			if (zPointMoved){
			if (dy>0){
				if (current_point_i<169)
				control_pts[current_point_i][2]+=sqrt(pow(dx,2)+pow(dy,2));
				else {
					float sum=sqrt(pow(dx,2)+pow(dy,2));
					specPoint0[2]+=sum;

					strap[3][2]+=(0.5*sum);
					strap[4][2]+=(0.5*sum);
					strap[5][2]+=(0.5*sum);

					strap[6][2]+=(sum);
					strap[7][2]+=(sum);
					strap[8][2]+=(sum);

					strap[9][2]+=(0.5*sum);
					strap[10][2]+=(0.5*sum);
					strap[11][2]+=(0.5*sum);

				}
			}
			else {
				if (current_point_i<169)
				control_pts[current_point_i][2]-=sqrt(pow(dx,2)+pow(dy,2));
				else{
					float sum =sqrt(pow(dx,2)+pow(dy,2));
					specPoint0[2]-= sum;

					strap[3][2]-=(0.5*sum);
					strap[4][2]-=(0.5*sum);
					strap[5][2]-=(0.5*sum);

					strap[6][2]-=(sum);
					strap[7][2]-=(sum);
					strap[8][2]-=(sum);

					strap[9][2]-=(0.5*sum);
					strap[10][2]-=(0.5*sum);
					strap[11][2]-=(0.5*sum);

				}
			}
			}
			else {
				if (current_point_i<169){
				control_pts[current_point_i][0] += dx;
				control_pts[current_point_i][1] += dy;
				}
				else {
				specPoint0[0]+=dx;
				specPoint0[1]+=dy;

					strap[3][0]+=(0.5*dx);
					strap[4][0]+=(0.5*dx);
					strap[5][0]+=(0.5*dx);

					strap[6][0]+=dx;
					strap[7][0]+=dx;
					strap[8][0]+=dx;

					strap[9][0]+=(0.5*dx);
					strap[10][0]+=(0.5*dx);
					strap[11][0]+=(0.5*dx);

					strap[3][1]+=(0.5*dy);
					strap[4][1]+=(0.5*dy);
					strap[5][1]+=(0.5*dy);

					strap[6][1]+=dy;
					strap[7][1]+=dy;
					strap[8][1]+=dy;

					strap[9][1]+=(0.5*dy);
					strap[10][1]+=(0.5*dy);
					strap[11][1]+=(0.5*dy);
				}
			}
		//control_pts[current_point_i][2] += dz;
		glutPostRedisplay();
	
	}
	copy(current_mouse_point,current_mouse_point+3,previous_mouse_point);
}

int color_code_pick(int x,int y){

	//glClearColor(0.0f,0.0f,0.0f,1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//glLoadIdentity();
	glPointSize(point_size);	
	glBegin(GL_POINTS);
	for(int i = 0; i < 169; i++){
		float rC=static_cast<float>(point_color[i][0])/255.0f;
		float gC=static_cast<float>(point_color[i][1])/255.0f;
		float bC=static_cast<float>(point_color[i][2])/255.0f;
		glColor3f(rC,gC,bC);
		glVertex3f( control_pts[i][0],control_pts[i][1],control_pts[i][2]+zDispl );
	}
	glEnd();
	glFlush();
	GLubyte data[3];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&data);
	GLubyte ubyteComponent;

	if (230 == (float)data[0] && 51== (float)data[1] && 230 == (float)data[2])
		return 555;

	int i;
	for (i=0;i<169;i++){
		ubyteComponent = static_cast<GLubyte>(point_color[i][0] );
		if (ubyteComponent != data[0])
			continue;
		ubyteComponent = static_cast<GLubyte>(point_color[i][1] );
		if (ubyteComponent != data[1])
			continue;
		ubyteComponent = static_cast<GLubyte>(point_color[i][2]);
		if (ubyteComponent != data[2])
			continue;
		break;
	}
	/*if (i>168) {
		data[0]=(GLubyte) 0;
		data[1]=(GLubyte) 0;
		data[2]=(GLubyte) 0;
		i=-1;
	}*/
	cout<<"Index of point is "<<i<<endl;
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	return i;
}

void screen_to_point(int x,int y,float d[3]){
	// This function will find 2 points in world space that are on the line into the screen defined by screen-space( ie. window-space ) point (x,y)
	double mvmatrix[16];
	double projmatrix[16];
	int viewport[4];
	double dX, dY, dZ, dClickY; // glUnProject uses doubles, but I'm using floats for these 3D vectors

	glGetIntegerv(GL_VIEWPORT, viewport);	
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	int g_WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	dClickY = double (g_WindowHeight - y); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top
	gluUnProject ((double) x, dClickY, 0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);
	//gluUnProject ((double) x, dClickY, 0, dispModMatrix, dispProjMatrix, dispViewMatrix, &dX, &dY, &dZ);
	d[0] = (float) dX;
	d[1] = (float) dY;
	d[2] = (float) dZ;
}
void display(){
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	output();
	set_cam();
	if (gridDrawn){
		glColor3f(0,1,0);
		draw_grid();
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	if (drawObj)
	{
		glDisable(GL_TEXTURE_2D);
		glmUnitize(pmodel1);
		glmDraw(pmodel1, GLM_SMOOTH| GLM_NONE);
		
	}
	if (drawNurbs){
		glEnable(GL_TEXTURE_2D);
		
		
		gluBeginSurface(myface);
		glColor3f(1.0f,1.0f,1.0f);
		glBindTexture(GL_TEXTURE_2D,texid[0]);
		gluBeginTrim (myface);
		gluPwlCurve (myface, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
		gluEndTrim (myface);
		gluBeginTrim (myface);
		gluNurbsCurve (myface, 8, curveKnots, 2, 
                     &curvePt[0][0], 4, GLU_MAP1_TRIM_2);
		gluPwlCurve (myface, 3, &pwlPt[0][0], 2, GLU_MAP1_TRIM_2);
		gluEndTrim (myface);
				gluBeginTrim (myface);
		gluNurbsCurve (myface, 8, curveKnots, 2, 
                     &curvePt2[0][0], 4, GLU_MAP1_TRIM_2);
		gluPwlCurve (myface, 3, &pwlPt2[0][0], 2, GLU_MAP1_TRIM_2);
		gluEndTrim (myface);
		
		gluNurbsSurface(myface,noKnots,&knots[0],noKnots,&knots[0],uStride,vStride,&texture_control_pts[0]

[0],uOrder,vOrder,GL_MAP2_TEXTURE_COORD_2);
		gluNurbsSurface(myface,noKnots,&knots[0],noKnots,&knots[0],uStride,vStride,&control_pts[0]

[0],uOrder,vOrder,GL_MAP2_VERTEX_3);
		gluNurbsSurface(myface,noKnots,&knots[0],noKnots,&knots[0],uStride,vStride,&control_pts[0]

[0],uOrder,vOrder,GL_MAP2_NORMAL);
		
		gluEndSurface(myface);
		glBindTexture(GL_TEXTURE_2D,texid[2]);
		glPushMatrix();
		glTranslatef(0.23f,-0.7,0.18f);
		glRotatef(5,0,0,1);
		glScalef(1.6f,1.0f,1.0f);
		glRotatef(90,0,0,1);
		glRotatef(80,-1,0,0);
		gluSphere(head,0.09,10,10);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0.78f,-0.6,0.18f);
		//glRotatef(5,0,0,1);
		glScalef(1.6f,1.0f,1.0f);
		glRotatef(90,0,0,1);
		glRotatef(80,-1,0,0);
		gluSphere(head,0.09,10,10);
		
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

		
	}
	if (getHair){
		glBindTexture(GL_TEXTURE_2D,texid[3]);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();


		/*glPushMatrix();
		glmUnitize(scalp);
		glTranslatef(0.5,-0.3,specPoint0[2]+1.25);
		glScalef(1.3,2,1.5);
		glRotatef(140,0,1,0);
		glmDraw(scalp, GLM_SMOOTH);
		glPopMatrix();
		*/











		//For cylinder
		//glTranslatef(0.5,1,-1);
		/*glEnable(GL_MAP1_VERTEX_3);
		glColor3f(0,1,0);
		glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= 30; i++) 
         glEvalCoord1f((GLfloat) i/30.0);
   glEnd();*/








		// This is texture hair
		glEnable(GL_TEXTURE_2D);
		
	//glRotatef(90,1,0,0);
	 //gluCylinder(head,1.1,1.23,1.45,20,20);
		//For sphere
glTranslatef(0.5,0.55,-1.01);
glDisable(GL_BLEND);

	gluSphere(head,1.27,20,20);
		glEnable (GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE);
		glColor4f(0,1,1,0.8);
	gluSphere(head,1.3,20,20);
	glColor4f(0,1,1,0.2);
	gluSphere(head,1.3,20,20);
	glDisable (GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
	glTranslatef(0.5,-0.4,-1.5);
	glDisable(GL_BLEND);
	gluSphere(head,1.27,20,20);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glColor4f(0,1,1,0.8);
	glBlendFunc(GL_ONE,GL_ONE);
	gluSphere(head,1.3,20,20);
	glColor4f(0,1,1,0.2);
	gluSphere(head,1.3,20,20);
	glDisable (GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	
	
	glPopMatrix();

	}
	if (drawSpec){
		glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glmUnitize(spec);
		glTranslatef(0.5f,-0.6,-0.5f);
		glRotatef(-90,1,0,0);
		glmDraw(spec, GLM_SMOOTH| GLM_NONE);
		glPopMatrix();
		glDisable(GL_LIGHTING);



		
		glEnable(GL_MAP2_VERTEX_3);
		glMap2f(GL_MAP2_VERTEX_3,0.0,1.0,3,3,0.0,1.0,9,5,&strap[0][0]);
		

		glBegin(GL_POINTS);
		glColor3f(0.9,0.2,0.9);
		glVertex3fv(specPoint0);
		glEnd();

		glPushMatrix();
		
		glTranslatef(-0.5,-0.75,-1.25);
		//glRotatef(-100,1,0,0);
		//glRotatef(90,0,1,0);
		glEnable(GL_LIGHTING);
		glColor3f(0,1,0);
		glEvalMesh2(GL_FILL,0,30,0,30);
		glPopMatrix();
		
		
		}

	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();

}


void set_cam(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glScalef(0.5,0.5,0.5);
	glRotatef(rotX,1,0,0);
	glRotatef(rotY,0,1,0);
	
	
}

void output()
{
	float x=-4;
	float y=3.75;
  glColor3f( 0, 1, 0 );
  glRasterPos2f(x, y);
  int len, i;
  char out[100];
  char lat[10];
  char longit[10];
  out[0]='\0';
  strcat(out,"Latitude : ");
  sprintf(lat,"%d\n",rotX);
  strcat(out,lat);
  strcat(out,"Longitude : ");
  sprintf(longit,"%d\n",rotY);
  strcat(out,longit);
  strcat(out,"Nurbs Surface : ");
  if (drawNurbs)
	  strcat(out,"Visible\n");
  else strcat(out,"Not Visible\n");
  len = (int)strlen(out);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, out[i]);
  }
}
void load_face(){

pmodel1 = glmReadOBJ("abhiram.obj"); 
if (!pmodel1){

	cout<<"Face with name : abhiram.obj  could not load"<<endl;
	exit(0);
}
spec = glmReadOBJ("spec2.obj"); 
if (!spec){

	cout<<"Face with name : spec.obj could not load"<<endl;
	exit(0);
}

scalp = glmReadOBJ("scalp.obj"); 
if (!scalp){

	cout<<"Face with name : scalp.obj  could not load"<<endl;
	exit(0);
}

	return ;
}


void load_tex(char* name, int i,int mode , float alpha){
	GLubyte* data=0;
	data=glmReadPPM(name,&img_width,&img_height);
	if (!data){
	cout<<"Texture with name :"<<name<<"  could not load"<<endl;
	exit(0);
	}
	glGenTextures(1,&texid[i]);

		glBindTexture(GL_TEXTURE_2D,texid[i]);	
		if (mode==0){
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		}
		else {
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glEnable(GL_TEXTURE_2D);
	
	
return ;

}

void draw_grid(){
	glDisable(GL_TEXTURE_2D);
	for (int q=0;q<13;q++){
	for (int p=0;p<12;p++){
		glBegin(GL_LINE_STRIP);
		glVertex3f(control_pts[(13*q)+p][0],control_pts[(13*q)+p][1],control_pts[(13*q)+p][2]+zDispl);
		glVertex3f(control_pts[(13*q)+p+1][0],control_pts[(13*q)+p+1][1],control_pts[(13*q)+p+1][2]+zDispl);
		glEnd();
	}
	}
	for (int q=0;q<12;q++){
	for (int p=0;p<13;p++){
		glBegin(GL_LINE_STRIP);
		glVertex3f(control_pts[p+(13*q)][0],control_pts[p+(13*q)][1],control_pts[p+(13*q)][2]+zDispl);
		glVertex3f(control_pts[p+(13*(q+1))][0],control_pts[p+(13*(q+1))][1],control_pts[p+(13*(q+1))][2]+zDispl);
		glEnd();
	}
	}
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
	for (int i=0;i<168;i++){
		//float rC=static_cast<float>(point_color[i][0])/255.0f;
		//float gC=static_cast<float>(point_color[i][1])/255.0f;
		//float bC=static_cast<float>(point_color[i][2])/255.0f;
		//glColor3f(rC,gC,bC);
		glVertex3f(control_pts[i][0],control_pts[i][1],control_pts[i][2]+zDispl);
	}
	glEnd();
	//glEnable(GL_TEXTURE_2D);
}

int main(int argc , char* argv[]){
srand(time(NULL));
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_ALPHA);
glutInitWindowSize(window_width,window_height);
glutCreateWindow(window_title);
	load_strap();
	load_face();
	//load_face(,"abhiram.obj");
	load_tex("abhiram.ppm",0, 0, 0);
	load_tex("hair.ppm",1, 0, 0);
	load_tex("eyeball3.ppm",2,0, 0);
	load_tex("hair2.ppm",3,0,0);
	load_control_points(fname,false);
	load_tex_control_points();
	
	//glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3,4, &hair[0][0]);
	//Initializing the trim points
	graphics_init();
	glutReshapeFunc(&reshape);
	glutDisplayFunc(&display);
	glutKeyboardFunc(&keyboard);
	glutSpecialFunc(&processSpecialKeys);
	glutMouseFunc(&mouse);
	glutMotionFunc(&motion);
	glutIdleFunc(&display);
	glutTimerFunc(TIMERMSECS, animate, 0);
	// main loop
	glutMainLoop();
	return 0;	
}

void animate(int t){
	float time_elapsed = TIMERMSECS/1000.0;
	float current_step = animation_step* time_elapsed;

	glutTimerFunc(TIMERMSECS, animate, 0);

	if(animation_enabled){
		//if(current_step < animation_step*2) // we never want a big step
		//{
			// Rotate all points by the given step
		//control_pts[72][0]=((1-u)*nonsmile_pts[0][0]+(u)*smile_pts[0][0]);
		//control_pts[72][1]=((1-u)*nonsmile_pts[0][1]+(u)*smile_pts[0][1]);
		
		
		control_pts[58][0]=((1-u)*nonsmile_pts[1][0]+(u)*smile_pts[1][0]);
		control_pts[58][1]=((1-u)*nonsmile_pts[1][1]+(u)*smile_pts[1][1]);
		
		
		control_pts[60][0]=((1-u)*nonsmile_pts[2][0]+(u)*smile_pts[2][0]);
		control_pts[60][1]=((1-u)*nonsmile_pts[2][1]+(u)*smile_pts[2][1]);
		
		
		control_pts[59][0]=((1-u)*nonsmile_pts[3][0]+(u)*smile_pts[3][0]);
		control_pts[59][1]=((1-u)*nonsmile_pts[3][1]+(u)*smile_pts[3][1]);
		
		
		control_pts[45][0]=((1-u)*nonsmile_pts[4][0]+(u)*smile_pts[4][0]);
		control_pts[45][1]=((1-u)*nonsmile_pts[4][1]+(u)*smile_pts[4][1]);
		
		
		control_pts[47][0]=((1-u)*nonsmile_pts[5][0]+(u)*smile_pts[5][0]);
		control_pts[47][1]=((1-u)*nonsmile_pts[5][1]+(u)*smile_pts[5][1]);
		
		
		control_pts[46][0]=((1-u)*nonsmile_pts[6][0]+(u)*smile_pts[6][0]);
		control_pts[46][1]=((1-u)*nonsmile_pts[6][1]+(u)*smile_pts[6][1]);
		
		
		control_pts[32][0]=((1-u)*nonsmile_pts[7][0]+(u)*smile_pts[7][0]);
		control_pts[32][1]=((1-u)*nonsmile_pts[7][1]+(u)*smile_pts[7][1]);
		
		
		control_pts[33][0]=((1-u)*nonsmile_pts[8][0]+(u)*smile_pts[8][0]);
		control_pts[33][1]=((1-u)*nonsmile_pts[8][1]+(u)*smile_pts[8][1]);
		
		
		control_pts[34][0]=((1-u)*nonsmile_pts[9][0]+(u)*smile_pts[9][0]);
		control_pts[34][1]=((1-u)*nonsmile_pts[9][1]+(u)*smile_pts[9][1]);
		
		

		
		u+=0.08;
		if (u>=1){
			animation_enabled=false;
			u=0;
			/*
			control_pts[72][0]=nonsmile_pts[0][0];
			control_pts[72][0]=nonsmile_pts[0][1];
			control_pts[72][0]=nonsmile_pts[0][2];
		*/
		}

		
		//}
	}
	if (animateSpec){

			specPoint0[0]=((1-v)*specPoint0_final[0])+(v*specPoint0_orig[0]);
			specPoint0[1]=((1-v)*specPoint0_final[1])+(v*specPoint0_orig[1]);
			specPoint0[2]=((1-v)*specPoint0_final[2])+(v*specPoint0_orig[2]);
			for (int i=0;i<15;i++){
				for(int j=0;j<3;j++){
					strap[i][j]=((1-v)*strap_final[i][j])+v*strap_orig[i][j];
				}
			}

			v+=0.1;

		if (v>=1){
			animateSpec=false;
			v=0;
		}
		
		
		}
	glutPostRedisplay();
}