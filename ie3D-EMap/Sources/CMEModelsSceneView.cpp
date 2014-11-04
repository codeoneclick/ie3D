//
//  CMEModelsSceneView.cpp
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/4/14.
//
//
#include "CMEModelsSceneView.h"
#include "QPainter.h"
#include "HCommon.h"

CMEModelsSceneView::CMEModelsSceneView(QWidget *parent) : QWidget(parent)
{
    
}

void CMEModelsSceneView::setImage(const QImage& image)
{
    m_image = image;
}

void CMEModelsSceneView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(rect(), m_image, m_image.rect());
}