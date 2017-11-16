// g++ main.cpp -o gl -lGL -lGLU -lglut -std=c++11
#include <GL/glut.h>
#include "logger.cpp"
#include <sstream>
#include <math.h>
#include "helpers.cpp"
#include "parser.cpp"
#include "handler.cpp"
#include <chrono>

// Viewer-9.38595
// 12.3535
// 18.622
// Looking-0.357373
// 0.461923
// -4.96577

#define WALKING_FACTOR 10
#define VIEWER_SCALE 5.0

int width = 800;
int height = 800;

GLdouble planeInit[] = {-1, 9.5, -28.5};

GLdouble viewer[] = {0, 0, 0};
// GLdouble lookingVector[] = {0, 0, 5};
GLdouble lookingVector[] = {1, -9.5, 33.5};
GLint lastX = 0.0;
GLint lastY = 0.0;
bool hasMovedMouse = false;
bool thirdPersonCamera = false;

double playerXZRotation = 0;
void rotatePlayerXZ(double angle)
{
  rotateXZ(angle, lookingVector);
  playerXZRotation += angle;
}

double playerYZRotation = 0;
void rotatePlayerYZ(double angle)
{
  rotateYZ(angle, lookingVector);
  playerYZRotation += angle;
}

Handler planeHandler;
Handler cityHandler;

void initScene(void)
{
  GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {0.0, 50.0, 0.0, 0.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
  GLfloat red_light[] = {1.0, 0.0, 0.0, 0.0};

  glClearColor(1.0, 1.0, 1.0, 1.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, red_light);
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

  glTranslated(0.0, 15.0, 0.0);
  glColor3f(0.0, 0, 0);
  glutSolidSphere(2, 50, 16);
  glTranslated(0.0, -15.0, 0.0);

  // glTranslated(viewer[0], viewer[1] - 1, viewer[2]);
  // glColor3f(0.5, 0.5, 1.0);
  // glutSolidSphere(0.5, 50, 16);
  // glTranslated(-viewer[0], -viewer[1] + 1, -viewer[2]);
}

void drawTestLine()
{
  GLdouble v[3] = {0, 0, 0};
  if (thirdPersonCamera)
  {
    v[0] = viewer[0];
    v[1] = viewer[1] + 5;
    v[2] = viewer[2] + 15;
  }
  else
  {
    v[0] = viewer[0];
    v[1] = viewer[1];
    v[2] = viewer[2];
  }

  glPushMatrix();
  glTranslated(v[0] + lookingVector[0], v[1] + lookingVector[1], v[2] + lookingVector[2]);
  glColor3f(0.0, 0, 0);
  glutSolidSphere(0.5, 50, 16);
  glPopMatrix();
}

void display(void)
{
  // Logger::log("Viewer");
  // Logger::logDouble(viewer[0]);
  // Logger::logDouble(viewer[1]);
  // Logger::logDouble(viewer[2]);
  // Logger::log("Looking");
  // Logger::logDouble(lookingVector[0]);
  // Logger::logDouble(lookingVector[1]);
  // Logger::logDouble(lookingVector[2]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  GLdouble v[3] = {0, 0, 0};
  if (thirdPersonCamera)
  {
    v[0] = viewer[0];
    v[1] = viewer[1] + 5;
    v[2] = viewer[2] + 15;
  }
  else
  {
    v[0] = viewer[0];
    v[1] = viewer[1];
    v[2] = viewer[2];
  }

  GLdouble pi[3] = {0, 0, 0};
  pi[0] = planeInit[0];
  pi[1] = planeInit[1];
  pi[2] = planeInit[2];

  rotateXZ(playerXZRotation, pi);

  gluLookAt(
      v[0] + pi[0], v[1] + pi[1], v[2] + pi[2],
      v[0] + lookingVector[0], v[1] + lookingVector[1], v[2] + lookingVector[2],
      0.0, 1.0, 0.0);

  // drawTestBalls();
  drawTestLine();

  planeHandler.setTranslate(viewer[0], viewer[1], viewer[2]);
  planeHandler.setRotateXZ(-playerXZRotation);
  planeHandler.setRotateYZ(-playerYZRotation);

  planeHandler.render();
  cityHandler.render();

  glutSwapBuffers();
}

void myReshape(GLsizei w, GLsizei h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, 1.0, 2.0, 500000.0);
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

std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

void mouseMove(GLint x, GLint y)
{
  // long double sysTime = time(0);
  // long double sysTimeMS = sysTime * 1000;
  std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time_span = time - lastTime;

  // Logger::logDouble(sysTimeMS - lastTime);
  // std::ostringstream s;
  // s << time_span.count() << "\n";
  // Logger::log(s.str());
  if (time_span.count() < 35)
    return;
  lastTime = time;
  // lastTime = sysTimeMS;
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

  rotatePlayerXZ(dx * 30);
  rotatePlayerYZ(dy * -30);
  // rotateYZ(dy * -30, lookingVector);
  // rotateXZ(dx * 30, lookingVector);

  lastX = x;
  lastY = y;
  display();
}

void walkFrontway(bool forward)
{
  int i = -1;
  if (forward)
    i = 1;

  viewer[0] += i * lookingVector[0] / WALKING_FACTOR;
  viewer[1] -= i * lookingVector[1] / WALKING_FACTOR;
  viewer[2] += i * lookingVector[2] / WALKING_FACTOR;
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

  viewer[0] += i * side[0] / WALKING_FACTOR;
  viewer[1] += i * side[1] / WALKING_FACTOR;
  viewer[2] += i * side[2] / WALKING_FACTOR;
}

void hover(bool up)
{
  if (up)
    viewer[1] += VIEWER_SCALE / WALKING_FACTOR;
  else
    viewer[1] -= VIEWER_SCALE / WALKING_FACTOR;
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
  case 'c':
    thirdPersonCamera = !thirdPersonCamera;
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
    rotatePlayerYZ(5);
    break;
  case GLUT_KEY_DOWN:
    rotatePlayerYZ(-5);
    break;
  case GLUT_KEY_LEFT:
    rotatePlayerXZ(-5);
    break;
  case GLUT_KEY_RIGHT:
    rotatePlayerXZ(5);
    break;
  }
  display();
}

int main(int argc, char **argv)
{
  Logger::clear();

  planeHandler.load("FA38_Airborne.obj");
  planeHandler.setScale(0.0005);

  cityHandler.load("mount.blend1.obj");
  cityHandler.setScale(3);
  cityHandler.setTranslate(0, -5, 0);

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