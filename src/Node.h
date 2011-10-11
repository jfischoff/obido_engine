#ifndef NODE_H
#define NODE_H

#include <Transform.h>

class Node
{
public:
    
private:
    Transform m_Transform
    vector<Node*> m_Children;
    void* m_Data;
    string m_DataType;

};

#endif // NODE_H
