#include "Node.h"

struct NodeChoice {
  char nodeA;
  char nodeB;
};

Node* ptrForNode(char node, std::map<char, Node*> mapNode);
void addEdge(char first, char second, std::map<char, Node*> mapNode);
void destroyEdge(char first, char second, std::map<char, Node*> mapNode);
bool checkConnected(char first, char second, std::map<char, Node*> mapNode);
NodeChoice randSelectNode(char network[]);
void threadLinks(std::map<char, Node*> mapNode, char network[]);

int main() {
  std::cout << "CPE 400 Project" << std::endl;
  
  return 0;
}

// pointer for node
Node* ptrForNode(char node, std::map<char, Node*> mapNode) {
  std::map<char, Node*>::iterator nodeIterator = mapNode.find(node);
  return (nodeIterator->second);
}

// add an edge between two nodes by adding pointers in connection list
void addEdge(char first, char second, std::map<char, Node*> mapNode) {
  Node* firstNodePtr = ptrForNode(first, mapNode);
  Node* secondNodePtr = ptrForNode(second, mapNode);

  firstNodePtr -> insConnect(second, secondNodePtr);
  secondNodePtr -> insConnect(first, firstNodePtr);
}

// destroy node edge by removing it
void destroyEdge(char first, char second, std::map<char, Node*> mapNode) {
  ptrForNode(first, mapNode)->removeConnect(second);
  ptrForNode(second, mapNode)->removeConnect(first);
}

// checks if two nodes are connected to each other
bool checkConnected(char first, char second, std::map<char, Node*> mapNode) {
  return ptrForNode(first, mapNode)->connected(second);
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

// simulating a thread, randomly creating edges
void threadLinks(std::map<char, Node*> mapNode, char network[]) {
  for (int i=0; i<(rand()%10); i++) {
    NodeChoice genNodes = randSelectNode(network);

    while (ptrForNode(genNodes.nodeA, mapNode)->connected(genNodes.nodeB)) {
      genNodes = randSelectNode(network);
    }
    std::cout << "Randomly destroyed edge between " << genNodes.nodeA << ", " << genNodes.nodeB << std::endl;
    destroyEdge(genNodes.nodeA, genNodes.nodeB, mapNode);
  }

  // randomly creating edges
  for (int i=0; i<(rand() % 10); i++) {
    NodeChoice genNodes = randSelectNode(network);
    std::cout << "" << genNodes.nodeA << ", " << genNodes.nodeB << ")" << std::endl;
    addEdge(genNodes.nodeA, genNodes.nodeB, mapNode);
  }
}

void sendRREQ(std::map<char, Node*> map_node, char src, int request_ID, char destination, char network[])
{
    time_t start = time(0);
    Node * this_node = ptrForNode(src, map_node);
    this_node->pathRec = true;
    
    std::vector<char> neighbor_vector;
    while(1)
    {
        bool check = true;
        for(int i = 0; i < this_node->prevReq.size(); i++)
        {
            if(    this_node->prevReq[i].origin_RREQ == src &&
                this_node->prevReq[i].ID_REQ_RREQ == request_ID)
            {
                check = false;
            }
        }
        
        if(check)
        {
            RequestCheck insert_record = {src, request_ID};
            this_node-> prevReq.push_back(insert_record);

            //if current node is desired node, no need to ask neighbors
            if(this_node->nodeName != destination)
            {
                        //for each of current node (this_node) neighbors, ask RREQ
                        for(std::map<char, Node*>::const_iterator node_iterator = this_node->nodeConnects.begin(); node_iterator != this_node->nodeConnects.end(); node_iterator++)

                {
                    Node * neighbor_node = node_iterator->second;
                    if(neighbor_node->pathRec == false)
                    {
                        //send neighbor_node current path taken from original
                        neighbor_node->RREQ_str = (this_node->RREQ_str + (this_node->nodeName));
                        neighbor_node->pathRec = true;
                        if(neighbor_node->nodeName == destination)
                        { 
                            //found destination here
                            std::cout     << "Node " << neighbor_node->nodeName << " received a RREQ from Node " << this_node->nodeName
                                    << " to get to this node, we begin RREP ["
                                    << (neighbor_node->RREQ_str + neighbor_node->nodeName) << "]" << std::endl;

                            //now, begin back to RREQ original by starting RREP
                            neighbor_node->sendRREP(destination, src, neighbor_node->RREQ_str, (neighbor_node->RREQ_str).size(), destination);
                        }
                        else
                        {
                            //did not find, so neighbor_node will now ask its neighbors
                            std::cout    << "Node " << neighbor_node->nodeName << " received a RREQ from Node " << this_node->nodeName
                                    << " to get to Node " << destination << ", list of letters: " << neighbor_node->RREQ_str << std::endl;
                        }
                    }
                    //add neighbor_node to queue, to forward RREQ
                    neighbor_vector.push_back(node_iterator->first);
                }
            }
            else {}
        }

        // Go to the next node in the queue and update this_node        
        if(neighbor_vector.size() > 0)
        {
            this_node = ptrForNode(neighbor_vector[0], map_node);
            neighbor_vector.erase(neighbor_vector.begin());
        }

        // The destination is not in the network if there is no response after 2 seconds
        if(difftime(time(0), start) > 2.0)
        {
            Node * validate_node = ptrForNode(src, map_node);
            if(!validate_node->responseRec)
            {
            std::cout << "ERROR: Unable to find a path to the node: " << destination << std::endl;
            }
            break;
        }
    }

    // Reset the node
    Node * node_reset = NULL;

    for(int i=0; i<NODE_NUM; i++)
    {    
        node_reset = ptrForNode(network[i], map_node);
        node_reset->pathRec = false;
        node_reset->RREP_str = "";
        node_reset->RREQ_str = "";
    }
    Node * source_node_reset = ptrForNode(src, map_node);
    source_node_reset->responseRec = false;
    source_node_reset->RREP_str = "";
    source_node_reset->RREQ_str = "";
}