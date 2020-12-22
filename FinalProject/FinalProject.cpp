#include <windows.h>          
#include <stdio.h>    
#include <math.h>  
#include <GL\glaux.h>
#include <GL\glut.h>
#include <GL\glu.h>
#include "tga.h"

#define TEXTURE_NUM 7   // �ؽ��� ���� 7��    // ���ϴ� ������ŭ ���� ���� 
GLuint texture[TEXTURE_NUM];
// �̹����� �ڵ尡 �ִ� ������ ����
char texture_name[TEXTURE_NUM][20] = { "forestfloor.bmp","forest.bmp", "white.bmp", "skin.bmp", "clothes.bmp", "Particle.bmp", "warning.bmp" };    // ������ �̸��� �迭�� �����ؼ� �迭�� �ش�Ǵ� ������ ���ؼ� ����     // �� �������� �ٸ� �ؽ�ó �̹����� �ְ� ���� ��, �̷��� �������� �ؽ�ó �̹����� �迭�� ����
float g_fDistance = -3.5f;

// ���� ���� ����
GLboolean isPlay = false;   // ���� ������ ���콺 Ŭ�� ���� 
GLfloat Delta = 0.0;    // Timer
GLUquadricObj* qobj = gluNewQuadric();  // ���帯 ��ü ���� 

// ���� �̵� ����
double posx, posy, posz;


// ��ƼŬ ����
#define MAX_PARTICLES   1000                   
float slowdown = 2.0f, xspeed, yspeed, zoom = -40.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;


typedef struct {
    bool    active;
    float   life;
    float   fade;
    float   r;
    float   g;
    float   b;
    float   x;
    float   y;
    float   z;
    float   xi;
    float   yi;
    float   zi;
    float   xg;
    float   yg;
    float   zg;
}
particles;
particles particle[MAX_PARTICLES];


// render ���÷��� ���� ����
GLuint g_textureID = -1;

bool g_bBlending = true;
bool g_bSortUsingCullModeTrick = true;

GLuint     filter;

struct Vertex {
    float tu, tv;
    float x, y, z;
};


Vertex g_Vertices[] = { // 1���� ���簢���� �׷���  // ���簢�� ���ؼ� ������ ������
    { 0.0f,0.0f, -1.0f,-1.0f, 1.0f },
    { 1.0f,0.0f,  1.0f,-1.0f, 1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f, 1.0f },
    { 0.0f,1.0f, -1.0f, 1.0f, 1.0f },
    
};

void MyReshape(int w, int h) {  // â�� ũ�⿡ ����ؼ� â ���� ������ �𵨸� �� �� �ְ�, �Ǵ� â�� ũ�⿡ ������� â���� ������ �״�� ������ �� ����
    // â�� ũ�⿡ ����ؼ� â ���� ������ �𵨸�
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 3�������� ���̰� �ϴ� �ڵ�
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);  // ī�޶��� �þ߰�

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
}

void MyDisplay1() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
   
    glShadeModel(GL_SMOOTH);
 
    glEnable(GL_TEXTURE_2D);
   
    glLoadIdentity();
    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    // Ű���� Ű�� ���� ī�޶�(����) �̵���

    glTranslatef(0.0f, 0.0f, g_fDistance);

    // ����� �ǰ� ������ ���
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // �ؽ��� ������ �ϰ� ���� �ؽ��ĸ� �־���� �� // ���� ��� ǥ���� ���� white�ؽ��ĸ� �־��� 
    glTranslatef(0.0, -1.4 - Delta, 0.0);       // TImer�� ���ؼ� �ٲ������ ���(�Ǹ���)�� ��ġ
    glRotatef(10, 0.0, 1.0, 0.0);
    gluCylinder(qobj, 0.02, 0.02, 0.4, 40, 8);  // �����
    glPopMatrix();

    // ��� �Ӹ�
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[3]);   // ����� �Ǻ� ǥ���� ���� �챸�� �ؽ��ĸ� �־���
    glTranslatef(0.0, -1.4, 0.0);
    glutSolidSphere(0.2, 50, 50);   // ��
    glPopMatrix();

    // ��� ��
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);   // �� ǥ���� ���� ���� �ִ� �ؽ��ĸ� �־���
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(0.3, 0.6, 50, 50);    // ����
    glPopMatrix();


    // ���
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // �ؽ��� ������ �ϰ� ���� �ؽ��ĸ� �־���� �� // ���� �Ʒ����� ���� forestfloor�ؽ��ĸ� �־��� 
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -2.0f, -1.0f);		// �Ʒ���
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f, 1.0f);

    glEnd();
    glPopMatrix();


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // texture[1]�� �ִ� ���� �������� (forest �ؽ�ó ����)
    glBegin(GL_QUADS);      // ���ο� �ؽ�ó�� �������ְ� ������ ������ end�� �������ְ�, ���ο� Begin�� end�� ������  

    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);		// �޸�
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);


    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, 1.0f);		// ������
    glTexCoord2f(1.0f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, 1.0f);

    glTexCoord2f(0.05f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);	// ������
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, 1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);

    glEnd();
    glPopMatrix();

    // ȯ�� ����
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, texture[filter + (filter + 1)]);
    glPushMatrix();
    glRotatef(30, 1.0f, 0.0f, 0.0f);
    glRotatef(30, 0.0f, 1.0f, 0.0f);

    gluSphere(qobj, 0.5f, 32, 32);  // �� ����     // ���� ���ߴ� �������� ����

    glPopMatrix();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glutSwapBuffers();  // ������� ���
}


