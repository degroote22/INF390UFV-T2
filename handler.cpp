#ifndef HANDLER
#define HANDLER

#include <GL/glut.h>
#include "parser.cpp"
#include <string>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <math.h>

class Handler
{
public:
  void render();
  void load(std::string filename);
  void setScale(int s) { scale = s; }
  void setTranslate(double x, double y, double z)
  {
    changeTranslate = true;
    translate[0] = x;
    translate[1] = y;
    translate[2] = z;
  }
  void setRotate(double deg)
  {
    rotateDeg = deg;
  }

private:
  int facesN;
  std::vector<FacePoint> *faces;
  GLdouble *vertices;
  Parser parser;
  int scale = 1;
  bool changeTranslate = false;
  double translate[3] = {0, 0, 0};
  bool changeRotate = false;
  double rotateDeg = 0;
};

void Handler::load(std::string filename)
{
  parser.parseFile(filename);
  facesN = parser.getNumFaces();
  faces = parser.getFaces();
  vertices = parser.getVertices();
  srand(time(NULL));
}

void Handler::render()
{
  /* initialize random seed: */

  if (scale != 1)
  {
    glScalef(scale, scale, scale);
  }
  if (changeTranslate)
  {
    glTranslated(translate[0], translate[1], translate[2]);
  }
  if (rotateDeg != 0)
  {
    glRotated(rotateDeg, 0, 1, 0);
  }
  for (int i = 0; i < facesN; i++)
  {
    glBegin(GL_POLYGON);

    for (auto it = faces[i].begin(); it != faces[i].end(); it++)
    {
      int verticeIndex = (*it).vertice - 1;

      GLdouble x = vertices[verticeIndex * 3];
      GLdouble y = vertices[verticeIndex * 3 + 1];
      GLdouble z = vertices[verticeIndex * 3 + 2];

      int c = verticeIndex % 100;
      glColor3f((double)c / 100, 1 - (double)c / 100, cos((double)c / 100));

      glVertex3f(x, y, z);
    }

    glEnd();
  }
  if (scale != 1)
  {
    glScalef(1 / scale, 1 / scale, 1 / scale);
  }
  if (changeTranslate)
  {
    glTranslated(-translate[0], -translate[1], -translate[2]);
  }
  if (rotateDeg != 0)
  {
    glRotated(-rotateDeg, 0, 1, 0);
  }
}

#endif