// g++ main.cpp -o gl -lGL -lGLU -lglut -std=c++11
#include <GL/glut.h>
#include "logger.cpp"
#include <sstream>
#include <math.h>
#include "helpers.cpp"
#include "parser.cpp"

Parser planeParser;

int width = 800;
int height = 800;
GLdouble viewer[] = {0.0, 0.0, 5.0};
GLdouble lookingVector[] = {0.0, 0.0, -5.0};
GLint lastX = 0.0;
GLint lastY = 0.0;
bool hasMovedMouse = false;

void initScene(void)
{
  GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
  GLfloat red_light[] = {1.0, 0.0, 0.0, 0.0};

  glClearColor(1.0, 1.0, 1.0, 1.0);

  //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, mat_shininess);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
}

void drawTestBalls()
{
  glColor3f(0, 0, 0.0);
  glutSolidSphere(0.5, 50, 16);

  glTranslated(1.0, 0.0, 0.0);
  glColor3f(1.0, 0, 0);
  glutSolidSphere(0.3, 50, 16);
  glTranslated(-1.0, 0.0, 0.0);

  glTranslated(0.0, 1.0, 0.0);
  glColor3f(0, 1.0, 0);
  glutSolidSphere(0.3, 50, 16);
  glTranslated(0.0, -1.0, 0.0);

  glTranslated(0.0, 0.0, 1.0);
  glColor3f(0, 0, 1.0);
  glutSolidSphere(0.3, 50, 16);
  glTranslated(0.0, 0.0, -1.0);

  glTranslated(viewer[0], viewer[1] - 1, viewer[2]);
  glColor3f(0.5, 0.5, 1.0);
  glutSolidSphere(0.5, 50, 16);
  glTranslated(-viewer[0], -viewer[1] + 1, -viewer[2]);
}

void drawTestLine()
{
  glLineWidth(5);
  glBegin(GL_LINES);
  glVertex3f(viewer[0], viewer[1] - 1, viewer[2]);
  glVertex3f(viewer[0] + lookingVector[0], viewer[1] + lookingVector[1] - 1, viewer[2] + lookingVector[2]);
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(
      viewer[0], viewer[1], viewer[2],
      viewer[0] + lookingVector[0], viewer[1] + lookingVector[1], viewer[2] + lookingVector[2],
      0.0, 1.0, 0.0);

  drawTestBalls();
  drawTestLine();

  glFlush();
}

void myReshape(GLsizei w, GLsizei h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, 1.0, 0.5, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Usa-se um "hack" para poder mexer o mouse sem mudar a visão
// porque senão o campo de visão fica muito limitado se só se usar
// o mous para olhar para os lados.
void mouseHack(int button, int state, int x, int y)
{
  hasMovedMouse = false;
}

void mouseMove(GLint x, GLint y)
{
  // Parte do Hack do mouse
  if (!hasMovedMouse)
  {
    lastX = x;
    lastY = y;
    hasMovedMouse = true;
    return;
  }

  GLdouble dx = (double)(x - lastX) / width;
  GLdouble dy = (double)(y - lastY) / height;

  rotateYZ(dy * -30, lookingVector);
  rotateXZ(dx * 30, lookingVector);

  lastX = x;
  lastY = y;
  display();
}

void walkFrontway(bool forward)
{
  int i = -1;
  if (forward)
    i = 1;

  viewer[0] += i * lookingVector[0] / 10;
  viewer[1] += i * lookingVector[1] / 10;
  viewer[2] += i * lookingVector[2] / 10;
}

void walkSideways(bool right)
{
  int i = -1;
  if (right)
    i = 1;

  GLdouble side[3];
  side[0] = lookingVector[0];
  side[1] = lookingVector[1];
  side[2] = lookingVector[2];

  rotateXZ(90, side);

  viewer[0] += i * side[0] / 10;
  viewer[1] += i * side[1] / 10;
  viewer[2] += i * side[2] / 10;
}

void hover(bool up)
{
  if (up)
    viewer[1] += 0.5;
  else
    viewer[1] -= 0.5;
}

void keyboard(unsigned char key, int x, int y)
{
  // std::ostringstream s;
  // s << key << "\n";
  // Logger::log(s.str());

  switch (key)
  {
  case 'w':
    walkFrontway(true);
    break;
  case 's':
    walkFrontway(false);
    break;
  case 'a':
    walkSideways(false);
    break;
  case 'd':
    walkSideways(true);
    break;
  case 'q':
    hover(true);
    break;
  case 'e':
    hover(false);
    break;

  default:
    break;
  }
  display();
}

void SpecialInput(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    rotateYZ(15.0, lookingVector);
    break;
  case GLUT_KEY_DOWN:
    rotateYZ(-15.0, lookingVector);
    break;
  case GLUT_KEY_LEFT:
    rotateXZ(-15.0, lookingVector);
    break;
  case GLUT_KEY_RIGHT:
    rotateXZ(15.0, lookingVector);
    break;
  }
  display();
}

int main(int argc, char **argv)
{
  Logger::clear();
  planeParser.parseFile("test.obj");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("78390");
  glutInitWindowPosition(0, 0);

  initScene();
  glutSpecialFunc(SpecialInput);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutReshapeFunc(myReshape);
  glutPassiveMotionFunc(mouseMove);
  glutMouseFunc(mouseHack);

  glutMainLoop();

  return 0;
}