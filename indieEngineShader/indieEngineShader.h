#ifndef INDIEENGINESHADER_H
#define INDIEENGINESHADER_H

#include <QMainWindow>

namespace Ui {
class indieEngineShader;
}

class indieEngineShader : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit indieEngineShader(QWidget *parent = 0);
    ~indieEngineShader();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::indieEngineShader *ui;
};

#endif // INDIEENGINESHADER_H
