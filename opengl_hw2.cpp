#include <stdio.h>
#include <GL\glaux.h>
#include <GL\glut.h>
#include <GL\glu.h>
// 추가 종속성에 glaux.lib 추가

#define TEXTURE_NUM 3   // 텍스쳐 개수 3개    // 원하는 갯수만큼 설정 가능 
GLuint texture[TEXTURE_NUM];
// 이미지를 코드가 있는 폴더에 저장
char texture_name[TEXTURE_NUM][20] = { "floor.bmp","wall.bmp", "Multischool.bmp" };    // 파일의 이름을 배열로 선언해서 배열에 해당되는 변수를 통해서 읽음     // 각 도형마다 다른 텍스처 이미지를 넣고 싶을 때, 이렇게 여러개의 텍스처 이미지를 배열로 선언
float g_fDistance = -4.5f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;

static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 텍스쳐 매핑을 하고 싶은 텍스쳐를 넣어줘야 함 // 먼저 아랫면을 위한 floor텍스쳐를 넣어줌 
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.51f); glVertex3f(-2.0f, -1.0f, -1.0f);		// 아랫면
    glTexCoord2f(0.336f, 0.51f); glVertex3f(2.0f, -1.0f, -1.0f);
    glTexCoord2f(0.336f, 1.0f); glVertex3f(2.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -1.0f, 1.0f);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, texture[1]);   // texture[1]에 있는 값을 매핑해줌 (wall에 있는 텍스처 매핑)
    glBegin(GL_QUADS);      // 새로운 텍스처를 적용해주고 싶으면 이전과 end로 나누어주고, 새로운 Begin과 end로 묶어줌  

    glTexCoord2f(0.34f, 0.0f); glVertex3f(2.0f, -1.0f, -1.0f);
    glTexCoord2f(0.684f, 0.0f); glVertex3f(-2.0f, -1.0f, -1.0f);		// 뒷면
    glTexCoord2f(0.684f, 0.5f); glVertex3f(-2.0f, 1.0f, -1.0f);
    glTexCoord2f(0.34f, 0.5f); glVertex3f(2.0f, 1.0f, -1.0f);


    glTexCoord2f(0.34f, 0.0f); glVertex3f(2.0f, -1.0f, 1.0f);		// 우측면
    glTexCoord2f(0.684f, 0.0f); glVertex3f(2.0f, -1.0f, -1.0f);
    glTexCoord2f(0.684f, 0.5f); glVertex3f(2.0f, 1.0f, -1.0f);
    glTexCoord2f(0.34f, 0.5f); glVertex3f(2.0f, 1.0f, 1.0f);
   
    glEnd();


    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 여러 학교의 사진이 한 사진에 담겨져 있는 텍스쳐 매핑함  // 각 사진에 대한 범위 설정 후 액자처럼 걸어줌 
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.5f, -0.95f);
    glTexCoord2f(0.565f, 0.0f); glVertex3f(-0.3f, -0.5f, -0.95f);
    glTexCoord2f(0.565f, 0.5f); glVertex3f(-0.3f, 0.5f, -0.95f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, 0.5f, -0.95f);

    glTexCoord2f(0.565f, 0.0f); glVertex3f(0.0f, -0.3f, -0.95f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -0.3f, -0.95f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(1.0f, 0.8f, -0.95f);
    glTexCoord2f(0.565f, 0.5f); glVertex3f(0.0f, 0.8f, -0.95f);

    glTexCoord2f(0.7f, 0.55f); glVertex3f(1.2f, -0.7f, -0.95f);
    glTexCoord2f(1.0f, 0.55f); glVertex3f(1.9f, -0.7f, -0.95f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.9f, 0.4f, -0.95f);
    glTexCoord2f(0.7f, 1.0f); glVertex3f(1.2f, 0.4f, -0.95f);
   
    glTexCoord2f(0.0f, 0.55f); glVertex3f(1.95f, -0.5f, -0.8f);		
    glTexCoord2f(0.65f, 0.55f); glVertex3f(1.95f, -0.5f, 0.9f);
    glTexCoord2f(0.65f, 1.0f); glVertex3f(1.95f, 0.5f, 0.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.95f, 0.5f, -0.8f);

    glEnd();

    glutSwapBuffers();
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

int LoadGLTextures() {                                   // 파일을 로드하고 텍스쳐로 변환
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

void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

    glutPostRedisplay();
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL hw2 20181030 신효정");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    LoadGLTextures();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutMainLoop();
}