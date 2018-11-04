#include "Model.h"

using namespace std;


vector<double> Model::getDouble(int vr) {
    error("Unknown value reference for type double: " + to_string(vr));
}

vector<float> Model::getFloat(int vr) {
    error("Unknown value reference for type float: " + to_string(vr));
}

vector<int8_t> Model::getInt8(int vr) {
    error("Unknown value reference for type int8: " + to_string(vr));
}

vector<uint8_t> Model::getUInt8(int vr) {
    error("Unknown value reference for type uint8: " + to_string(vr));
}

vector<int16_t> Model::getInt16(int vr) {
    error("Unknown value reference for type int16: " + to_string(vr));
}

vector<uint16_t> Model::getUInt16(int vr) {
    error("Unknown value reference for type uint16: " + to_string(vr));
}

vector<int32_t> Model::getInt32(int vr) {
    error("Unknown value reference for type int32: " + to_string(vr));
}

vector<uint32_t> Model::getUInt32(int vr) {
    error("Unknown value reference for type uint32: " + to_string(vr));
}

vector<int64_t> Model::getInt64(int vr) {
    error("Unknown value reference for type int64: " + to_string(vr));
}

vector<uint64_t> Model::getUInt64(int vr) {
    error("Unknown value reference for type uint64: " + to_string(vr));
}

vector<bool> Model::getBool(int vr) {
    error("Unknown value reference for type bool: " + to_string(vr));
}

vector<const string*> Model::getString(int vr) {
    error("Unknown value reference for type string: " + to_string(vr));
}

void Model::setDouble(int vr,  const double* value, int* index) {
    error("Unknown value reference for type double: " + to_string(vr));
}

void Model::setInt32(int vr, int32_t value) {
    error("Unknown value reference for type int32: " + to_string(vr));
}

void Model::setUInt64(int vr, uint64_t value) {
    error("Unknown value reference for type uint64: " + to_string(vr));
}

void Model::setBool(int vr, bool value) {
    error("Unknown value reference for type bool: " + to_string(vr));
}

void Model::setString(int vr, const string& value) {
    error("Unknown value reference for type string: " + to_string(vr));
}
