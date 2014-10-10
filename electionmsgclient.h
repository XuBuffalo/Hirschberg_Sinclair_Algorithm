
#ifndef _ELECTIONMSGCLIENT_H_
#define _ELECTIONMSGCLIENT_H_

#include <jsonrpc/rpc.h>

class ElectionMsgClient
{
    public:
        ElectionMsgClient(jsonrpc::AbstractClientConnector* conn)
        {
            this->client = new jsonrpc::Client(conn);
        }
        ~ElectionMsgClient()
        {
            delete this->client;
        }

        std::string send_msg(const std::string& direction, const int& hop_count, const int& uid) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["direction"] = direction; 
            p["hop_count"] = hop_count; 
            p["uid"] = uid; 

            Json::Value result = this->client->CallMethod("send_msg",p);
            if (result.isString())
                return result.asString();
            else 
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());

        }

    private:
        jsonrpc::Client* client;
};
#endif //_ELECTIONMSGCLIENT_H_
