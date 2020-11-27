#include <GL\glut.h>
#include <GL\glu.h>

// ���� ���� ����
GLfloat tXr = 0.0, tYr = 0.0;   // �簢���� �ش�Ǵ� x,y���� ��ȭ
GLfloat tXt = 0.0, tYt = 0.0;   // �ﰢ���� �ش�Ǵ� x,y���� ��ȭ
GLboolean isTie = true, isHat = false;  // ��Ÿ�̸� �����ߴ��� ����(�ﰢ��) �����ߴ����� ���� bool�� 
GLfloat Delta = 0.0;    // Timer

GLUquadricObj* qobj = gluNewQuadric();

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);


    // ����� �Ӹ�
    glColor3f(0.7, 0.0 + Delta, 0.3);  // Timer�� ���ؼ� �ٲ������ ������ ��
    glutSolidSphere(1.5, 50, 50);

    // ����� ����
    glColor3f(0.8, 0.0 + Delta, 0.7);
    glTranslatef(0.0, 3.0, 0.0);
    glutSolidSphere(1.2, 50, 50);

    //gluDisk�� ���� ����    //������� ��
    gluQuadricDrawStyle(qobj, GLU_LINE);    // ��� �׸� ����
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    gluQuadricTexture(qobj, GL_FALSE);

    glColor3f(0.0, 0.0, 0.0);
    glRotatef(0.0, 5.0, 0.0, 0.0);
    glTranslatef(-0.5, 0.5, 0.0);
    gluDisk(qobj,0.05, 0.2, 50, 50);

    glTranslatef(1.0, 0.0, 0.0);
    gluDisk(qobj,0.05, 0.2, 50, 50);

    // ������� ��
    glTranslatef(-0.6, -0.8, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);   // Green
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.2, 0.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);   // Blue
    glVertex3f(0.2, 0.6, 0.0);
    glVertex3f(0.0, 0.6, 0.0);
    glEnd();

    // ������� ��
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(0.0, 5.0, 0.0, 0.0);
    glTranslatef(0.1, -0.3, 0.0);
    gluPartialDisk(qobj, 0.2, 0.3, 50, 50, 90, 180);

    // ����� ���� // ���ϴ� ��ġ�� ���� ����
    glTranslatef(-1.0, 3.0, 0.0);
    glColor3f(0.3, 0.0 + Delta, 0.7);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.5 + tXt, -0.5 + tYt, 0.0);
    glVertex3f(1.5 + tXt, -0.5 + tYt, 0.0);
    glVertex3f(1.0 + tXt, 0.5 + tYt, 0.0);
    glEnd();


    // ��Ÿ��
    glTranslatef(1.0, -5.4, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.5, 0.3);
    glVertex3f(0.0 + tXr, 0.0 + tYr, 0.0);   // ��ȭ�� �������� �����־ �׸��� �����̴� ��ó�� ���̰���
    glVertex3f(0.3 + tXr, 0.5 + tYr, 0.0);
    glColor3f(0.7, 0.1, 0.6);
    glVertex3f(0.0 + tXr, 1.5 + tYr, 0.0);
    glVertex3f(-0.3 + tXr, 0.5 + tYr, 0.0);
    glEnd();


    // ������ ��
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(60.0, 2.0, 1.5, 0.0);
    glTranslatef(2.0, 4.0 -Delta, 0.0);
    glutSolidSphere(0.2, 50, 50);

    glTranslatef(-3.0, -1.0, 0.0);
    glutSolidSphere(0.2, 50, 50);

    glTranslatef(-1.0, -1.0, 0.0);
    glutSolidSphere(0.2, 50, 50);

    //glFlush();
    glutSwapBuffers();  // ������� ���
}

