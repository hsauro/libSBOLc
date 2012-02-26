#include <stdlib.h>
#include <string.h>
//#include "api.h"
#include "collection.h"
#include "component.h"

/**************************
	create/destroy
***************************/

Collection* createCollection(const char* id) {
	Collection* col;
	col = (Collection*)malloc(sizeof(Collection));
	col->id          = NULL;
	col->name        = NULL;
	col->description = NULL;
	col->numComponents  = 0;
	col->numCollections = 0;
	col->components  = NULL;
	col->collections = NULL;
	setCollectionID(col, id);
	return col;
}

void deleteCollection(Collection* p)
{
	if (p) {
		if (p->id) free( p->id );
		if (p->name) free( p->name );
		if (p->description) free( p->description );
		if (p->collections) free( p->collections );
		if (p->components) free( p->components );
		if (p->numComponents) free( &p->numComponents );
		if (p->numCollections) free( &p->numCollections );
		free(p);
	}
}

/**************************
	get... functions
***************************/

int getNumDNAComponentsIn(Collection* collection)
{
	return collection->numComponents;
}

struct _DNAComponent getNthDNAComponentIn(Collection* col, int n) {
	struct _DNAComponent p = {0,0,0,0,0,0,0};
	if (col->components)
		p = *(col->components[n]);
	return p;
}

/**************************
	set... functions
***************************/

void setCollectionID(Collection* col, const char* id) {
	if (col && id) {
		col->id = (char*)malloc(sizeof(char) * strlen(id)+1);
		strcpy(col->id, id);
	}
}

void setCollectionName(Collection* col, const char* name)
{
	if (col && name) {
		col->name = (char*)malloc(sizeof(char) * strlen(name)+1);
		strcpy(col->name, name);
	}
}

void setCollectionDescription(Collection* col, const char* descr)
{
	if (col && descr) {
		col->description = (char*)malloc(sizeof(char) * strlen(descr)+1);
		strcpy(col->description, descr);
	}
}

/**************************
	add component
***************************/

void addComponentToCollection(DNAComponent * component, Collection * collection)
{
	Collection   ** p1 = 0;
	DNAComponent ** p2 = 0;
	int i = 0;
	int n = 0;
	if (component && collection)
	{
		p1 = component->collections;
		
		// create a slightly longer array and copy things over,
		// then add the new collection
		if (p1)
		{
			//i = 0;
			//while (p1[i])
			//	++i;
			//n = i; // current number of collections
            n = component->numCollections;

			component->collections = (Collection**)malloc((n+1) * sizeof(Collection*));
			for (i=0; i < n-1; ++i) //TODO remove multiple nulls?
				component->collections[i] = p1[i];
			component->collections[n-1] = collection;
			component->collections[n] = 0;
			free(p1);
		}

		// create an array and add the new collection
		else
		{
			component->collections = (Collection**)malloc(2 * sizeof(Collection*));
			component->collections[0] = collection;
			component->collections[1] = 0;
		}
		component->numCollections++;

		//while (component->collections[i])
		//	i++;

		p2 = collection->components;

		// create a new array and copy things over,
		// then add the new component to it
		if (p2)
		{
			//i = 0;
			//while (p2[i])
			//	++i;
			//n = i; // current number of components
			n = collection->numComponents;

			collection->components = (DNAComponent**)malloc((n+1) * sizeof(DNAComponent*));
			for (i=0; i < n-1; ++i)
				collection->components[i] = p2[i];
			collection->components[n-1] = component;
			collection->components[n] = 0;
			free(p2);
		}

		// start an array of components and add the new one
		else
		{
			collection->components = (DNAComponent**)malloc(2 * sizeof(DNAComponent*));
			collection->components[0] = component;
			collection->components[1] = 0;
		}
		collection->numComponents++;

		//while (collection->components[i])
		//	i++;
	}
}

