#ifndef UTAHA_ROOT_RENDER_ENTITY_H_
#define UTAHA_ROOT_RENDER_ENTITY_H_

#include "render-entity.h"

namespace utaha {

class RootRenderEntity : public RenderEntity {
public:
    RootRenderEntity();
    virtual ~RootRenderEntity();

    virtual void OnRender(SDL_Renderer *rederer) override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(RootRenderEntity);

private:
    void DoRenderAll(RenderEntity *node, SDL_Renderer *renderer);
};

} // namespace utaha

#endif // UTAHA_ROOT_RENDER_ENTITY_H_
