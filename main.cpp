#include "Node.h"

struct NodeChoice {
  char nodeA;
  char nodeB;
};

NodeChoice randSelectNode(char network[]);

int main() {
  std::cout << "CPE 400 Project" << std::endl;
  
  return 0;
}

NodeChoice randSelectNode(char network[]) {
  NodeChoice selectedNode;
  selectedNode.nodeA = network[rand() % NODE_NUM];
  selectedNode.nodeB = network[rand() % NODE_NUM];

  // while the nodes are the same
  while (selectedNode.nodeA == selectedNode.nodeB) {
    selectedNode.nodeA = network[rand() % NODE_NUM];
    selectedNode.nodeB = network[rand() % NODE_NUM];
  }

  return selectedNode;
}
