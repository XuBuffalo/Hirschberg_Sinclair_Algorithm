/*************************************************************************
 * main
 *************************************************************************
 * @file    main.cpp
 * @date    10.09.2014
 * @author  Xu Cang
 * @license The MIT License (MIT)
 ************************************************************************/
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <jsonrpc/rpc.h>
#include <jsonrpc/connectors/httpserver.h>
#include "abstract_election_msg_server.h"
#include <jsonrpc/connectors/httpclient.h>
#include "election_msg_client.h"
#include "abstract_election_msg_server.h"

#include "network_node.cpp"

using namespace jsonrpc;
using namespace std;

int main(int argc, char* argv[]) 
{
    if (argc != 6) {
        cout << "argument number wrong " << argc<< endl;
        exit (EXIT_FAILURE);
    }
    
    int server_port = atoi(argv[1]);
    int network_size = atoi(argv[2]);
    string left_port_str = argv[3];
    string right_port_str = argv[4];
    int self_uid = atoi(argv[5]);

 
    string httpclient_left_str = "http://localhost:" + left_port_str;
    string httpclient_right_str = "http://localhost:" + right_port_str;
    NetworkNode network_node(server_port,network_size,left_port_str,right_port_str,self_uid);
    network_node.StartListening();

    // sleep til all servers are ready.
    sleep(10);

    HttpClient* httpClient_left = new HttpClient(httpclient_left_str);
    HttpClient* httpClient_right = new HttpClient(httpclient_right_str);
    ElectionMsgClient client_left(httpClient_left);
    ElectionMsgClient client_right(httpClient_right);
    try
    {
        cout << client_left.send_msg("out",1,self_uid,self_uid) << endl;
    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }

    try
    {     
        cout << client_right.send_msg("out",1,self_uid,self_uid) << endl;
    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }

    char c;
    puts ("Enter text. Include a dot ('.') in a sentence to exit:");
    do {
        c=getchar();
        putchar (c);
    } while (c != '.');
  
    delete httpClient_left;
    delete httpClient_right;
    network_node.StopListening();

    return 0;
}
