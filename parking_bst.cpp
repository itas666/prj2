#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include "helper.h"

//Declaration of our data structure
//It will contain the enter time, plate, and node addresses
//It is ordered binary tree so that random search can be more efficient than in linked lists
struct bstnode {
    std::string plate;
    std::string startDate;
    std::string startTime;
    std::string endDate;
    std::string endTime;
    double finalPrice;
    struct bstnode *left;
    struct bstnode *right;
};

// create a new BST node struct 
bstnode* newNode(std::string key, std::string inDate, std::string inTime, std::string outDate, std::string outTime) {
    struct bstnode *temp = new struct bstnode();
    temp->plate = key;

    temp->startDate = inDate;
    temp->startTime = inTime;
    if(outTime == ""){

    }
    temp->endTime = outTime;
    temp->endDate = outDate;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

// perform inorder traversal of BST
void inorder(struct bstnode *root) {
    if (root != NULL) {
        inorder(root->left);
        std::cout << root->plate << " " << std::endl;
        std::cout << "Entered: " << root->startDate << " " << root->startTime << std::endl;
        inorder(root->right);
    }
}


//returns the node with minimum value struct
bstnode * minValueNode(struct bstnode* node) { 
    struct bstnode* current = node; 
    //search the leftmost tree
    while (current && current->left != NULL) 
        current = current->left; 
    return current; 
} 


//When we delete a node, commit to permanent memory
void write_to_logfile(bstnode* node){
    std::string filePath = give_file_ready_string(node->startDate) + ".log";
    std::ofstream outFile(filePath, std::ios::out | std::ios::app);
    std::string outString;

    if(file_empty(filePath)) {
        outString = outString + "DATE, VRN, TIME-IN, TIME-OUT\r\n";
    }
    
    outString = outString + node->startDate + ", " + node->plate + ", " + node->startTime + ", " + node->endTime + "\n";
    // \n to fix issue with new line not correct

    outFile << outString;
    outFile.flush();
    outFile.close();
}

 //function to delete the node with given key and rearrange the root struct
 bstnode* deleteNode(struct bstnode* root, std::string key,
        std::string outDate, std::string outTime) {
    // empty tree returns null (root) 

    if (root == NULL){
        return root;
    }
    
    // search the tree and if key < root, go for lefmost tree
    // if key > root, go for rightmost tree else
    if (key < root->plate){
        root->left = deleteNode(root->left, key, outDate, outTime);
    }else if (key > root->plate){
        root->right = deleteNode(root->right, key, outDate, outTime);
    }else {
        root->endDate = outDate;
        root->endTime = outTime;
        root->finalPrice = calculate_price(root->startDate, root->startTime, root->endDate, root->endTime);

        std::cout << "Saving to file" << std::endl;
        std::cout << "Plate: " << root->plate <<std::endl;
        std::cout << "In DateTime: " << root->startDate << " " << root->startTime << std::endl;
        std::cout << "End DateTime: " << root->endDate << " " << root->endTime << std::endl;
        std::cout << "Price to pay: " << root->finalPrice <<std::endl;
        
        write_to_logfile(root);

        if (root->left == NULL) {
            struct bstnode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct bstnode *temp = root->left;
            free(root);
            return temp;
        }

        // node with both children; get successor and then delete the node struct
        // Copy the inorder successor's content to this node
        // Delete the inorder successor to "shrink" the tree there
        
        bstnode* temp = minValueNode(root->right);
        root->plate = temp->plate;
        root->right = deleteNode(root->right, temp->plate, outDate, outTime);
    }
    return root;
 }

// insert a new node in BST with given key
bstnode* insert(struct bstnode* node, std::string key, std::string inDate, std::string inTime) {
    //tree is empty;
    //return a new node
    if (node == NULL){
        return newNode(key, inDate, inTime, "", "");
    }

    if(node->plate == key){
        node = deleteNode(node, key, inDate, inTime); //inDate = outDate in this case
    }else{
        //if tree is not empty find the proper place to insert new node
        if (key < node->plate){
            node->left = insert(node->left, key, inDate, inTime);
        }else{
            node->right = insert(node->right, key, inDate, inTime);
        }
    }
    //return the node pointer
    return node;
}

// insert a new node in BST with given key; does not delete the data
// if it finds the provided key, specifically for the LIST option
bstnode* insertNoDelete(struct bstnode* node, std::string key,
        std::string inDate, std::string inTime, std::string outDate, std::string outTime) {
    //tree is empty;
    //return a new node
    if (node == NULL){
        return newNode(key, inDate, inTime, outDate, outTime);
    }

    
    // if tree is not empty find the proper place to insert new node
    // Compare by time string (brute force but works) instead of by plate
    if (inTime < node->startTime){
        node->left = insertNoDelete(node->left, key, inDate, inTime, outDate, outTime);
    }else{
        node->right = insertNoDelete(node->right, key, inDate, inTime, outDate, outTime);
    }
    //return the node pointer
    return node;
}

bstnode* init(bstnode *root){
    std::string line;
    std::string plate;
    std::string inTime;
    std::string dateTime;
    std::string outTime;
    std::string filePath = ".\\FILE.TXT";
    std::ifstream myFile;
    std::string splitWord;

    std::string delimiter = ",";
    int pos = 0;
    myFile.open(filePath);

    if(!myFile.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    getline(myFile, line); //Get header out of way
    while(getline(myFile, line)) {
        //Reset out time because sometimes it won't be filled
        //inTime will always be needed though
        outTime = ""; 
        pos = 0;
        for(int i = 0; pos != std::string::npos; i++){
            pos = line.find(delimiter);
            splitWord = line.substr(0, pos);
            switch(i){
                case 0:
                    dateTime = splitWord;
                    break;
                case 1:
                    plate = splitWord;
                    break;
                case 2:
                    inTime = splitWord;
                    break;
                case 3:
                    outTime = splitWord;
                    break;
            }
            
            line.erase(0, pos + (delimiter.length()+1));
        }
        root = insert(root, plate, dateTime, inTime);
        if(outTime.length() == 9){
            root = insert(root, plate, dateTime, outTime);
        }
    }

    myFile.close();

    return root;
 }