//
//  CMEModelsSceneView.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/4/14.
//
//
#include "CMEModelsSceneView.h"
#include "QPainter.h"

CMEModelsSceneView::CMEModelsSceneView(QWidget *parent) : QWidget(parent)
{
    
}

void CMEModelsSceneView::paintEvent(QPaintEvent *event)
{
    QImage sign(50, 50, QImage::Format_ARGB32_Premultiplied);
    QPainter painter_01(&sign);
    painter_01.setRenderHint(QPainter::Antialiasing, true);
    painter_01.fillRect(sign.rect(), QColor(255, 255, 255, 0));
    painter_01.setBrush(Qt::blue);
    painter_01.setPen(Qt::NoPen);
    painter_01.drawEllipse(0, 0, sign.width(), sign.height());
    painter_01.end();
    
    QPainter painter(this);
    painter.drawImage(rect(), sign, sign.rect());
}