void MyDisplay2() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);     // Ű���� Ű�� ���� ī�޶�(����) �̵���

    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);

    glTranslatef(0.0f, 0.0f, g_fDistance);

   
    // �Ҳ� ��ƼŬ �Լ� 
    glPushMatrix();
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0, -1.95f, 40.57f);

    for (int loop = 0; loop < MAX_PARTICLES; loop++) {
        if (particle[loop].active) {
            float x = particle[loop].x;
            float y = particle[loop].y;
            float z = particle[loop].z + zoom;

            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);

            glBegin(GL_TRIANGLE_FAN);
            glTexCoord2d(0, 0); glVertex3f(x - 0.05f, y - 0.05f, z);
            glTexCoord2d(1, 0); glVertex3f(x + 0.05f, y - 0.05f, z);
            glTexCoord2d(1, 1); glVertex3f(x + 0.05f, y + 0.05f, z);
            glTexCoord2d(0, 1); glVertex3f(x - 0.05f, y + 0.05f, z);
            glEnd();

            particle[loop].x += particle[loop].xi / (slowdown * 50000);
            particle[loop].y += particle[loop].yi / (slowdown * 50000);
            particle[loop].z += particle[loop].zi / (slowdown * 50000);
            particle[loop].xi += particle[loop].xg;
            particle[loop].yi += particle[loop].yg;
            particle[loop].zi += particle[loop].zg;
            particle[loop].life -= particle[loop].fade;

            if (particle[loop].life < 0.0f) {
                particle[loop].life = 1.0f;
                particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;
                if (loop % 2 == 0) {
                    particle[loop].x = 1.0f;
                    particle[loop].y = 0.0f;
                    particle[loop].z = 0.0f;
                }
                if (loop % 2 == 1) {
                    particle[loop].x = 0.0f;
                    particle[loop].y = 0.0f;
                    particle[loop].z = 0.0f;
                }
                particle[loop].xi = xspeed + float((rand() % 60) - 32.0f);
                particle[loop].yi = yspeed + float((rand() % 60) - 30.0f);
                particle[loop].zi = float((rand() % 60) - 30.0f);
                particle[loop].r = 1.0f;
                particle[loop].g = 0.3f;
                particle[loop].b = 0.2f;
            }
        }
    }
    glPopMatrix();



    // ���
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // ���� �Ʒ����� ���� forestfloor�ؽ��ĸ� �־��� 
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -2.0f, -1.0f);		// �Ʒ���
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f, 1.0f);

    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // texture[1]�� �ִ� ���� �������� (forest �ؽ�ó ����)
    glBegin(GL_QUADS);      // ���ο� �ؽ�ó�� �������ְ� ������ ������ end�� �������ְ�, ���ο� Begin�� end�� ������  
   
    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);		// �޸�
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);


    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, 1.0f);		// ������
    glTexCoord2f(1.0f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, 1.0f);

    glTexCoord2f(0.05f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);	// ������
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, 1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);

    glEnd();
    glPopMatrix();


    // ����� ������ �� ��� 1
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // ��� ǥ���� ���� white�ؽ��ĸ� �־��� 
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(10, 0.0, 1.0, 0.0);
    gluCylinder(qobj, 0.02, 0.02, 0.4, 40, 8);  // �����
    glPopMatrix();

    // ����� ������ �� ��� 2
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // ��� ǥ���� ���� white�ؽ��ĸ� �־��� 
    glTranslatef(1.0, -2.0, 0.0);
    glRotatef(10, 0.0, 1.0, 0.0);
    gluCylinder(qobj, 0.02, 0.02, 0.4, 40, 8);  // �����
    glPopMatrix();

    glutPostRedisplay();
    glutSwapBuffers();  // ���� ���� ���
}


