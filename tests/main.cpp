#include "acsetup.hpp"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int
main(int, char *[]) {
	
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry("core").makeTest());
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry("server").makeTest());
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry("template").makeTest());
	return runner.run("", false) ? EXIT_SUCCESS : EXIT_FAILURE;
}
