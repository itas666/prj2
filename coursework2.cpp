#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "parking_bst.h"
#include "helper.h"

 int main() {
    std::string option;
    struct bstnode *root = NULL;

    std::cout << "Initiating" << std::endl;
    std::cout << "Inserting from file: FILE.TXT" << std::endl;

    //root = init(root);

    std::cout << "Binary Search Tree created (Inorder traversal):" << std::endl;
    inorder(root);

    std::cout << "Exit -- type 'exit' (in lower case)"
        << std::endl;
    std::cout << "List plates in memory (not saved) type 'list'"
        << std::endl;
    std::cout << "List plates commited to file type 'LIST'"
        << std::endl << std::endl;
    std::cout << "The program is supposed to act like a automatic scanner,"
        << "always input plate numbers," << std::endl << "if it exists"
        << "(vehicle entered and is exiting parking)it will be sent to file"
        << std::endl << std::endl;
    do{
        std::cout << "Scan plate: ";
        std::getline(std::cin, option);
        
        if(option == "list"){
            inorder(root);
            option = "0";
        }else if (option == "LIST"){
            std::cout << "Enter a date in 'DD/MM/YYYY' format (empty for 'today'): ";
            std::getline(std::cin, option);
            if(option == ""){
                option = get_date_string();
                std::cout << "Date is: " << option << std::endl;
            }
            read_from_file(option);
        }else{
            root = insert(root, option, get_date_string(), get_time_string());
        }
    }while((option != "") || (option != "exit"));

    return 0;
 }