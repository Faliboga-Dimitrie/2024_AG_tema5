#ifndef GRAF_H
#define GRAF_H

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <map>
#include <unordered_set>
#include <stack>
#include <unordered_map>
#include <utility>
#include <string>
#include "edge.h"
#include "tinyxml2.h"

class Graf
{
public:
    Graf() = default;

    double m_scaleFactorX;
    double m_scaleFactorY;
    double m_minLat;
    double m_maxLat;
    double m_minLong;
    double m_maxLong;

private:
    std::unordered_map<int, std::shared_ptr<Node>> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<std::pair<int,int>>> m_adiacentList;
    std::unordered_map<int, std::shared_ptr<Node>> m_path;
    std::vector<int> p;

    void updateAdiacentList(bool isNode, int cost);

public:
    Graf(const Graf& graf) = default;
    Graf& operator=(const Graf& graf) = default;

    std::unordered_map<int, std::shared_ptr<Node>>& getNode() {return m_nodes;}
    std::vector<Edge>& getEdge() {return m_edges;}
    std::unordered_map<int, std::shared_ptr<Node>>& getPath() {return m_path;}

    void addEdge(std::shared_ptr<Node> first, std::shared_ptr<Node> second, int cost);
    void buildAdiacentlist();

    void algDijkstra(std::shared_ptr<Node> from,std::shared_ptr<Node> to);
    void clearPath(){m_path.clear();}

    void readLuxemburgMap();
    void scaleMap(int windowWidth, int windowHeight);
    void printInFileAdiacentList();

    struct CompareEdges
    {
        bool operator()(const std::pair<int, Edge>& a, const std::pair<int, Edge>& b) const
        {
            return a.first > b.first;
        }
    };

    struct Hasher
    {
        std::size_t operator()(const std::shared_ptr<Node>& nodePtr) const
        {
            std::size_t h1 = std::hash<double>()(nodePtr->GetPos().x()) ^ std::hash<double>()(nodePtr->GetPos().y());
            std::size_t h2 = std::hash<int>()(nodePtr->GetValue());
            return h1 ^ (h2 << 1);
        }
    };

    struct Equal
    {
        bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const
        {
            return *lhs == *rhs;
        }
    };
};

#endif
