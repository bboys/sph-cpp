#include "scene.h"
#include "plane.h"
#include "particle.h"

namespace Graphics {

namespace OpenGL {

Scene::Scene()
:
    Base::Scene(std::make_shared<Particles>(), std::make_shared<Planes>(), 0, 0)
{
}

Scene::Scene(int width, int height)
:
    Base::Scene(std::make_shared<Particles>(), std::make_shared<Planes>(), width, height)
{
}

}

}
