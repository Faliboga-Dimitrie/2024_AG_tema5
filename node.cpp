#include "node.h"

Node::Node() {}

Node &Node::operator=(const Node &node)
{
    if(this == &node)
        return *this;
    this->m_pos = node.m_pos;
    this->m_value = node.m_value;
    return *this;
}

Node::Node(QPoint pozition, int value):
    m_pos{pozition},m_value{value}
{
}

bool Node::operator==(const Node &other) const
{
    return m_pos == other.m_pos && m_value == other.m_value;
}
