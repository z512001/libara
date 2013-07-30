/*
 * $FU-Copyright$
 */

#include "Testbed.h"
#include "CLibs.h"
#include "TestbedARAClient.h"
#include "PacketDispatcher.h"
#include "BasicConfiguration.h"
#include "SimpleLogger.h"
#include "Environment.h"
#include "TestbedClock.h"

TESTBED_NAMESPACE_BEGIN

TestbedARAClient::TestbedARAClient(Configuration& config) : AbstractARAClient(config){
    //TODO Make configurable
    Logger* logger = new SimpleLogger("ara");
    setLogger(logger);
    logDebug("Initialized testbedARAClient");
    initializeNetworkInterfaces();
    logDebug("Initialized testbedARAClient network Interfaces");
    Environment::setClock(new TestbedClock());
}

TestbedARAClient::~TestbedARAClient() { }

void TestbedARAClient::sendPacket(Packet* packet) {
    AbstractARAClient::sendPacket(packet);
}

void TestbedARAClient::receivePacket(Packet* packet, ARA::NetworkInterface* interface) {
    logDebug("receiving packet # %u type %c over interface at %s", packet->getSequenceNumber(), packet->getType(), interface->getLocalAddress()->toString().c_str());
    AbstractARAClient::receivePacket(packet, interface);
    //TODO: persistRoutingTableData
}

void TestbedARAClient::deliverToSystem(const Packet* packet) {
    logDebug("sending packet # %u to System via TAP", packet->getSequenceNumber());
    dessert_syssend((void*) packet->getPayload(), packet->getPayloadLength());
    std::cout << "deliverToSystem " << packet->getPayload() << " delivered to system." << std::endl;
}

void TestbedARAClient::packetNotDeliverable(const Packet* packet) {
    logDebug("packet # %u is undeliverable", packet->getSequenceNumber());
    delete packet;
    //nrOfNotDeliverablePackets++;
}

void TestbedARAClient::initializeNetworkInterfaces() {
    dessert_meshif_t* dessertInterfaces = dessert_meshiflist_get();
    while(dessertInterfaces != NULL) {
        NetworkInterface* newInterface = new NetworkInterface(dessertInterfaces, this, packetFactory, 400000);
        addNetworkInterface(newInterface);
        logDebug("initialized network interface: %s", dessertInterfaces->if_name);
        dessertInterfaces = dessertInterfaces->next;
    }
}

bool TestbedARAClient::isLocalAddress(AddressPtr address) const {
    TestbedAddressPtr tapAddress(new TestbedAddress(dessert_l25_defsrc));
    return(address.get()->equals(tapAddress) || AbstractNetworkClient::isLocalAddress(address));
}

TESTBED_NAMESPACE_END
