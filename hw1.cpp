#include <GL\glut.h>
#include <GL\glu.h>

// 전역 변수 선언
GLfloat tXr = 0.0, tYr = 0.0;   // 사각형에 해당되는 x,y값의 변화
GLfloat tXt = 0.0, tYt = 0.0;   // 삼각형에 해당되는 x,y값의 변화
GLboolean isTie = true, isHat = false;  // 넥타이를 선택했는지 모자(삼각형) 선택했는지에 대한 bool값 
GLfloat Delta = 0.0;    // Timer

GLUquadricObj* qobj = gluNewQuadric();

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);


    // 눈사람 머리
    glColor3f(0.7, 0.0 + Delta, 0.3);  // Timer에 의해서 바뀌어지는 원구의 색
    glutSolidSphere(1.5, 50, 50);

    // 눈사람 몸통
    glColor3f(0.8, 0.0 + Delta, 0.7);
    glTranslatef(0.0, 3.0, 0.0);
    glutSolidSphere(1.2, 50, 50);

    //gluDisk에 대한 설정    //눈사람의 눈
    gluQuadricDrawStyle(qobj, GLU_LINE);    // 어떻게 그릴 건지
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    gluQuadricTexture(qobj, GL_FALSE);

    glColor3f(0.0, 0.0, 0.0);
    glRotatef(0.0, 5.0, 0.0, 0.0);
    glTranslatef(-0.5, 0.5, 0.0);
    gluDisk(qobj,0.05, 0.2, 50, 50);

    glTranslatef(1.0, 0.0, 0.0);
    gluDisk(qobj,0.05, 0.2, 50, 50);

    // 눈사람의 코
    glTranslatef(-0.6, -0.8, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);   // Green
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.2, 0.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);   // Blue
    glVertex3f(0.2, 0.6, 0.0);
    glVertex3f(0.0, 0.6, 0.0);
    glEnd();

    // 눈사람의 입
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(0.0, 5.0, 0.0, 0.0);
    glTranslatef(0.1, -0.3, 0.0);
    gluPartialDisk(qobj, 0.2, 0.3, 50, 50, 90, 180);

    // 눈사람 모자 // 원하는 위치에 조정 가능
    glTranslatef(-1.0, 3.0, 0.0);
    glColor3f(0.3, 0.0 + Delta, 0.7);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.5 + tXt, -0.5 + tYt, 0.0);
    glVertex3f(1.5 + tXt, -0.5 + tYt, 0.0);
    glVertex3f(1.0 + tXt, 0.5 + tYt, 0.0);
    glEnd();


    // 넥타이
    glTranslatef(1.0, -5.4, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.5, 0.3);
    glVertex3f(0.0 + tXr, 0.0 + tYr, 0.0);   // 변화된 변수값을 더해주어서 그림이 움직이는 것처럼 보이게함
    glVertex3f(0.3 + tXr, 0.5 + tYr, 0.0);
    glColor3f(0.7, 0.1, 0.6);
    glVertex3f(0.0 + tXr, 1.5 + tYr, 0.0);
    glVertex3f(-0.3 + tXr, 0.5 + tYr, 0.0);
    glEnd();


    // 내리는 눈
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(60.0, 2.0, 1.5, 0.0);
    glTranslatef(2.0, 4.0 -Delta, 0.0);
    glutSolidSphere(0.2, 50, 50);

    glTranslatef(-3.0, -1.0, 0.0);
    glutSolidSphere(0.2, 50, 50);

    glTranslatef(-1.0, -1.0, 0.0);
    glutSolidSphere(0.2, 50, 50);

    //glFlush();
    glutSwapBuffers();  // 더블버퍼 사용
}

