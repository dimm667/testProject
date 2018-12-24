#ifndef WORLDDATACONTAINER_H
#define WORLDDATACONTAINER_H

#include <array>
enum class Material : unsigned int
{
    AIR,
    WATER,
    DIRT,
    GRAS,
    ROCK
};

class Chunk
{
public:
    Chunk(unsigned int x, unsigned int y, unsigned int z) :
        pos_x(x), pos_y(y), pos_z(z) {}

    void setVoxelMaterial(unsigned x, unsigned y, unsigned z, Material material)
    {
        voxels[x*dimension*dimension + y*dimension + z] = material;
    }

    Material getVoxel(unsigned x, unsigned y, unsigned z)
    {
        return voxels[x*dimension*dimension + y*dimension + z];
    }

private:
    unsigned int pos_x, pos_y, pos_z;
    static constexpr unsigned int dimension = 16;

    std::array<Material, dimension * dimension * dimension> voxels;
};

class WorldDataContainer
{
public:
    WorldDataContainer();
};

#endif // WORLDDATACONTAINER_H
