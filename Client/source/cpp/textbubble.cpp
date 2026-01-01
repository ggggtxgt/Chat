#include "global.h"
#include "textbubble.h"

#include <QFont>
#include <QTextBlock>
#include <QTextLayout>
#include <QFontMetricsF>
#include <QTextDocument>

TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent) : BubbleFrame(role, parent) {
    _text_edit = new QTextEdit();
    _text_edit->setReadOnly(true);
    _text_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _text_edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _text_edit->installEventFilter(this);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    _text_edit->setFont(font);
    setPlainText(text);
    SetWidget(_text_edit);
    initStyleSheet();
}

TextBubble::~TextBubble() noexcept {}

bool TextBubble::eventFilter(QObject *object, QEvent *event) {
    if (object == _text_edit && QEvent::Paint == event->type()) {
        adjustTextHeight();
    }
    return BubbleFrame::eventFilter(object, event);
}

void TextBubble::setPlainText(const QString &text) {
    _text_edit->setPlainText(text);
    // 找到段落中最大宽度
    qreal doc_margin = _text_edit->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    QFontMetricsF fm(_text_edit->font());
    QTextDocument *doc = _text_edit->document();
    int max_width = 0;
    // 遍历每一段找到 最宽的那一段
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    // 字体总长
    {
        int txtW = int(fm.width(it.text()));
        max_width = max_width < txtW ? txtW : max_width;                    // 找到最长的那段
    }
    // 设置这个气泡的最大宽度 只需要设置一次
    // 设置最大宽度
    setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right));
}

void TextBubble::adjustTextHeight() {
    qreal doc_margin = _text_edit->document()->documentMargin();    // 字体到边框的距离默认为4
    QTextDocument *doc = _text_edit->document();
    qreal text_height = 0;
    // 把每一段的高度相加等于文本高
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();
        text_height += text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();
    // 设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
    setFixedHeight(text_height + doc_margin * 2 + vMargin * 2);
}

void TextBubble::initStyleSheet() {
    _text_edit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}