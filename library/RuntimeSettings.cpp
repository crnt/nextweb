#include "acsetup.hpp"

#include "nextweb/RuntimeSettings.hpp"

namespace nextweb {

RuntimeSettings::RuntimeSettings()
{
}

RuntimeSettings::~RuntimeSettings() {
}

off_t
RuntimeSettings::getPostSizeThreshold() const {
	return static_cast<off_t>(1024 * 1024);
}

ThreadPerConnectSettings::ThreadPerConnectSettings()
{
}

ThreadPerConnectSettings::~ThreadPerConnectSettings() {
}

RuntimeSettings::ThreadingModel
ThreadPerConnectSettings::getThreadingModel() const {
	return RuntimeSettings::THREAD_PER_CONNECT;
}

ThreadPoolSettings::ThreadPoolSettings()
{
}

ThreadPoolSettings::~ThreadPoolSettings() {
}

RuntimeSettings::ThreadingModel
ThreadPoolSettings::getThreadingModel() const {
	return RuntimeSettings::THREAD_POOL;
}

} // namespaces
