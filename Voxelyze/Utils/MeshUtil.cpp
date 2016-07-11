
#include <assert.h>
#include "MeshUtil.h"

#if defined(_WIN32) || defined(_WIN64) //to get fmax, fmin to work on Windows/Visual Studio
#define fmax max
#define fmin min
#endif


MeshData::MeshData()
{
    vertices = NULL;
    normals = NULL;
    faces = NULL;
    center_of_meshes = NULL;
    is_loaded = false;
}

MeshData::~MeshData()
{
    delete vertices;
    delete normals;
    delete faces;
    delete center_of_meshes;
}

void MeshData::draw(bool is_wired, float g_angle){

    if(is_wired) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glPushMatrix();
    glRotatef(g_angle, 0.0f, 1.0f, 0.0f);

    for(unsigned int i=0; i<this->number_of_vertices*3; i=i+9){
        glBegin(GL_TRIANGLES);

        GLfloat normal[3] = {this->normals[i],this->normals[i+1],this->normals[i+2]};
        GLfloat p1[3] = {this->vertices[i],this->vertices[i+1],this->vertices[i+2]};
        GLfloat p2[3] = {this->vertices[i+3],this->vertices[i+4],this->vertices[i+5]};
        GLfloat p3[3] = {this->vertices[i+6],this->vertices[i+7],this->vertices[i+8]};

        glNormal3fv(normal);
        glColor3f(1.0,0,0);
        glVertex3fv(p1);
        glVertex3fv(p2);
        glVertex3fv(p3);

        glEnd();
    }

    glPopMatrix();

}

void MeshData::drawWires(float angle){
    bool is_wired = true;
    this->draw(is_wired, angle);
}

void MeshData::drawFaces(float angle){
    bool is_wired = false;
    this->draw(is_wired, angle);
}


Box* MeshData::getBoundingBox(){
    return &bounding_box;
}

float* MeshData::getCenter(){

    //Initialize min and max values
    float min_x = this->vertices[0];
    float min_y = this->vertices[0];
    float min_z = this->vertices[0];
    float max_x = this->vertices[0];
    float max_y = this->vertices[0];
    float max_z = this->vertices[0];

    //Calculate center of meshes
    for(int i=0; i<this->number_of_vertices; i++){
        if(min_x > this->vertices[i*3]) min_x = this->vertices[i*3];
        if(min_y > this->vertices[i*3+1]) min_y = this->vertices[i*3+1];
        if(min_z > this->vertices[i*3+2]) min_z = this->vertices[i*3+2];

        if(max_x < this->vertices[i*3]) max_x = this->vertices[i*3];
        if(max_y < this->vertices[i*3+1]) max_y = this->vertices[i*3+1];
        if(max_z < this->vertices[i*3+2]) max_z = this->vertices[i*3+2];
    }

    center_of_meshes = new float[3];
    center_of_meshes[0] = (max_x+min_x)/2.0f;
    center_of_meshes[1] = (max_y+min_y)/2.0f;
    center_of_meshes[2] = (max_z+min_z)/2.0f;

    //set value to AABB
    for(int i=0; i<3; i++){
        bounding_box.position[i] = center_of_meshes[i];
    }

    bounding_box.length[0] = (max_x-min_x);// *1.1 is for avoiding rounding(?) error.
    bounding_box.length[1] = (max_y-min_y);
    bounding_box.length[2] = (max_z-min_z);

    bounding_box.max_length = fmax(fmax(bounding_box.length[0],bounding_box.length[1]), bounding_box.length[2]);
    bounding_box.max_length *= sqrt(2.0);
    return center_of_meshes;
}

void MeshData::translate(float* destination){

    for(int i=0; i<this->number_of_vertices; i++){
        this->vertices[i*3] += destination[0];
        this->vertices[i*3+1] += destination[1];
        this->vertices[i*3+2] += destination[2];
    }

}

void MeshData::translateToOrigin(){

    float* center = this->getCenter();
    float destination[3] = {-center[0],-center[1],-center[2]};

    translate(destination);

    //set center of AABB to origin
    for(int i=0; i<3; i++){
        bounding_box.position[i] = 0;
    }

}

void MeshData::scale(float magni){

    for(int i=0; i<this->number_of_vertices*3; i++){
        this->vertices[i] *= magni;
    }
    this->getCenter();
}

void MeshData::rotate(float angle){

    float radian = angle*M_PI/180.0;
    Eigen::Quaternionf rotate(Eigen::AngleAxisf(radian, Eigen::Vector3f::UnitX()));

    for(int i=0; i<this->number_of_vertices*3; i=i+3){
        float vec[3] = {this->vertices[i],this->vertices[i+1],this->vertices[i+2]};
        Eigen::Vector3f vec_origin = Eigen::Map<Eigen::Vector3f>(vec);
        Eigen::Vector3f vec_rotate = rotate * vec_origin;
        this->vertices[i] = vec_rotate[0];
        this->vertices[i+1] = vec_rotate[1];
        this->vertices[i+2] = vec_rotate[2];
    }

}

