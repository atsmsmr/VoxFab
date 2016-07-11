
#ifndef __Voxelizer__voxelize__
#define __Voxelizer__voxelize__

#if defined(_WIN32) || defined(_WIN64) //to get fmax, fmin to work on Windows/Visual Studio
#include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>

class CVXC_Voxelizer
{
private:
    int get_index(int x, int y, int z);
    float* getAABB(float* vertices, int num_vertices);
    float* getTriAABB(float vertices[3][3]);
    float* getTargetIndexInAABB(float* aabb, float* start_position, float* voxel_size);
    void fillInside();

    // Define states of cells
    unsigned static const int Empty_Cell = 0;
    unsigned static const int Collision_Cell = 1;
    unsigned static const int Surface_Cell = 2;
    unsigned static const int Inner_Cell = 3;
    unsigned static const int Pre_Process_Cell = 4;
    unsigned static const int Processing_Cell = 5;
    unsigned static const int Processed_Cell = 6;

public:
    CVXC_Voxelizer();
    ~CVXC_Voxelizer();
    int* Voxelize(float* vertices, int num_vertices, int vox_num[3], float unit_voxel[3], bool is_fill_inside = false);

    unsigned int voxel_num[3];
    float voxel_origin[3];
    float voxel_size[3];
    int* voxel_data;
};


class CVXC_VoxelizerUtil
{
private:
    void SUB(float* dest, float v1[3], float v2[3]);
    void CROSS(float* dest, float v1[3], float v2[3]);
    float DOT(float v1[3], float v2[3]);

    bool AXISTEST_X01(float a, float b, float fa, float fb);
    bool AXISTEST_X2(float a, float b, float fa, float fb);
    bool AXISTEST_Y02(float a, float b, float fa, float fb);
    bool AXISTEST_Y1(float a, float b, float fa, float fb);
    bool AXISTEST_Z12(float a, float b, float fa, float fb);
    bool AXISTEST_Z0(float a, float b, float fa, float fb);

    bool planeBoxOverlap(float normal[3], float vert[3], float maxbox[3]);

    float v0[3],v1[3],v2[3];
    float vmin,vmax,p0,p1,p2,rad,fex,fey,fez;		// -NJMP- "d" local variable removed
    float normal[3],e0[3],e1[3],e2[3];
    float boxhalfsize[3];

public:
    CVXC_VoxelizerUtil();
    ~CVXC_VoxelizerUtil();
    bool is_collision(float* boxcenter, float* unit_voxel, float triverts[3][3]);
};


#endif /* defined(__Voxelizer__voxelize__) */
