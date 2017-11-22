#ifndef PLAYER
#define PLAYER
#include <GL/glut.h>
#include "helpers.cpp"
#include "handler.cpp"

#define WALKING_FACTOR 200.0
#define VIEWER_SCALE 35.0
#define SPEED_DECAY 0.95
#define TURNING_ANGLE 1.0
#define GRAVITY 0.1

const GLdouble playerInit[] = {-1, 9.5, -28.5};
const GLdouble lookingVectorInit[] = {0, 0.0, VIEWER_SCALE};

class Player
{
public:
  void mouseHack(int button, int state, int x, int y);
  void mouseMove(GLint x, GLint y);
  void render();
  void setWidth(int w);
  void setHeight(int h);
  void getPos(bool thirdPersonCamera, GLdouble *position);
  void getLooking(GLdouble *looking);
  void getHeadlightFocus(GLdouble *looking);
  void updateLoop();
  void keyDown(unsigned char key);
  void keyUp(unsigned char key);
  void specialUp(int key);
  void specialDown(int key);
  void reset();
  void load();

private:
  void hover(bool up);
  void walkFrontway(bool forward);
  void rotateXZ(double angle);
  void rotateYZ(double angle);
  GLdouble pos[3] = {0, 0, -20};
  GLdouble vel[3] = {0, 0, 0};
  int width = 0;
  int height = 0;
  bool hasMovedMouse = false;
  GLint lastX = 0.0;
  GLint lastY = 0.0;
  double xzRotation = 0;
  double xzAngleBucket = 0;
  double yzRotation = 0;
  double yzAngleBucket = 0;
  Handler handler;

  bool wPressed = false;
  bool sPressed = false;
  bool qPressed = false;
  bool ePressed = false;
  bool aPressed = false;
  bool dPressed = false;
  bool leftPressed = false;
  bool rightPressed = false;
  bool upPressed = false;
  bool downPressed = false;
};

void Player::load()
{
  handler.load("plane2/TAL16OBJ");
  handler.setScale(5);
}

void Player::reset()
{
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;
  vel[0] = 0;
  vel[1] = 0;
  vel[2] = 0;

  xzRotation = 0;
  xzAngleBucket = 0;
  yzRotation = 0;
  yzAngleBucket = 0;
  // pos[3] = {0, 0, 0};
  // vel[3] = {0, 0, 0};
}

void Player::specialUp(int key)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    upPressed = false;
    break;
  case GLUT_KEY_DOWN:
    downPressed = false;
    break;
  case GLUT_KEY_LEFT:
    leftPressed = false;
    break;
  case GLUT_KEY_RIGHT:
    rightPressed = false;
    break;
  }
}

void Player::specialDown(int key)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    upPressed = true;
    break;
  case GLUT_KEY_DOWN:
    downPressed = true;
    break;
  case GLUT_KEY_LEFT:
    leftPressed = true;
    break;
  case GLUT_KEY_RIGHT:
    rightPressed = true;
    break;
  }
}

void Player::keyUp(unsigned char key)
{
  switch (key)
  {
  case 'w':
    wPressed = false;
    break;
  case 's':
    sPressed = false;
    break;
  case 'a':
    aPressed = false;
    break;
  case 'd':
    dPressed = false;
    break;
  case 'q':
    qPressed = false;
    break;
  case 'e':
    ePressed = false;
    break;
  default:
    break;
  }
}

void Player::keyDown(unsigned char key)
{
  switch (key)
  {
  case 'r':
    reset();
    break;
  case 'w':
    wPressed = true;
    break;
  case 's':
    sPressed = true;
    break;
  case 'a':
    aPressed = true;
    break;
  case 'd':
    dPressed = true;
    break;
  case 'q':
    qPressed = true;
    break;
  case 'e':
    ePressed = true;
    break;
  default:
    break;
  }
}

