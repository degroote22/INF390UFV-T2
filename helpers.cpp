#ifndef HELPERS
#define HELPERS

#define PI 3.14159265

double degToRad(double deg)
{
  const double halfC = PI / 180;
  return deg * halfC;
}

void rotateYZVec3(double degree, GLdouble vec[3])
{
  GLdouble y = vec[1];
  GLdouble z = vec[2];

  double deg = degToRad(degree);
  double cosDeg = cos(deg);
  double sinDeg = sin(deg);
  GLdouble ny = y * cosDeg - z * sinDeg;
  GLdouble nz = y * sinDeg + z * cosDeg;

  vec[1] = ny;
  vec[2] = nz;
}

void rotateXYVec3(double degree, GLdouble vec[3])
{
  GLdouble x = vec[0];
  GLdouble y = vec[1];

  double deg = degToRad(degree);
  double cosDeg = cos(deg);
  double sinDeg = sin(deg);
  GLdouble nx = x * cosDeg - y * sinDeg;
  GLdouble ny = x * sinDeg + y * cosDeg;

  vec[0] = nx;
  vec[1] = ny;
}

void rotateXZVec3(double degree, GLdouble vec[3])
{
  GLdouble x = vec[0];
  GLdouble z = vec[2];

  double deg = degToRad(degree);
  double cosDeg = cos(deg);
  double sinDeg = sin(deg);
  GLdouble nx = x * cosDeg - z * sinDeg;
  GLdouble nz = x * sinDeg + z * cosDeg;

  vec[0] = nx;
  vec[2] = nz;
}

#endif
