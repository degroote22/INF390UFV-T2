#ifndef MTL_PARSER
#define MTL_PARSER

#include "logger.cpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>

struct Material
{
  double ka[3];
  double kd[3];
  double ks[3];
  double d;
  double ns;
  std::string textureName;
};

class MtlParser
{
private:
  int count = 0;
  std::map<std::string, int> nameMap;
  std::vector<Material> materials;

public:
  void
  parseFile(std::string filename);
  std::vector<Material> getMaterials() { return materials; };
  const std::map<std::string, int> getMaterialsNameMap() { return nameMap; };
  void parseLine(std::string str, Material &mat);
};

void MtlParser::parseLine(std::string str, Material &mat)
{
  if (str[0] == 'm' && str[1] == 'a')
  {
    std::stringstream ss(str);
    std::string name;
    ss >> name;
    ss >> name;
    mat.textureName = name;
  }
  else if (str[0] == 'K' && str[1] == 'a')
  {
    int cursor = 2;
    std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
    for (int i = 0; i < 3; i++)
    {
      double x = std::stod(&str[cursor], &offset);
      mat.ka[i] = x;
      cursor += offset;
    }
  }
  else if (str[0] == 'K' && str[1] == 'd')
  {
    int cursor = 2;
    std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
    for (int i = 0; i < 3; i++)
    {
      double x = std::stod(&str[cursor], &offset);
      mat.kd[i] = x;
      cursor += offset;
    }
  }
  else if (str[0] == 'K' && str[1] == 's')
  {
    int cursor = 2;
    std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
    for (int i = 0; i < 3; i++)
    {
      double x = std::stod(&str[cursor], &offset);
      mat.ks[i] = x;
      cursor += offset;
    }
  }
  else if (str[0] == 'N' && str[1] == 's')
  {
    int cursor = 2;
    std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
    double x = std::stod(&str[cursor], &offset);
    mat.ns = x;
  }
  else if (str[0] == 'd')
  {
    int cursor = 1;
    std::size_t offset = 0; //offset will be set to the length of characters of the "value" - 1.
    double x = std::stod(&str[cursor], &offset);
    mat.d = x;
  }
}

void MtlParser::parseFile(std::string filename)
{
  std::ifstream mtl((filename + ".mtl").c_str());

  std::string str;
  std::getline(mtl, str);
  while (true)
  {

    if (str[0] == 'n' && str[1] == 'e' && str[2] == 'w')
    {
      // eh o começo do material novo.

      std::stringstream ss(str);
      std::string name;
      ss >> name;
      ss >> name;
      nameMap[name] = count;

      std::getline(mtl, str);
      Material mat;
      while ((str[0] != 'n' || str[1] != 'e' || str[2] != 'w') && !mtl.eof())
      {
        parseLine(str, mat);
        std::getline(mtl, str);
      }
      materials.push_back(mat);
      count++;
    }
    else
    {
      std::getline(mtl, str);
    }

    if (mtl.eof())
    {
      // hack feio pra colocar o nome da pasta no nome dos arquivos...
      std::string sub = filename.substr(0, filename.find('/'));
      for (int i = 0; i < materials.size(); i++)
      {
        if (materials[i].textureName != "")
        {
          materials[i].textureName = sub + '/' + materials[i].textureName;
        }
      }
      return;
    }
  }
}

#endif