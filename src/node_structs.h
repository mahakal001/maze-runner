/*
 * The following structure will be used in the graph search alogrithm
 * */
 #include "tinyxml2.h"
#include "tileMap.h"
#include <queue>

struct m_graphNode {
    unsigned int x;
    unsigned int y;
    unsigned int typeOfNode;
    unsigned int nodeIndex;
    unsigned int action;
    char ch ;
    m_graphNode* left;
    m_graphNode* right;
    m_graphNode* above;
    m_graphNode* below;
};


enum node_Expand_Direction{left,right,above,below};

std::queue<int> frame_graph_search_solution(m_graphNode* rootNode,m_graphNode* goalNode, TileMap* tileMap);
std::queue<int> graph_search(m_graphNode* startNode,m_graphNode* goalNode,
                             TileMap* tileMap);
                             