void MyReshape(int w, int h) {      // â�� ũ�⿡ ����ؼ� â ���� ������ �𵨸� �� �� �ְ�, �Ǵ� â�� ũ�⿡ ������� â���� ������ �״�� ������ �� ����
    // â�� ũ�⿡ ����ؼ� â ���� ������ �𵨸�
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-2.5, 2.5, -2.5 * (GLfloat)h / (GLfloat)w, 2.5 * (GLfloat)h / (GLfloat)w, -2.5, 2.5);   // ���鿡�� ���� �ڵ� 

    // 3�������� ���̰� �ϴ� �ڵ�
    gluPerspective(60.0f, 1, 0, 20);    // ī�޶��� �þ߰�
    gluLookAt(0, -1.3, 4, 0, 0, 0, 0, 1, 0);   // ī�޶��� ��ġ gluLookAt(x,y,z, ...) 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'Q':
    case 'q':
    case 27:    // ESCŰ
        exit(0); break;     // window ����
    }
    glutPostRedisplay();    // �׸� �ٽ� �׷���  // MyDisplay() ������ ȣ�� (glut�Լ���) // MyDisplay();�� ȣ���ϸ� �ȵ�

}

void MySpecial(int key, int x, int y) {
    switch (key) {
        // ȭ��ǥ �Է� �� â ���� ������ ������  // â ���� ������ mydisplay�� �׸�  // ����� ���� �ʿ� (��������) tX, tY
    case GLUT_KEY_DOWN:    // �Ʒ����� ȭ��ǥ
        if (isTie)  tYr -= 0.1;         // ��Ÿ�̰� ���� ���� ��
        else if (isHat) tYt -= 0.1;     // ���ڰ� �� �õ��� ��
        break;
    case GLUT_KEY_UP:      // ������ ȭ��ǥ
        if (isTie)  tYr += 0.1;
        else if (isHat) tYt += 0.1;
        break;
    case GLUT_KEY_LEFT:    // ���ʹ��� ȭ��ǥ
        if (isTie)  tXr -= 0.1;
        else if (isHat) tXt -= 0.1;
        break;
    case GLUT_KEY_RIGHT:   // �����ʹ��� ȭ��ǥ
        if (isTie)  tXr += 0.1;
        else if (isHat) tXt += 0.1;
        break;
    }
    glutPostRedisplay();    // MyDisplay(); ȣ��  // �׸� �ٽ� �׷���
}

void MyModelMenu(int entryID) {
    if (entryID == 1) {     // �簢�� ����
        isTie = true;
        isHat = false;
    }
    else if (entryID == 2) {    // �ﰢ�� ����
        isTie = false;
        isHat = true;
    }
    glutPostRedisplay();
}

void MyMainMenu(int entryID) {
    if (entryID == 9)    exit(0);   // ����޴��� ó���� �ʿ� ����, AddMenuEntry�� ���� �κи� ó�����ָ� ��
}

void MyTimer(int Value) {
    Delta += 0.03;
    if (Delta > 2.0)    Delta = 0.0;    // ��(ũ��)�� ���� ���ڰ� �Ǹ� �ٲ��� �ʱ� ������ �ٽ� ���� �ٲ�� �������ֱ� ���ؼ� Delta���� �ʱ�ȭ���� --> �ݺ������� ��������
    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);  // �� �ҷ��� ������ �Է��� ���� �ʾƵ� �ڵ������� �ִϸ��̼� ȿ����
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // �ִϸ��̼��� ���� ������� ���
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(200, 100);

    glutCreateWindow("My First Creative Graphics - OpenGL HW1");
    glClearColor(0.5, 0.5, 0.6, 0.5); // â�� ����

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutKeyboardFunc(MyKeyboard);
    glutSpecialFunc(MySpecial);
    GLint MyModelID = glutCreateMenu(MyModelMenu);
    glutAddMenuEntry("��Ÿ��", 1);  // ��Ÿ�̿� �ش�Ǵ� ���̵� �־��� = 1 // ���̵��� �ߺ��� �� �Ǹ� ��
    glutAddMenuEntry("����", 2); // ���ڿ� �ش�Ǵ� ID�� = 2   // �ߺ����� �ʰ� ����

    glutCreateMenu(MyMainMenu);
    glutAddSubMenu("��", MyModelID);
    glutAddMenuEntry("Exit", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // ���콺�� ������ ��ư�� ����

    glutTimerFunc(40, MyTimer, 1);    // 40mesc // �󸶸�ŭ ��ٸ� ���ΰ��� ���� �� �־���
    glutMainLoop();
    return 0;
}
