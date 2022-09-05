#include "node.h"



    void Node::print(std::ofstream& out)
    {
        switch (type_id) {
        case 0:
            out << _Text.type.symbol  << type_id << _Text.value.symbol << std::get<int>(value) << _Text.delim.symbol;
            break;
        case 1:
            out << _Text.type.symbol  << type_id << _Text.value.symbol << std::get<double>(value) << _Text.delim.symbol;
            break;
        case 2:
            //  Replace all \n in text with \r for easier deserialization
            std::replace(std::get<std::string>(value).begin(), std::get<std::string>(value).end(), '\n', '\r');
            out << _Text.type.symbol  << type_id << _Text.value.symbol << std::get<std::string>(value) << _Text.delim.symbol;
            break;
        default:
            break;
        }

//      out << std::visit(printValueFunctor(), value);
//      This function works but with non-adjustable 6 digits after comma for double :/

    }


    //  Print only needed info for serializing
    //  Nodes without children are printed only as other nodes' children
    void Node::printNodes(std::ofstream& out)
    {
        print(out);

        if (children.size() == 0)
        {
            return;
        }

        out << _Text.children.symbol;
        for (auto& child : children)
        {
            child.get()->print(out);
        }
        out << std::endl;

        for (auto& child : children)
        {
            if (child.get()->children.size() > 0)
                child.get()->printNodes(out);
        }
    }


    //  Search node with passed value, in case of success write its address into passed pointer
    void Node::searchNode(Type& val, Node** pointer)
    {
        if (value == val)
        {
            *pointer = this;
            return;
        }

        if (children.size() > 0)
        {
            for (auto& child : children)
            {
                child.get()->searchNode(val, pointer);
            }
        }
    }


    Node& Node::search(Type& val)
    {
        Node* tmp_ptr {nullptr};

        searchNode(val, &tmp_ptr);

        //  If success and pointer isn't nullptr - return, else - throw exception
        if (tmp_ptr)
            return *tmp_ptr;
        else
            throw val;
    }
