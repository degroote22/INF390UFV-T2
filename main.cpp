// g++ main.cpp -o gl -lGL -lGLU -lglut -std=c++11
#include <GL/glut.h>
#include "logger.cpp"
#include <sstream>
#include <math.h>
#include "helpers.cpp"
#include "handler.cpp"
#include "player.cpp"
#include "textureLoader.cpp"
#include <chrono>

GLuint textures[2];

Player player;

Handler planeHandler;
Handler planeHandler2;
Handler missileHandler;
Handler roadHandler;
Handler buildingHandler;

int width = 800;
int height = 800;
bool thirdPersonCamera = false;

void setLights(void)
{

  GLfloat mat_ambient[] = {0.33, 0.33, 0.33, 1.0};
  GLfloat mat_diffuse[] = {0.33, 0.33, 0.33, 1.0};
  GLfloat mat_specular[] = {0.13, 0.13, 0.13, 1.0};
  GLfloat mat_shininess[] = {0.33, 0.33, 0.33, 1.0};
  // GLfloat light_position[] = {35.0, 25.0, 100.0, 1.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  // glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  // glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.000002f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.001f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0009f);
  glEnable(GL_LIGHT0);

  // glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
  // glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.000002f);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.001f);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0009f);
  // glEnable(GL_LIGHT1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
  // glEnable(GL_COLOR_MATERIAL);
}

void renderObjects()
{

  // glColor3f(0.0, 0.0, 1.0);
  if (thirdPersonCamera)
  {
    player.render();
  }

  missileHandler.render();
  planeHandler.render();
  planeHandler2.render();

  // glColor3f(1.0, 0.0, 0.0);
  for (int i = -20; i < 20; i++)
  {
    roadHandler.setTranslate(-25, 2, 68 * i);
    roadHandler.render();
  }
  for (int i = 0; i < 10; i++)
  {
    buildingHandler.setTranslate(55, -5, 50 * i);
    buildingHandler.render();
  }

  // glPushMatrix();
  // glTranslatef(40, 25, 100);
  // glutSolidSphere(5, 5, 5);
  // glPopMatrix();
}

void renderGrass(void)
{
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000, 0, -1000);
  glTexCoord2f(100.0, 0.0);
  glVertex3f(-1000, 0.0, 1000);
  glTexCoord2f(100.0, 100.0);
  glVertex3f(1000, 0.0, 1000);
  glTexCoord2f(0.0, 100.0);
  glVertex3f(1000, 0.0, -1000);
  glEnd();

  glDisable(GL_TEXTURE_2D);
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

  setLights();

  renderObjects();

  renderGrass();

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTranslatef(viewer[0], viewer[1], viewer[2]);
  glutSolidSphere(450, 50, 30);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

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

void SpecialInput(int key, int x, int y)
{
  player.specialDown(key);
}

void loadObjects()
{
  planeHandler.load("plane2/TAL16OBJ.obj");
  planeHandler.setRotateYZ(-90);
  planeHandler.setScale(10);
  planeHandler.setTranslate(-40, 5, -70);

  planeHandler2.load("plane3/AN-24PB_obj.obj");
  planeHandler2.setRotateYZ(-90);
  planeHandler2.setRotateXZ(90);
  planeHandler2.setTranslate(-40, 0, 0);

  missileHandler.load("missile/AVMT300.obj");
  missileHandler.setScale(0.5);
  missileHandler.setRotateXZ(90);
  missileHandler.setTranslate(-40, 0, 50);

  roadHandler.load("road/roadV2.obj");
  roadHandler.setScale(20);

  buildingHandler.load("building/building.obj");
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

void updateLogic(int v)
{
  // Logger::log("upd");
  // if (wClicked)
  //   player.goForward();
  player.updateLoop();
  glutTimerFunc(16, updateLogic, 0);
}

void SpecialInputUp(int key, int x, int y)
{
  player.specialUp(key);
}

void keyboard(unsigned char key, int x, int y)
{
  if (key == 'c')
    thirdPersonCamera = !thirdPersonCamera;
  else
    player.keyDown(key);
}

void keyboardUp(unsigned char key, int x, int y)
{
  player.keyUp(key);
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
  loadTextures(textures);
  glutSpecialFunc(SpecialInput);
  glutSpecialUpFunc(SpecialInputUp);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutDisplayFunc(display);
  glutReshapeFunc(myReshape);
  glutPassiveMotionFunc(mouseMove);
  glutMouseFunc(mouseHack);
  glutIdleFunc(display);
  glutTimerFunc(5, updateLogic, 0);
  glutMainLoop();

  return 0;
}