#include "Model.h"

using namespace std;


void Model::getDouble(int vr, double value[], int* index) {
    error("Unknown value reference for type double: " + to_string(vr));
}

void Model::getFloat(int vr, float value[], int* index) {
    error("Unknown value reference for type float: " + to_string(vr));
}

void Model::getInt8(int vr, int8_t value[], int* index) {
    error("Unknown value reference for type int8: " + to_string(vr));
}

void Model::getUInt8(int vr, uint8_t value[], int* index) {
    error("Unknown value reference for type uint8: " + to_string(vr));
}

void Model::getInt16(int vr, int16_t value[], int* index) {
    error("Unknown value reference for type int16: " + to_string(vr));
}

void Model::getUInt16(int vr, uint16_t value[], int* index) {
    error("Unknown value reference for type uint16: " + to_string(vr));
}

void Model::getInt32(int vr, int32_t value[], int* index) {
    error("Unknown value reference for type int32: " + to_string(vr));
}

void Model::getUInt32(int vr, uint32_t value[], int* index) {
    error("Unknown value reference for type uint32: " + to_string(vr));
}

void Model::getInt64(int vr, int64_t value[], int* index) {
    error("Unknown value reference for type int64: " + to_string(vr));
}

void Model::getUInt64(int vr, uint64_t value[], int* index) {
    error("Unknown value reference for type uint64: " + to_string(vr));
}

void Model::getBoolean(int vr, boolean value[], int* index) {
    error("Unknown value reference for type bool: " + to_string(vr));
}

void Model::getString(int vr, const char* value[], int* index) {
    error("Unknown value reference for type string: " + to_string(vr));
}

void Model::setDouble(int vr, const double value[], int* index) {
    error("Unknown value reference for type double: " + to_string(vr));
}

void Model::setInt32(int vr, const int32_t value[], int* index) {
    error("Unknown value reference for type int32: " + to_string(vr));
}

void Model::setUInt64(int vr, uint64_t value) {
    error("Unknown value reference for type uint64: " + to_string(vr));
}

void Model::setBoolean(int vr, const boolean value[], int* index) {
    error("Unknown value reference for type bool: " + to_string(vr));
}

void Model::setString(int vr, const string& value) {
    error("Unknown value reference for type string: " + to_string(vr));
}

void* Model::getFMUState(){
    error("Not implemented");
}

void Model::setFMUState(void *fmuState) { 
    error("Not implemented");
}

void Model::freeFMUState(void *fmuState) { 
    error("Not implemented");
}

size_t Model::getSerializedFMUStateSize(void *fmuState) {
    error("Not implemented");
}


void Model::deserializeFMUState(const char *memory, size_t size, void** fmuState) {
    error("Not implemented");
}


void Model::serializeFMUState(void *fmuState, char *memory, size_t size) { 
    error("Not implemented");
}
