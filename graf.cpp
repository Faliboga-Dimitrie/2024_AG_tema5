#include "graf.h"

void Graf::buildAdiacentlist()
{
    if(!m_adiacentList.empty())
        m_adiacentList.clear();
    m_adiacentList.resize(m_nodes.size());
    int firstNode,secondNode;
    for (const auto& edge : m_edges)
    {
        firstNode = edge.GetFirstNode()->GetValue();
        secondNode = edge.GetSecondNode()->GetValue();
        m_adiacentList[firstNode].push_back({secondNode,edge.GetCost()});
    }
}

void Graf::algDijkstra(std::shared_ptr<Node> from,std::shared_ptr<Node> to)
{
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> costQueue;
    std::vector<int> costMemory(m_nodes.size(), INT_MAX);
    std::vector<bool> unvisitedNodes(m_nodes.size(), true);

    p.clear();
    p.resize(m_nodes.size(),INT_MAX);
    p[from->GetValue()] = -1;

    costQueue.push({0, from->GetValue()});
    costMemory[from->GetValue()] = 0;

    while (!costQueue.empty())
    {
        int y_index = costQueue.top().second;
        costQueue.pop();

        if(unvisitedNodes[y_index] == false)
            continue;
        if(to->GetValue() == m_nodes[y_index]->GetValue())
            break;

        unvisitedNodes[y_index] = false;

        for (int i = 0; i < m_adiacentList[y_index].size(); i++)
        {
            int y_prim_index = m_adiacentList[y_index][i].first;
            if (unvisitedNodes[y_prim_index])
            {
                std::shared_ptr<Node> y_prim = m_nodes[y_prim_index];
                int edgeCost = m_adiacentList[y_index][i].second;

                if (costMemory[y_prim_index] > costMemory[y_index] + edgeCost)
                {
                    costMemory[y_prim_index] = costMemory[y_index] + edgeCost;
                    costQueue.push({costMemory[y_index] + edgeCost, y_prim_index});
                    p[y_prim_index] = y_index;
                }
            }
        }
    }

    int currentNode = to->GetValue();
    while (currentNode != -1 && currentNode!=INT_MAX) {
        m_path[currentNode] = m_nodes[currentNode];
        currentNode = p[currentNode];
    }
}

void Graf::updateAdiacentList(bool isNode, int cost)
{
    if(isNode)
    {
        m_adiacentList.push_back({});
    }
    else if (!m_edges.empty())
    {
        int firstNode = m_edges.back().GetFirstNode()->GetValue();
        int secondNode = m_edges.back().GetSecondNode()->GetValue();

        m_adiacentList[firstNode].push_back({secondNode,cost});
    }
}
void Graf::addEdge(std::shared_ptr<Node> first, std::shared_ptr<Node> second, int cost)
{
    Edge e(first,second,cost);
    m_edges.push_back(e);

    updateAdiacentList(false,cost);
}

void Graf::readLuxemburgMap()
{
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile("C:/Users/Dimitrie_U/1Facultate/Anul2/Semestrul1/AG/Laboratoare/Tema5/resources/Harta_Luxemburg.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Nu s-a putut incarca fisierul XML!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("map");
    if (!root) {
        std::cerr << "Structura fișierului XML nu este validă!" << std::endl;
        return;
    }

    m_nodes.clear();
    m_edges.clear();

    tinyxml2::XMLElement* nodesElement = root->FirstChildElement("nodes");
    if (nodesElement)
    {
        for (tinyxml2::XMLElement* node = nodesElement->FirstChildElement("node"); node != nullptr; node = node->NextSiblingElement("node"))
        {
            std::shared_ptr<Node> n = std::make_shared<Node>();

            int value;
            if (node->QueryIntAttribute("id", &value) == tinyxml2::XML_SUCCESS)
            {
                n->SetValue(value);
            } else
            {
                std::cerr << "Eroare la citirea ID-ului nodului! ID-ul: " << node->Attribute("id") << std::endl;
                continue;
            }

            QPointF coordonates;
            if (node->QueryDoubleAttribute("latitude", &coordonates.rx()) == tinyxml2::XML_SUCCESS &&
                node->QueryDoubleAttribute("longitude", &coordonates.ry()) == tinyxml2::XML_SUCCESS)
            {
                n->SetPos(coordonates);
            } else
            {
                std::cerr << "Eroare la citirea coordonatelor pentru nodul " << value << std::endl;
                continue;
            }

            m_nodes[value] = n;
            if(m_adiacentList.empty())
                buildAdiacentlist();
            else
            {
                updateAdiacentList(true,0);
            }
        }
    }

    tinyxml2::XMLElement* arcsElement = root->FirstChildElement("arcs");
    if (arcsElement)
    {
        for (tinyxml2::XMLElement* arc = arcsElement->FirstChildElement("arc"); arc != nullptr; arc = arc->NextSiblingElement("arc"))
        {
            int from, to, length;

            arc->QueryIntAttribute("from", &from);
            arc->QueryIntAttribute("to", &to);
            arc->QueryIntAttribute("length", &length);

            auto fromNode = m_nodes.find(from);
            auto toNode = m_nodes.find(to);

            if (fromNode != m_nodes.end() && toNode != m_nodes.end())
            {
                addEdge(fromNode->second,toNode->second,length);
            } else
            {
                std::cerr << "Noduri nevalide pentru arc: " << from << " -> " << to << std::endl;
            }
        }
    }
}

void Graf::scaleMap(int windowWidth, int windowHeight)
{
    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::min();
    double minLong = std::numeric_limits<double>::max();
    double maxLong = std::numeric_limits<double>::min();

    for (const auto& node : m_nodes) {
        const QPointF& pos = node.second->GetPos();
        double lat = pos.y();
        double lon = pos.x();
        minLat = std::min(minLat, lat);
        maxLat = std::max(maxLat, lat);
        minLong = std::min(minLong, lon);
        maxLong = std::max(maxLong, lon);
    }

    double scaleFactorX = windowWidth / (maxLong - minLong);
    double scaleFactorY = windowHeight / (maxLat - minLat);

    m_scaleFactorX = scaleFactorX;
    m_scaleFactorY = scaleFactorY;
    m_minLat = minLat;
    m_maxLat = maxLat;
    m_minLong = minLong;
    m_maxLong = maxLong;
}

void Graf::printInFileAdiacentList()
{
    int i=0;
    std::ofstream fout("C:/Users/Dimitrie_U/1Facultate/Anul2/Semestrul1/AG/Laboratoare/Tema5/resources/AdiacentList.txt");
    if (!fout)
    {
        std::cerr << "Eroare la deschiderea fișierului " << "AdiacentList.txt" << std::endl;
        return;
    }
    std::cout<<m_adiacentList.size();
    for (const auto& row : m_adiacentList)
    {
        fout<<i<<": ";
        for (std::pair<int,int> value : row)
        {
            fout << value.first <<" ";
        }
        fout << std::endl;
        ++i;
    }
    fout.close();
}

