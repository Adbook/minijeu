//
// Created by adrien on 6/3/17.
//

#ifndef PGM_BALLON_H
#define PGM_BALLON_H


#include <src/lib/3d/vec3.hpp>
#include <src/lib/3d/vec4.hpp>
#include "model.h"

class Ballon {
public:
    Ballon();
    void draw();
    Model *getModel();
    void setModel(Model *m);
    void update(float dt);

    void addForce(cpe::vec3 F);
    void reset();

private:
    cpe::vec3 acc;
    cpe::vec3 vit;
    cpe::vec3 pos;

    const float mass = 1;
    Model *m;
};


#endif //PGM_BALLON_H
