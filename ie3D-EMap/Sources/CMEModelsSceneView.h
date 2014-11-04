//
//  CMEModelsSceneView.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 11/4/14.
//
//

#ifndef CMEModelsSceneView_h
#define CMEModelsSceneView_h

#include <QObject>
#include <QWidget>

class CMEModelsSceneView : public QWidget
{
    Q_OBJECT
    
public:
    CMEModelsSceneView(QWidget *parent = 0);
    
protected:
    
private slots:

private:

    void paintEvent(QPaintEvent *event);
    
};

#endif
