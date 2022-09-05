#include "tree.h"

Tree::Tree (File& file)
    {
        std::vector<std::string> nodes_with_children {};
        for (std::string line; std::getline(file.in_file, line);)
        {

            //  If line contains Root (1st line should)
            if (!(line.find(_Text.root.symbol) == std::string::npos))
            {
                Node::Type val;
                Node::type_num data_type;

                data_type = getDataType(line);

                try
                {
                    val = getValue(line, data_type);

                } catch (Node::type_num data_type)
                {
                    std::cerr << "An ERROR while reading data type!";
                }

                //  Create root node with value Val
                root = std::make_unique<Node>(val);
            }
            //   If line doesn't contain Root
            else
            { 
                nodes_with_children.push_back(line);
            }

        }

        addChildrenFromText(nodes_with_children);
        
        // file.in_file.close();
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


    //  Add children from vector of strings
    void Tree::addChildrenFromText(std::vector<std::string>& vec)
    {
        for (auto string : vec)
        {
            //  Data_type and value of parent node
            Node::type_num parent_data_type { getDataType(string) };
            Node::Type parent_val { getValue(string, parent_data_type) };

            //  String with all children of current node
            std::string children_list { 
                string.substr(string.find(_Text.children.symbol) + _Text.children.symbol.length(),
                              string.length() - (string.find(_Text.children.symbol) + _Text.children.symbol.length()))
            };
            
            //  Vector of strings containing one child of parent node each
            std::vector<std::string> children {};
            std::string child_str;
            
            std::size_t pos {0};

            //  Splitting string with all children to single-child strings by delimiter
            while ((pos = children_list.find(_Text.delim.symbol)) != std::string::npos) 
            {
                child_str = children_list.substr(0, pos);
                children.push_back(child_str);
                children_list.erase(0, pos + _Text.delim.symbol.length());
            }

            //  Parse each child string and call addChild to parent node
            for (auto& child : children)
            {
                Node::type_num child_data_type { getDataType(child) };
                Node::Type child_val { getValue(child, child_data_type) };

                (*this)[parent_val].addChild(child_val);
            }
        }
    }


    void Tree::printFile(std::ofstream& out)
    {
        out << _Text.root.symbol;
        root.get()->print(out);
        out << std::endl;

        root.get()->printNodes(out);
    }

    void Tree::serialize(File& file)
    {
        printFile(file.out_file);
        file.out_file.close();
    }



