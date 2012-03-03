#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "property.h"
#include "genericarray.h"
#include "collection.h"
#include "dnacomponent.h"

static GenericArray* allCollections;

/**************************
	create/destroy
***************************/

void registerCollection(Collection* col) {
	if (!allCollections)
		allCollections = createGenericArray();
	insertIntoGenericArray(allCollections, col);
}

Collection* createCollection(const char* id) {
	Collection* col;
	col = (Collection*)malloc(sizeof(Collection));
	col->id          = createProperty();
	col->name        = NULL;
	col->description = NULL;
	col->components  = createGenericArray();
	col->collections = createGenericArray();
	setCollectionID(col, id);
	registerCollection(col);
	return col;
}

void removeCollection(Collection* col) {
	if (!allCollections)
		allCollections = createGenericArray();
	int index = indexByPtr(allCollections, col);
	if (index >= 0)
		removeFromGenericArray(allCollections, index);
}

void deleteCollection(Collection* col) {
	if (col) {
		removeCollection(col);
		if (col->id) {
			deleteProperty(col->id);
			col->id = NULL;
		}
		if (col->name) {
			free(col->name);
			col->name = NULL;
		}
		if (col->description) {
			free(col->description);
			col->description = NULL;
		}
		if (col->collections) {
			deleteGenericArray(col->collections);
			col->collections = NULL;
		}
		if (col->components) {
			deleteGenericArray(col->components);
			col->components = NULL;
		}
		free(col);
	}
}

/**************************
 *	is... functions
 **************************/

int isCollectionPtr(const void* pointer) {
	if (!allCollections)
		allCollections = createGenericArray();
	return (int) indexByPtr(allCollections, pointer) >= 0;
}

int isCollectionID(const char* id) {
	if (!allCollections)
		allCollections = createGenericArray();
	if (!id)
		return 0;
	int index;
	Collection* col;
	for (index=0; index<allCollections->numInUse; index++) {
		col = (Collection*) allCollections->array[index];
		if (compareProperty(col->id, id) == 0)
			return 1;
	}
	return 0;
}

/**************************
	get... functions
***************************/

Collection* getCollection(const char* id) {
	if (!allCollections)
		allCollections = createGenericArray();
	if (!id)
		return NULL;
	int index;
	Collection* col;
	for (index=0; index<allCollections->numInUse; index++) {
		col = (Collection*) allCollections->array[index];
		if (compareProperty(col->id, id) == 0)
			return col;
	}
	return NULL;
}

char* getCollectionID(const Collection* col) {
	if (col && col->id) {
		return getProperty(col->id);
	} else
		return NULL;
}

char* getCollectionName(const Collection* col) {
	if (col && col->name) {
		char* name = (char*)malloc(sizeof(char) * strlen(col->name)+1);
		strcpy(name, col->name);
		return name;
	} else
		return NULL;
}

char* getCollectionDescription(const Collection* col) {
	if (col && col->description) {
		char* descr = (char*)malloc(sizeof(char) * strlen(col->description)+1);
		strcpy(descr, col->description);
		return descr;
	} else
		return NULL;
}

/**************************
 *	getNum... functions
 **************************/

int getNumCollections() {
	if (allCollections)
		return allCollections->numInUse;
	else
		return 0;
}

int getNumDNAComponentsIn(const Collection* col) {
	if (col)
		return col->components->numInUse;
	else
		return -1;
}

int getNumCollectionsIn(const Collection* col) {
	if (col)
		return col->collections->numInUse;
	else
		return -1;
}

/**************************
 *	getNth... functions
 **************************/

Collection* getNthCollection(int n) {
	if (!allCollections || allCollections->numInUse<=n)
	    return NULL;
	else
		return allCollections->array[n];
}

DNAComponent* getNthDNAComponentIn(const Collection* col, int n) {
	if (col && col->components->numInUse >= n)
		return col->components->array[n];
	else
		return NULL;
}

Collection* getNthCollectionIn(const Collection* col, int n) {
	if (col && col->collections->numInUse >= n)
		return col->components->array[n];
	else
		return NULL;
}

/**************************
	set... functions
***************************/

void setCollectionID(Collection* col, const char* id) {
    setProperty(col->id, id);
}

void setCollectionName(Collection* col, const char* name) {
	if (col && name) {
		col->name = (char*)malloc(sizeof(char) * strlen(name)+1);
		strcpy(col->name, name);
	}
}

void setCollectionDescription(Collection* col, const char* descr) {
	if (col && descr) {
		col->description = (char*)malloc(sizeof(char) * strlen(descr)+1);
		strcpy(col->description, descr);
	}
}

/**************************
	add component
***************************/

void addDNAComponentToCollection(DNAComponent* com, Collection* col) {
	if (com && col) {
		insertIntoGenericArray(com->collections, col);
		insertIntoGenericArray(col->components,  com);
	}
}

void cleanupCollections() {
	if (allCollections) {
		int index;
		for (index=allCollections->numInUse; index>0; index--)
			deleteCollection( allCollections->array[index] );
		deleteGenericArray(allCollections);
		allCollections = NULL;
	}
}

void printCollection(const Collection* col, int tabs) {
    if (!col)
        return;
    indent(tabs);   printProperty(col->id);
    indent(tabs+1); printf("name: %s\n", col->name);
    indent(tabs+1); printf("description: %s\n", col->description);
    int i;
    int num;
    if (col->components) {
        DNAComponent* com;
        num = getNumDNAComponentsIn(col);
        if (num > 0) {
            indent(tabs+1); printf("%i components:\n", num);
            for (i=0; i<num; i++) {
                com = getNthDNAComponentIn(col, i);
                indent(tabs+2); printf("%s\n", com->id);
            }
        }
    }
    if (col->collections) {
        Collection* col2;
        num = getNumCollectionsIn(col);
        if (num > 0) {
            indent(tabs+1); printf("%i collections:\n", num);
            for (i=0; i<num; i++) {
                col2 = getNthCollectionIn(col, i);
                indent(tabs+2); printf("%s\n", col2->id);
            }
        }
    }
}

void printAllCollections() {
    int n;
    int num = getNumCollections();
    if (num > 0) {
        printf("%i collections:\n", num);
        for (n=0; n<num; n++)
            printCollection(getNthCollection(n), 1);
    }
}
