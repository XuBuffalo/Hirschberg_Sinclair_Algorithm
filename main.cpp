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

#include <jsonrpc/rpc.h>
#include <jsonrpc/connectors/httpserver.h>
#include <jsonrpc/connectors/httpclient.h>

#include "abstract_election_msg_server.h"
#include "election_msg_client.h"
#include "network_node.cpp"

using namespace jsonrpc;
using namespace std;

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
        exit (EXIT_FAILURE);
    int port = atoi(argv[1]);
    string port_str = to_string(port);
    string httpclient_str = "http://localhost:" + port_str;

    NetworkNode network_node(port);
    network_node.StartListening();


    HttpClient* httpClient = new HttpClient(httpclient_str);
    ElectionMsgClient client(httpClient);

    try
    {
        cout << client.send_msg("out",3,10001) << endl;
    

    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }

    delete httpClient;
    network_node.StopListening();

    return 0;
}
