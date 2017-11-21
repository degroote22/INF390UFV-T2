// g++ main.cpp -o gl -lGL -lGLU -lglut -std=c++11
#include <GL/glut.h>
#include "logger.cpp"
#include <sstream>
#include <math.h>
#include "helpers.cpp"
#include "parser.cpp"
#include "handler.cpp"
#include <chrono>

#define WALKING_FACTOR 10
#define VIEWER_SCALE 33.5

Handler playerHandler;

Handler planeHandler;
Handler planeHandler2;
Handler missileHandler;
Handler roadHandler;
Handler buildingHandler;

int width = 800;
int height = 800;

const GLdouble playerInit[] = {-1, 9.5, -28.5};
const GLdouble lookingVectorInit[] = {0, 0.0, VIEWER_SCALE};

GLdouble viewer[] = {0, 0, 0};
GLdouble lookingVector[] = {lookingVectorInit[0], lookingVectorInit[1], lookingVectorInit[2]};
GLint lastX = 0.0;
GLint lastY = 0.0;
bool hasMovedMouse = false;
bool thirdPersonCamera = false;
double playerXZRotation = 0;
double playerYZRotation = 0;

void initScene(void)
{

  GLfloat mat_ambient[] = {0.0, 0.0, 1.0, 1.0};
  GLfloat mat_diffuse[] = {1.0, 0.0, 0.0, 1.0};
  GLfloat mat_specular[] = {0.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {0.0, 1.0, 0.0, 1.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  // glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.009f);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  GLdouble playerDisplacement[3] = {0, 0, 0};
  if (thirdPersonCamera)
  {
    playerDisplacement[0] = playerInit[0];
    playerDisplacement[1] = playerInit[1];
    playerDisplacement[2] = playerInit[2];
  }
  else
  {
    playerDisplacement[0] = playerInit[0];
    playerDisplacement[1] = playerInit[1] - 5;
    playerDisplacement[2] = playerInit[2] + 40;
  }
  rotateXZ(playerXZRotation, playerDisplacement);

  lookingVector[0] = lookingVectorInit[0];
  lookingVector[1] = lookingVectorInit[1];
  lookingVector[2] = lookingVectorInit[2];
  rotateXZ(playerXZRotation, lookingVector);
  rotateYZ(playerYZRotation, lookingVector);

  gluLookAt(
      viewer[0] + playerDisplacement[0], viewer[1] + playerDisplacement[1], viewer[2] + playerDisplacement[2],
      viewer[0] + lookingVector[0], viewer[1] + lookingVector[1], viewer[2] + lookingVector[2],
      0.0, 1.0, 0.0);

  initScene();
  // drawTestLine();

  if (thirdPersonCamera)
  {
    playerHandler.setTranslate(viewer[0], viewer[1], viewer[2]);
    playerHandler.setRotateXZ(-playerXZRotation);
    playerHandler.setRotateYZ(-playerYZRotation);
    playerHandler.render();
  }

  missileHandler.render();
  planeHandler.render();
  planeHandler2.render();

  for (int i = 0; i < 10; i++)
  {
    roadHandler.setTranslate(0, 0, 68 * i);
    roadHandler.render();
  }
  for (int i = 0; i < 10; i++)
  {
    buildingHandler.setTranslate(75, -5, 50 * i);
    buildingHandler.render();
  }

  glutSwapBuffers();
}

void myReshape(GLsizei w, GLsizei h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, (double)w / h, 2.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void rotatePlayerXZ(double angle)
{
  playerXZRotation += angle;
}

void rotatePlayerYZ(double angle)
{
  playerYZRotation += angle;
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
  // Como não há um game loop na implementação
  // e a arquitetura é baseada em eventos,
  // é feito um throttle do movimento do mouse
  // para que a taxa de quadros fique aceitável.

  std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time_span = time - lastTime;
  if (time_span.count() < 70)
    return;
  lastTime = time;

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
  // viewer[1] += i * side[1] / WALKING_FACTOR;
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

void loadObjects()
{
  playerHandler.load("plane/FA38_Airborne.obj");
  playerHandler.setScale(0.0005);

  planeHandler.load("plane2/TAL16OBJ.obj");
  planeHandler.setRotateYZ(-90);
  planeHandler.setScale(10);
  planeHandler.setTranslate(-25, 0, -50);

  planeHandler2.load("plane3/AN-24PB_obj.obj");
  planeHandler2.setRotateYZ(-90);
  planeHandler2.setRotateXZ(90);
  planeHandler2.setTranslate(-25, 0, 0);

  missileHandler.load("missile/AVMT300.obj");
  missileHandler.setScale(0.5);
  missileHandler.setRotateXZ(90);
  missileHandler.setTranslate(-25, 0, 50);

  roadHandler.load("road/roadV2.obj");
  roadHandler.setScale(20);

  buildingHandler.load("building.obj");
  buildingHandler.setScale(0.2);
}

int main(int argc, char **argv)
{
  Logger::clear();
  loadObjects();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glEnable(GL_MULTISAMPLE);

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