/*
 * INCLUDES
 */

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <map>

#define NODE_NUM 10

/*
 * STRUCT: RequestCheck
 * holds origin RREQ and ID Request RREQ
 */

struct RequestCheck {
  char origin_RREQ;
  int ID_REQ_RREQ;
};

/*
 * CLASS: Node
 * contains definitions for Node object traits
 */

class Node {
  public:
    Node(char nodeLetter, std::map<char, Node*> connects);
    Node& operator=(const Node& other);
    Node* memLocation();
    void connectAssign(std::map<char, Node*> connects);
    void insConnect(char nodeDes, Node* loc);
    void removeConnect(char nodeDes);
    void sendRREP(char src, char dest, int size, std::string RREQ_reply, char RREP_SRC);
    bool connected(char nodeName);

    std::map<char, Node*> nodeConnects;
    std::vector<RequestCheck> prevReq;
    std::string RREQ_str;
    std::string RREP_str;
    char nodeName;
    bool pathRec;
    bool responseRec;
};

/*
 * FUNCTION: Parameterized Ctor
 * sets a parameterized instance of the Node object
 * RETURNS: The current class instance
 */
Node::Node(char nodeLetter, std::map<char, Node*> connects) {
  // setting node designation to letter
  nodeName = nodeLetter;

  // creating Node connections
  connectAssign(connects);

  // setting path and response recieved to false
  pathRec = false;
  responseRec = false;
}

/*
 * FUNCTION: Assignment Operator
 * assigns lhs to rhs then returns
 * RETURNS: The value of the operator specified by left operand
 */
Node& Node::operator=(const Node& rhs) {
  if (this == &rhs) {
    return *this;
  }
  return *this;
}

/*
 * FUNCTION: memLocation
 * sends the memory location of the Node
 * RETURNS: Node pointer to memory location
 */
Node* Node::memLocation() {
  return this;
}

/*
 * FUNCTION: connectAssign
 * connects map of nodes to nodes being passed in
 * RETURNS: nothing (void)
 */
void Node::connectAssign(std::map<char, Node*> connects) {
  nodeConnects = connects;
}

/*
 * FUNCTION: insConnect
 * inserting newly creat3ed node connections into the Node map
 * RETURNS: nothing (void)
 */
void Node::insConnect(char nodeDes, Node* loc) {
  nodeConnects.insert(std::pair<char, Node*>(nodeDes, loc));
}

/*
 * FUNCTION: removeConnect
 * removing node connections
 * RETURNS: nothing (void)
 */
void Node::removeConnect(char nodeDes) {
  nodeConnects.erase(nodeDes);
}

/*
 * FUNCTION: sendRREP
 * simulates sending an RREQ message
 * RETURNS: nothing (void)
 */
void Node::sendRREP(char src, char dest, int size, std::string RREQ_reply, char RREP_SRC) {
  if (size==0) {
    std::cout << "Node " << nodeName
              << " successfully received path to " << RREP_SRC
              << " route [" << RREQ_reply << RREP_SRC << "]"
              << std::endl;
  } else if (size==(RREQ_reply.size()-1)) {
    std::cout << "Node " << nodeName
              << " received RREP from " << RREP_SRC
              << " to " << dest
              << " with route [" << RREQ_reply << RREP_SRC << "]"
              << std::endl;
  } else if (size!=RREQ_reply.size()) {
    std::cout << "Node " << nodeName
              << " received RREP from " << RREQ_reply[size+1]
              << " to Node " << dest
              << " with rout [" << RREQ_reply << RREP_SRC << "]"
              << std::endl;
  } else {}

  // sending RREP to path
  responseRec = true;
  std::map<char, Node*>::iterator nodeIterator;
  Node* nextNode = NULL;

  if ((size-1)>=0) {
    if (size!=0) {
      nodeIterator = nodeConnects.find(RREQ_reply[size-1]);
    }
    nextNode = nodeIterator->second;
  } if (nextNode!=NULL) {
    nextNode->RREP_str = RREQ_reply;
    nextNode->sendRREP(nextNode->nodeName, dest, size-1, RREQ_reply, RREP_SRC);
  }
}

/*
 * FUNCTION: connected
 * checks if a node is connected
 * RETURNS: true/false (bool)
 */
bool Node::connected(char nodeName) {
  if (nodeConnects.count(nodeName)>0) {
    return true;
  } else {
    return false;
  }
}
