/* This file attempts to test the retired branches instruction	*/
/* performance counter PAPI_BR_INS				*/

/* by Vince Weaver, <vincent.weaver@maine.edu>			*/

/* This test seems to work on:		*/
/* + x86				*/
/* + ARMv6 (Raspberry Pi)		*/
/* It is known to not work on:		*/
/* + ARMv7 (Pi2 CortexA7 Panda CortexA9 */
/*   failure is odd, 1/3 missing but not consistent	*/
/*   something weird with event 0xC PC_WRITE ?		*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "papi.h"
#include "papi_test.h"

#include "display_error.h"
#include "testcode.h"


int main(int argc, char **argv) {

	int num_runs=100,i;
	long long high=0,low=0,average=0,expected=1500000;
	double error;

	long long count,total=0;
	int quiet=0,retval,ins_result;
	int eventset=PAPI_NULL;

	quiet=tests_quiet(argc,argv);

	if (!quiet) {
		printf("\nTesting the PAPI_BR_INS event.\n");
	}

	/* Init the PAPI library */
	retval = PAPI_library_init( PAPI_VER_CURRENT );
	if ( retval != PAPI_VER_CURRENT ) {
		test_fail( __FILE__, __LINE__, "PAPI_library_init", retval );
	}

	retval=PAPI_create_eventset(&eventset);
	if (retval!=PAPI_OK) {
		test_fail( __FILE__, __LINE__, "PAPI_create_eventset", retval );
	}

	retval=PAPI_add_named_event(eventset,"PAPI_BR_INS");
	if (retval!=PAPI_OK) {
		test_skip( __FILE__, __LINE__, "adding PAPI_BR_INS", retval );
	}

	if (!quiet) {
		printf("Testing a loop with %lld branches (%d times):\n",
			expected,num_runs);
	}

	for(i=0;i<num_runs;i++) {
		PAPI_reset(eventset);
		PAPI_start(eventset);

		ins_result=branches_testcode();

		retval=PAPI_stop(eventset,&count);

		if (ins_result==CODE_UNIMPLEMENTED) {
			fprintf(stderr,"\tCode unimplemented\n");
			test_skip( __FILE__, __LINE__, "unimplemented", 0);
		}

		if (retval!=PAPI_OK) {
			test_fail( __FILE__, __LINE__,
				"reading PAPI_TOT_INS", retval );
		}

		if (count>high) high=count;
		if ((low==0) || (count<low)) low=count;
		total+=count;
	}

	average=(total/num_runs);

	error=display_error(average,high,low,expected,quiet);

	if ((error > 1.0) || (error<-1.0)) {
		if (!quiet) printf("Instruction count off by more than 1%%\n");
		test_fail( __FILE__, __LINE__, "Error too high", 1 );
	}

	if (!quiet) printf("\n");

	test_pass( __FILE__ );

	PAPI_shutdown();

	return 0;
}
