/**
 * @file AttackPcfgClient.cpp
 * @brief Source file for creation of Pcfg Attack gRPC client
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 29. 8. 2019
 * @license MIT, see LICENSE
 */

#include <AttackPcfgClient.h>
#include <Config.h>


PretermClient::PretermClient(uint64_t packageId)
{
    uint64_t pcfgPort = 50050 + (packageId % 1000);
    std::string pcfgAddress = "localhost:" + std::to_string(pcfgPort);

    grpc::ChannelArguments ch_args;
    ch_args.SetMaxReceiveMessageSize(-1);

    stub_ = PCFG::NewStub(grpc::CreateCustomChannel(pcfgAddress, grpc::InsecureChannelCredentials(), ch_args));
}


// Assembles the client's payload, sends it and presents the response back
// from the server.
std::string PretermClient::GetNextItems(uint64_t & keyspace)
{
    // Data we are sending to the server.
    NextRequest request;
    request.set_terminals(keyspace);

    // Container for the data we expect from the server.
    Items reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetNextItems(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
        keyspace = reply.terminalscount();

        Tools::printDebug("LOG: gRPC Succeeded: Real keyspace %" PRIu64 "\n", keyspace);

        std::string result;
        reply.SerializeToString(&result);

        return result;
    } else {
        std::string details(status.error_details());
        std::string msg(status.error_message());
        Tools::printDebug("LOG: gRPC call failed! Code: %d, %s, %s\n", status.error_code(), details.c_str(), msg.c_str());
        return std::string();
    }
}


bool PretermClient::Connect()
{
    Empty request;
    ConnectResponse reply;
    ClientContext context;
    Status status = stub_->Connect(&context, request, &reply);

    if (status.ok()) {
        Tools::printDebug("LOG: gRPC Connected!\n");
        return true;
    }
    else {
        Tools::printDebug("LOG: gRPC could not connect!\n");
        return false;
    }
}


bool PretermClient::Disconnect()
{
    Empty req_res;
    ClientContext context;

    Status status = stub_->Disconnect(&context, req_res, &req_res);
    if (status.ok()) {
        Tools::printDebug("LOG: gRPC successfully disconnected!\n");
        return true;
    }
    else {
        Tools::printDebug("LOG: gRPC failed to disconnect!\n");
        return false;
    }
}


bool PretermClient::Acknowledge()
{
    CrackingResponse request;
    ResultResponse reply;
    ClientContext context;

    Status status = stub_->SendResult(&context, request, &reply);
    if (status.ok()) {
        Tools::printDebug("LOG: gRPC successfully acked!\n");
        return true;
    }
    else {
        Tools::printDebug("LOG: gRPC failed to acked!\n");
        return false;
    }
}


bool PretermClient::Kill()
{
    Empty req_res;
    ClientContext context;

    Status status = stub_->Kill(&context, req_res, &req_res);
    if (status.ok()) {
        Tools::printDebug("LOG: PCFG-Manager successfully killed!\n");
        return true;
    }
    else {
        Tools::printDebug("LOG: PCFG-Manager failed to be killed!\n");
        return false;
    }
}
