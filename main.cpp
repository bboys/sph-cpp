#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"
#include "scene.h"
#include "particle.h"
#include "plane.h"
#include "bucket.h"
#include "particlesource.h"
#include "utils.h"
#include "unistd.h"

int w_width=1024, w_height=768;
Scene scene(w_width, w_height);

enum class MouseMode {
    IDLE,
    ZOOMING,
    PANNING,
    ROTATING
};

float camera_x = 0;
float camera_y = 0;

float camera_pitch = 0;
float camera_heading = 0;
float camera_zoom = 0.5;

int mouse_x = 0;
int mouse_y = 0;

int mouse_dx = 0;
int mouse_dy = 0;

float max_zpos = 200.0f;
float fps = 100.0f;
extern float num_neighbour_particles;

bool paused = false;

GLfloat light_position[] = {0.0, 10.0, 5.0, 1.0};

MouseMode mouse_mode = MouseMode::IDLE;

size_t max_particles = 3000;
float max_particle_boundary = 0.5f;
float particle_middle = 0.0f;
size_t bucket_num = 20;
Buckets *buckets = new Buckets(bucket_num, bucket_num, bucket_num);
std::vector<ParticleSource> particlesources;
float tstep[3] = {1e-2f, 1e-3f,5e-2f};
bool shit_is_aan = false;
bool fps_toggle = false;
int tstep_toggle = 0;
float mps = mass_0 * 50;

int png_iter = 0;

void init()
{
    Particle::init_wall();
    float color[4] = {0.0f, 0.0f, 1.0f, 1.0f};

    //add particle source
    float pos[3] = {0.9f,0.9f,0.9f};
    float velocity[3] = {0.0f,0.0f,0.0f};
    velocity[0] = -3.0f;
    particlesources.push_back(ParticleSource(pos, color, buckets, scene.particles->get_particles().get(), mps, velocity, max_particles/2));
    pos[0] = -0.9f;
    pos[2] = -0.9f;
    velocity[0] = 3.0f;
    particlesources.push_back(ParticleSource(pos, color, buckets, scene.particles->get_particles().get(), mps, velocity, max_particles/2));


    scene.init_shaders();
}

void construct_planes()
{
    float pos[3] = {0.0f, 0.0f, -10.0f};
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float direction[3] = {0.0f, 0.0f, 0.0f};

    for (size_t idx = 0; idx != 3; ++idx)
    {
        direction[idx] = 1.0f;
        pos[idx] = -3.0f;
        Graphics::Base::Plane obj(pos, color, direction);
        scene.planes->push_back(std::move(obj));
        direction[idx] = 0.0f;
        pos[idx] = 0.0f;
    }
}

void set_lights()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void display_fps()
{
    size_t time = glutGet(GLUT_ELAPSED_TIME);
    static size_t frame = 0;
    static size_t timebase = time;
    char fps_text[12];
    char nnp_text[12];
    char particle_text[12];

    frame++;
    if (time - timebase > 1000)
    {
        fps = frame * 1000.0 / static_cast<float>(time-timebase);
        timebase = time;
        frame = 0;
    }
    sprintf(fps_text, "FPS: %6.2f", fps);
    sprintf(nnp_text, "NNP: %6.2f", num_neighbour_particles);
    sprintf(particle_text, " NP: %6zu", scene.particles->get_particles()->size());

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w_width, 0, w_height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2i(w_width - 80, w_height - 30);
    for (size_t i = 0; i != strlen(fps_text); ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, fps_text[i]);
    glRasterPos2i(w_width - 80, w_height - 50);
    for (size_t i = 0; i != strlen(nnp_text); ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, nnp_text[i]);
    glRasterPos2i(w_width - 80, w_height - 70);
    for (size_t i = 0; i != strlen(particle_text); ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, particle_text[i]);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void draw_cube()
{
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glEnd();
}

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.bind_fbo();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    scene.set_perspective(45.0f, (float)w_width/w_height, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 3.0 / camera_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    scene.draw_planes();

    glEnable(GL_RESCALE_NORMAL);

    if (shit_is_aan)
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // panning
    glTranslatef(camera_x, camera_y, 0.0);

    // pitch
    glRotatef(camera_pitch, 1.0, 0.0, 0.0);

    // heading
    glRotatef(camera_heading, 0.0, 1.0, 0.0);

    if (!shit_is_aan)
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    scene.draw_particles_depth_only();

    scene.draw_particles_color_only();

    draw_cube();

    scene.bind_fbo(2);

    scene.draw_normpass();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    scene.draw_contours();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    display_fps();

    glutSwapBuffers();
    
    if (!paused)
    {
        float tstep2;
        if (fps_toggle)
            tstep2 = 1.0f / fps * 0.5f;
        else
            tstep2 = tstep[tstep_toggle];
        scene.particles->update(tstep2);
        for (std::vector<ParticleSource>::iterator it = particlesources.begin(); it != particlesources.end(); ++it)
            it->update(tstep2);
    }
}

// Pressing R resets the camera
void reset_camera()
{
    camera_x = 0;
    camera_y = 0;

    camera_pitch = 0;
    camera_heading = 0;
    camera_zoom = 0.5;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'r':
        case 'R':
            reset_camera();
            return;
        case 'o':
        case 'O':
            scene.toggle_opengl();
            return;
        case 'd':
        case 'D':
            scene.toggle_deferred();
            return;
        case 'p':
        case 'P':
            scene.toggle_vbo();
            return;
        case 'c':
        case 'C':
            scene.toggle_contour();
            return;
        case 'n':
        case 'N':
            scene.toggle_normals();
            return;
        case 's':
        case 'S':
            scene.toggle_shadows();
            return;
        case 'w':
        case 'W':
            scene.toggle_water();
            return;
        case 'y':
        case 'Y':
            scene.particles->toggle_2d_mode();
            return;
        case 'g':
        case 'G':
            shit_is_aan = !shit_is_aan;
            return;
        case 'f':
        case 'F':
            fps_toggle = !fps_toggle;
            return;
        //~ case 'q':
        //~ case 'Q':
        case 27: // ESC key
             glutLeaveMainLoop();
            return;
        case ' ':
            paused = !paused;
            return;
        case 'T':
        case 't':
            tstep_toggle = (tstep_toggle + 1) % 3;
            std::cout << "timestep: "; dump(tstep[tstep_toggle]);
            return;
        case 'B':
        case 'b':
        {
            std::stringstream ss;
            ss << "screenshots/" << png_iter++ << ".png";
            scene.save_to_png(ss.str());
            return;
        }
        default:
            break;
    }
}

