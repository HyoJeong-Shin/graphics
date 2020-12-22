#include <windows.h>          
#include <stdio.h>    
#include <math.h>  
#include <GL\glaux.h>
#include <GL\glut.h>
#include <GL\glu.h>
#include "tga.h"

#define TEXTURE_NUM 7   // 텍스쳐 개수 7개    // 원하는 갯수만큼 설정 가능 
GLuint texture[TEXTURE_NUM];
// 이미지를 코드가 있는 폴더에 저장
char texture_name[TEXTURE_NUM][20] = { "forestfloor.bmp","forest.bmp", "white.bmp", "skin.bmp", "clothes.bmp", "Particle.bmp", "warning.bmp" };    // 파일의 이름을 배열로 선언해서 배열에 해당되는 변수를 통해서 읽음     // 각 도형마다 다른 텍스처 이미지를 넣고 싶을 때, 이렇게 여러개의 텍스처 이미지를 배열로 선언
float g_fDistance = -3.5f;

// 전역 변수 선언
GLboolean isPlay = false;   // 왼쪽 오른쪽 마우스 클릭 변수 
GLfloat Delta = 0.0;    // Timer
GLUquadricObj* qobj = gluNewQuadric();  // 쿼드릭 객체 생성 

// 시점 이동 변수
double posx, posy, posz;


// 파티클 변수
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


// render 디스플레이 전역 변수
GLuint g_textureID = -1;

bool g_bBlending = true;
bool g_bSortUsingCullModeTrick = true;

GLuint     filter;

struct Vertex {
    float tu, tv;
    float x, y, z;
};


Vertex g_Vertices[] = { // 1개의 정사각형을 그려줌  // 정사각형 대해서 매핑을 시켜줌
    { 0.0f,0.0f, -1.0f,-1.0f, 1.0f },
    { 1.0f,0.0f,  1.0f,-1.0f, 1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f, 1.0f },
    { 0.0f,1.0f, -1.0f, 1.0f, 1.0f },
    
};

void MyReshape(int w, int h) {  // 창의 크기에 비례해서 창 안의 내용이 모델링 될 수 있고, 또는 창의 크기에 관계없이 창안의 내용은 그대로 유지될 수 있음
    // 창의 크기에 비례해서 창 안의 내용이 모델링
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 3차원으로 보이게 하는 코드
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);  // 카메라의 시야각

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
}

void MyDisplay1() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
   
    glShadeModel(GL_SMOOTH);
 
    glEnable(GL_TEXTURE_2D);
   
    glLoadIdentity();
    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    // 키보드 키에 의해 카메라(시점) 이동함

    glTranslatef(0.0f, 0.0f, g_fDistance);

    // 사람이 피고 버리는 담배
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 텍스쳐 매핑을 하고 싶은 텍스쳐를 넣어줘야 함 // 먼저 담배 표현을 위한 white텍스쳐를 넣어줌 
    glTranslatef(0.0, -1.4 - Delta, 0.0);       // TImer에 의해서 바뀌어지는 담배(실린더)의 위치
    glRotatef(10, 0.0, 1.0, 0.0);
    gluCylinder(qobj, 0.02, 0.02, 0.4, 40, 8);  // 원기둥
    glPopMatrix();

    // 사람 머리
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[3]);   // 사람의 피부 표현을 위해 살구색 텍스쳐를 넣어줌
    glTranslatef(0.0, -1.4, 0.0);
    glutSolidSphere(0.2, 50, 50);   // 구
    glPopMatrix();

    // 사람 몸
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);   // 옷 표현을 위해 색깔 있는 텍스쳐를 넣어줌
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(0.3, 0.6, 50, 50);    // 원뿔
    glPopMatrix();


    // 배경
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 텍스쳐 매핑을 하고 싶은 텍스쳐를 넣어줘야 함 // 먼저 아랫면을 위한 forestfloor텍스쳐를 넣어줌 
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -2.0f, -1.0f);		// 아랫면
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f, 1.0f);

    glEnd();
    glPopMatrix();


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // texture[1]에 있는 값을 매핑해줌 (forest 텍스처 매핑)
    glBegin(GL_QUADS);      // 새로운 텍스처를 적용해주고 싶으면 이전과 end로 나누어주고, 새로운 Begin과 end로 묶어줌  

    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);		// 뒷면
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);


    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, 1.0f);		// 우측면
    glTexCoord2f(1.0f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, 1.0f);

    glTexCoord2f(0.05f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);	// 좌측면
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, 1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);

    glEnd();
    glPopMatrix();

    // 환경 매핑
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, texture[filter + (filter + 1)]);
    glPushMatrix();
    glRotatef(30, 1.0f, 0.0f, 0.0f);
    glRotatef(30, 0.0f, 1.0f, 0.0f);

    gluSphere(qobj, 0.5f, 32, 32);  // 구 생성     // 숲을 비추는 수정구슬 컨셉

    glPopMatrix();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glutSwapBuffers();  // 더블버퍼 사용
}


