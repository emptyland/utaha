#ifndef UTAHA_RENDER_ENTITY_H_
#define UTAHA_RENDER_ENTITY_H_

#include "base.h"
#include SDL_H

namespace utaha {

class RenderEntity {
public:
    RenderEntity();
    virtual ~RenderEntity();

    virtual int OnRender(SDL_Renderer *renderer);

    DEF_PTR_PROP_RW(RenderEntity, lchild);
    DEF_PTR_PROP_RW(RenderEntity, rchild);
    DEF_PTR_PROP_RW(RenderEntity, parent);

    void Remove();

    void InsertRR(RenderEntity *node) {
        node->rchild_ = rchild_;
        node->parent_ = this;
        rchild_ = node;
    }

    void InsertRL(RenderEntity *node) {
        node->lchild_ = rchild_;
        node->parent_ = this;
        rchild_ = node;
    }

    void InsertLL(RenderEntity *node) {
        node->lchild_ = lchild_;
        node->parent_ = this;
        lchild_ = node;
    }

    void InsertLR(RenderEntity *node) {
        node->rchild_ = lchild_;
        node->parent_ = this;
        lchild_ = node;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(RenderEntity);
private:
    RenderEntity *lchild_ = nullptr;
    RenderEntity *rchild_ = nullptr;
    RenderEntity *parent_ = nullptr;
};

} // namespace utaha

#endif // UTAHA_RENDER_ENTITY_H_
