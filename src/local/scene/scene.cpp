

#include <GL/glew.h>

#include "scene.hpp"

#include "../../lib/opengl/glutils.hpp"
#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"
#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>


using namespace cpe;

cpe::mesh *create_plane(float xmin, float xmax, float ymin, float ymax, int Nu, int Nv){

    cpe::mesh * new_mesh = new cpe::mesh;

    float dir_piste = 1;
    vec2 p1 = vec2(xmin +  (xmax-xmin)/2, ymin +  (ymax-ymin)/2);
    vec2 p2 = vec2(xmin  +4  +(xmax-xmin)/2, ymin + 0.5+ (ymax-ymin)/2);
    for(int kv = 0; kv < Nv; kv++){
        for(int ku = 0; ku < Nu; ku++){
            float kx = xmin + ku * (xmax - xmin)/Nu;
            float ky = ymin + kv * (ymax - ymin)/Nv;
            float kz = 0;
            new_mesh->add_vertex({kx, kz, ky});
            new_mesh->add_texture_coord({(float)kv/(float)Nv, (float)ku/(float)Nu});

            //new_mesh->add_color({abs(kx/xmax), kz/5, abs(ky/ymax)});
            int k0 = ku + kv * Nu;
            int k1 = ku + 1 + kv * Nu;
            int k2 = ku + (kv + 1) * Nu;
            int k3 = ku + 1 + (kv + 1) * Nu;

            if(ku < Nu - 1 && kv < Nv - 1){
                new_mesh->add_triangle_index({k0, k1, k3});
                new_mesh->add_triangle_index({k0, k2, k3});
            }
        }
    }


    new_mesh->fill_normal();
    new_mesh->fill_empty_field_by_default();
    return new_mesh;
}

