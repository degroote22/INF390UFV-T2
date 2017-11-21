#ifndef TEXTURELOADER
#define TEXTURELOADER

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

// GLuint texture[2];

struct Image
{
  unsigned long sizeX;
  unsigned long sizeY;
  char *data;
};

typedef struct Image Image;

float xrot;
float yrot;
float zrot;
float ratio;

char c;

int ImageLoad(char *filename, Image *image)
{
  FILE *file;
  int size;                  // size of the image in bytes.
  unsigned long i;           // standard counter.
  unsigned short int planes; // number of planes in image (must be 1)
  unsigned short int bpp;    // number of bits per pixel (must be 24)

  char temp; // temporary color storage for bgr-rgb conversion.
  // make sure the file is there.

  if ((file = fopen(filename, "rb")) == NULL)
  {
    printf("File Not Found : %s\n", filename);
    return 0;
  }

  // seek through the bmp header, up to the width/height:
  fseek(file, 18, SEEK_CUR);

  // read the width
  if ((i = fread(&image->sizeX, 4, 1, file)) != 1)
  {
    printf("Error reading width from %s.\n", filename);
    return 0;
  }
  //printf("Width of %s: %lu\n", filename, image->sizeX);

  // read the height
  if ((i = fread(&image->sizeY, 4, 1, file)) != 1)
  {
    printf("Error reading height from %s.\n", filename);
    return 0;
  }
  // Logger::log("er\n");
  // Logger::logInt(image->sizeY);
  // Logger::log("er\n");

  // Logger::logInt(image->sizeX);
  // Logger::log(image->sizeX);
  //printf("Height of %s: %lu\n", filename, image->sizeY);

  // calculate the size (assuming 24 bits or 3 bytes per pixel).

  size = image->sizeX * image->sizeY * 3;
  // read the planes
  if ((fread(&planes, 2, 1, file)) != 1)
  {
    printf("Error reading planes from %s.\n", filename);
    return 0;
  }

  if (planes != 1)
  {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }

  // read the bitsperpixel

  if ((i = fread(&bpp, 2, 1, file)) != 1)
  {
    printf("Error reading bpp from %s.\n", filename);
    return 0;
  }

  if (bpp != 24)
  {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }
  // seek past the rest of the bitmap header.

  fseek(file, 24, SEEK_CUR);

  // read the data.
  // Logger::logInt(size);
  image->data = (char *)malloc(size);
  if (image->data == NULL)
  {
    printf("Error allocating memory for color-corrected image data");
    return 0;
  }
  if ((i = fread(image->data, size, 1, file)) != 1)
  {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }
  for (i = 0; i < size; i += 3)
  { // reverse all of the colors. (bgr -> rgb)
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }
  // we're done.
  return 1;
}

Image *loadTexture(std::string f)
{
  Image *image1;
  // allocate space for texture

  image1 = (Image *)malloc(sizeof(Image));
  if (image1 == NULL)
  {
    printf("Error allocating space for image");
    exit(0);
  }

  if (!ImageLoad((char *)f.c_str(), image1))
  {
    exit(1);
  }

  return image1;
}

void loadTextures(GLuint *textures)
{

  Image *image1 = loadTexture("grass.bmp");
  if (image1 == NULL)
  {
    printf("Image was not returned from loadTexture\n");
    exit(0);
  }

  Image *image2 = loadTexture("sky.bmp");
  if (image2 == NULL)
  {
    printf("Image was not returned from loadTexture\n");
    exit(0);
  }

  glGenTextures(2, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
               GL_RGB, GL_UNSIGNED_BYTE, image1->data);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glBindTexture(GL_TEXTURE_2D, textures[1]);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                //scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //scale linearly when image smalled than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image2->sizeX, image2->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image2->data);

  // glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,
  //              GL_RGB, GL_UNSIGNED_BYTE, image2->data);
}

#endif