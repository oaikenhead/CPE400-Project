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
void sendRREQ(std::map<char, Node*> mapNode, char src, int request_ID, char destination, char network[]);

int main() {
  std::cout << "CPE 400 Project" << std::endl;
  char network[NODE_NUM] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

	// Instantiate nodes A - J
	std::map<char, Node*>	nothing;
	std::map<char, Node*>	mapNode;
	Node A('A', nothing);
	Node B('B', nothing);
	Node C('C', nothing);
	Node D('D', nothing);
	Node E('E', nothing);
	Node F('F', nothing);
	Node G('G', nothing);
	Node H('H', nothing);
	Node I('I', nothing);
	Node J('J', nothing);

	//define map
	mapNode.insert(std::pair<char, Node*>('A', A.memLocation()));
	mapNode.insert(std::pair<char, Node*>('B', B.memLocation()));
	mapNode.insert(std::pair<char, Node*>('C', C.memLocation()));
	mapNode.insert(std::pair<char, Node*>('D', D.memLocation()));
	mapNode.insert(std::pair<char, Node*>('E', E.memLocation()));
	mapNode.insert(std::pair<char, Node*>('F', F.memLocation()));
	mapNode.insert(std::pair<char, Node*>('G', G.memLocation()));
	mapNode.insert(std::pair<char, Node*>('H', H.memLocation()));
	mapNode.insert(std::pair<char, Node*>('I', I.memLocation()));
	mapNode.insert(std::pair<char, Node*>('J', J.memLocation()));

	//create the edges of the map
	addEdge('A', 'B', mapNode);
	addEdge('B', 'C', mapNode);
	addEdge('C', 'D', mapNode);
	addEdge('D', 'E', mapNode);
	addEdge('D', 'F', mapNode);
	addEdge('E', 'F', mapNode);
	addEdge('E', 'H', mapNode);
	addEdge('F', 'G', mapNode);
	addEdge('G', 'H', mapNode);
  addEdge('G', 'I', mapNode);
	addEdge('H', 'I', mapNode);
	addEdge('H', 'J', mapNode);


	// run the simulation
	srand(time(NULL));
	
	//fixed test number
	std::cout << std::endl << "**************************************" << std::endl;
	std::cout << "Fixed network simulation:" << std::endl;
	std::cout << std::endl << "Route Discovery from A to J" << std::endl;
	
	sendRREQ(mapNode, 'A', rand(), 'J', network);
	
	std::cout << std::endl << "-->Destroyed edge between B and C." << std::endl; 

	
	std::cout << std::endl << "-->Restore edge between B and C, to get get original network back." << std::endl; 
	addEdge('B', 'C', mapNode);
	
	//highly volatile example
	std::cout << std::endl << "**************************************" << std::endl;
	std::cout << "Highly volatile network simulation test :" << std::endl;
	
	for(int i=0; i<5; i++)
	{
		std::cout << std::endl << "--------------------------------" << std::endl;
		std::cout << std::endl << "TEST #" << i+1 << std::endl;

	// Randomly pick a connection between 2 nodes
		NodeChoice generated_nodes = randSelectNode(network);
		std::cout << "\nConnection between nodes " << generated_nodes.nodeA << " and " << generated_nodes.nodeB << ".\n" << std::endl;
		
		sendRREQ(mapNode, generated_nodes.nodeA, rand(), generated_nodes.nodeB, network);
		
	// Call the simulated connection
		threadLinks(mapNode, network);
		
		std::cout << std::endl;
	}
	
	//redo fixed network, to see if path between A and J changed
	std::cout << std::endl << "**************************************" << std::endl;
	std::cout << "Lastly, lets see if path between A to J changed after all of those edges being destroyed/created:" << std::endl;
	std::cout << std::endl << "Route Discovery: A to J" << std::endl;
	std::cout<< std::endl <<"*Note: Every test run will have different outcomes*"<<std::endl<<std::endl;
	

	sendRREQ(mapNode, 'A', rand(), 'J', network);
	
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

void sendRREQ(std::map<char, Node*> mapNode, char src, int request_ID, char destination, char network[])
{
    time_t start = time(0);
    Node * this_node = ptrForNode(src, mapNode);
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
                            neighbor_node->sendRREP(destination, src, (neighbor_node->RREQ_str).size(),neighbor_node->RREQ_str, destination);
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
            this_node = ptrForNode(neighbor_vector[0], mapNode);
            neighbor_vector.erase(neighbor_vector.begin());
        }

        // The destination is not in the network if there is no response after 2 seconds
        if(difftime(time(0), start) > 2.0)
        {
            Node * validate_node = ptrForNode(src, mapNode);
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
        node_reset = ptrForNode(network[i], mapNode);
        node_reset->pathRec = false;
        node_reset->RREP_str = "";
        node_reset->RREQ_str = "";
    }
    Node * source_node_reset = ptrForNode(src, mapNode);
    source_node_reset->responseRec = false;
    source_node_reset->RREP_str = "";
    source_node_reset->RREQ_str = "";
}