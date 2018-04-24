#include "render-entity.h"

namespace utaha {

RenderEntity::RenderEntity() {
}

/*virtual*/ RenderEntity::~RenderEntity() {
}

/*virtual*/ int RenderEntity::OnRender(SDL_Renderer *renderer) {
    return 0;
}

void RenderEntity::Remove() {
    if (parent_->lchild_ == this) {
        parent_->lchild_ = lchild_;
    } else if (parent_->rchild_ == this) {
        parent_->rchild_ = lchild_;
    } else {
        // TODO:
    }

    RenderEntity *leaf_node = lchild_;
    if (leaf_node != nullptr) {
        while (leaf_node->rchild_) {
            leaf_node = leaf_node->rchild_;
        }
    }
    leaf_node->lchild_ = rchild_;
    rchild_->parent_ = leaf_node;

    rchild_ = nullptr;
    lchild_ = nullptr;
    parent_ = nullptr;
}

} // namespace utaha