void Player::updateLoop()
{
  if (wPressed)
  {
    walkFrontway(true);
  }
  if (sPressed)
  {
    walkFrontway(false);
  }
  if (qPressed)
  {
    hover(true);
  }
  if (ePressed)
  {
    hover(false);
  }
  if (aPressed)
  {
    rotateXZVec3((double)-TURNING_ANGLE, vel);
  }
  if (dPressed)
  {
    rotateXZVec3((double)TURNING_ANGLE, vel);
  }

  // O avião só troca de direção se estiver com velocidade.
  double velSq = (vel[0] * vel[0] + vel[2] * vel[2]) / 10;

  if (upPressed)
  {
    yzAngleBucket = TURNING_ANGLE * velSq;
  }
  else if (downPressed)
  {
    yzAngleBucket = -TURNING_ANGLE * velSq;
  }
  else
  {
    yzAngleBucket = -yzRotation / 25;
  }

  if (leftPressed)
  {
    xzAngleBucket = 2 * -TURNING_ANGLE * velSq;
  }
  if (rightPressed)
  {
    xzAngleBucket = 2 * TURNING_ANGLE * velSq;
  }

  pos[0] += vel[0];
  pos[1] += vel[1];
  pos[2] += vel[2];

  // Collider com o chão.
  pos[1] = std::max<double>(pos[1], 3);

  // Gravidade
  vel[1] -= GRAVITY * (1.3 - velSq) * (1.3 - velSq);
  // Decaimento da velocidade
  vel[0] = vel[0] * SPEED_DECAY;
  vel[1] = vel[1] * SPEED_DECAY;
  vel[2] = vel[2] * SPEED_DECAY;

  // Decaimento dos giros
  xzRotation += xzAngleBucket;
  xzAngleBucket = xzAngleBucket * SPEED_DECAY;
  yzRotation += yzAngleBucket;
  yzAngleBucket = yzAngleBucket * SPEED_DECAY;

  // Não passa do ângulo máximo.
  yzRotation = std::max<double>(yzRotation, -15);
  yzRotation = std::min<double>(yzRotation, 15);
}

void Player::render()
{
  handler.setTranslate(pos[0], pos[1] + 3, pos[2]);
  handler.setRotateXZ(-xzRotation);
  handler.setRotateYZ(-90 - yzRotation);
  handler.render();
}

void Player::getLooking(GLdouble *looking)
{
  GLdouble lookingVector[3];
  lookingVector[0] = lookingVectorInit[0];
  lookingVector[1] = lookingVectorInit[1];
  lookingVector[2] = lookingVectorInit[2];

  rotateYZVec3(yzRotation, lookingVector);
  rotateXZVec3(xzRotation, lookingVector);

  looking[0] = pos[0] + lookingVector[0];
  looking[1] = pos[1] + lookingVector[1];
  looking[2] = pos[2] + lookingVector[2];
}

void Player::getHeadlightFocus(GLdouble *looking)
{
  GLdouble lookingVector[3];
  lookingVector[0] = lookingVectorInit[0];
  lookingVector[1] = lookingVectorInit[1];
  lookingVector[2] = lookingVectorInit[2];

  rotateYZVec3(yzRotation, lookingVector);
  rotateXZVec3(xzRotation, lookingVector);

  looking[0] = lookingVector[0];
  looking[1] = -lookingVector[1];
  looking[2] = +lookingVector[2];
}

void Player::getPos(bool thirdPersonCamera, GLdouble *position)
{
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
  rotateXZVec3(xzRotation, playerDisplacement);

  position[0] = pos[0] + playerDisplacement[0];
  position[1] = pos[1] + playerDisplacement[1];
  position[2] = pos[2] + playerDisplacement[2];
  // return pos;
}

void Player::setWidth(int w)
{
  width = w;
}

void Player::setHeight(int h)
{
  height = h;
}

void Player::rotateXZ(double angle)
{
  xzRotation += angle;
}

void Player::rotateYZ(double angle)
{
  yzRotation += angle;
  yzRotation = std::max<double>(yzRotation, -30);
  yzRotation = std::min<double>(yzRotation, 30);
}

void Player::mouseMove(GLint x, GLint y)
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

  rotateXZ(dx * 360);
  rotateYZ(dy * -90);

  lastX = x;
  lastY = y;
  // display();
}

void Player::mouseHack(int button, int state, int x, int y)
{
  hasMovedMouse = false;
}

void Player::hover(bool up)
{
  if (up)
    vel[1] += (double)VIEWER_SCALE / WALKING_FACTOR;
  else
    vel[1] -= (double)VIEWER_SCALE / WALKING_FACTOR;
}

void Player::walkFrontway(bool forward)
{
  GLdouble lookingVector[3];
  lookingVector[0] = lookingVectorInit[0];
  lookingVector[1] = lookingVectorInit[1];
  lookingVector[2] = lookingVectorInit[2];
  rotateYZVec3(yzRotation, lookingVector);
  rotateXZVec3(xzRotation, lookingVector);

  int i = -1;
  if (forward)
    i = 1;

  vel[0] += i * lookingVector[0] / WALKING_FACTOR;
  vel[1] -= i * lookingVector[1] / WALKING_FACTOR;
  vel[2] += i * lookingVector[2] / WALKING_FACTOR;
}

#endif
