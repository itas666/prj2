#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "parking_bst.h"

//For the datetime function
using sysclock_t = std::chrono::system_clock;

//Function for datetime as string:
//https://stackoverflow.com/questions/34963738/c11-get-current-date-and-time-as-string
std::string get_date_string() {
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());

    char buf[20] = { 0 };
    std::strftime(buf, sizeof(buf), "%d/%m/%Y", std::localtime(&now));

    return buf;
}

std::string get_time_string() {
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());

    char buf[20] = { 0 };
    std::strftime(buf, sizeof(buf), "%H:%m:%S", std::localtime(&now));

    return buf;
}

//Function to return the number of seconds in the current year
//Not used, the calculation for this would be inefficient
//we would need to calculate the number of seconds until current year
//Basically the epoch, C++ does not seem to have this like java for example
int get_number_of_days(int month, int year)
{
    int secondsUntilMonth = 0;
	//leap year condition, if month is 2
    for(int monthCounter = month-1; monthCounter > 0; monthCounter--){
        if( month == 2)
        {
            if((year%400==0) || (year%4==0 && year%100!=0)){
                secondsUntilMonth += 29*24*60*60;
            }else{
                secondsUntilMonth += 28*24*60*60;
            }
        }
        //months which has 31 days
        else if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||month == 10 || month==12){
            secondsUntilMonth = 31*24*60*60;
        }else{
            secondsUntilMonth = 30*24*60*60;
        }
    }
    return secondsUntilMonth;
} 

//Helper function to calculate price
//Is not expected for a car to stay days in a parking slot
//Does not take into account days or months or years...
double calculate_price(std::string inDate, std::string inTime, std::string outDate, std::string outTime) {
    int startDateSeconds = (std::stoi(inTime.substr(6, 2))) +                         //Seconds
    (std::stoi(inTime.substr(3, 2)))*60 +                                             //Minutes
    (std::stoi(inTime.substr(0, 2)))*60*60 +                                          //Hours
    (std::stoi(inDate.substr(8, 2)))*60*60*24;                                        //Days
    //get_number_of_days ((startDate::substr(5, 2), (startDate::substr(0, 4)) //Months
    
    int endDateSeconds = (std::stoi(inTime.substr(6, 2))) +                             //Seconds
    (std::stoi(inTime.substr(3, 2)))*60 +                                               //Minutes
    (std::stoi(inTime.substr(0, 2)))*60*60 +                                            //Hours
    (std::stoi(outDate.substr(8, 2)))*60*60*24;                                          //Days
    
    int minutesDifference = (endDateSeconds*60) - (startDateSeconds*60);
    
    if((minutesDifference > 30)&&(minutesDifference <= 60)){
        return 1.50;
    }else if((minutesDifference > 60)&&(minutesDifference <= (60*2))){
        return 3.00;
    }else if((minutesDifference > (60*2))&&(minutesDifference <= (60*4))){
        return 5.00;
    }else if((minutesDifference > (60*4))&&(minutesDifference <= (60*8))){
        return 15.00;
    }else if((minutesDifference > (60*8))){
        return 30.00;
    }
    return 0.00;
}

std::string give_file_ready_string(std::string dateString) {
    std::string returnString = dateString.substr(0,2);
    returnString.append(dateString.substr(3,2));
    returnString.append(dateString.substr(6,4));
    
    return returnString;
}

bool file_empty(std::string filePath){
    std::ifstream fin;
    fin.open(filePath);
    std::string aux = "";
    getline(fin, aux);

    if(aux == ""){
        return true;
    }
    return false;
}

void read_from_file(std::string dateString){
    bstnode *auxRoot = NULL;

    std::string timeIn;
    std::string dateIn;
    std::string timeOut;
    std::string dateOut;
    std::string plate;

    std::string filePath = ".\\" + give_file_ready_string(dateString) + ".log";
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

    inorder(auxRoot);
    free(auxRoot);
    auxRoot = NULL;
    return;
 }