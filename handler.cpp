#ifndef HANDLER
#define HANDLER

#include <GL/glut.h>
#include "parser.cpp"
#include "helpers.cpp"
#include <string>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <math.h>

class Handler
{
public:
  void render();
  void load(std::string filename);
  void setScale(double s) { scale = s; }
  void setTranslate(double x, double y, double z)
  {
    changeTranslate = true;
    translate[0] = x;
    translate[1] = y;
    translate[2] = z;
  }
  void setRotateXZ(double deg)
  {
    rotateXZDeg = deg;
  }
  void setRotateYZ(double deg)
  {
    rotateYZDeg = deg;
  }

private:
  int facesN;
  std::vector<FacePoint> *faces;
  GLdouble *vertices;
  GLdouble *normals;
  Parser parser;
  double scale = 1;
  bool changeTranslate = false;
  double translate[3] = {0, 0, 0};
  bool changeRotate = false;
  double rotateXZDeg = 0;
  double rotateYZDeg = 0;
  std::string file;
};

void Handler::load(std::string filename)
{
  file = filename;
  parser.parseFile(filename);
  facesN = parser.getNumFaces();
  faces = parser.getFaces();
  vertices = parser.getVertices();
  normals = parser.getNormals();
}

void Handler::render()
{
  glPushMatrix();

  glTranslated(translate[0], translate[1], translate[2]);

  glRotated(rotateXZDeg, 0, 1, 0);
  glRotated(rotateYZDeg, 1, 0, 0);

  glScalef(scale, scale, scale);

  for (int i = 0; i < facesN; i++)
  {
    glBegin(GL_POLYGON);

    for (auto it = faces[i].begin(); it != faces[i].end(); it++)
    {
      int verticeIndex = (*it).vertice - 1;

      GLdouble x = vertices[verticeIndex * 3];
      GLdouble y = vertices[verticeIndex * 3 + 1];
      GLdouble z = vertices[verticeIndex * 3 + 2];

      int normalIndex = (*it).normal - 1;
      if (normalIndex != -1)
      {
        GLdouble nx = normals[normalIndex * 3];
        GLdouble ny = normals[normalIndex * 3 + 1];
        GLdouble nz = normals[normalIndex * 3 + 2];
        glNormal3f(nx, ny, nz);
      }

      glVertex3f(x, y, z);
    }

    glEnd();
  }
  glPopMatrix();
}

#endif