#pragma once
#include <Network/ArkNet/ArkPeer.hpp>
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class NetworkPeer : public Instance
    {
    protected:
        ArkNet::ArkPeer* m_peer;
        ArkNet::ArkSocket* m_socket;
    public:
        NetworkPeer();
        ~NetworkPeer();

        virtual std::string getClassName() { return "NetworkPeer"; }
    };
}