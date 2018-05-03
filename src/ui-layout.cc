#include "ui-layout.h"
#include "ui-component.h"
#include "glog/logging.h"

namespace utaha {

UILayout::UILayout(SDL_Window *window)
    : window_(DCHECK_NOTNULL(window)) { UpdateRect(); }

/*virtual*/ UILayout::~UILayout() {
    for (auto row : rows_) {
        delete row;
    }
}

/*virtual*/ int UILayout::OnEvent(SDL_Event *event, bool *is_break) {
    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                UpdateRect(event->window.data1, event->window.data2);
                break;

            default:
                break;
        }
    }

    for (auto row : rows_) {
        row->OnEvent(event, is_break);
        if (*is_break) {
            return 0;
        }
    }
    return 0;
}

/*virtual*/ int UILayout::OnRender(SDL_Renderer *renderer) {
    if (debug_mode_) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0, 0xff, 0);
        SDL_Rect rect = rect_;
        rect.x -= 1;
        rect.y -= 1;
        rect.w += 2;
        rect.h += 2;
        SDL_RenderDrawRect(renderer, &rect);
    }
    for (auto row : rows_) {
        row->OnRender(renderer);
    }
    return 0;
}

void UILayout::UpdateRect() {
    int w = 0, h = 0;
    SDL_GetWindowSize(window_, &w, &h);
    UpdateRect(w, h);
}

void UILayout::UpdateRect(int w, int h) {
    int max_row_w = CalculateMaxRowWidth();
    switch (horizontal_aligment_) {
        case ALIGN_LEFT_OR_TOP:
            rect_.x = 0;
            rect_.w = max_row_w;
            break;
        case ALIGN_RIGHT_OR_BOTTOM:
            rect_.x = w - max_row_w;
            rect_.w = max_row_w;
            break;
        case ALIGN_CENTER:
            rect_.x = (w - max_row_w) / 2;
            rect_.w = max_row_w;
            break;
        default:
            break;
    }

    int rows_h = CalculateRowsHeight();
    switch (vertical_alignment_) {
        case ALIGN_LEFT_OR_TOP:
            rect_.y = 0;
            rect_.h = rows_h;
            break;
        case ALIGN_RIGHT_OR_BOTTOM:
            rect_.y = h - rows_h;
            rect_.h = rows_h;
            break;
        case ALIGN_CENTER:
            rect_.y = (h - rows_h) / 2;
            rect_.h = rows_h;
            break;
        default:
            break;
    }

    int y = rect_.y;
    for (auto row : rows_) {
        y += row->UpdateComponentsRect(row->CalculateWidth(),
                                       row->CalculateHeight(), rect_.x, y);
    }
}

UILayout::Row *UILayout::AddRow(Alignment horizontal_aligment) {
    Row *row = new Row(this, horizontal_aligment);
    rows_.push_back(row);
    return row;
}

int UILayout::CalculateMaxRowWidth() {
    int w = 0;
    for (auto row : rows_) {
        int row_w = row->CalculateWidth();
        if (row_w > w) {
            w = row_w;
        }
    }
    return w;
}

int UILayout::CalculateRowsHeight() {
    int h = 0;
    for (auto row : rows_) {
        h += row->CalculateHeight();
    }
    return h;
}

////////////////////////////////////////////////////////////////////////////////
// class UILayoutRow
////////////////////////////////////////////////////////////////////////////////

UILayoutRow::UILayoutRow(UILayout *layout, UILayout::Alignment horizontal_aligment)
    : layout_(DCHECK_NOTNULL(layout))
    , horizontal_aligment_(horizontal_aligment) {
}

/*virtual*/ UILayoutRow::~UILayoutRow() {
    for (auto component : components_) {
        delete component;
    }
}

/*virtual*/ int UILayoutRow::OnEvent(SDL_Event *event, bool *is_break) {
    for (auto component : components_) {
        component->OnEvent(event, is_break);
        if (*is_break) {
            return 0;
        }
    }
    return 0;
}

/*virtual*/ int UILayoutRow::OnRender(SDL_Renderer *renderer) {
    if (layout_->debug_mode()) {
        SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0);
        SDL_RenderDrawRect(renderer, &rect_);
    }
    for (auto component : components_) {
        component->OnRender(renderer);
    }
    return 0;
}

UILayoutRow *UILayoutRow::AddComponent(UIComponent *component) {
    components_.push_back(DCHECK_NOTNULL(component));
    layout_->UpdateRect();
    return this;
}

int UILayoutRow::CalculateWidth() {
    if (components_.empty()) {
        return 0;
    }

    int w = 0;
    for (auto component : components_) {
        w += (component->rect().w + layout_->padding_size());
    }
    w += layout_->padding_size();
    return w;
}

int UILayoutRow::CalculateHeight() {
    if (components_.empty()) {
        return 0;
    }

    int h = 0;
    for (auto component : components_) {
        if (component->rect().h > h) {
            h = component->rect().h;
        }
    }
    return h + layout_->padding_size() * 2;
}

int UILayoutRow::UpdateComponentsRect(const int row_w, const int row_h,
                                      int original_x, int original_y) {
    rect_.w = row_w;
    rect_.h = row_h;
    rect_.y = original_y;

    switch (horizontal_aligment_) {
        case UILayout::ALIGN_LEFT_OR_TOP:
            rect_.x = original_x;
            break;
        case UILayout::ALIGN_RIGHT_OR_BOTTOM:
            rect_.x = original_x + (layout_->rect().w - row_w);
            break;
        case UILayout::ALIGN_CENTER:
            rect_.x = original_x + (layout_->rect().w - row_w) / 2;
            break;
        default:
            break;
    }

    int x = rect_.x + layout_->padding_size();
    for (auto component : components_) {
        component->mutable_rect()->x = x;
        component->mutable_rect()->y = original_y + (row_h - component->rect().h) / 2;
        x += (layout_->padding_size() + component->rect().w);
    }
    return row_h;
}

} // namespace utaha
