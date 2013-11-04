#ifndef INDIEENGINESHADER_H
#define INDIEENGINESHADER_H

#include <QMainWindow>

namespace Ui {
class indieEngineShader;
}

class indieEngineShader : public QMainWindow
{
    Q_OBJECT
    
protected:
    void closeEvent(QCloseEvent* _event);
    
public:
    explicit indieEngineShader(QWidget *parent = 0);
    ~indieEngineShader(void);
    void Execute(void);
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::indieEngineShader *ui;
};

#endif // INDIEENGINESHADER_H
