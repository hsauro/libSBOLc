#include <stdio.h>
#include "CuTest.h"
#include "utilities.h"
#include "dnasequence.h"

#define NUM_FAST_TESTS 10000

void TestCreateEmptyDNASequence(CuTest* tc) {
	DNASequence* seq = createDNASequence("");
	setNucleotides(seq, "");
	CuAssertStrEquals(tc, "", getDNASequenceID(seq));
	CuAssertStrEquals(tc, "", getNucleotides(seq));
}

void TestCreateNullDNASequence(CuTest* tc) {
	DNASequence* seq = createDNASequence(NULL);
	CuAssertStrEquals(tc, NULL, getDNASequenceID(seq));
	CuAssertPtrEquals(tc, NULL, getNucleotides(seq));
}

void TestCreateRandomDNASequence(CuTest* tc) {
	char* id;
	char* nt;
	DNASequence* seq;
	int repeat;
	for (repeat=0; repeat<NUM_FAST_TESTS; repeat++) {
		id = randomString();
		nt = randomString();
		// avoid duplicates
		while (strcmp(id, nt) == 0)
			nt = randomString();
		seq = createDNASequence(id);
		setNucleotides(seq, nt);
		CuAssertStrEquals(tc, id, getDNASequenceID(seq));
		CuAssertStrEquals(tc, nt, getNucleotides(seq));
		deleteDNASequence(seq);
	}
}

void PrintDNASequenceTestInfo() {
	printf("testing core/dnasequence\n");
}

CuSuite* DNASequenceGetSuite() {
	CuSuite* dnaSequenceTests = CuSuiteNew();
	SUITE_ADD_TEST(dnaSequenceTests, PrintDNASequenceTestInfo);
	SUITE_ADD_TEST(dnaSequenceTests, TestCreateEmptyDNASequence);
	SUITE_ADD_TEST(dnaSequenceTests, TestCreateNullDNASequence);
	SUITE_ADD_TEST(dnaSequenceTests, TestCreateRandomDNASequence);
	return dnaSequenceTests;
}
