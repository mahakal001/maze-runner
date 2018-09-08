#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <deque>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

#include "node_structs.h"
#include "tinyxml2.h"
#include "tileMap.h"

std::queue<int> frame_graph_search_solution(m_graphNode* rootNode,m_graphNode* goalNode, TileMap* tileMap)
{
  
    std::vector<int> rev_path;
    std::queue<int> path;

    m_graphNode* curNode = goalNode;

    tileMap->resetVisiblePath();
    
    while( curNode != rootNode )
    {     
    // tileMap->setSpriteColor(curNode->nodeIndex,sf::Color::Cyan);
    sf::RectangleShape *rect = new sf::RectangleShape();
    rect->setSize(sf::Vector2f(32,32));
    rect->setPosition(curNode->x,curNode->y);
    rect->setFillColor(sf::Color(0,255,0,150));
    tileMap->pushVisiblePath(rect);

        switch(curNode->action)
        {
            case left:
                rev_path.push_back(left);
	        	std::cout << " left " ;
                curNode = curNode->right;
                break;
            case right:
                rev_path.push_back(right);
		        std::cout << " right " ;
                curNode = curNode->left;
                break;
            case above:
                rev_path.push_back(above);
		        std::cout << " up " ;
                curNode = curNode->below;
                break;
            case below:
                rev_path.push_back(below);
        		std::cout << " down " ;
                curNode = curNode->above;
                break;
            default:
                std::cout << "Someting Fatal happen\n";
                exit(EXIT_FAILURE);
                break;
        }
    }


    std::cout << std::endl;
for(int i = rev_path.size()-1 ; i >= 0 ; i-- ){
    std::cout << "\n" << i << "\t\t" << rev_path.size()<<"\n";
        path.push(rev_path[i]);
    }

    return path;
}


std::queue<int> graph_search(m_graphNode* startNode,m_graphNode* goalNode,
                             TileMap* tileMap) {
  std::queue<int> solution;
  std::deque<m_graphNode *> frontier;
  std::unordered_map<m_graphNode *, bool> explored_set;



  frontier.push_front(startNode);

  auto is_nodeInFrontier =
      [=](m_graphNode *node) {
        auto size = frontier.size();
        for (auto i = 0; i < size; i++)
          if (node == frontier[i])
            return true;
        return false;
      };

  while (true) {
    if (frontier.empty()) {
      return solution;
    }

    m_graphNode *curNode = frontier.front();
    frontier.pop_front();

    if (curNode == goalNode)
      return frame_graph_search_solution(startNode,curNode,tileMap) ;

    explored_set[curNode] = true;

/* Expand  curNode */
    if (curNode->left && !explored_set[curNode->left] &&
        !is_nodeInFrontier(curNode->left) && (curNode->left->typeOfNode != WallId)){
      frontier.push_back(curNode->left);
      curNode->left->action = left;
        }

   if (curNode->right && !explored_set[curNode->right ] &&
        !is_nodeInFrontier(curNode->right) && (curNode->right->typeOfNode != WallId)){
      frontier.push_back(curNode->right );
      curNode->right->action = right;
        }

         if (curNode->above && !explored_set[curNode->above] &&
        !is_nodeInFrontier(curNode->above)&& (curNode->above->typeOfNode != WallId)){
      frontier.push_back(curNode->above);
      curNode -> above -> action = above;
        }

         if (curNode->below && !explored_set[curNode->below] &&
        !is_nodeInFrontier(curNode->below) && (curNode->below->typeOfNode != WallId)){
      frontier.push_back(curNode->below);
      curNode -> below -> action = below ;
        }

  }

  return solution;
}

