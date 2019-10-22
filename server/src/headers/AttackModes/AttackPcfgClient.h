/**
 * @file AttackPcfgClient.h
 * @brief Header file for creation of Pcfg Attack gRPC client
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 29. 8. 2019
 * @license MIT, see LICENSE
 */


#ifndef SERVER_ATTACKPCFGCLIENT_H
#define SERVER_ATTACKPCFGCLIENT_H


#include <protocol.pb.h>
#include <protocol.grpc.pb.h>
#include <grpcpp/grpcpp.h>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using proto::PCFG;
using proto::Items;
using proto::NextRequest;
using proto::Empty;
using proto::ConnectResponse;
using proto::CrackingResponse;
using proto::ResultResponse;


/** Class for gRPC calls */
class PretermClient {
public:

    PretermClient() = default;
    explicit PretermClient(uint64_t jobId);

    /**
     * Ask for next batch of preterminals
     * @param keyspace Number of passwords we ask for
     * @return Dictionary of passwords, as a single string
     */
    std::string GetNextItems(uint64_t & keyspace);

    /**
     * Connect to PCFG Manager
     * @return True if connected successfully, False otherwise
     */
    bool Connect();

    /**
    *
     * Disconnect from PCFG Manager
    * @return True if disconnected successfully, False otherwise
    */
    bool Disconnect();

    /**
     * Acknowledge the workunit was received
     */
    bool Acknowledge();

    /**
     * Kill the PCFG manager
     */
    bool Kill();


private:
    std::unique_ptr<PCFG::Stub> stub_;
};


#endif //SERVER_ATTACKPCFGCLIENT_H
