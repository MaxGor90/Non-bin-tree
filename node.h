#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <iostream>
#include <variant>
#include <string>
#include <fstream>
#include <algorithm> // for std::replace



/*-------------------------------------------------------------------*/
//  Dictionary with phrases for (de)serializing

struct delimeter
    {
        std::string_view symbol;
    };

static const struct
    {
        delimeter type {"type_id#"};
        delimeter value {"with_value#"};
        delimeter children {"children_num:"};
        delimeter delim {";;"};
    } _Text;

/*-------------------------------------------------------------------*/


//  Node class can contain data of int, double and string types.
//  We can edit types by editing variant and ENUM,
//  also we'll have to edit Node::printValueFunctor and Node::print()

class Node
{
public:

    typedef std::unique_ptr<Node> ptr;
    typedef std::variant<int, double, std::string> Type;

    Type value{};
    enum type_num {INT, DOUBLE, STR} type_id;

    std::vector<ptr> children {};

    Node(Type val)
        : value {val}, type_id {static_cast<type_num>(val.index())}
    {}

    ~Node()
    {}


    //  Add multiple children via variadic template
    //  Returning parent node

    template <typename T>
    Node& addChildren(T val)
    {
        ptr node_pt = std::make_unique<Node>(val);
        children.push_back(std::move(node_pt));

        return *this;
    }

    template <typename T, typename... Args>
    Node& addChildren (T val, Args... args)
    {
        addChildren(val);

        addChildren(args...);

        return *this;
    }


    //  Add a single child and return pointer to that child
    Node* addChild(Type val)
    {
        ptr node_pt = std::make_unique<Node>(val);
        children.push_back(std::move(node_pt));

        return children.back().get();
    }


    //  Functor for printing value in exception message
    struct printValueFunctor
    {
        std::string operator() (int x) const                    { return std::to_string(x); }
        std::string operator() (double x) const                 { return std::to_string(x); }
        std::string operator() (const std::string& str) const   { return str; }
    };

    
    
    void print(std::ofstream& out);


    void printChild_num(std::ofstream& out);


    void serialize(std::ofstream& out);


    //  Search node with passed value, in case of success write its address into passed pointer
    void searchNode(Type& val, Node** pointer);


    Node& search(Type& val);
};
#endif // NODE_H
