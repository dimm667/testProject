#ifndef VOXEXPORTER_H
#define VOXEXPORTER_H

#include <fstream>
#include <iostream>
#include <iostream>
#include <vector>
#include <array>
#include <stdexcept>

#include "shaderprogram.h"

namespace vox {

typedef char chunkName[4];

const char MAIN[4] = {'V', 'O', 'X', ' '};
const char SIZE[4] = {'S', 'I', 'Z', 'E'};
const char XYZI[4] = {'X', 'Y', 'Z', 'I'};
const char RGBA[4] = {'R', 'G', 'B', 'A'};

enum class chunkType
{
    MAIN,
    PACK,
    SIZE,
    XYZI,
    RGBA,
    MATT,
};

struct VOX
{
    chunkName   name;
    int         version;
};


struct ChunkInfo
{
    chunkName   name;
    int         contentSize;
    int         childrensSize;
};

struct packContent
{
    int         numModels;
};

struct Size
{
    unsigned int         x;
    unsigned int         y;
    unsigned int         z;      // gravity direction
};


union Voxel
{
    unsigned int    all;
    struct
    {
        unsigned char   x;
        unsigned char   y;
        unsigned char   z;      // gravity direction
        unsigned char   colorIdx;
    } component;
};

struct Voxels
{
    int                 numVoxels;
    std::vector<Voxel>  voxel;
};

class Model
{
public:
    unsigned int getNumVoxels() const {return voxels.numVoxels;}
    Voxel getVoxel(unsigned int idx) const {return voxels.voxel[idx];}

    Size size;
    Voxels voxels;
};

union Color
{
    unsigned int    all;
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } component;
};

struct Palette
{
    std::array<Color, 256> color;
};

enum class MaterialType
{
    diffuse,
    metal,
    glass,
    emissive,
};

union MaterialProperty
{
    unsigned int all;
    struct
    {
        unsigned int plastic        : 1;
        unsigned int roughness      : 1;
        unsigned int specular       : 1;
        unsigned int ior            : 1;
        unsigned int attenuation    : 1;
        unsigned int power          : 1;
        unsigned int glow           : 1;
        unsigned int isTotalPower   : 1;
    } component;
};

struct Material
{
    MaterialType type;
    float weight;
    MaterialProperty property;
    std::vector<float> value;
};


class VoxExporter
{
public:
    VoxExporter(const std::string & pathToFile);

    void draw(ShaderProgram & shader);

    const Model & getModel(unsigned int idx) {return models[0];}
    Color getColor(unsigned int index) {return palette.color[index];}

private:
    std::vector<Model> models;
    Palette palette;
};

}

#endif // VOXEXPORTER_H
