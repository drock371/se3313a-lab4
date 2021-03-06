


#ifndef DMURRA47_SERVER_HPP
#define DMURRA47_SERVER_HPP


#include <msg/instance.hpp>
#include <msg/error.hpp>
#include <msg/visitor.hpp>

#include <networking/flex_waiter.hpp>
#include <networking/socket.hpp>
#include <networking/socket_server.hpp>

#include <string>
#include <vector>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/optional.hpp>


namespace dmurra47 
{
class server final:
public se3313::networking::flex_waiter::activity_visitor,
  public se3313::msg::request::abstract_message_visitor<>,
    public std::enable_shared_from_this<server>
{

public:

    typedef se3313::networking::port_t port_t;
    typedef se3313::networking::flex_waiter flex_waiter;
    typedef se3313::networking::socket socket;
    typedef se3313::networking::socket_server socket_server;
    
private:
    
    const port_t _serverPort;
    
    std::shared_ptr<flex_waiter> waiter;				// Holds all the sockets
    bool killed;							// Holds whether or not server.stop() was called.
    std::vector<std::pair<std::shared_ptr<socket>,std::string>> sockets;// The client sockets and associated usernames
    std::shared_ptr<socket_server> server_socket;			// The server socket
    
    //std::vector<std::string> usernames;				// Holds the usernames
    
    std::vector<std::shared_ptr<se3313::msg::instance>> messageQueue;	// Holds the messages to be sent to all clients.
    std::mutex messageQueueLock;					// MessageQueue lock
    std::mutex socketsLock;						// Lock the sockets vector
    
public:

    inline
    server(const port_t serverPort)
        : _serverPort(serverPort), killed(false)
    { 
    }

    ~server();

    /*!
     * \brief Start the server.
     */
    void start();

    /*!
     * \brief Stop the server.
     */
    void stop();
    

private:
    
    // INSERT YOUR OPERATIONS BELOW
  void listen();
  
  virtual void onSTDIN(const std::string& line);
  virtual void onSocket(const se3313::networking::flex_waiter::socket_ptr_t );
  virtual void onSocketServer(const std::shared_ptr<se3313::networking::socket_server>);
  
  virtual void sendMessages();
  
  /// Called when a login message is passed
  virtual return_t visitLogin(const se3313::msg::request::login& /* request */ );

  /// Called when a message is passed
  virtual return_t visitMessage(const se3313::msg::request::message& /* request */ );
    
  /// Called when an error occurs
  //virtual return_t error(const std::string& /*originator*/,const se3313::msg::ErrorCode /*code*/, const std::string& /*message*/);

};

} // end namespace

#endif // DMURRA47_SERVER_HPP
