/*
 * $FU-Copyright$
 */

#include "TestbedAddress.h" 

TESTBED_NAMESPACE_BEGIN

TestbedAddress::TestbedAddress(u_char* address)
    std::copy(this->address, this->address+6, address);
}

std::string TestbedAddress::toString() const {
    std::ostringstream result;
    /// set output to hex values
    result << std::hex;
    /// insert colons between hex values
    std::copy(this->address, this->address+6, std::ostream_iterator<short>(result, ":"));
    /// since we don't want to write an infix operator and git rid of the last colon we return a substring
    return result.str().substr(0,11);
}

bool TestbedAddress::equals(const Address* otherAddress) const{
    const TestbedAddress* otherTestbedAddress = dynamic_cast<const TestbedAddress*>(otherAddress);

    if (otherTestbedAddress == nullptr) {
        return false;
    }

    std::string address = this->toString();
    return (address.compare(otherAddress->toString()) == 0)
}

bool TestbedAddress::equals(const std::shared_ptr<Address> otherAddress) const {
    return this->equals(otherAddress.get());
}

size_t TestbedAddress::getHashValue() const {
    int leastSignificantByte = address[5];
    int nextLeastSignificantByte = address[4];
    return leastSignificantByte * 256 + nextLeastSignificantByte;
}

u_int8_t* TestbedAddress::getDessertValue() const {
    //TODO: Find a better way to return acceptable value for cross platform
    return (u_int8_t*) this->address;
}

TESTBED_NAMESPACE_END
