#ifndef TREE_H
#define TREE_H

#include <vector>
#include <memory>
#include <iostream>
#include <variant>
#include <string>
#include <fstream>

#include "node.h"
#include "file.h"


class Tree
{
public:
    Node::ptr root;

    //  Constructor from a value - creates root node
    Tree(Node::Type val)
        : root {std::make_unique<Node>(val)}
    {}


    //  Constructor from a file - deserializes structure
    Tree(File& file);

    ~Tree()
    {}

    //  Parse data type
    Node::type_num getDataType(const std::string& str);

    //  Parse value
    Node::Type getValue(const std::string& str, Node::type_num data_type);

    int getChildNum(const std::string& str);

    void printFile(std::ofstream& out);


    void serialize(File& file);


    void deserialize(std::ifstream& in, Node* node);


    Node& operator[] (Node::Type val)
    {
        return root.get()->search(val);
    }



};

#endif // TREE_H