void MyReshape(int w, int h) {      // 창의 크기에 비례해서 창 안의 내용이 모델링 될 수 있고, 또는 창의 크기에 관계없이 창안의 내용은 그대로 유지될 수 있음
    // 창의 크기에 비례해서 창 안의 내용이 모델링
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-2.5, 2.5, -2.5 * (GLfloat)h / (GLfloat)w, 2.5 * (GLfloat)h / (GLfloat)w, -2.5, 2.5);   // 정면에서 보는 코드 

    // 3차원으로 보이게 하는 코드
    gluPerspective(60.0f, 1, 0, 20);    // 카메라의 시야각
    gluLookAt(0, -1.3, 4, 0, 0, 0, 0, 1, 0);   // 카메라의 위치 gluLookAt(x,y,z, ...) 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'Q':
    case 'q':
    case 27:    // ESC키
        exit(0); break;     // window 종료
    }
    glutPostRedisplay();    // 그림 다시 그려줌  // MyDisplay() 간접적 호출 (glut함수로) // MyDisplay();로 호출하면 안됨

}

void MySpecial(int key, int x, int y) {
    switch (key) {
        // 화살표 입력 시 창 안의 도형을 움직임  // 창 안의 도형은 mydisplay가 그림  // 공통된 변수 필요 (전역변수) tX, tY
    case GLUT_KEY_DOWN:    // 아래방향 화살표
        if (isTie)  tYr -= 0.1;         // 넥타이가 선택 됐을 시
        else if (isHat) tYt -= 0.1;     // 모자가 선 택됐을 시
        break;
    case GLUT_KEY_UP:      // 위방향 화살표
        if (isTie)  tYr += 0.1;
        else if (isHat) tYt += 0.1;
        break;
    case GLUT_KEY_LEFT:    // 왼쪽방향 화살표
        if (isTie)  tXr -= 0.1;
        else if (isHat) tXt -= 0.1;
        break;
    case GLUT_KEY_RIGHT:   // 오른쪽방향 화살표
        if (isTie)  tXr += 0.1;
        else if (isHat) tXt += 0.1;
        break;
    }
    glutPostRedisplay();    // MyDisplay(); 호출  // 그림 다시 그려줌
}

void MyModelMenu(int entryID) {
    if (entryID == 1) {     // 사각형 선택
        isTie = true;
        isHat = false;
    }
    else if (entryID == 2) {    // 삼각형 선택
        isTie = false;
        isHat = true;
    }
    glutPostRedisplay();
}

void MyMainMenu(int entryID) {
    if (entryID == 9)    exit(0);   // 서브메뉴는 처리할 필요 없고, AddMenuEntry에 대한 부분만 처리해주면 됨
}

void MyTimer(int Value) {
    Delta += 0.03;
    if (Delta > 2.0)    Delta = 0.0;    // 색(크기)이 일정 숫자가 되면 바뀌지 않기 때문에 다시 색이 바뀌도록 설정해주기 위해서 Delta값을 초기화해줌 --> 반복적으로 실행해줌
    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);  // 또 불러서 별도의 입력을 넣지 않아도 자동적으로 애니메이션 효과됨
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // 애니메이션을 위한 더블버퍼 사용
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(200, 100);

    glutCreateWindow("My First Creative Graphics - OpenGL HW1");
    glClearColor(0.5, 0.5, 0.6, 0.5); // 창의 배경색

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutKeyboardFunc(MyKeyboard);
    glutSpecialFunc(MySpecial);
    GLint MyModelID = glutCreateMenu(MyModelMenu);
    glutAddMenuEntry("넥타이", 1);  // 넥타이에 해당되는 아이디값 넣어줌 = 1 // 아이디값은 중복만 안 되면 됨
    glutAddMenuEntry("모자", 2); // 모자에 해당되는 ID값 = 2   // 중복되지 않게 선언

    glutCreateMenu(MyMainMenu);
    glutAddSubMenu("모델", MyModelID);
    glutAddMenuEntry("Exit", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // 마우스의 오른쪽 버튼에 붙임

    glutTimerFunc(40, MyTimer, 1);    // 40mesc // 얼마만큼 기다릴 것인가에 대한 값 넣어줌
    glutMainLoop();
    return 0;
}
