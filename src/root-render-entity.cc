#include "root-render-entity.h"

namespace utaha {

RootRenderEntity::RootRenderEntity() {
}

/*virtual*/ RootRenderEntity::~RootRenderEntity() {
}

/*virtual*/ int RootRenderEntity::OnRender(SDL_Renderer *renderer) {
    if (lchild()) {
        DoRenderAll(lchild(), renderer);
    }
    if (rchild()) {
        DoRenderAll(rchild(), renderer);
    }
    return 0;
}

void RootRenderEntity::DoRenderAll(RenderEntity *node, SDL_Renderer *renderer) {
    if (node->OnRender(renderer) != 0) {
        // render error;
    }
    if (node->lchild()) {
        DoRenderAll(node->lchild(), renderer);
    }
    if (node->rchild()) {
        DoRenderAll(node->rchild(), renderer);
    }
}

} // namespace utaha
