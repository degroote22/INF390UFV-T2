#ifndef HANDLER
#define HANDLER

#include <GL/glut.h>
#include "parser.cpp"
#include "helpers.cpp"
#include <string>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <math.h>
#include "textureLoader.cpp"

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
  GLuint *_textures;
  int facesN;
  std::vector<FacePoint> *faces;
  GLdouble *vertices;
  GLdouble *normals;
  GLdouble *textures;
  Parser parser;
  double scale = 1;
  bool changeTranslate = false;
  double translate[3] = {0, 0, 0};
  bool changeRotate = false;
  double rotateXZDeg = 0;
  double rotateYZDeg = 0;
  std::string file;
  std::vector<Material> materials;
  bool hasTextures = false;
};
// extern GLuint skyGrass[2];

void Handler::load(std::string filename)
{
  file = filename;
  parser.parseFile(filename);
  facesN = parser.getNumFaces();
  faces = parser.getFaces();
  vertices = parser.getVertices();
  normals = parser.getNormals();
  textures = parser.getTextures();

  materials = parser.materialParser.getMaterials();

  int materialsN = materials.size();
  _textures = new GLuint[materialsN];
  glGenTextures(materialsN, _textures);
  int texindex = 0;
  for (auto it = materials.begin(); it != materials.end(); it++)
  {
    std::string name = (*it).textureName;
    if (name != "")
    {
      hasTextures = true;
      Image *image = loadTexture(name);
      glBindTexture(GL_TEXTURE_2D, _textures[texindex]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
      glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, image->data);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }
    texindex++;
  }
}

void Handler::render()
{
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);

  glTranslated(translate[0], translate[1], translate[2]);

  glRotated(rotateXZDeg, 0, 1, 0);
  glRotated(rotateYZDeg, 1, 0, 0);

  glScalef(scale, scale, scale);

  for (int i = 0; i < facesN; i++)
  {
    int materialN = faces[i][0].material;
    Material mat = materials[materialN];
    GLfloat ka[3] = {(float)mat.ka[0], (float)mat.ka[1], (float)mat.ka[2]};
    GLfloat kd[3] = {(float)mat.kd[0], (float)mat.kd[1], (float)mat.kd[2]};
    GLfloat ks[3] = {(float)mat.ks[0], (float)mat.ks[1], (float)mat.ks[2]};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
    if (hasTextures)
    {
      glBindTexture(GL_TEXTURE_2D, _textures[materialN]);
    }
    // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_POLYGON);

    for (auto it = faces[i].begin(); it != faces[i].end(); it++)
    {

      int texIndex = (*it).texture - 1;
      GLdouble tx = textures[texIndex * 2];
      GLdouble ty = textures[texIndex * 2 + 1];
      glTexCoord2f(tx, ty);

      int normalIndex = (*it).normal - 1;
      if (normalIndex != -1)
      {
        GLdouble nx = normals[normalIndex * 3];
        GLdouble ny = normals[normalIndex * 3 + 1];
        GLdouble nz = normals[normalIndex * 3 + 2];
        glNormal3f(nx, ny, nz);
      }

      int verticeIndex = (*it).vertice - 1;
      GLdouble x = vertices[verticeIndex * 3];
      GLdouble y = vertices[verticeIndex * 3 + 1];
      GLdouble z = vertices[verticeIndex * 3 + 2];

      glVertex3f(x, y, z);
    }

    glEnd();
  }
  glPopMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);

  glDisable(GL_TEXTURE_2D);
}

#endif