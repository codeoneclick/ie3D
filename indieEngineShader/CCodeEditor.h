/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CCODEEDITOR_H
#define CCODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class ILineWidget
{
private:
    
protected:
    
public:
    
    ILineWidget(void) = default;
    ~ILineWidget(void) = default;

    virtual void OnPaint(QPaintEvent* _event) = 0;
    virtual int OnSizeChanged(void) = 0;
};

class CLineWidget;

class CCodeEditor : public QPlainTextEdit, public ILineWidget
{
    Q_OBJECT

public:
    CCodeEditor(QWidget *parent = 0);

    void OnPaint(QPaintEvent* _event);
    int OnSizeChanged(void);

protected:
    void resizeEvent(QResizeEvent* _event);

private slots:
    
    void UpdateLineWidgetWidth(int _count);
    void HighlightLine(void);
    void UpdateLineWidget(const QRect &, int);

private:
    QWidget* m_sourceLineWidget;
};

class CLineWidget : public QWidget
{
public:
    CLineWidget(CCodeEditor* _editor) : QWidget(_editor),
    m_editor(_editor)
    {

    };

    QSize sizeHint(void) const
    {
        return QSize(m_editor->OnSizeChanged(), 0);
    };

protected:
    void paintEvent(QPaintEvent *_event)
    {
        m_editor->OnPaint(_event);
    };

private:
    CCodeEditor* m_editor;
};

#endif