void MyDisplay2() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);     // 키보드 키에 의해 카메라(시점) 이동함

    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);

    glTranslatef(0.0f, 0.0f, g_fDistance);

   
    // 불꽃 파티클 함수 
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



    // 배경
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 먼저 아랫면을 위한 forestfloor텍스쳐를 넣어줌 
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -2.0f, -1.0f);		// 아랫면
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f, 1.0f);

    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // texture[1]에 있는 값을 매핑해줌 (forest 텍스처 매핑)
    glBegin(GL_QUADS);      // 새로운 텍스처를 적용해주고 싶으면 이전과 end로 나누어주고, 새로운 Begin과 end로 묶어줌  
   
    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);		// 뒷면
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);


    glTexCoord2f(0.05f, 0.05f); glVertex3f(2.0f, -2.0f, 1.0f);		// 우측면
    glTexCoord2f(1.0f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(2.0f, 2.0f, 1.0f);

    glTexCoord2f(0.05f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);	// 좌측면
    glTexCoord2f(1.0f, 0.05f); glVertex3f(-2.0f, -2.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 2.0f, 1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);

    glEnd();
    glPopMatrix();


    // 사람이 버리고 간 담배 1
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 담배 표현을 위한 white텍스쳐를 넣어줌 
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(10, 0.0, 1.0, 0.0);
    gluCylinder(qobj, 0.02, 0.02, 0.4, 40, 8);  // 원기둥
    glPopMatrix();

    // 사람이 버리고 간 담배 2
    glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 담배 표현을 위한 white텍스쳐를 넣어줌 
    glTranslatef(1.0, -2.0, 0.0);
    glRotatef(10, 0.0, 1.0, 0.0);
    gluCylinder(qobj, 0.02, 0.02, 0.4, 40, 8);  // 원기둥
    glPopMatrix();

    glutPostRedisplay();
    glutSwapBuffers();  // 더블 버퍼 사용
}


void MyDisplay3() {     // 경고 문구 디스플레이
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    // 키보드에 따른 시점 이동

    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);

    glTranslatef(0.0f, 0.0f, g_fDistance);


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[6]);   // texture[6]에 있는 값을 매핑해줌 (경고 문구 텍스처 매핑)
    glBegin(GL_QUADS);      // 새로운 텍스처를 적용해주고 싶으면 이전과 end로 나누어주고, 새로운 Begin과 end로 묶어줌  

    glTexCoord2f(0.05f, 0.05f); glVertex3f(-2.0f, -2.0f, -1.0f);
    glTexCoord2f(1.0f, 0.05f); glVertex3f(2.0f, -2.0f, -1.0f);		
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, 2.0f, -1.0f);
    glTexCoord2f(0.05f, 1.0f); glVertex3f(-2.0f, 2.0f, -1.0f);

    glEnd();
    glPopMatrix();

    glutPostRedisplay();
    glutSwapBuffers();  // 더블 버퍼
}


void render() { // 조심할 경우 보존 되는 맑은 하늘과 단풍 나무 표현 디스플레이   // .tga 파일을 이용하여 투명한 텍스처 매핑
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0 + posx, 0.0 + posy, 3.0 + posz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);    // 키보드에 따라 시점 이동

    glTranslatef(0.0f, 0.0f, g_fDistance);

    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, g_textureID);

    glInterleavedArrays(GL_T2F_V3F, 0, g_Vertices);     // 전역변수로 정의한 g_Vertices 불러와 그림 
    glDrawArrays(GL_QUADS, 0, 24);

    glutSwapBuffers();  // 다중 버퍼 
}

AUX_RGBImageRec* LoadBMP(char* Filename) {
    FILE* File = NULL;
    if (!Filename) return NULL;

    File = fopen(Filename, "r");
    if (File) {
        fclose(File);

        return auxDIBImageLoad(Filename);	     // 파일로부터 메모리로
    }
    return NULL;
}



