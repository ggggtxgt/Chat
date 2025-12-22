#include "global.h"

std::function<void(QWidget *)> repolish = [](QWidget *widget) {
    widget->style()->unpolish(widget);      // 去掉旧样式
    widget->style()->polish(widget);        // 加载新样式
};