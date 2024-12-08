#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QInputDialog>
#include "graf.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *m) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *ev) override;

private slots:

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    Graf m_graf;
    std::shared_ptr<Node> m_selectedNodeFrom{nullptr};
    std::shared_ptr<Node> m_selectedNodeTo{nullptr};
    bool m_isFromTurn{true};
    bool notOverlapingNode(QMouseEvent *m);
    void reconstructPath(std::shared_ptr<Node> to, std::vector<int> p);
};
#endif