float distance2(vec2 A, vec2 B){
    return (A.x() - B.x() ) * (A.x() - B.x() ) + (A.y() - B.y() ) * (A.y() - B.y() );
}
cpe::mesh *create_terrain(float xmin, float xmax, float ymin, float ymax, int Nu, int Nv){

    cpe::mesh * new_mesh = new cpe::mesh;

    //définition de la position et des dimensions de la piste
    vec2 piste_topleft = vec2(xmin +  (xmax-xmin)/4, ymin +  (ymax-ymin)/4);
    float piste_w = 3;
    float piste_h = 0.3;
    float piste_z = -0.9;

    //génération de chaque triangle
    for(int kv = 0; kv < Nv; kv++){
        for(int ku = 0; ku < Nu; ku++){

            //calcul des coordonnées du vertex
            float kx = xmin + ku * (xmax - xmin)/Nu;
            float ky = ymin + kv * (ymax - ymin)/Nv;
            float kz = perlin(30, 0.6).operator()(vec2(kx/xmax,ky/ymax)) -2; //bruit de perlin redescendu


            //si le point appartient à la piste, on modifie ses coordonnées et sa couleur
            if(kx > piste_topleft.x() && kx < piste_topleft.x() + piste_w && ky > piste_topleft.y() && ky < piste_topleft.y() + piste_h){//|| (0&(kx > piste_topleft.x() && kx < p2.x() && ky > piste_topleft.y() && ky < p2.y()))) {
                    kz = piste_z;
                    new_mesh->add_vertex({kx, kz, ky});
                    new_mesh->add_color({0.97, 1, 1});
            }
            else { //si le point est hors de la piste

                //calcul de la "distance" carrée du point par rapport au rectangle de la piste
                float dx = fmax(fabs(kx - (piste_topleft.x() + piste_w/2.f)) - piste_w / 2, 0);
                float dy = fmax(fabs(ky - (piste_topleft.y() + piste_h/2.f)) - piste_h / 2, 0);
                float d = dx * dx + dy * dy;
                float dlimit = 0.6f; //seuil de distance à la piste à partir duquel on commence à interpoler

                if(d <= dlimit * dlimit){ //si le point se trouve proche de la piste, on interpole la hauteur générée par le bruit entre la hauteur à dlimit et la hauteur de la piste
                    float t = sqrt(d)/(dlimit * dlimit); //coefficient pour l'interpolation, 0  < t < 1
                    kz = ( t) * kz + (1 - t) * piste_z; //interpolation
                }


                if (kz < -1.0) kz = -1.0f; //gestion du niveau de l'eau, on aplatit ce qui est au dessous

                new_mesh->add_vertex({kx, kz, ky}); //on ajoute le point

                //gestion des couleurs. on remonte la hauteur pour avoir des valeurs plus faciles à gérer
                kz += 1;
                if (kz <= 0.0) {
                    new_mesh->add_color({0.663, 0.663, 0.99});
                } else if (kz <= 0.2) {
                    new_mesh->add_color({0.2, 0.01, 0.01});
                } else if (kz <= 0.4) {
                    new_mesh->add_color({0.2, 0.5, 0.01});
                } else if (kz <= 0.8) {
                    new_mesh->add_color({0.45, 0.80, 0.01});
                } else {
                    new_mesh->add_color({0.97, 1, 1});
                }
                }

            //calcul des triangles
            int k0 = ku + kv * Nu;
            int k1 = ku + 1 + kv * Nu;
            int k2 = ku + (kv + 1) * Nu;
            int k3 = ku + 1 + (kv + 1) * Nu;

            if(ku < Nu - 1 && kv < Nv - 1){
                new_mesh->add_triangle_index({k0, k1, k3});
                new_mesh->add_triangle_index({k0, k2, k3});
            }
        }
    }

    return new_mesh;
}
void scene::load_scene()
{


    shader_program_id = read_shader("shaders/shader_mesh.vert",
                                    "shaders/shader_mesh.frag");

    modele_ballon.load("data/ballon.obj", load_texture_file("data/ballon.png"), shader_program_id);
    modele_gant.load("data/glove.obj", load_texture_file("data/glove.png"), shader_program_id);
    modele_gant.translate({0, 1, 0});

    modele_terrain.load(*create_plane(-10, 10, -15, 15, 10, 15), load_texture_file("data/terrain.jpg"), shader_program_id);
    modele_terrain.setScale({10, 10, 10});

    cpe::mesh cage_mesh = load_mesh_file("data/cage.obj");
    cage_mesh.fill_empty_field_by_default();
    cage_mesh.fill_color({1, 1, 1});
    modele_cage.load(cage_mesh, load_texture_file("data/cage.png"), shader_program_id);
    modele_cage.setScale({2, 2, 2});
    modele_cage.translate({-4.7, 0.45, 0.3});
    modele_cage.rotate({0, M_PI/2.0f, 0});

    cpe::mat4 world_matrix_zoom; world_matrix_zoom.set_translation({0.0f,0.0f,10});
    cpe::mat4 world_matrix_translation; world_matrix_translation.set_translation({-5, 0, 0});
    //cpe::mat4 world_matrix_rotation=;
    cpe::mat4 world_matrix=world_matrix_zoom/**world_matrix_rotation*/*world_matrix_translation;

    pwidget->nav.tr = {0, 0, 0};
   /* pwidget->nav.track.set_2d_coords(0, 0, 1, 1);
    pwidget->nav.track.apply_rotation();*/
    pwidget->nav.trackball_rotate(0,0);
}

void scene::draw_scene()
{
    camera.set_translation({5, -1, 0});
    cpe::mat4 rotx; rotx.set_rotation({1,0,0}, 0.1);
    cpe::mat4 roty; roty.set_rotation({0,1,0}, 180);
    cpe::mat4 rotz; rotz.set_rotation({0,0,1}, 0.1);
    camera *= rotx * roty * rotz;
    //camera.set_rotation({1, 0, 0}, 0.1);
    //Setup uniform parameters
    glUseProgram(shader_program_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_modelview"),1,false,camera.pointer());     PRINT_OPENGL_ERROR();
    //glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_projection"),1,false,camera.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();

    modele_gant.draw();
    modele_ballon.draw();
    modele_terrain.draw();
    modele_cage.draw();



}


scene::scene()
    :shader_program_id(0)
{}


GLuint scene::load_texture_file(std::string const& filename)
{
    return pwidget->load_texture_file(filename);
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}