int LoadGLTextures() {  // 파일을 로드하고 텍스쳐로 변환 (bmp)
    int Status = TRUE, i;
    AUX_RGBImageRec* TextureImage;

    for (i = 0; i < TEXTURE_NUM; i++) {     // 여러개의 텍스처가 들어올 수 있음   // 텍스쳐 개수만큼 반복
        TextureImage = NULL;
        if (TextureImage = LoadBMP(texture_name[i])) {  // LoadBMP를 통해서 auxDIBImageLoad하는 것을 별도의 파일로 만들어줌 // 올바르게 이미지가 로드됐다면, 아래 코드를 통해 텍스쳐 설정을 함 
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

void loadTexture(void){     // 파일을 로드하고 텍스쳐로 변환 (tga)

    tgaImageFile tgaImage;
    tgaImage.load((char*)"maple_tree.tga"); // tga 파일 필요 // (char *)로 캐스트

    glGenTextures(1, &g_textureID);

    glBindTexture(GL_TEXTURE_2D, g_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, tgaImage.m_texFormat,
        tgaImage.m_nImageWidth, tgaImage.m_nImageHeight,
        0, tgaImage.m_texFormat, GL_UNSIGNED_BYTE,
        tgaImage.m_nImageData);
}


void MyInit(void) {     // Display1 디스플레이에 대한 init 함수     
    // 조명 설정
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

    // 환경 매핑 설정
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}


int InitGL(GLvoid) {    // Display2, 3 디스플레이에 대한 init 함수   
    LoadGLTextures();

    // 파티클 설정

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
        particle[loop].xg = 0.0f;   // 불 방향 : 위(y축)로 향함
        particle[loop].yg = 0.6f;
        particle[loop].zg = 0.0f;
    }
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    return TRUE;
}

void init3(void) {  // render 디스플레이에 대한 init함수
    loadTexture();

    glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
}

void MyTimer(int Value) {
    if(isPlay) Delta += 0.007;
    if (Delta > 0.6)    Delta = 0.0;    // 위치가 일정 숫자가 되면 바뀌지 않기 때문에 다시 위치가 바뀌도록 설정해주기 위해서 Delta값을 초기화해줌 --> 반복적으로 실행해줌
    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);  // 또 불러서 별도의 입력을 넣지 않아도 자동적으로 애니메이션 효과됨
}

void MyTimerFunc(int value)
{
    
    if (value == 0) // passed in in main
    {

        MyInit();
        glutTimerFunc(40, MyTimer, 1);    
        
        glutDisplayFunc(MyDisplay1);
        
        glutTimerFunc(10000, MyTimerFunc, 1);   // 10초후, value값을 바꿔주면서 새로운 디스플레이 함수로 변환한다. 
    }
    else if (value == 1)
    {
        InitGL();
        glutDisplayFunc(MyDisplay2);    // Display2로 화면 전환됨
        
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
    case GLUT_KEY_RIGHT:    // 카메라 좌측 이동
        posx -= 0.1f;
        break;
    case GLUT_KEY_LEFT:     // 카메라 우측 이동
        posx += 0.1f;
        break;
    case GLUT_KEY_UP:       // 카메라 위로 이동
        posy -= 0.1f;
        break;

    case GLUT_KEY_DOWN:     // 카메라 아래로 이동
        posy += 0.1f;
        break;

    case GLUT_KEY_PAGE_UP:  // 카메라 줌 인
        if(posz > -0.7f){   // 모든 객체들이 윈도우 영역을 벗어나지 않는 제약조건
            posz -= 0.1f;
        }
        break;

    case GLUT_KEY_PAGE_DOWN:    // 카메라 줌 아웃
        if (posz < 4.0f) {  // 모든 객체들을 제대로 볼 수 있는 최소한의 위치 
            posz += 0.1f;
        }
        break;
    }
    glutPostRedisplay();
}

void MyMouse(int button, int state, int x, int y) {
    if ((state == GLUT_DOWN) )
    {
        // 왼쪽 버튼을 누르면 애니메이션 진행 변수를 true로 설정합니다.
        if (button == GLUT_LEFT_BUTTON)
        {
            isPlay = true;
        }
        // 오른쪽 버튼을 누르면 애니메이션 진행 변수를 false로 설정합니다.
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
    glutInitWindowSize(1024, 768);      // 윈도우 해상도 : 1024*768
    glutCreateWindow("forest fires");

    glutSpecialFunc(MySpecial);
    glutMouseFunc(MyMouse);
    glutReshapeFunc(MyReshape);
   
    glutTimerFunc(1000, MyTimerFunc, 0);
    

    LoadGLTextures();


    glutMainLoop();
}