//
//  polyhedron.h
//  Voxelizer
//
//  Created by atsmsmr on 2015/10/29.
//  Copyright (c) 2015 Atsushi Masumori. All rights reserved.
//

#ifndef __Voxelizer__MeshData__
#define __Voxelizer__MeshData__
#pragma once

//#include <GL/glew.h>
#include <iostream>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

#include <Eigen/Core>
#include <Eigen/Geometry>

struct Box
{
    float position[3];
    float length[3];
    float max_length;
};

class MeshData
{
private:

    
public:
	MeshData();
	~MeshData();
    
    void init();
    void fillHole();
    void scale(float magni);
    void rotate(float angle);
    void translate(float* destination);
    void translateToOrigin();
    void draw(bool is_wired, float angle);
    void drawWires(float angle);
    void drawFaces(float angle);
    float* getCenter();
    Box* getBoundingBox();
    
    float *vertices;
    float *normals;
    unsigned int *faces;
    unsigned int number_of_vertices;
    unsigned int number_of_faces;
    
    Box bounding_box;
    float* center_of_meshes;

    bool is_loaded;
};

class FileIO
{
private:

public:
    FileIO();
    ~FileIO();
    bool import(const char *filepath);
    void clear();

    public :
    class MeshEntry {
    public:
        MeshEntry(aiMesh *a_mesh);
        ~MeshEntry();
        MeshData* getMeshData();

        MeshData* mesh;

        //        std::vector<float> texCoords;
    };
    bool is_loaded;
    std::vector<MeshEntry*> meshEntries;
};


#endif /* defined(__Voxelizer__polyhedron__) */
