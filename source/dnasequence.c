#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "property.h"
#include "array.h"
#include "object.h"
#include "document.h"
#include "dnasequence.h"

// TODO constrain to actg and sometimes n?
DNASequence* createDNASequence(Document* doc, const char* uri) {
	if (!doc || !uri || isSBOLObjectURI(doc, uri))
	    return NULL;
	DNASequence* seq = malloc(sizeof(DNASequence));
	seq->doc         = doc;
	seq->base        = createSBOLObject(doc, uri);
	seq->nucleotides = createNucleotidesProperty();
	registerDNASequence(seq);
	return seq;
}

static void setDNASequenceURI(DNASequence* seq, const char* uri) {
	if (seq)
		setSBOLObjectURI(seq->base, uri);
}

void setDNASequenceNucleotides(DNASequence* seq, const char* nucleotides) {
	if (seq)
		setNucleotidesProperty(seq->nucleotides, nucleotides);
}

void deleteDNASequence(DNASequence* seq) {
	if (seq) {
		if (seq->base) {
			deleteSBOLObject(seq->base);
			seq->base = NULL;
		}
		if (seq->nucleotides) {
			deleteNucleotidesProperty(seq->nucleotides);
			seq->nucleotides = NULL;
		}
		if (seq->doc) {
			removeDNASequence(seq->doc, seq);
			seq->doc = NULL;
		}
		free(seq);
		seq = NULL;
	}
}

char* getDNASequenceURI(const DNASequence* seq) {
	if (seq)
		return getSBOLObjectURI(seq->base);
	else
		return NULL;
}

char* getDNASequenceNucleotides(const DNASequence* seq) {
	if (seq)
	    return getNucleotidesProperty(seq->nucleotides);
}

void printDNASequence(const DNASequence* seq, int tabs) {
	if (!seq)
	    return;
	indent(tabs); printf("%s\n", getDNASequenceURI(seq));
	
	// print nucleotides, truncating them if there's a lot
	indent(tabs+1);
	printf("nucleotides: ");
    char* nt = getDNASequenceNucleotides(seq);
	if (nt && strlen(nt) > atoi(NUCLEOTIDES_TO_PRINT))
		printf("%." NUCLEOTIDES_TO_PRINT "s", nt);
	else
		printf("%s", nt);
	printf("\n");
}

