#ifndef SPH_GRAPHICS_OPENGL_SCENE_H
#define SPH_GRAPHICS_OPENGL_SCENE_H

#include "graphics/base/scene.h"

namespace Graphics {

namespace OpenGL {

class Scene : public Base::Scene
{
public:
    Scene();
    Scene(int width, int height);
};

}

}

#endif
