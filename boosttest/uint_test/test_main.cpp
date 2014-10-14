// 嵌入源码--动态链接--静态链接
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
//int BOOST_TEST_CALL_DECL
//	main( int argc, char* argv[] )
//{
//	// prototype for user's unit test init function
//#ifdef BOOST_TEST_ALTERNATIVE_INIT_API
//	extern bool init_unit_test();
//
//	boost::unit_test::init_unit_test_func init_func = &init_unit_test;
//#else
//	extern ::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] );
//
//	boost::unit_test::init_unit_test_func init_func = &init_unit_test_suite;
//#endif
//
//	return ::boost::unit_test::unit_test_main( init_func, argc, argv );
//}