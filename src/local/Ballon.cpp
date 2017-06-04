
#include <iostream>
#include "Ballon.h"

Ballon::Ballon(){

}
void Ballon::draw() {

}

Model *Ballon::getModel() {
    m->translateTo(pos);
    return m;
}

void Ballon::setModel(Model *pm) {
    m = pm;
}

void Ballon::update(float dt) {
    vit += acc *dt;
    pos += vit *dt;
    if(pos.z() > 7) reset();
    std::cout << pos.z() << std::endl;
}

void Ballon::addForce(cpe::vec3 F) {
    acc += F/mass;
}

void Ballon::reset() {
    pos = {0, 0.2, 0};
    vit = {0, 0, 0};
    acc = {0, (-10)*mass, 0};
}
