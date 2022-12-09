#include <iostream>
#include "tree.h"
#include "node.h"
#include "file.h"


    //  Compare two files
    bool compare(const std::string& name1, const std::string& name2)
    {
        File file1(File::READ, name1);
        File file2(File::READ, name2);
        if ((!file1.in_file)  ||  (!file2.in_file))    
        {
            std::cerr << "File is not opened for reading!\n";
            return false;
        }

        char ch1, ch2;

        while (file1.in_file.get(ch1) && file2.in_file.get(ch2))
        {
            if (ch1 != ch2)     {   return false;   }
        }

        return true;    
    }



int main()
{

/* -----------   Create and serialize tree    ----------- */  
////////////////////////////////////////////////////////////



   Tree tree {"TREE"};

   try 
   {
       tree["TREE"].addChildren(1, "Johny I hardly knew ya", 12.5);
       tree[1].addChildren(2, 3, 4, 5);
       tree["Johny I hardly knew ya"].addChildren("Hello\nWorld!", 42);
       tree[12.5].addChildren(13.5);
       tree[13.5].addChildren(14.5, 15.5);
       tree[15.5].addChildren(16.5);
       tree[2].addChildren(2.5, 2.6);
       tree[3].addChildren(3.5, 3.6);
       tree[4].addChildren(4.5, 4.6);
       tree[1].addChildren(2.5, "5 point 6");
   }
   catch (Node::Type value)
   {
       std::cout << "Element " << std::visit(Node::printValueFunctor(), value) << " not found!" << std::endl;

       return -1;
   }

    File file(File::WRITE, "file1.txt");

    tree.serialize(file);


/* -----------   Deserialize tree1 from file   ----------- */  
////////////////////////////////////////////////////////////


    File file_in(File::READ, "file1.txt");

    Tree* tree1;

    try
    {
        tree1 = new Tree(file_in);

    }
    catch (Node::type_num data_type)
    {
        std::cerr << "An ERROR while reading data type: " << data_type;
        return -1;
    }


    File file_out(File::WRITE, "file2.txt");

    //  Serialize to compare in and out files
    tree1->serialize(file_out);
    
    delete tree1;


    std::cout << (compare("file1.txt", "file2.txt") ? "Files are identical.\n" : "Files are not identical.\n");



    return 0;
}