//void MeshData::rotate()
//{
//    float* axis;
//    axis = new float[3]{0.0f, 1.0f, 0.0f};
//
//    for(int i=0; i<this->number_of_vertices*3; i++)
//    {
//        this->rotateVertex(&this->vertices[i], axis, M_PI/2);
//    }
//
//}
//
//void MeshData::rotateVertex(float* vertex, float* axis, float radians)//const point3D& axis, const float radians, const point3D& point )
//{
//    float matrix[3][3];
//
//    float sn = sinf(radians);
//    float cs = cosf(radians);
//
//    float xSin = axis[0] * sn;
//    float ySin = axis[1] * sn;
//    float zSin = axis[2] * sn;
//    float oneMinusCS = 1.0f - cs;
//    float xym = axis[0] * axis[1] * oneMinusCS;
//    float xzm = axis[0] * axis[2] * oneMinusCS;
//    float yzm = axis[1] * axis[2] * oneMinusCS;
//
//    matrix[0][0] = (axis[0]*axis[0]) * oneMinusCS + cs;
//    matrix[0][1] = xym + zSin;
//    matrix[0][2] = xzm - ySin;
//    matrix[1][0] = xym - zSin;
//    matrix[1][1] = (axis[1]*axis[1]) * oneMinusCS + cs;
//    matrix[1][2] = yzm + xSin;
//    matrix[2][0] = xzm + ySin;
//    matrix[2][1] = yzm - xSin;
//    matrix[2][2] = (axis[2]*axis[2]) * oneMinusCS + cs;
//
//    vertex[0] = matrix[0][0] * vertex[0] + matrix[0][1] * vertex[1] + matrix[0][2] * vertex[2];
//    vertex[1] = matrix[1][0] * vertex[0] + matrix[1][1] * vertex[1] + matrix[1][2] * vertex[2];
//    vertex[2] = matrix[2][0] * vertex[0] + matrix[2][1] * vertex[1] + matrix[2][2] * vertex[2];
//}


FileIO::MeshEntry::MeshEntry(aiMesh *a_mesh) {

    this->mesh = new MeshData;
    this->mesh->number_of_vertices = a_mesh->mNumVertices;
    this->mesh->number_of_faces = a_mesh->mNumFaces;

    if(a_mesh->HasPositions()) {
        this->mesh->vertices = new float[a_mesh->mNumVertices*3];
        for(int i = 0; i < a_mesh->mNumVertices; ++i) {
            this->mesh->vertices[i * 3] = a_mesh->mVertices[i].x;
            this->mesh->vertices[i * 3 + 1] = a_mesh->mVertices[i].y;
            this->mesh->vertices[i * 3 + 2] = a_mesh->mVertices[i].z;
        }
    }

    if(a_mesh->HasNormals()) {
        this->mesh->normals = new float[a_mesh->mNumVertices*3];
        for(int i = 0; i < a_mesh->mNumVertices; ++i) {
            this->mesh->normals[i * 3] = a_mesh->mNormals[i].x;
            this->mesh->normals[i * 3 + 1] = a_mesh->mNormals[i].y;
            this->mesh->normals[i * 3 + 2] = a_mesh->mNormals[i].z;
        }
    }

    if(a_mesh->HasFaces()) {
        this->mesh->faces = new GLuint[a_mesh->mNumFaces*3];
        for(int i = 0; i < a_mesh->mNumFaces; ++i) {
            this->mesh->faces[i * 3] = a_mesh->mFaces[i].mIndices[0];
            this->mesh->faces[i * 3 + 1] = a_mesh->mFaces[i].mIndices[1];
            this->mesh->faces[i * 3 + 2] = a_mesh->mFaces[i].mIndices[2];
        }
    }

    //    if(mesh->HasTextureCoords(0)) {
    //		float *texCoords = new float[mesh->mNumVertices * 2];
    //		for(int i = 0; i < mesh->mNumVertices; ++i) {
    //			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
    //			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
    //		}
    //	}
}

FileIO::MeshEntry::~MeshEntry(){
    delete mesh;
}

MeshData* FileIO::MeshEntry::getMeshData(){
    return this->mesh;
}

FileIO::FileIO(){
    is_loaded = false;
}


FileIO::~FileIO()
{
    clear();
}

bool FileIO::import(const char *filepath){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath,
                                             aiProcess_CalcTangentSpace         |
                                             aiProcess_Triangulate              |
                                             aiProcess_SplitLargeMeshes         |
//                                             aiProcess_OptimizeMeshes           |
//                                             aiProcess_JoinIdenticalVertices    |
//                                             aiProcess_RemoveRedundantMaterials |
//                                             aiProcess_GenNormals               |
                                             aiProcess_SortByPType);
    if(!scene) {
        printf("Unable to laod mesh: %s\n", importer.GetErrorString());
    }

    for(int i=0; i<scene->mNumMeshes; ++i) {
        meshEntries.push_back(new FileIO::MeshEntry(scene->mMeshes[i]));
    }

    return 1;
}

void FileIO::clear(){
    for(int i = 0; i < meshEntries.size(); ++i) {
        delete meshEntries.at(i);
    }
    meshEntries.clear();
}
