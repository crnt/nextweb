#include "acsetup.hpp"

#include <cstdlib>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int
main(int, char *[]) {
	
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
	return runner.run("", false) ? EXIT_SUCCESS : EXIT_FAILURE;
}
