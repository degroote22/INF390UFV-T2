#ifndef PARSER
#define PARSER

#include <string>
#include <fstream>
#include <GL/glut.h>
#include "logger.cpp"
#include <sstream>
#include <vector>

struct FacePoint
{
  int vertice;
  int texture;
  int normal;
};

class Parser
{
private:
  GLdouble *vertices;
  GLdouble *normals;
  GLdouble *textures;
  GLdouble *params;

  std::vector<FacePoint> *faces;

  int numberFaces = 0;
  int facesRead = 0;

  int numberVertices = 0;
  int verticesRead = 0;

  int numberNormals = 0;
  int normalsRead = 0;

  int numberTextures = 0;
  int texturesRead = 0;

  int numberParams = 0;
  int paramsRead = 0;

  void preParseLine(const std::string str);
  void parseLine(const std::string str);
  void parseVerticeLine(const std::string str);
  void parseFaceLine(const std::string str);
  void parseNormalLine(const std::string str);
  void parseTextureLine(const std::string str);
  void parseParamLine(const std::string str);

public:
  void parseFile(const std::string filename);

  GLdouble *getVertices() { return vertices; }
  int getNumVertices() { return numberVertices; }

  GLdouble *getNormals() { return normals; }
  int getNumNormals() { return numberNormals; }

  GLdouble *getTextures() { return textures; }
  int getNumTextures() { return numberTextures; }

  GLdouble *getParams() { return params; }
  int getNumParams() { return numberParams; }

  std::vector<FacePoint> *getFaces() { return faces; }
  int getNumFaces() { return numberFaces; }
};

void Parser::preParseLine(const std::string str)
{
  if (str[0] == 'f')
    numberFaces++;
  else if (str[0] == 'v')
  {
    if (str[1] == ' ')
      numberVertices++;
    else if (str[1] == 't')
      numberTextures++;
    else if (str[1] == 'p')
      numberParams++;
    else if (str[1] == 'n')
      numberNormals++;
  }
}

void Parser::parseTextureLine(const std::string str)
{
  //vertice
  int cursor = 2;
  std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
  for (int i = 0; i < 2; i++)
  {
    double x = std::stod(&str[cursor], &offset);
    textures[texturesRead] = x;
    texturesRead++;
    cursor += offset;
  }
}

void Parser::parseParamLine(const std::string str)
{
  //vertice
  int cursor = 2;
  std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
  for (int i = 0; i < 3; i++)
  {
    double x = std::stod(&str[cursor], &offset);
    params[paramsRead] = x;
    paramsRead++;
    cursor += offset;
  }
}

void Parser::parseNormalLine(const std::string str)
{
  //vertice
  int cursor = 2;
  std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
  for (int i = 0; i < 3; i++)
  {
    double x = std::stod(&str[cursor], &offset);
    normals[normalsRead] = x;
    normalsRead++;
    cursor += offset;
  }
}

void Parser::parseVerticeLine(const std::string str)
{
  //vertice
  int cursor = 1;
  std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
  for (int i = 0; i < 3; i++)
  {
    double x = std::stod(&str[cursor], &offset);
    vertices[verticesRead] = x;
    verticesRead++;
    cursor += offset;
  }
}

