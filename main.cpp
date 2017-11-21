// g++ main.cpp -o gl -lGL -lGLU -lglut -std=c++11
#include <GL/glut.h>
#include "logger.cpp"
#include <sstream>
#include <math.h>
#include "helpers.cpp"
#include "handler.cpp"
#include "player.cpp"
#include <chrono>

Player player;

Handler planeHandler;
Handler planeHandler2;
Handler missileHandler;
Handler roadHandler;
Handler buildingHandler;

int width = 800;
int height = 800;

bool thirdPersonCamera = false;

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

  GLdouble viewer[3];
  player.getPos(thirdPersonCamera, viewer);

  GLdouble lookingPosition[3];
  player.getLooking(lookingPosition);

  gluLookAt(
      viewer[0], viewer[1], viewer[2],
      lookingPosition[0], lookingPosition[1], lookingPosition[2],
      0.0, 1.0, 0.0);

  initScene();

  if (thirdPersonCamera)
  {
    player.render();
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

  // O player que trata o movimento do mouse, portanto precisa saber
  // qual é o tamanho da tela pra saber qual a posição do mouse.
  player.setHeight(height);
  player.setWidth(width);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, (double)w / h, 2.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'w':
    player.goForward();
    break;
  case 's':
    player.goBackwards();
    break;
  // case 'a':
  //   break;
  // case 'd':
  //   break;
  case 'q':
    player.goUp();
    break;
  case 'e':
    player.goDown();
    break;
  case 'c':
    thirdPersonCamera = !thirdPersonCamera;
    break;
  default:
    break;
  }
}

void SpecialInput(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    player.lookUp();
    break;
  case GLUT_KEY_DOWN:
    player.lookDown();
    break;
  case GLUT_KEY_LEFT:
    player.lookLeft();
    break;
  case GLUT_KEY_RIGHT:
    player.lookRight();
    break;
  }
}

void loadObjects()
{
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

void mouseMove(GLint x, GLint y)
{
  player.mouseMove(x, y);
}

void mouseHack(int button, int state, int x, int y)
{
  player.mouseHack(button, state, x, y);
}

int main(int argc, char **argv)
{
  Logger::clear();
  loadObjects();

  // O player que trata o movimento do mouse, portanto precisa saber
  // qual é o tamanho da tela pra saber qual a posição do mouse.
  player.setHeight(height);
  player.setWidth(width);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glEnable(GL_MULTISAMPLE);

  glutInitWindowSize(width, height);
  glutCreateWindow("78390");
  glutInitWindowPosition(0, 0);

  glutSpecialFunc(SpecialInput);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutReshapeFunc(myReshape);
  glutPassiveMotionFunc(mouseMove);
  glutMouseFunc(mouseHack);
  glutIdleFunc(display);
  glutMainLoop();

  return 0;
}