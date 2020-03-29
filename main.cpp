#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace boost::asio;
static const int buf_size=500;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
static io_service service;



void sync_echo(std::string& msg,int port_num)
{
    socket_ptr sock(new ip::tcp::socket(service));
    ip::tcp::endpoint ep( ip::address::from_string("192.168.1.2"), std::move(port_num));
    sock->connect(ep);
    char buff[200];
    int counter=0;
    std::string data="client list\n",data1="ping\n";
    bool check=true;
    int c=msg.size();
    sock->write_some(boost::asio::buffer(&c,4));
    sock->write_some(boost::asio::buffer(msg,c));
    std::string request;
      while (true)
    {

        std::getline(std::cin, request);


//          if (check)
          {
              int a=request.size();
              sock->write_some(boost::asio::buffer(&a,4));
              sock->write_some(boost::asio::buffer(request,a));

          }

 //         else
//          {
//              int a=data.size();
//              sock->write_some(boost::asio::buffer(&a,4));
//              sock->write_some(boost::asio::buffer(data,a));
//
//          }
          int b;
          sock->read_some(boost::asio::buffer(&b,4));
        sock->read_some(boost::asio::buffer(buff,b));
        fflush(stdout);
        std::cout<<" "+std::string(buff)<<std::endl;
        boost::this_thread::sleep(boost::posix_time::millisec(4));
 //      check=(!check);
    }
}
struct server
{
    std::string _reply;
    ip::tcp::socket _sock{service};
    void connect()
    {
        ip::tcp::endpoint ep( ip::address::from_string("192.168.1.2"), std::move(8001));
        _sock.connect(ep);
    }
    void send_name()
    {
        std::string request;
        std::cin>>request;
        request+="\r\n\r\n";
        std::cout<<request;
        _sock.write_some(boost::asio::buffer(request,request.size()));
        read_reply();

    }
    void read_reply()
    {
        boost::asio::streambuf buffer{};
        read_until(_sock, buffer, "\n");
        std::string reply((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
        _reply=reply;
        //std::cout<<reply;
    }
    void send_ping()
    {
        std::string request="ping\n";
        request+="\r\n";
        _sock.write_some(boost::asio::buffer(request,request.size()));
    }
    void send_list()
    {
        std::string request="list\n";
        request+="\r\n";
        _sock.write_some(boost::asio::buffer(request,request.size()));
    }
    void get_client_list()
    {
        bool flag=true;
        while(flag)
        {
            boost::asio::streambuf buffer{};
            read_until(_sock, buffer, "\n");
            std::string reply((std::istreambuf_iterator<char>(&buffer)), std::istreambuf_iterator<char>());
            std::cout<<reply;
            if (reply=="end\n"){flag=false;}
        }
    }

    void tester()
    {
        std::string request;
        connect();
        send_name();

        while(true)
        {
            send_ping();
            read_reply();
            if (_reply=="ping_ok\n\r\n")
            {
               // std::cout<<_reply;
                continue;
            } else{
                if (_reply=="changed\n\r\n")
                {
                    send_list();
                    read_reply();
                    std::cout<<"list:\n";
                    std::cout<<_reply;
                }
            }
        }

    }
};


int main(int argc, char* argv[])
{
    server one;
   one.tester();
    return 0;
}
