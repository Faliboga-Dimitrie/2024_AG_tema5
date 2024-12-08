#ifndef EDGE_H
#define EDGE_H
#include <memory>
#include "node.h"
class Edge
{
public:
    Edge() = default;
private:
    std::shared_ptr<Node> m_first{nullptr};
    std::shared_ptr<Node> m_second{nullptr};
    int m_cost;
public:

    Edge(std::shared_ptr<Node> first_node,std::shared_ptr<Node> second_node);
    Edge(std::shared_ptr<Node> first_node,std::shared_ptr<Node> second_node, int cost);
    Edge(const Edge& edge) = default;
    Edge& operator=(const Edge& edge) = default;

    std::shared_ptr<Node> GetFirstNode() const {return m_first;}
    std::shared_ptr<Node> GetSecondNode() const {return m_second;}
    const int GetCost() const {return m_cost;}
};

#endif
