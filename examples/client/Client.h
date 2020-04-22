#pragma once

#include <cstdint>
#include <string>
#include <iterator>
#include <vector>

#include <boost/array.hpp>

#include "common/boost_wrap.h"

#include "demo1/Message.h"
#include "demo1/input/ClientInputMessages.h"
#include "demo1/frame/Frame.h"

namespace demo1
{

namespace client    
{

class Client
{
public:
    Client(common::boost_wrap::io& io, const std::string& server, std::uint16_t port);

    bool start();

    using InputMsg = 
        demo1::Message<
            comms::option::ReadIterator<const std::uint8_t*>,
            comms::option::Handler<Client> 
        >;

    using InAckMsg = demo1::message::Ack<InputMsg>;
    
    void handle(InAckMsg& msg);
    void handle(InputMsg&);

private:
    using Socket = boost::asio::ip::tcp::socket;

    using OutputMsg = 
        demo1::Message<
            comms::option::WriteIterator<std::back_insert_iterator<std::vector<std::uint8_t> > >,
            comms::option::LengthInfoInterface,
            comms::option::IdInfoInterface,
            comms::option::NameInterface
        >;

    using AllInputMessages = demo1::input::ClientInputMessages<InputMsg>;

    using Frame = demo1::frame::Frame<InputMsg, AllInputMessages>;


    void readDataFromServer();
    void readDataFromStdin();
    void sendSimpleInts();
    void sendScaledInts();
    void sendFloats();
    void sendEnums();
    void sendSets();
    void sendBitfields();
    void sendStrings();
    void sendDatas();
    void sendLists();
    void sendOptionals();
    void sendVariants();
    void sendMessage(const OutputMsg& msg);
    void waitForAck();
    void processInput();

    Socket m_socket;
    boost::asio::deadline_timer m_timer;
    std::string m_server;
    std::uint16_t m_port = 0U;
    Frame m_frame;
    demo1::MsgId m_sentId = demo1::MsgId_Ack;
    boost::array<std::uint8_t, 32> m_readBuf;
    std::vector<std::uint8_t> m_inputBuf;
};

} // namespace client

} // namespace demo1