void reshape(int w, int h)
{
    w_width = w;
    w_height = h;
    scene.set_size(w, h);
    glViewport(0, 0, w_width, w_height);
    scene.reshape();
    glMatrixMode(GL_MODELVIEW);

}

void mouse(int button, int state, int x, int y)
{
    mouse_x = x;
    mouse_y = y;

    mouse_dx = 0;
    mouse_dy = 0;

    if (state == GLUT_UP)
    {
        mouse_mode = MouseMode::IDLE;
        return;
    }

    if (glutGetModifiers() & GLUT_ACTIVE_CTRL)
        mouse_mode = MouseMode::ZOOMING;
    else if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
        mouse_mode = MouseMode::PANNING;
    else
        mouse_mode = MouseMode::ROTATING;
}

void motion(int x, int y)
{
    mouse_dx = x - mouse_x;
    mouse_dy = y - mouse_y;
}

// rotate along the x and y axis only
void rotate(float vec[3])
{
    float temp[3];
    memcpy(temp, vec, 3*sizeof(float));
    vec[0] = temp[0] * cos(M_PI * -camera_heading / 180.0f) + temp[1] * sin(M_PI * -camera_pitch / 180.0f) * sin(M_PI * -camera_heading / 180.0f);
    vec[1] = temp[1] * cos(M_PI * -camera_pitch / 180.0f);
    vec[2] = temp[1] * cos(M_PI * -camera_heading / 180.0f) * sin(M_PI * -camera_pitch / 180.0f) - temp[0] * sin(M_PI * -camera_heading / 180.0f);
}

void idle()
{
    set_zero(Acc_ext);
    memcpy(gravity, gravity_0, 3*sizeof(float));

    if (shit_is_aan)
        rotate(gravity);

    switch (mouse_mode)
    {
        case MouseMode::ZOOMING:
            camera_zoom *= exp(mouse_dy/1000.0);
            camera_zoom = std::max(0.5f, std::min(camera_zoom, 10.0f));
            break;

        case MouseMode::PANNING:
            camera_x += 1.0 / 5000 * mouse_dx;
            camera_y -= 1.0 / 5000 * mouse_dy;
            Acc_ext[0] = -1.0 / 5000 * mouse_dx / tstep[tstep_toggle];
            Acc_ext[1] = 1.0 / 5000 * mouse_dy / tstep[tstep_toggle];
            rotate(Acc_ext);
            break;

        case MouseMode::ROTATING:
            camera_heading += 1.0 / 100 * mouse_dx;
            camera_pitch   += 1.0 / 100 * mouse_dy;
            break;

        default:
        case MouseMode::IDLE:
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    // parsing command line options
    char c = 0;
    while ( (c = getopt(argc, argv, "p:s:u:r:f:m:d:n:q:")) != -1)
    {
        switch (c)
        {
            case 'p':
                stiffness = atof(optarg);
                break;
            case 's':
                sigma = atof(optarg);
                break;
            case 'u':
                mu = atof(optarg);
                break;
            case 'r':
                r_0 = atof(optarg);
                break;
            case 'f':
                radius_factor = atof(optarg);
                break;
            case 'm':
                mass_0 = atof(optarg);
                break;
            case 'd':
                density_0 = atof(optarg);
                break;
            case 'n':
                max_particles = atoi(optarg);
                break;
            case 'q':
                mps = atof(optarg);
                break;
            default:
                break;
        }
    }
    std::cout << "stiffness (p): "; dump(stiffness);
    std::cout << "surface tension (s): "; dump(sigma);
    std::cout << "dynamic viscosity (u): "; dump(mu);
    std::cout << "particle radius (r): "; dump(r_0);
    std::cout << "smoothing factor (f): "; dump(radius_factor);
    std::cout << "particle mass (m): "; dump(mass_0);
    std::cout << "density (d): "; dump(density_0);
    std::cout << "number of particles (n): "; dump(max_particles);
    std::cout << "particle spawning rate (q): "; dump(mps);
    std::cout << "timestep: "; dump(tstep[tstep_toggle]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w_width, w_height);
    glutCreateWindow("GPU ray-casting of quadratic surfaces");

    glewInit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    set_lights();

    init();

    glutMainLoop();

    delete buckets;

    return 0;
}
