#pragma once
#include <glm/glm.hpp>

struct alignas(4*16) TViewProjMatrices
{
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 proj_view;
};
