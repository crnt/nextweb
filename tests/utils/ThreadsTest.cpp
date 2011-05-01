#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockThread.hpp"
#include "nextweb/utils/Threads.hpp"
#include "nextweb/utils/ThreadQueue.hpp"

namespace nextweb { namespace tests { 

class ThreadsTest : public CppUnit::TestFixture {

public:
	void testQueue();

private:
	CPPUNIT_TEST_SUITE(ThreadsTest);
	CPPUNIT_TEST(testQueue);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ThreadsTest, "ThreadsTest");
CPPUNIT_REGISTRY_ADD("ThreadsTest", "plain");

void
ThreadsTest::testQueue() {
	
	using namespace utils;
	
	UnboundedThreadQueue<size_t> queue;
	SharedPtr<Thread> thread(new MockThread(queue, 15));
	thread->start();

	std::size_t i = 0, item;
	for (; i < 15 && queue.pop(item); ++i) {
		CPPUNIT_ASSERT_EQUAL(i, item);
	}
	queue.stop();
	thread->join();
	CPPUNIT_ASSERT_EQUAL(i, static_cast<std::size_t>(15));
}

}} // namespaces
