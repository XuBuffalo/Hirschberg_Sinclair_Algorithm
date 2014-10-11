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
        NetworkNode(int port, int M);
        virtual string send_msg(const std::string& direction, const int& hop_count, const int& uid) ;
    private:
        int uid;
        Message send_right;
        Message send_left;
        NODE_STATUS node_status;
        int phase;

        int left_uid;
        int right_uid;
};

NetworkNode::NetworkNode(int port, int M) :
    AbstractElectionMsgServer(new HttpServer(port)),uid(0),node_status(UNKNOWN),phase(0)
{
    left_uid = (uid-1) > 0 ? uid-1 : M-1;
    right_uid = (uid+1) > M-1 ? 0: uid+1;
}


//TODO: chagne this funcion's name. Not so accurate.
string  NetworkNode::send_msg(const string& direction, const int& hop_count, const int& uid)
{
    //if message from i-1 

    return "Success!";
}

