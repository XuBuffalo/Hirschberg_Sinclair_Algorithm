
#ifndef _ABSTRACTELECTIONMSGSERVER_H_
#define _ABSTRACTELECTIONMSGSERVER_H_

#include <jsonrpc/rpc.h>

class AbstractElectionMsgServer : public jsonrpc::AbstractServer<AbstractElectionMsgServer>
{
    public:
        AbstractElectionMsgServer(jsonrpc::AbstractServerConnector* conn) :
            jsonrpc::AbstractServer<AbstractElectionMsgServer>(conn) 
        {
            this->bindAndAddMethod(new jsonrpc::Procedure("send_msg", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_STRING, "direction",jsonrpc::JSON_STRING,"hop_count",jsonrpc::JSON_INTEGER,"send_id",jsonrpc::JSON_INTEGER,"uid",jsonrpc::JSON_INTEGER, NULL), &AbstractElectionMsgServer::send_msgI);

        }
        
        inline virtual void send_msgI(const Json::Value& request, Json::Value& response) 
        {
            response = this->send_msg(request["direction"].asString(), request["hop_count"].asInt(), request["send_id"].asInt(),request["uid"].asInt());
        }


        virtual std::string send_msg(const std::string& direction, const int& hop_count,  const int& send_id, const int& uid) = 0;

};
#endif //_ABSTRACTELECTIONMSGSERVER_H_
