#include <QtWidgets>
#include "CCodeEditor.h"

CCodeEditor::CCodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    m_sourceLineWidget = new CLineWidget(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineWidgetWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(UpdateLineWidget(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(HighlightLine(void)));

    UpdateLineWidgetWidth(0);
    HighlightLine();
}

int CCodeEditor::OnSizeChanged(void)
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 15 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void CCodeEditor::UpdateLineWidgetWidth(int)
{
    setViewportMargins(OnSizeChanged(), 0, 0, 0);
}

void CCodeEditor::UpdateLineWidget(const QRect &rect, int dy)
{
    if (dy)
    {
        m_sourceLineWidget->scroll(0, dy);
    }
    else
    {
        m_sourceLineWidget->update(0, rect.y(), m_sourceLineWidget->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        UpdateLineWidgetWidth(0);
    }
}

void CCodeEditor::resizeEvent(QResizeEvent *_event)
{
    QPlainTextEdit::resizeEvent(_event);

    QRect rect = contentsRect();
    m_sourceLineWidget->setGeometry(QRect(rect.left(), rect.top(), OnSizeChanged(), rect.height()));
}

void CCodeEditor::HighlightLine(void)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor color = QColor(Qt::darkGreen);
        selection.format.setBackground(color);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CCodeEditor::OnPaint(QPaintEvent *_event)
{
    QPainter painter(m_sourceLineWidget);
    painter.fillRect(_event->rect(), Qt::darkGray);
    
    QFont font("Menlo");
    font.setBold(true);
    font.setPointSize(11);
    painter.setFont(font);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= _event->rect().bottom())
    {
        if (block.isVisible() && bottom >= _event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::white);
            painter.drawText(0, top, m_sourceLineWidget->width(), fontMetrics().height(), Qt::AlignLeft, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

