#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm> // For std::copy

class SessionIDGenerator {
private:
    const uint32_t MAX_SESSION_IDS = 4294967296; // 2^32, maximum number of 32-bit session IDs

    uint32_t* bitVector;
    uint32_t bitVectorSize;

public:
    SessionIDGenerator() : bitVector(nullptr), bitVectorSize(1) {
        bitVector = new uint32_t[bitVectorSize]();
    }

    ~SessionIDGenerator() {
        delete[] bitVector;
    }

    uint32_t getUniqueSessionID() {
        for (uint32_t i = 0; i < bitVectorSize; ++i) {
            if (bitVector[i] != 0xFFFFFFFF) { // Check if there's an available ID in this 32-bit integer
                uint32_t bitPos = findFirstUnsetBit(bitVector[i]);
                uint32_t sessionID = i * 32 + bitPos; // Calculate the unique session ID
                bitVector[i] |= (1 << bitPos); // Set the bit to mark the ID as in use
                return sessionID;
            }
        }
        // If no available ID is found, expand the bit vector
        expandBitVector();
        bitVector[bitVectorSize - 1] |= 1; // Set the first bit of the newly expanded part
        return (bitVectorSize - 1) * 32;
    }

    void releaseSessionID(uint32_t sessionID) {
        if (sessionID >= MAX_SESSION_IDS) {
            return; // Invalid session ID, ignore
        }

        uint32_t bitIndex = sessionID / 32;
        uint32_t bitPos = sessionID % 32;
        if (bitIndex < bitVectorSize) {
            bitVector[bitIndex] &= ~(1 << bitPos); // Clear the bit to mark the ID as available
        }
    }

private:
    uint32_t findFirstUnsetBit(uint32_t num) {
        return __builtin_ctz(~num); // Count trailing zeros using a built-in function (only works on GCC/Clang)
    }

    void expandBitVector() {
        uint32_t newBitVectorSize = bitVectorSize * 2;
        uint32_t* newBitVector = new uint32_t[newBitVectorSize]();
        std::copy(bitVector, bitVector + bitVectorSize, newBitVector);
        delete[] bitVector;
        bitVector = newBitVector;
        bitVectorSize = newBitVectorSize;
    }
};

int main() {
    SessionIDGenerator sessionIDGenerator;

    // Test case: Generate and print 10 unique session IDs
    std::cout << "Generated session IDs: ";
    for (int i = 0; i < 10; ++i) {
        uint32_t sessionID = sessionIDGenerator.getUniqueSessionID();
        std::cout << sessionID << " ";
    }
    std::cout << std::endl;

    // Test case: Release a session ID and generate a new one
    uint32_t sessionToRelease = 5;
    sessionIDGenerator.releaseSessionID(sessionToRelease);
    uint32_t newSessionID = sessionIDGenerator.getUniqueSessionID();
    std::cout << "Released session ID " << sessionToRelease << ", new session ID: " << newSessionID << std::endl;

    // Test case: Generate more session IDs after releasing
    std::cout << "Generated session IDs after releasing: ";
    for (int i = 0; i < 5; ++i) {
        uint32_t sessionID = sessionIDGenerator.getUniqueSessionID();
        std::cout << sessionID << " ";
    }
    std::cout << std::endl;

    return 0;
}
