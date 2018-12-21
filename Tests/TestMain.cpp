#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int _argc, char** _argv)
{
 	return Catch::Session().run(_argc, _argv);
}