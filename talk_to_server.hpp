//
// Created by Александр Волков on 29.11.18.
//

#ifndef ASIO_CLIENT_LAB_TALK_TO_SERVER_H
#define ASIO_CLIENT_LAB_TALK_TO_SERVER_H

#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

using namespace boost::asio;
io_service service;

struct talk_to_server
{
    talk_to_server(const std::string & username): sock_(service), started_(true), username_(username) {}

    void connect(ip::tcp::endpoint ep){
        sock_.connect(ep);
    }

    void answer(){
        write("login " + username_ + "\n");
        read_answer();
        while (started_){
            write_request();
            read_answer();
            boost::this_thread::sleep(boost::posix_time::millisec(rand() % 7000));
        }
    }
    std::string username() const { return username_; }

    void write_request(){
        write("ping\n");
    }
    void read_answer(){
        already_read_ =	read(sock_, buffer(buff_),
                                boost::bind(&talk_to_server::read_complete,this,_1, _2));
        process_msg();
    }
    void process_msg(){
        std::string msg(buff_, already_read_);

        if ( msg.find("login ") == 0) on_login(msg);
        else if ( msg.find("ping ") == 0) on_ping(msg);
        else if ( msg.find("clients ") == 0) on_clients(msg);
        else std::cerr << "invalid msg " << msg << std::endl;
    }

    void on_login(const std::string& msg) {
        std::cout<<msg<<std::endl;
        do_ask_clients();
    }

    void on_ping(const std::string & msg){
        std::cout<<msg<<std::endl;
        std::istringstream in(msg);
        std::string answer;
        in >> answer;
        if ( answer == "client_list_changed")
            do_ask_clients();
    }
    void on_clients(const std::string & msg){
        std::string clients = msg.substr(8);
        std::cout << username_ << ", new client list:" << clients;
    }
    void do_ask_clients(){
        write("ask_clients\n");
        read_answer();
    }
    void write(const std::string& msg) {
        sock_.write_some(buffer(msg));
    }

    size_t read_complete(const boost::system::error_code & err, size_t bytes){
        if (err) return 0;
        bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
        return found ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
};








#endif //ASIO_CLIENT_LAB_TALK_TO_SERVER_H
