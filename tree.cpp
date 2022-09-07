#include "tree.h"


    Tree::Tree(File& file)
    {
        std::string str;
        std::getline(file.in_file, str);

        Node::type_num data_type { getDataType(str) };
        Node::Type val { getValue(str, data_type) };
        int childNum { getChildNum(str) };

        root = std::make_unique<Node>(val);

        for (int i {0}; i < childNum; ++i)
            deserialize(file.in_file, root.get());
    }

    Node::type_num Tree::getDataType(const std::string& str)
    {
        return static_cast<Node::type_num>(
                    std::stoi(str.substr(str.find(_Text.type.symbol) +
                                         _Text.type.symbol.length(), 1)));
    }

    //  Parse value
    Node::Type Tree::getValue(const std::string& str, Node::type_num data_type)
    {

        std::string str_val { str.substr(str.find(_Text.value.symbol) + _Text.value.symbol.length(),
                              str.find(_Text.delim.symbol) - (str.find(_Text.value.symbol) + _Text.value.symbol.length())) };

        //  Convert value to its data type
        switch (data_type)
        {
        case 0:
            return std::stoi(str_val);
        case 1:
            return std::stod(str_val);
        case 2:
            //  Replace \r back to \n 
            std::replace(str_val.begin(), str_val.end(), '\r', '\n');
            return str_val;
        default:
            throw (data_type);
        }
    }

    int Tree::getChildNum(const std::string& str)
    {
        return std::stoi(
                    str.substr(str.find(_Text.children.symbol) + _Text.children.symbol.length(),
                               str.find(_Text.delim.symbol) ) );
    }


    void Tree::serialize(File& file)
    {
        root.get()->serialize(file.out_file);
        file.out_file.close();
    }


    void Tree::deserialize(std::ifstream& in, Node* node)
    {
        std::string line;
        std::getline(in, line);

        Node::type_num data_type { getDataType(line) };
        Node::Type val { getValue(line, data_type) };
        int childNum { getChildNum(line) };

        Node* child_node = node->addChild(val);

        if (childNum > 0)
        {
            for (int i {0}; i < childNum; ++i)
                deserialize(in, child_node);
        }

        return;
    }


