#include <iomanip>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <fstream>
using namespace std;


class TestLog{
    private:
    static string log;
    public:
    static void appendLog(string message);
    static void clearLog();
    static void printLog();
    static void writeLogToFile();
};
string TestLog::log = string("");

void TestLog::appendLog (string message){
    time_t now = time(0);
    char* date = ctime(&now);
    date[strlen(date) -1] ='\0';
    string outputMessage(date);
    outputMessage = "\n["+outputMessage+"]  "+message+"";
    // cout<<outputMessage;
    TestLog::log.append(outputMessage); 
}

void TestLog::clearLog(){
    TestLog::log = string("");
}
void TestLog::printLog(){
    cout<<TestLog::log ;
}
void TestLog::writeLogToFile(){
    fstream file;
    file.open("log.txt",ios::app);
    file.write(TestLog::log.c_str(), strlen(TestLog::log.c_str()));
    file.close();
    TestLog::clearLog();
}