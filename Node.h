#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <map>

struct RequestCheck {
  char origin_RREQ;
  int ID_REQ_RREQ;
};

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
    char nodeName;
    std::string RREQ_string;
    std::string RREP_string;
    bool pathRec;
    bool responseRec;
};

Node::Node(char nodeLetter, std::map<char, Node*> connects) {
  // setting node designation to letter
  nodeName = nodeLetter;

  // creating Node connections
  connectAssign(connects);

  // setting path and response recieved to false
  pathRec = false;
  responseRec = false;
}

Node& Node::operator=(const Node& rhs) {
  if (this == &rhs) {
    return *this;
  }
  return *this;
}

Node* Node::memLocation() {
  return this;
}

void Node::connectAssign(std::map<char, Node*> connects) {
  nodeConnects = connects;
}

void Node::insConnect(char nodeDes, Node* loc) {
  nodeConnects.insert(std::pair<char, Node*>(nodeDes, loc));
}

void Node::removeConnect(char nodeDes) {
  nodeConnects.erase(nodeDes);
}

/*void sendRREP(char src, char dest, int size, string RREQ_reply, char RREP_SRC) {

}*/
