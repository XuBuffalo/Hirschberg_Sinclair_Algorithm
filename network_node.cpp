/*************************************************************************
 * network_node
 *************************************************************************
 * @file    network_node.cpp
 * @date    10.09.2014
 * @author  Xu Cang
 * @license The MIT License (MIT)
 ************************************************************************/
#include <cmath>
#include <jsonrpc/rpc.h>
#include <iostream>
#include "abstract_election_msg_server.h"
#include <jsonrpc/connectors/httpserver.h>
#include "election_msg_client.h"
#include <jsonrpc/connectors/httpclient.h>


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
        NetworkNode(int port, int M, string left_port_str, string right_port_str, int self_uid);
        virtual ~NetworkNode();
        virtual string send_msg(const std::string& direction, const int& hop_count, const int& send_id,  const int& uid) ;
        string send_toleft(const string& direction, const int hop_count, const int& uid, const int& candidate_uid);
        string send_toright(const string& direction, const int hop_count, const int& uid, const int& candidate_uid);
    

    private:
        int uid;
        Message send_right;
        Message send_left;
        NODE_STATUS node_status;
        int phase;

        int left_uid;
        int right_uid;

        bool left_next_phase_ready;
        bool right_next_phase_ready;

        ElectionMsgClient* client_left;

        ElectionMsgClient* client_right;
};

NetworkNode::NetworkNode(int port, int M,string left_port_str, string right_port_str,int self_uid) :
    AbstractElectionMsgServer(new HttpServer(port)),uid(self_uid),node_status(UNKNOWN),phase(0)
{
    left_uid = (uid-1) > 0 ? uid-1 : M;
    right_uid = (uid+1) > M ? 1: uid+1;


    string httpclient_str_left = "http://localhost:" + left_port_str;
    string httpclient_str_right = "http://localhost:" + right_port_str;

    HttpClient* httpClient_left = new HttpClient(httpclient_str_left); // dont forget delete it!
    client_left = new ElectionMsgClient(httpClient_left);

    HttpClient* httpClient_right = new HttpClient(httpclient_str_right);
    client_right = new ElectionMsgClient(httpClient_right);
    // delete httpClient_left;
    // delete httpClient_right;
}

NetworkNode::~NetworkNode() 
{
  
    delete client_left;
    delete client_right;
}

string NetworkNode::send_toleft(const string& direction, const int hop_count, const int& uid, const int& candidate_uid){
    cout << "send to left!"<<" " << direction<< endl;
    cout << client_left->send_msg(direction,hop_count,uid,candidate_uid) << endl;
    return "success";
}
string NetworkNode::send_toright(const string& direction, const int hop_count, const int& uid, const int& candidate_uid){
    cout << "send to right!" << " "<< direction << endl;
    cout << client_right->send_msg(direction,hop_count,uid,candidate_uid)<< endl;
    return "success";
}

//TODO: chagne this funcion's name. Not so accurate.
string  NetworkNode::send_msg(const string& direction, const int& hop_count,const int& send_id, const int& candidate_uid)
{
    cout << "right uid is" << right_uid << "left_uid is " << left_uid << endl;
    cout << "received msg" << hop_count <<" " << send_id << " " << candidate_uid << " " << direction  <<endl;
    //if message from i-1 

    if ((send_id == left_uid) && (direction == "out")) {
        if (candidate_uid == uid) {
            node_status = LEADER;
            cout << "i am leader" << endl;
        } else if( (candidate_uid > uid) && (hop_count > 1)) {
            //send_right (out, hop_count-1,uid, candidate_uid)
            send_toright("out",hop_count-1, uid,candidate_uid);
        } else if ( (candidate_uid > uid) && (hop_count  == 1)) {
            //send_left(in, 1,uid, candidate_uid)
            send_toleft("in", 1, uid, candidate_uid);
        }
    }
    else if ((send_id == right_uid) && (direction == "out")) {
        if (( candidate_uid > uid) && (hop_count > 1)) {
            //send_left(out, hop_count-1, uid, candidate_uid)
            send_toleft("out", hop_count-1,uid,candidate_uid);
        } else if (( candidate_uid > uid) && (hop_count == 1)) {
            //send_right(in, 1, uid, candidate_uid)
            send_toright("in",1,uid,candidate_uid);
        } else if (candidate_uid == uid) {
            node_status = LEADER;
            cout << "i am leader now" << endl;
        }
    } else if ((send_id == left_uid) && (direction == "in") && (hop_count == 1) && (candidate_uid != uid)) {
        //send_right(in, 1, uid, candidate_uid)
        send_toright("in",1,uid,candidate_uid);
    }  if((send_id == right_uid) && (direction == "in") && (hop_count == 1) && (candidate_uid != uid)) {
        //send_left(in, 1, uid, candidate_uid)
        send_toleft("in",1,uid,candidate_uid);
    }

    if ((send_id == right_uid) && (direction == "in") && (hop_count == 1) && (candidate_uid == uid)) {
        cout << "phase is " <<phase << "right next phase ready"<< endl;
        right_next_phase_ready = true;
    }
    if ((send_id == left_uid) && (direction == "in") && (hop_count == 1) && (candidate_uid == uid)) {
        cout << "phase is " <<phase <<"left next phase ready"<< endl;
        left_next_phase_ready = true;
    }
    if (left_next_phase_ready && right_next_phase_ready) {
        phase += 1;
        cout << "phase is now " << phase << endl;
        left_next_phase_ready = 0;
        right_next_phase_ready = 0;
        //send_right(out, pow(2,phase), uid, uid)
        send_toright("out",pow(2,phase),uid,uid);
        //send_left(out,  pow(2,phase), uid, uid)
        send_toleft("out",pow(2,phase),uid,uid);
    }
    


    return "Success!";
}

