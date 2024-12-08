#ifndef NODE_H
#define NODE_H
#include <QPoint>

class Node
{
public:
    Node();

private:
    QPointF m_pos;
    int m_value{};

public:
    Node(QPoint pozition,int value);
    Node(const Node& nod) = default;
    Node& operator=(const Node& node);

    QPointF GetPos() const {return m_pos;}
    int GetValue() const {return m_value;}

    void SetPos(QPointF point) {m_pos = point;}
    void SetValue(int value) {m_value = value;}

    bool operator==(const Node& other) const;
};

#endif