void MyDisplay3() {     // ��� ���� ���÷���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    // Ű���忡 ���� ���� �̵�

    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);

    glTranslatef(0.0f, 0.0f, g_fDistance);


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[6]);   // texture[6]�� �ִ� ���� �������� (��� ���� �ؽ�ó ����)
    glBegin(GL_QUADS);      // ���ο� �ؽ�ó�� �������ְ� ������ ������ end�� �������ְ�, ���ο� Begin�� end�� ������  

    glTexCoord2f(0.05f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);		
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);

    glEnd();
    glPopMatrix();

    glutPostRedisplay();
    glutSwapBuffers();  // ���� ����
}


void render() { // ������ ��� ���� �Ǵ� ���� �ϴð� ��ǳ ���� ǥ�� ���÷���   // .tga ������ �̿��Ͽ� ������ �ؽ�ó ����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    // Ű���忡 ���� ���� �̵�

    glTranslatef(0.0f, 0.0f, g_fDistance);

    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, g_textureID);

    glInterleavedArrays(GL_T2F_V3F, 0, g_Vertices);     // ���������� ������ g_Vertices �ҷ��� �׸� 
    glDrawArrays(GL_QUADS, 0, 24);

    glutSwapBuffers();  // ���� ���� 
}

AUX_RGBImageRec* LoadBMP(char* Filename) {
    FILE* File = NULL;
    if (!Filename) return NULL;

    File = fopen(Filename, "r");
    if (File) {
        fclose(File);

        return auxDIBImageLoad(Filename);	     // ���Ϸκ��� �޸𸮷�
    }
    return NULL;
}



int LoadGLTextures() {  // ������ �ε��ϰ� �ؽ��ķ� ��ȯ (bmp)
    int Status = TRUE, i;
    AUX_RGBImageRec* TextureImage;

    for (i = 0; i < TEXTURE_NUM; i++) {     // �������� �ؽ�ó�� ���� �� ����   // �ؽ��� ������ŭ �ݺ�
        TextureImage = NULL;
        if (TextureImage = LoadBMP(texture_name[i])) {  // LoadBMP�� ���ؼ� auxDIBImageLoad�ϴ� ���� ������ ���Ϸ� ������� // �ùٸ��� �̹����� �ε�ƴٸ�, �Ʒ� �ڵ带 ���� �ؽ��� ������ �� 
            glGenTextures(1, &texture[i]);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY,
                0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
        }
        else {
            MessageBox(NULL, "Image file has a error !", texture_name[i], MB_ICONERROR);
            Status = false;
        }

        if (TextureImage) {
            if (TextureImage->data) free(TextureImage->data);
            free(TextureImage);
        }
    }
    return Status;
}

void loadTexture(void){     // ������ �ε��ϰ� �ؽ��ķ� ��ȯ (tga)

    tgaImageFile tgaImage;
    tgaImage.load((char*)"maple_tree.tga"); // tga ���� �ʿ� // (char *)�� ĳ��Ʈ

    glGenTextures(1, &g_textureID);

    glBindTexture(GL_TEXTURE_2D, g_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, tgaImage.m_texFormat,
        tgaImage.m_nImageWidth, tgaImage.m_nImageHeight,
        0, tgaImage.m_texFormat, GL_UNSIGNED_BYTE,
        tgaImage.m_nImageData);
}


