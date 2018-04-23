#include "root-render-entity.h"

namespace utaha {

RootRenderEntity::RootRenderEntity() {
}

/*virtual*/ RootRenderEntity::~RootRenderEntity() {
}

/*virtual*/ void RootRenderEntity::OnRender(SDL_Renderer *renderer) {
    if (lchild()) {
        DoRenderAll(lchild(), renderer);
    }
    if (rchild()) {
        DoRenderAll(rchild(), renderer);
    }
}

void RootRenderEntity::DoRenderAll(RenderEntity *node, SDL_Renderer *renderer) {
    node->OnRender(renderer);
    if (node->lchild()) {
        DoRenderAll(node->lchild(), renderer);
    }
    if (node->rchild()) {
        DoRenderAll(node->rchild(), renderer);
    }
}

} // namespace utaha
