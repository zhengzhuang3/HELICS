/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once

#include "../CommsInterface.hpp"
#include <atomic>
#include <set>
#include <string>

#if (BOOST_VERSION_LEVEL >= 2)
namespace boost
{
namespace asio
{
class io_context;
using io_service = io_context;
}  // namespace asio
}  // namespace boost
#else
namespace boost
{
namespace asio
{
class io_service;
}
}  // namespace boost
#endif

namespace boost
{
namespace system
{
class error_code;
}
}  // namespace boost

namespace helics
{
namespace tcp
{
class TcpRxConnection;
class TcpConnection;

/** implementation for the communication interface that uses TCP messages to communicate*/
class TcpCommsSS final : public CommsInterface
{
  public:
    /** default constructor*/
    TcpCommsSS () noexcept;
    TcpCommsSS (const std::string &brokerTarget,
              const std::string &localTarget,
              interface_networks targetNetwork = interface_networks::local);
    TcpCommsSS (const NetworkBrokerData &netInfo);
    /** destructor*/
    ~TcpCommsSS ();
    /** set the port numbers for the broker port*/
    void setBrokerPort (int brokerPortNumber);
    /** set the port numbers for the local port*/
    void setPortNumber (int localPortNumber);
    /** set the controls to be running a server, or running connections*/
    void setServerMode(bool mode = true);
  private:
    int brokerPort = -1;
    int localPort = -1;
    bool serverMode = true;
    std::atomic<bool> hasBroker{false};
    virtual void queue_rx_function () override;  //!< the functional loop for the receive queue
    virtual void queue_tx_function () override;  //!< the loop for transmitting data

    virtual void closeReceiver () override;  //!< function to instruct the receiver loop to close
    /** process an incoming message
    return code for required action 0=NONE, -1 TERMINATE*/
    int processIncomingMessage (ActionMessage &cmd);
    ActionMessage generateReplyToIncomingMessage (ActionMessage &cmd);
    // promise and future for communicating port number from tx_thread to rx_thread

    void txReceive (const char *data, size_t bytes_received, const std::string &errorMessage);

    void txPriorityReceive (std::shared_ptr<TcpConnection> connection,
                            const char *data,
                            size_t bytes_received,
                            const boost::system::error_code &error);
    /** callback function for receiving data asynchronously from the socket
    @param connection pointer to the connection
    @param data the pointer to the data
    @param bytes_received the length of the received data
    @return a the number of bytes used by the function
    */
    size_t dataReceive (std::shared_ptr<TcpRxConnection> connection, const char *data, size_t bytes_received);

    bool commErrorHandler (std::shared_ptr<TcpRxConnection> connection, const boost::system::error_code &error);
    //  bool errorHandle()
  public:
    /** get the port number of the comms object to push message to*/
    int getPort () const { return localPort; };

    std::string getAddress () const;
};

}  // namespace tcp
}  // namespace helics