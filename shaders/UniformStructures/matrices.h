#pragma once
#include <glm/glm.hpp>

struct TViewProjMatrices
{
    glm::mat4 alignas(4*16) view;
    glm::mat4 alignas(4*16) projection;
    glm::mat4 alignas(4*16) proj_view;
};