void Parser::parseFaceLine(const std::string str)
{

  std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
  int cursor = 1;

  int x = std::stoi(&str[cursor], &offset);
  cursor += offset;

  bool onlyVertices = str[cursor] == ' ';
  bool textureHidden = str[cursor + 1] == '/';
  bool hasTexture = str[cursor] == '/' && !textureHidden;

  if (hasTexture)
  {

    cursor++;

    int texture = std::stoi(&str[cursor], &offset);
    cursor += offset;

    bool hasNormal = str[cursor] != ' ';
    if (hasNormal)
    {

      cursor++;
      int normal = std::stoi(&str[cursor], &offset);
      cursor += offset;

      FacePoint fp0;
      fp0.vertice = x;
      fp0.normal = normal;
      fp0.texture = texture;
      faces[facesRead].push_back(fp0);

      while (true)
      {
        // Logger::log("Vertice: \n");
        // Logger::logInt(x);
        // Logger::log("Has texture: \n");
        // Logger::logInt(texture);
        // Logger::log("Has normal: \n");
        // Logger::logInt(normal);
        // Logger::log("\n");

        try
        {
          x = std::stoi(&str[cursor], &offset);
          cursor = cursor + offset + 1;
          texture = std::stoi(&str[cursor], &offset);
          cursor = cursor + offset + 1;
          normal = std::stoi(&str[cursor], &offset);
          cursor += offset;

          FacePoint fp;
          fp.vertice = x;
          fp.normal = normal;
          fp.texture = texture;
          faces[facesRead].push_back(fp);
        }
        catch (std::invalid_argument err)
        {
          facesRead++;
          return;
        }
      }
    }
    else
    {
      // nao tem normal, so vertice e textura
      FacePoint fp0;
      fp0.vertice = x;
      fp0.texture = texture;
      faces[facesRead].push_back(fp0);
      while (true)
      {
        // Logger::log("Vertice: \n");
        // Logger::logInt(x);
        // Logger::log("Has texture: \n");
        // Logger::logInt(texture);
        // Logger::log("\n");
        try
        {
          x = std::stoi(&str[cursor], &offset);
          cursor = cursor + offset + 1;
          texture = std::stoi(&str[cursor], &offset);
          cursor += offset;

          FacePoint fp;
          fp.vertice = x;
          fp.texture = texture;
          faces[facesRead].push_back(fp);
        }
        catch (std::invalid_argument err)
        {
          facesRead++;
          return;
        }
      }
    }
  }
  else if (onlyVertices)
  {
    FacePoint fp0;
    fp0.vertice = x;
    faces[facesRead].push_back(fp0);
    while (true)
    {
      try
      {
        x = std::stoi(&str[cursor], &offset);
        cursor += offset;
        FacePoint fp;
        fp.vertice = x;
        faces[facesRead].push_back(fp);
      }
      catch (std::invalid_argument err)
      {
        facesRead++;
        return;
      }
    }
  }
  else if (textureHidden)
  {
    cursor += 2;
    int normal = std::stoi(&str[cursor], &offset);
    cursor += offset;

    FacePoint fp0;
    fp0.vertice = x;
    fp0.normal = normal;
    faces[facesRead].push_back(fp0);

    while (true)
    {
      try
      {
        x = std::stoi(&str[cursor], &offset);
        cursor = cursor + offset + 2;

        normal = std::stoi(&str[cursor], &offset);
        cursor += offset;

        FacePoint fp;
        fp.vertice = x;
        fp.normal = normal;
        faces[facesRead].push_back(fp);
      }
      catch (std::invalid_argument err)
      {
        facesRead++;
        return;
      }
    }
  }
}

void Parser::parseLine(const std::string str)
{
  if (str[0] == 'f')
  {
    parseFaceLine(str);
    // std::ostringstream s;
    // s << faces[facesRead - 1].at(0).vertice
    //   << " " << faces[facesRead - 1].at(1).vertice
    //   << " " << faces[facesRead - 1].at(2).vertice
    //   << " "
    //   << "\n";
    // Logger::log(s.str());
  }
  else if (str[0] == 'v')
  {
    if (str[1] == ' ')
    {
      parseVerticeLine(str);
      // std::ostringstream s;
      // s << vertices[verticesRead - 3] << " " << vertices[verticesRead - 2] << " " << vertices[verticesRead - 1] << "\n";
      // Logger::log(s.str());
    }
    else if (str[1] == 't')
    {
      parseTextureLine(str);
    }
    else if (str[1] == 'p')
    {
      parseParamLine(str);
    }
    else if (str[1] == 'n')
    {
      parseNormalLine(str);
      // std::ostringstream s;
      // s << normals[normalsRead - 3] << " " << normals[normalsRead - 2] << " " << normals[normalsRead - 1] << "\n";
      // Logger::log(s.str());
    }
  }
}

void Parser::parseFile(const std::string filename)
{
  std::ifstream file(filename.c_str());
  std::string str;
  while (std::getline(file, str))
  {
    preParseLine(str);
  }
  vertices = new GLdouble[numberVertices * 3];
  normals = new GLdouble[numberNormals * 3];
  params = new GLdouble[numberTextures * 3];
  textures = new GLdouble[numberTextures * 2];
  faces = new std::vector<FacePoint>[numberFaces];

  file.clear();
  file.seekg(0, std::ios::beg);
  while (std::getline(file, str))
  {
    parseLine(str);
  }
}

#endif