//
// Created by adrien on 5/30/17.
//

#ifndef PGM_MODEL_H
#define PGM_MODEL_H


#include <GL/glew.h>
#include <src/lib/mesh/mesh.hpp>
#include <src/lib/opengl/mesh_opengl.hpp>
#include <src/lib/mesh/mesh_io.hpp>

class Model {

public:

    Model();
    void translate(cpe::vec3);
    void translateTo(cpe::vec3);

    void rotate(cpe::vec3);
    void rotateTo(cpe::vec3);

    void scale(cpe::vec3);
    void setScale(cpe::vec3);

    void draw();
    void load(std::string mesh_file, GLuint tex, GLuint sh);
    void load(cpe::mesh msh, GLuint tex, GLuint sh);

    cpe::mat4 getTransformationMatrix();

private:

    GLuint texture;
    cpe::mesh m;
    cpe::mesh_opengl m_ogl;

    cpe::vec3 pos;
    cpe::vec3 rot;
    cpe::vec3 s; //scale

    GLuint shader;
    int type;


};


#endif //PGM_MODEL_H
