#define CATCH_CONFIG_MAIN

#include "catch.hpp"
//Including header files did not work here; had to include complete cpp file
#include "helper.cpp"
#include "parking_bst.cpp"

TEST_CASE("Parking data structure tests"){
    SECTION("Test tree structure and insertion is in order"){
        struct bstnode *root = NULL;

        root = insert(root, "AAAA AAA", get_date_string(), get_time_string());
        root = insert(root, "BBBB BBB", get_date_string(), get_time_string());
        root = insert(root, "CCCC CCC", get_date_string(), get_time_string());
        root = insert(root, "DDDD DDD", get_date_string(), get_time_string());
        root = insert(root, "EEEE EEE", get_date_string(), get_time_string());

        //Since the tree is ordered B should be root,
        //A should be left, C should be right
        REQUIRE(root->plate == "AAAA AAA");
        REQUIRE(root->right->plate == "BBBB BBB");
        REQUIRE(root->left == NULL);
        REQUIRE(root->right->right->right->right->plate == "EEEE EEE");
    }
}
TEST_CASE("Functional tests"){
    std::string timeIn;
    std::string dateIn;
    std::string timeOut;
    std::string dateOut;
    std::string plate;

    bstnode *root = NULL;
    root = init(root);
    std::cout << "Init run" << std::endl;
    std::string filePath = "";
    bstnode *auxRoot = NULL;
    std::cout << "auxroot null" << std::endl;
    
    SECTION("Test that the tree is created correctly"){
        REQUIRE(root->plate == "GD12 FDD");
        REQUIRE(root->right->plate == "KS06 OOS");
        REQUIRE(root->left->plate == "AL05 EBW");
    }
    
    SECTION("Test that the data is saved"){
        //Copy of my read_from_file function
        filePath = ".\\12032022.log";
        std::ifstream myFile;

        std::string splitWord;
        std::string line;
        
        std::string delimiter = ",";
        int pos = 0;
        myFile.open(filePath);

        
        if(!myFile.is_open()) {
            std::cout << "Error opening file " << filePath << ". Maybe the file doesn't exist\n";
            return;
        }
        getline(myFile, line); //Get header out of way
        while(getline(myFile, line)) { 
            pos = 0;
            for(int i = 0; pos != std::string::npos; i++){
                pos = line.find(delimiter);
                splitWord = line.substr(0, pos);
                switch(i){
                    case 0:
                        dateIn = splitWord;
                        dateOut = splitWord;
                        break;
                    case 1:
                        plate = splitWord;
                        break;
                    case 2:
                        timeIn = splitWord;
                        break;
                    case 3:
                        timeOut = splitWord;
                        auxRoot = insertNoDelete(auxRoot, plate, dateIn, timeIn, dateOut, timeOut);
                        break;
                }
                line.erase(0, pos + (delimiter.length()+1));
            }
        }
        myFile.close();

        REQUIRE(auxRoot->plate == "XS21 SBG");
        //Ordered by date
        REQUIRE(auxRoot->right->plate == "BV62 DLE");

        free(auxRoot);
        auxRoot = NULL;
    }
}