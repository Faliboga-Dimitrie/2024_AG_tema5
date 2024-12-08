#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_graf.readLuxemburgMap();
    m_graf.scaleMap(this->width(),this->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(m_isFromTurn)
    {
        m_selectedNodeFrom = nullptr;
    }

    QPointF clickPos = event->pos();

    std::unordered_map<int, std::shared_ptr<Node>> nodes = m_graf.getNode();

    std::shared_ptr<Node> nearestNode{nullptr};
    double minDistance = std::numeric_limits<double>::max();

    for (const auto& nodePair : nodes)
    {
        const std::shared_ptr<Node>& node = nodePair.second;
        QPointF nodePos = node->GetPos();

        double scaledNodeX = (nodePos.x() - m_graf.m_minLong) * m_graf.m_scaleFactorX;
        double scaledNodeY = (nodePos.y() - m_graf.m_minLat) * m_graf.m_scaleFactorY;

        double distance = (clickPos.x() - scaledNodeX) * (clickPos.x() - scaledNodeX) +
                          (clickPos.y() - scaledNodeY) * (clickPos.y() - scaledNodeY);

        if (distance < minDistance)
        {
            minDistance = distance;
            nearestNode = node;
        }
    }

    if (nearestNode)
    {
        if (m_selectedNodeFrom != nullptr)
        {
            m_isFromTurn = true;
            m_selectedNodeTo = nearestNode;
            std::cout << "To are ID-ul: " << m_selectedNodeTo->GetValue() << std::endl;
        }
        else
        {
            m_isFromTurn = false;
            m_selectedNodeFrom = nearestNode;
            std::cout << "From are ID-ul: " << m_selectedNodeFrom->GetValue() << std::endl;
        }
    }
    update();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int width = newSize.width();
    int height = newSize.height();

    qDebug() << "Noua dimensiune a ferestrei: " << width << "x" << height;
    m_graf.scaleMap(width,height);

    QMainWindow::resizeEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    std::vector<Edge> edges = m_graf.getEdge();

    for (const auto& edge : edges) {
        auto firstNode = edge.GetFirstNode();
        auto secondNode = edge.GetSecondNode();

        QPen pen;

        QPointF p1 = firstNode->GetPos();
        QPointF p2 = secondNode->GetPos();

        p1.setX((p1.x() - m_graf.m_minLong) * m_graf.m_scaleFactorX);
        p1.setY((p1.y() - m_graf.m_minLat) * m_graf.m_scaleFactorY);
        p2.setX((p2.x() - m_graf.m_minLong) * m_graf.m_scaleFactorX);
        p2.setY((p2.y() - m_graf.m_minLat) * m_graf.m_scaleFactorY);

        if(m_graf.getPath().find(firstNode->GetValue())!= m_graf.getPath().end() &&
            m_graf.getPath().find(secondNode->GetValue())!= m_graf.getPath().end())
        {
            pen.setColor(Qt::green);
            pen.setWidth(5);
            painter.setPen(pen);
            painter.drawLine(p1, p2);
        }
        painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
        painter.drawLine(p1, p2);
    }

    if(m_selectedNodeFrom != nullptr)
    {
        QPointF nodePosFrom = m_selectedNodeFrom->GetPos();

        double scaledFromX = (nodePosFrom.x() - m_graf.m_minLong) * m_graf.m_scaleFactorX;
        double scaledFromY = (nodePosFrom.y() - m_graf.m_minLat) * m_graf.m_scaleFactorY;

        painter.setBrush(QBrush(Qt::red));
        painter.setPen(QPen(Qt::black));

        painter.drawEllipse(QPointF(scaledFromX, scaledFromY), 5, 5);
    }

    if(m_selectedNodeTo != nullptr)
    {
        QPointF nodePosTo = m_selectedNodeTo->GetPos();

        double scaledToX = (nodePosTo.x() - m_graf.m_minLong) * m_graf.m_scaleFactorX;
        double scaledToY = (nodePosTo.y() - m_graf.m_minLat) * m_graf.m_scaleFactorY;

        painter.setBrush(QBrush(Qt::red));
        painter.setPen(QPen(Qt::black));

        painter.drawEllipse(QPointF(scaledToX, scaledToY), 5, 5);
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        if(m_selectedNodeFrom)
        {
            if(m_selectedNodeTo)
            {
                m_graf.algDijkstra(m_selectedNodeFrom,m_selectedNodeTo);
            }
            else
                std::cout<<"You haven't selected a destination\n";
        }
        else
            std::cout<<"You haven't selected a starting point\n";
    }
    else
    {
        m_graf.clearPath();
    }
    update();
}
