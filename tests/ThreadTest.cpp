#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "details/Queue.hpp"
#include "details/Mutex.hpp"
#include "details/Thread.hpp"

namespace nextweb { namespace tests {

class QueueThread : public details::Thread {

public:
	virtual void run();
	static SharedPtr<details::Thread> create(details::Queue<int>::PtrType const &queue);

protected:
	QueueThread(details::Queue<int>::PtrType const &queue);
	virtual ~QueueThread();

private:
	details::Queue<int>::PtrType queue_;
};

class ThreadTest : public CppUnit::TestFixture {

public:
	void testQueue();
	void testThreadGroup();

private:
	CPPUNIT_TEST_SUITE(ThreadTest);
	CPPUNIT_TEST(testQueue);
	CPPUNIT_TEST(testThreadGroup);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ThreadTest, "ThreadTest");
CPPUNIT_REGISTRY_ADD("ThreadTest", "core");

QueueThread::QueueThread(details::Queue<int>::PtrType const &queue) :
	queue_(queue)
{
}

QueueThread::~QueueThread() {
}

void
QueueThread::run() {
	for (int i = 0; i < 10; ++i) {
		queue_->push(i);
	}
	queue_->finish();
}

SharedPtr<details::Thread>
QueueThread::create(details::Queue<int>::PtrType const &queue) {
	return SharedPtr<details::Thread>(new QueueThread(queue));
}

void
ThreadTest::testQueue() {
	
	using namespace nextweb::details;
	
	Queue<int>::PtrType queue(new Queue<int>());
	SharedPtr<Thread> thread = QueueThread::create(queue);
	thread->start();

	int item;
	for (int count = 0; queue->pop(item); ++count) {
		CPPUNIT_ASSERT_EQUAL(count, item);
	}
	thread->join();
}

void
ThreadTest::testThreadGroup() {
	
}

}} // namespaces
