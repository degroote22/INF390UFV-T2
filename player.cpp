#ifndef PLAYER
#define PLAYER
#include <GL/glut.h>
#include "helpers.cpp"
#include "handler.cpp"

#define WALKING_FACTOR 10
#define VIEWER_SCALE 33.5

const GLdouble playerInit[] = {-1, 9.5, -28.5};
const GLdouble lookingVectorInit[] = {0, 0.0, VIEWER_SCALE};

class Player
{
public:
  void goForward();
  void goBackwards();
  void goUp();
  void goDown();
  void lookUp();
  void lookDown();
  void lookLeft();
  void lookRight();
  void mouseHack(int button, int state, int x, int y);
  void mouseMove(GLint x, GLint y);
  void render();
  void setWidth(int w);
  void setHeight(int h);
  void getPos(bool thirdPersonCamera, GLdouble *position);
  void getLooking(GLdouble *looking);
  Player();

private:
  void hover(bool up);
  void walkFrontway(bool forward);
  void rotateXZ(double angle);
  void rotateYZ(double angle);
  GLdouble pos[3] = {0, 0, 0};
  int width = 0;
  int height = 0;
  bool hasMovedMouse = false;
  GLint lastX = 0.0;
  GLint lastY = 0.0;
  double xzRotation = 0;
  double yzRotation = 0;
  Handler handler;
};

void Player::render()
{
  handler.setTranslate(pos[0], pos[1], pos[2]);
  handler.setRotateXZ(-xzRotation);
  handler.setRotateYZ(-yzRotation);
  handler.render();
}

Player::Player()
{
  handler.load("plane/FA38_Airborne.obj");
  handler.setScale(0.0005);
}

void Player::getLooking(GLdouble *looking)
{
  GLdouble lookingVector[3];
  lookingVector[0] = lookingVectorInit[0];
  lookingVector[1] = lookingVectorInit[1];
  lookingVector[2] = lookingVectorInit[2];
  rotateXZVec3(xzRotation, lookingVector);
  rotateYZVec3(yzRotation, lookingVector);

  looking[0] = pos[0] + lookingVector[0];
  looking[1] = pos[1] + lookingVector[1];
  looking[2] = pos[2] + lookingVector[2];
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

void Player::lookUp()
{
  rotateYZ(5);
}

void Player::lookDown()
{
  rotateYZ(-5);
}

void Player::lookLeft()
{
  rotateXZ(-5);
}

void Player::lookRight()
{
  rotateXZ(5);
}

void Player::rotateXZ(double angle)
{
  xzRotation += angle;
}

void Player::rotateYZ(double angle)
{
  yzRotation += angle;
}

void Player::mouseMove(GLint x, GLint y)
{
  // Como não há um game loop na implementação
  // e a arquitetura é baseada em eventos,
  // é feito um throttle do movimento do mouse
  // para que a taxa de quadros fique aceitável.

  // std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double, std::milli> time_span = time - lastTime;
  // if (time_span.count() < 70)
  //   return;
  // lastTime = time;

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

  rotateXZ(dx * 30);
  rotateYZ(dy * -30);

  lastX = x;
  lastY = y;
  // display();
}

void Player::mouseHack(int button, int state, int x, int y)
{
  hasMovedMouse = false;
}

void Player::goUp()
{
  hover(true);
}

void Player::goDown()
{
  hover(false);
}

void Player::hover(bool up)
{
  if (up)
    pos[1] += VIEWER_SCALE / WALKING_FACTOR;
  else
    pos[1] -= VIEWER_SCALE / WALKING_FACTOR;
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

  pos[0] += i * lookingVector[0] / WALKING_FACTOR;
  pos[1] -= i * lookingVector[1] / WALKING_FACTOR;
  pos[2] += i * lookingVector[2] / WALKING_FACTOR;
}

void Player::goForward()
{
  walkFrontway(true);
}

void Player::goBackwards()
{
  walkFrontway(false);
}

#endif
