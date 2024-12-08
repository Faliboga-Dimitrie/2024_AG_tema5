#include "edge.h"


Edge::Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node, int cost):
    m_first{first_node}, m_second{second_node}, m_cost{cost}
{
}

Edge::Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node):
    m_first{first_node}, m_second{second_node}
{
    m_cost = -1;
}
