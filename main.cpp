#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <chrono>
#include "talk_to_server.h"

using namespace boost::asio;




ip::tcp::endpoint ep(ip::address::from_string( "127.0.0.1" ),8001);


void processing_client (const std::string &name_client) {
    //std::cout<<"test"<<std::endl;
    talk_to_server client(name_client);
    try{
        //std::cout<<"test1"<<std::endl;
        client.connect(ep);
        client.answer();
    }
    catch (boost::system::system_error &err) {
        std::cout << "client " << client.username() << " " << err.code() <<std::endl;
    }


}

int main(int argc , char* argv[]) {

    std::string str1;

    std::cin >> str1;
    std::cin.clear();

   // std::vector <std::string> arr;

    //std::cout << " enter logins , if u wanna stop enter 1" <<std::endl;

   // std::string str1 = "sashas";
    processing_client(str1);

   // std::string str2 = "dima";
   // processing_client(str2);

    //while ( str1 != "1") {
      //getline(std::cin , str1);
       //arr.push_back(str1);
    //}

    //arr.erase(arr.begin() +3);



    //for ( auto it = arr.begin()  ; it != arr.end() ; ++it){
        //std::thread process(processing_client,*it);
        //std::this_thread::sleep_for(chrono::milliseconds(4000));
    //}
