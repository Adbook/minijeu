//
// Created by adrien on 5/30/17.
//

#include <src/lib/opengl/glutils.hpp>
#include <iostream>
#include <src/lib/3d/mat4.hpp>
#include "model.h"

void Model::translate(cpe::vec3 v) {
    pos += v;
}

void Model::translateTo(cpe::vec3 v) {
    pos = v;
}

void Model::rotate(cpe::vec3 r) {
    rot += r;
}

void Model::rotateTo(cpe::vec3 r) {
    rot = r;
}

void Model::scale(cpe::vec3 v) {
    s.x() *= v.x();
    s.y() *= v.y();
    s.z() *= v.z();
}

void Model::setScale(cpe::vec3 v) {
    s = v;
}

void Model::draw() {
    glUniformMatrix4fv(get_uni_loc(shader,"transformation_matrix"),1,false, getTransformationMatrix().pointer());
    if(texture != -1) glBindTexture(GL_TEXTURE_2D, texture); PRINT_OPENGL_ERROR();
    m_ogl.draw();
}

cpe::mat4 Model::getTransformationMatrix() {
    cpe::mat4 transfo, rotation_x, rotation_y, rotation_z, translation, scaling;
    translation.set_translation(pos);
    scaling.set_scaling(s.x(), s.y(), s.z(), 1);

    rotation_x.set_rotation({1, 0, 0}, rot.x());
    rotation_y.set_rotation({0, 1, 0}, rot.y());
    rotation_z.set_rotation({0, 0, 1}, rot.z());

    return rotation_x * rotation_y * rotation_z * translation * scaling;
}

void Model::load(std::string mesh_file, GLuint tex, GLuint sh) {
    m = cpe::load_mesh_file(mesh_file);
    m.transform_apply_scale(0.1f);
    m.transform_apply_auto_scale_and_center();
    m_ogl.fill_vbo(m);
    texture = tex;
    shader = sh;
}

void Model::load(cpe::mesh msh, GLuint tex, GLuint sh) {
    m = msh; //par copie
    m.transform_apply_scale(0.1f);
    m.transform_apply_auto_scale_and_center();
    m_ogl.fill_vbo(m);
    texture = tex;
    shader = sh;
}

Model::Model() {
    s = {1, 1, 1};
    pos = {0, 0, 0};
    rot = {0, 0, 0};
    texture = 0;
    shader = 0;
}





