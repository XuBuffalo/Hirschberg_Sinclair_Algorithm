/*************************************************************************
 * network_node
 *************************************************************************
 * @file    network_node.cpp
 * @date    10.09.2014
 * @author  Xu Cang
 * @license The MIT License (MIT)
 ************************************************************************/
#include <jsonrpc/rpc.h>
#include <iostream>
#include <jsonrpc/connectors/httpserver.h>
#include "election_msg_client.h"
#include <jsonrpc/connectors/httpclient.h>
#include "abstract_election_msg_server.h"

using namespace jsonrpc;
using namespace std;
enum NODE_STATUS{
    UNKNOWN,
    LEADER
};

struct Message {
    int send_uid;
    bool is_inbound; //true for inbound, false for outbound
    int hop_cound;
};

class NetworkNode : public AbstractElectionMsgServer
{
    public:
        NetworkNode(int port);
        virtual string send_msg(const std::string& direction, const int& hop_count, const int& uid) ;
    private:
        int uid;
        Message send_right;
        Message send_left;
        NODE_STATUS node_status;
        int phase;

};

NetworkNode::NetworkNode(int port) :
    AbstractElectionMsgServer(new HttpServer(port)),uid(0),node_status(UNKNOWN),phase(0)
{
}


//TODO: chagne this funcion's name. Not so accurate.
string  NetworkNode::send_msg(const string& direction, const int& hop_count, const int& uid)
{
    //if message from i-1 

    return "Success!";
}