void MyInit(void) {     // Display1 ���÷��̿� ���� init �Լ�     
    // ���� ����
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat model_ambient[] = { 0.5, 0.4, 0.3, 1.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // ȯ�� ���� ����
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}


int InitGL(GLvoid) {    // Display2, 3 ���÷��̿� ���� init �Լ�   
    LoadGLTextures();

    // ��ƼŬ ����

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);

    for (int loop = 0; loop < MAX_PARTICLES; loop++) {
        particle[loop].active = true;
        particle[loop].life = 1.0f;
        particle[loop].fade = float(rand() % 80) / 1000.0f + 0.002f;
        particle[loop].r = 1.0f;
        particle[loop].g = 0.3f;
        particle[loop].b = 0.2f;
        particle[loop].xi = float((rand() % 50) - 26.0f);
        particle[loop].yi = float((rand() % 50) - 25.0f);
        particle[loop].zi = float((rand() % 50) - 25.0f);
        particle[loop].xg = 0.0f;   // �� ���� : ��(y��)�� ����
        particle[loop].yg = 0.6f;
        particle[loop].zg = 0.0f;
    }
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    return TRUE;
}

void init3(void) {  // render ���÷��̿� ���� init�Լ�
    loadTexture();

    glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
}

void MyTimer(int Value) {
    if(isPlay) Delta += 0.007;
    if (Delta > 0.6)    Delta = 0.0;    // ��ġ�� ���� ���ڰ� �Ǹ� �ٲ��� �ʱ� ������ �ٽ� ��ġ�� �ٲ�� �������ֱ� ���ؼ� Delta���� �ʱ�ȭ���� --> �ݺ������� ��������
    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);  // �� �ҷ��� ������ �Է��� ���� �ʾƵ� �ڵ������� �ִϸ��̼� ȿ����
}

void MyTimerFunc(int value)
{
    
    if (value == 0) // passed in in main
    {

        MyInit();
        glutTimerFunc(40, MyTimer, 1);    
        
        glutDisplayFunc(MyDisplay1);
        
        glutTimerFunc(10000, MyTimerFunc, 1);   // 10����, value���� �ٲ��ָ鼭 ���ο� ���÷��� �Լ��� ��ȯ�Ѵ�. 
    }
    else if (value == 1)
    {
        InitGL();
        glutDisplayFunc(MyDisplay2);    // Display2�� ȭ�� ��ȯ��
        
        glutTimerFunc(10000, MyTimerFunc, 2);
        
    }
    else if (value == 2) {

        InitGL();
        glutDisplayFunc(MyDisplay3);

        glutTimerFunc(10000, MyTimerFunc, 3);
    }
    else if (value == 3) {
        init3();
        glutDisplayFunc(render);
    }
}


void MySpecial(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_RIGHT:    // ī�޶� ���� �̵�
        posx -= 0.1f;
        break;
    case GLUT_KEY_LEFT:     // ī�޶� ���� �̵�
        posx += 0.1f;
        break;
    case GLUT_KEY_UP:       // ī�޶� ���� �̵�
        posy -= 0.1f;
        break;

    case GLUT_KEY_DOWN:     // ī�޶� �Ʒ��� �̵�
        posy += 0.1f;
        break;

    case GLUT_KEY_PAGE_UP:  // ī�޶� �� ��
        if(posz > -0.7f){   // ��� ��ü���� ������ ������ ����� �ʴ� ��������
            posz -= 0.1f;
        }
        break;

    case GLUT_KEY_PAGE_DOWN:    // ī�޶� �� �ƿ�
        if (posz < 4.0f) {  // ��� ��ü���� ����� �� �� �ִ� �ּ����� ��ġ 
            posz += 0.1f;
        }
        break;
    }
    glutPostRedisplay();
}

void MyMouse(int button, int state, int x, int y) {
    if ((state == GLUT_DOWN) )
    {
        // ���� ��ư�� ������ �ִϸ��̼� ���� ������ true�� �����մϴ�.
        if (button == GLUT_LEFT_BUTTON)
        {
            isPlay = true;
        }
        // ������ ��ư�� ������ �ִϸ��̼� ���� ������ false�� �����մϴ�.
        else if (button == GLUT_RIGHT_BUTTON)
        {
            isPlay = false;
        }

    }

    glutPostRedisplay();
}


void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1024, 768);      // ������ �ػ� : 1024*768
    glutCreateWindow("forest fires");

    glutSpecialFunc(MySpecial);
    glutMouseFunc(MyMouse);
    glutReshapeFunc(MyReshape);
   
    glutTimerFunc(1000, MyTimerFunc, 0);
    

    LoadGLTextures();


    glutMainLoop();
}