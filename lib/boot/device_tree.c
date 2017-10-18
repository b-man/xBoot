/*
 * Copyright (c) 2005 Apple Computer, Inc.  All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <boot/device_tree.h>

/*
-------------------------------------------------------------------------------
 Foundation Types
-------------------------------------------------------------------------------
*/
enum {
	kDTPathNameSeparator	= '/'				/* 0x2F */
};


/* Property Name Definitions (Property Names are C-Strings)*/
enum {
	kDTMaxPropertyNameLength=31	/* Max length of Property Name (terminator not included) */
};

typedef char DTPropertyNameBuf[32];


/* Entry Name Definitions (Entry Names are C-Strings)*/
enum {
	kDTMaxEntryNameLength		= 63	/* Max length of a C-String Entry Name (terminator not included) */
};

/* length of DTEntryNameBuf = kDTMaxEntryNameLength +1*/


/* Entry*/
typedef struct OpaqueDTEntry* DTEntry;

/* Entry Iterator*/
typedef struct OpaqueDTEntryIterator* DTEntryIterator;

/* Property Iterator*/
typedef struct OpaqueDTPropertyIterator* DTPropertyIterator;


/* status values*/
enum {
		kError = -1,
		kIterationDone = 0,
		kSuccess = 1
};

#define kfree(x, y) free(x)
#define kalloc(x) malloc(x)

/*

Structures for a Flattened Device Tree
 */

#define kPropNameLength	32

typedef struct OpaqueDTEntry {
    uint32_t		nProperties;	// Number of props[] elements (0 => end)
    uint32_t		nChildren;	// Number of children[] elements
//  DeviceTreeNodeProperty	props[];// array size == nProperties
//  DeviceTreeNode	children[];	// array size == nChildren
} DeviceTreeNode;

#if 1
/*
 
 Structures for a Flattened Device Tree 
 */

#define kPropNameLength 32

/* Entry Name Definitions (Entry Names are C-Strings)*/

typedef struct DeviceTreeNodeProperty {
    char name[kPropNameLength];
    unsigned long length;
} DeviceTreeNodeProperty;

/* length of DTEntryNameBuf = kDTMaxEntryNameLength +1*/
typedef char DTEntryNameBuf[32];
#endif

#if 0
#define DPRINTF(args...) printf(args)
#else
#define DPRINTF(args...)
#endif

#define RoundToLong(x)	(((x) + 3) & ~3)

static struct _DTSizeInfo {
    uint32_t numNodes;
    uint32_t numProperties;
    uint32_t totalPropertySize;
} DTInfo;

#define kAllocSize 4096

static Node *rootNode;

static Node *freeNodes, *allocedNodes;
static Property *freeProperties, *allocedProperties;

Property *DT__AddProperty(Node * node, const char *name, uint32_t length,
                          void *value)
{
    Property *prop;

    DPRINTF("DT__AddProperty([Node '%s'], '%s', %d, 0x%x)\n",
            DT__GetName(node), name, length, value);
    if (freeProperties == NULL) {
        void *buf = malloc(kAllocSize);
        int i;

        DPRINTF("Allocating more free properties\n");
        if (buf == 0)
            return 0;
        bzero(buf, kAllocSize);
        // Use the first property to record the allocated buffer
        // for later freeing.
        prop = (Property *) buf;
        prop->next = allocedProperties;
        allocedProperties = prop;
        prop->value = buf;
        prop++;
        for (i = 1; i < (kAllocSize / sizeof(Property)); i++) {
            prop->next = freeProperties;
            freeProperties = prop;
            prop++;
        }
    }
    prop = freeProperties;
    freeProperties = prop->next;

    prop->name = name;
    prop->length = length;
    prop->value = value;

    // Always add to end of list
    if (node->properties == 0) {
        node->properties = prop;
    } else {
        node->last_prop->next = prop;
    }
    node->last_prop = prop;
    prop->next = 0;

    DPRINTF("Done [0x%x]\n", prop);

    DTInfo.numProperties++;
    DTInfo.totalPropertySize += RoundToLong(length);

    return prop;
}

Node *DT__AddChild(Node * parent, const char *name)
{
    Node *node;

    if (freeNodes == NULL) {
        void *buf = malloc(kAllocSize);
        int i;

        DPRINTF("Allocating more free nodes\n");
        if (buf == 0)
            return 0;
        bzero(buf, kAllocSize);
        node = (Node *) buf;
        // Use the first node to record the allocated buffer
        // for later freeing.
        node->next = allocedNodes;
        allocedNodes = node;
        node->children = (Node *) buf;
        node++;
        for (i = 1; i < (kAllocSize / sizeof(Node)); i++) {
            node->next = freeNodes;
            freeNodes = node;
            node++;
        }
    }
    DPRINTF("DT__AddChild(0x%x, '%s')\n", parent, name);
    node = freeNodes;
    freeNodes = node->next;
    DPRINTF("Got free node 0x%x\n", node);
    DPRINTF("prop = 0x%x, children = 0x%x, next = 0x%x\n", node->properties,
            node->children, node->next);

    if (parent == NULL) {
        rootNode = node;
        node->next = 0;
    } else {
        node->next = parent->children;
        parent->children = node;
    }
    DTInfo.numNodes++;
    if(name)
        DT__AddProperty(node, "name", strlen(name) + 1, (void *)name);
    return node;
}

void DT__FreeProperty(Property * prop)
{
    prop->next = freeProperties;
    freeProperties = prop;
}

void DT__FreeNode(Node * node)
{
    node->next = freeNodes;
    freeNodes = node;
}

Node *DT__RootNode(void)
{
    return rootNode;
}

void DT__Initialize(void)
{
    DPRINTF("DT__Initialize\n");

    freeNodes = 0;
    allocedNodes = 0;
    freeProperties = 0;
    allocedProperties = 0;

    DTInfo.numNodes = 0;
    DTInfo.numProperties = 0;
    DTInfo.totalPropertySize = 0;

    rootNode = DT__AddChild(NULL, "device-tree");
    DPRINTF("DT__Initialize done\n");
}

/*
 * Free up memory used by in-memory representation
 * of device tree.
 */
void DT__Finalize(void)
{
    Node *node;
    Property *prop;

    DPRINTF("DT__Finalize\n");
    for (prop = allocedProperties; prop != NULL; prop = prop->next) {
        free(prop->value);
        free((char*)prop->name);
    }
    allocedProperties = NULL;
    freeProperties = NULL;

    for (node = allocedNodes; node != NULL; node = node->next) {
        free((void *)node->children);
    }
    allocedNodes = NULL;
    freeNodes = NULL;
    rootNode = NULL;

    // XXX leaks any created strings

    DTInfo.numNodes = 0;
    DTInfo.numProperties = 0;
    DTInfo.totalPropertySize = 0;
}

static void *FlattenNodes(Node * node, void *buffer)
{
    Property *prop;
    DeviceTreeNode *flatNode;
    DeviceTreeNodeProperty *flatProp;
    int count;

    if (node == 0)
        return buffer;

    flatNode = (DeviceTreeNode *) buffer;
    buffer += sizeof(DeviceTreeNode);

    for (count = 0, prop = node->properties; prop != 0;
         count++, prop = prop->next) {
        flatProp = (DeviceTreeNodeProperty *) buffer;
        strcpy(flatProp->name, prop->name);
        flatProp->length = prop->length;
        buffer += sizeof(DeviceTreeNodeProperty);
        bcopy(prop->value, buffer, prop->length);
        buffer += RoundToLong(prop->length);
    }
    flatNode->nProperties = count;

    for (count = 0, node = node->children; node != 0;
         count++, node = node->next) {
        buffer = FlattenNodes(node, buffer);
    }
    flatNode->nChildren = count;

    return buffer;
}

/*
 * Flatten the in-memory representation of the device tree
 * into a binary DT block.
 * To get the buffer size needed, call with result = 0.
 * To have a buffer allocated for you, call with *result = 0.
 * To use your own buffer, call with *result = &buffer.
 */

void DT__FlattenDeviceTree(void **buffer_p, uint32_t * length)
{
    uint32_t totalSize;
    void *buf;

    DPRINTF("DT__FlattenDeviceTree(0x%x, 0x%x)\n", buffer_p, length);
#if 0
    if (buffer_p)
        DT__PrintTree(rootNode);
#endif

    totalSize = DTInfo.numNodes * sizeof(DeviceTreeNode) +
        DTInfo.numProperties * sizeof(DeviceTreeNodeProperty) +
        DTInfo.totalPropertySize;

    DPRINTF("Total size 0x%x\n", totalSize);
    if (buffer_p != 0) {
        if (totalSize == 0) {
            buf = 0;
        } else {
            if (*buffer_p == 0) {
                buf = malloc(totalSize);
            } else {
                buf = *buffer_p;
            }
            bzero(buf, totalSize);

            FlattenNodes(rootNode, buf);
        }
        *buffer_p = buf;
    }
    if (length)
        *length = totalSize;
}

char *DT__GetName(Node * node)
{
    Property *prop;

    //DPRINTF("DT__GetName(0x%x)\n", node);
    //DPRINTF("Node properties = 0x%x\n", node->properties);
    for (prop = node->properties; prop; prop = prop->next) {
        //DPRINTF("Prop '%s'\n", prop->name);
        if (strcmp(prop->name, "name") == 0) {
            return prop->value;
        }
    }
    //DPRINTF("DT__GetName returns 0\n");
    return "(null)";
}

Node *DT__FindNode(const char *path, bool createIfMissing)
{
    Node *node, *child;
    DTPropertyNameBuf nameBuf;
    char *bp;
    int i;

    DPRINTF("DT__FindNode('%s', %d)\n", path, createIfMissing);

    // Start at root
    node = rootNode;
    DPRINTF("root = 0x%x\n", rootNode);

    while (node) {
        // Skip leading slash
        while (*path == '/')
            path++;

        for (i = 0, bp = nameBuf;
             ++i < kDTMaxEntryNameLength && *path && *path != '/'; bp++, path++)
            *bp = *path;
        *bp = '\0';

        if (nameBuf[0] == '\0') {
            // last path entry
            break;
        }
        DPRINTF("Node '%s'\n", nameBuf);

        for (child = node->children; child != 0; child = child->next) {
            DPRINTF("Child 0x%x\n", child);
            if (strcmp(DT__GetName(child), nameBuf) == 0) {
                break;
            }
        }
        if (child == 0 && createIfMissing) {
            DPRINTF("Creating node\n");
            char *str = malloc(strlen(nameBuf) + 1);
            // XXX this will leak
            strcpy(str, nameBuf);

            child = DT__AddChild(node, str);
        }
        node = child;
    }
    return node;
}

#if 1

void DT__PrintNode(Node * node, int level)
{
    char spaces[10], *cp = spaces;
    Property *prop;

    if (level > 9)
        level = 9;
    while (level--)
        *cp++ = ' ';
    *cp = '\0';

    printf("%s===Node===\n", spaces);
    for (prop = node->properties; prop; prop = prop->next) {
        char c = *((char *)prop->value);
        if (prop->length < 64 && (strcmp(prop->name, "name") == 0 ||
                                  (c >= '0' && c <= '9') ||
                                  (c >= 'a' && c <= 'z') ||
                                  (c >= 'A' && c <= 'Z') || c == '_')) {
            printf("%s Property '%s' [%d] = '%s'\n", spaces,
                   prop->name, prop->length, prop->value);
        } else {
            printf("%s Property '%s' [%d] = (data)\n", spaces,
                   prop->name, prop->length);
        }
    }
    printf("%s==========\n", spaces);
}

static void _PrintTree(Node * node, int level)
{
    DT__PrintNode(node, level);
    level++;
    for (node = node->children; node; node = node->next)
        _PrintTree(node, level);
}

void DT__PrintTree(Node * node)
{
    if (node == 0)
        node = rootNode;
    _PrintTree(node, 0);
}

#endif


#ifndef NULL
#define       NULL    ((void *) 0)
#endif

#define round_long(x)	(((x) + 3UL) & ~(3UL))
#define next_prop(x)	((DeviceTreeNodeProperty *) (((uintptr_t)x) + sizeof(DeviceTreeNodeProperty) + round_long(x->length)))

/* Entry*/
typedef DeviceTreeNode *RealDTEntry;

typedef struct DTSavedScope {
	struct DTSavedScope * nextScope;
	RealDTEntry scope;
	RealDTEntry entry;
	unsigned long index;		
} *DTSavedScopePtr;

/* Entry Iterator*/
typedef struct OpaqueDTEntryIterator {
	RealDTEntry outerScope;
	RealDTEntry currentScope;
	RealDTEntry currentEntry;
	DTSavedScopePtr savedScope;
	unsigned long currentIndex;		
} *RealDTEntryIterator;

/* Property Iterator*/
typedef struct OpaqueDTPropertyIterator {
	RealDTEntry entry;
	DeviceTreeNodeProperty *currentProperty;
	unsigned long currentIndex;
} *RealDTPropertyIterator;

static int DTInitialized;
static RealDTEntry DTRootNode;

/*
 * Support Routines
 */
static RealDTEntry
skipProperties(RealDTEntry entry)
{
	DeviceTreeNodeProperty *prop;
	unsigned int k;

	if (entry == NULL || entry->nProperties == 0) {
		return NULL;
	} else {
		prop = (DeviceTreeNodeProperty *) (entry + 1);
		for (k = 0; k < entry->nProperties; k++) {
			prop = next_prop(prop);
		}
	}
	return ((RealDTEntry) prop);
}

static RealDTEntry
skipTree(RealDTEntry root)
{
	RealDTEntry entry;
	unsigned int k;

	entry = skipProperties(root);
	if (entry == NULL) {
		return NULL;
	}
	for (k = 0; k < root->nChildren; k++) {
		entry = skipTree(entry);
	}
	return entry;
}

static RealDTEntry
GetFirstChild(RealDTEntry parent)
{
	return skipProperties(parent);
}

static RealDTEntry
GetNextChild(RealDTEntry sibling)
{
	return skipTree(sibling);
}

static const char *
GetNextComponent(const char *cp, char *bp)
{
	size_t length = 0;
	char *origbp = bp;

	while (*cp != 0) {
		if (*cp == kDTPathNameSeparator) {
			cp++;
			break;
		}
		if (++length > kDTMaxEntryNameLength) {
			*origbp = '\0';
			return cp;
		}
		*bp++ = *cp++;
	}
	*bp = 0;
	return cp;
}

static RealDTEntry
FindChild(RealDTEntry cur, char *buf)
{
	RealDTEntry	child;
	unsigned long	index;
	char *			str;
	unsigned int	dummy;

	if (cur->nChildren == 0) {
		return NULL;
	}
	index = 1;
	child = GetFirstChild(cur);
	while (1) {
		if (DTGetProperty(child, "name", (void **)&str, &dummy) != kSuccess) {
			break;
		}
		if (strcmp(str, buf) == 0) {
			return child;
		}
		if (index >= cur->nChildren) {
			break;
		}
		child = GetNextChild(child);
		index++;
	}
	return NULL;
}


/*
 * External Routines
 */
void
DTInit(void *base)
{
	DTRootNode = (RealDTEntry) base;
	DTInitialized = (DTRootNode != 0);
}

int
DTEntryIsEqual(const DTEntry ref1, const DTEntry ref2)
{
	/* equality of pointers */
	return (ref1 == ref2);
}

static char *startingP;		// needed for find_entry
int find_entry(const char *propName, const char *propValue, DTEntry *entryH);

int DTFindEntry(const char *propName, const char *propValue, DTEntry *entryH)
{
	if (!DTInitialized) {
		return kError;
	}

	startingP = (char *)DTRootNode;
	return(find_entry(propName, propValue, entryH));
}

int find_entry(const char *propName, const char *propValue, DTEntry *entryH)
{
	DeviceTreeNode *nodeP = (DeviceTreeNode *) (void *) startingP;
	unsigned int k;

	if (nodeP->nProperties == 0) return(kError);	// End of the list of nodes
	startingP = (char *) (nodeP + 1);

	// Search current entry
	for (k = 0; k < nodeP->nProperties; ++k) {
		DeviceTreeNodeProperty *propP = (DeviceTreeNodeProperty *) (void *) startingP;

		startingP += sizeof (*propP) + ((propP->length + 3) & -4);

		if (strcmp (propP->name, propName) == 0) {
			if (propValue == NULL || strcmp( (char *)(propP + 1), propValue) == 0)
			{
				*entryH = (DTEntry)nodeP;
				return(kSuccess);
			}
		}
	}

	// Search child nodes
	for (k = 0; k < nodeP->nChildren; ++k)
	{
		if (find_entry(propName, propValue, entryH) == kSuccess)
			return(kSuccess);
	}
	return(kError);
}

int
DTLookupEntry(const DTEntry searchPoint, const char *pathName, DTEntry *foundEntry)
{
	DTEntryNameBuf	buf;
	RealDTEntry	cur;
	const char *	cp;

	if (!DTInitialized) {
		return kError;
	}
	if (searchPoint == NULL) {
		cur = DTRootNode;
	} else {
		cur = searchPoint;
	}
	cp = pathName;
	if (*cp == kDTPathNameSeparator) {
		cp++;
		if (*cp == 0) {
			*foundEntry = cur;
			return kSuccess;
		}
	}
	do {
		cp = GetNextComponent(cp, buf);

		/* Check for done */
		if (*buf == 0) {
			if (*cp == 0) {
				*foundEntry = cur;
				return kSuccess;
			}
			break;
		}

		cur = FindChild(cur, buf);

	} while (cur != NULL);

	return kError;
}

int
DTCreateEntryIterator(const DTEntry startEntry, DTEntryIterator *iterator)
{
	RealDTEntryIterator iter;

	if (!DTInitialized) {
		return kError;
	}

	iter = (RealDTEntryIterator) kalloc(sizeof(struct OpaqueDTEntryIterator));
	if (startEntry != NULL) {
		iter->outerScope = (RealDTEntry) startEntry;
		iter->currentScope = (RealDTEntry) startEntry;
	} else {
		iter->outerScope = DTRootNode;
		iter->currentScope = DTRootNode;
	}
	iter->currentEntry = NULL;
	iter->savedScope = NULL;
	iter->currentIndex = 0;

	*iterator = iter;
	return kSuccess;
}

int
DTDisposeEntryIterator(DTEntryIterator iterator)
{
	RealDTEntryIterator iter = iterator;
	DTSavedScopePtr scope;

	while ((scope = iter->savedScope) != NULL) {
		iter->savedScope = scope->nextScope;
		kfree(scope, sizeof(struct DTSavedScope));
	}
	kfree(iterator, sizeof(struct OpaqueDTEntryIterator));
	return kSuccess;
}

int
DTEnterEntry(DTEntryIterator iterator, DTEntry childEntry)
{
	RealDTEntryIterator iter = iterator;
	DTSavedScopePtr newScope;

	if (childEntry == NULL) {
		return kError;
	}
	newScope = (DTSavedScopePtr) kalloc(sizeof(struct DTSavedScope));
	newScope->nextScope = iter->savedScope;
	newScope->scope = iter->currentScope;
	newScope->entry = iter->currentEntry;
	newScope->index = iter->currentIndex;		

	iter->currentScope = childEntry;
	iter->currentEntry = NULL;
	iter->savedScope = newScope;
	iter->currentIndex = 0;

	return kSuccess;
}

int
DTExitEntry(DTEntryIterator iterator, DTEntry *currentPosition)
{
	RealDTEntryIterator iter = iterator;
	DTSavedScopePtr newScope;

	newScope = iter->savedScope;
	if (newScope == NULL) {
		return kError;
	}
	iter->savedScope = newScope->nextScope;
	iter->currentScope = newScope->scope;
	iter->currentEntry = newScope->entry;
	iter->currentIndex = newScope->index;
	*currentPosition = iter->currentEntry;

	kfree(newScope, sizeof(struct DTSavedScope));

	return kSuccess;
}

int
DTIterateEntries(DTEntryIterator iterator, DTEntry *nextEntry)
{
	RealDTEntryIterator iter = iterator;

	if (iter->currentIndex >= iter->currentScope->nChildren) {
		*nextEntry = NULL;
		return kIterationDone;
	} else {
		iter->currentIndex++;
		if (iter->currentIndex == 1) {
			iter->currentEntry = GetFirstChild(iter->currentScope);
		} else {
			iter->currentEntry = GetNextChild(iter->currentEntry);
		}
		*nextEntry = iter->currentEntry;
		return kSuccess;
	}
}

int
DTRestartEntryIteration(DTEntryIterator iterator)
{
	RealDTEntryIterator iter = iterator;
#if 0
	// This commented out code allows a second argument (outer)
	// which (if true) causes restarting at the outer scope
	// rather than the current scope.
	DTSavedScopePtr scope;

	if (outer) {
		while ((scope = iter->savedScope) != NULL) {
			iter->savedScope = scope->nextScope;
			kfree((vm_offset_t) scope, sizeof(struct DTSavedScope));
		}
		iter->currentScope = iter->outerScope;
	}
#endif
	iter->currentEntry = NULL;
	iter->currentIndex = 0;
	return kSuccess;
}

int
DTGetProperty(const DTEntry entry, const char *propertyName, void **propertyValue, unsigned int *propertySize)
{
	DeviceTreeNodeProperty *prop;
	unsigned int k;

	if (entry == NULL || entry->nProperties == 0) {
		return kError;
	} else {
		prop = (DeviceTreeNodeProperty *) (entry + 1);
		for (k = 0; k < entry->nProperties; k++) {
			if (strcmp(prop->name, propertyName) == 0) {
				*propertyValue = (void *) (((uintptr_t)prop)
						+ sizeof(DeviceTreeNodeProperty));
				*propertySize = prop->length;
				return kSuccess;
			}
			prop = next_prop(prop);
		}
	}
	return kError;
}

int
DTCreatePropertyIterator(const DTEntry entry, DTPropertyIterator *iterator)
{
	RealDTPropertyIterator iter;

	iter = (RealDTPropertyIterator) kalloc(sizeof(struct OpaqueDTPropertyIterator));
	iter->entry = entry;
	iter->currentProperty = NULL;
	iter->currentIndex = 0;

	*iterator = iter;
	return kSuccess;
}

int
DTDisposePropertyIterator(DTPropertyIterator iterator)
{
	kfree(iterator, sizeof(struct OpaqueDTPropertyIterator));
	return kSuccess;
}

int
DTIterateProperties(DTPropertyIterator iterator, char **foundProperty)
{
	RealDTPropertyIterator iter = iterator;

	if (iter->currentIndex >= iter->entry->nProperties) {
		*foundProperty = NULL;
		return kIterationDone;
	} else {
		iter->currentIndex++;
		if (iter->currentIndex == 1) {
			iter->currentProperty = (DeviceTreeNodeProperty *) (iter->entry + 1);
		} else {
			iter->currentProperty = next_prop(iter->currentProperty);
		}
		*foundProperty = iter->currentProperty->name;
		return kSuccess;
	}
}

int
DTRestartPropertyIteration(DTPropertyIterator iterator)
{
	RealDTPropertyIterator iter = iterator;

	iter->currentProperty = NULL;
	iter->currentIndex = 0;
	return kSuccess;
}
 
static Node* node[9];

void
DT__RecreateFlattenedNode(DTEntry entry, int level)
{
    DTPropertyIterator                  propIter;
    char *name;
    void *prop;
    unsigned int propSize;

    if (kSuccess != DTCreatePropertyIterator(entry, &propIter)) {
        return;
    }
    while( kSuccess == DTIterateProperties( propIter, &name)) {
        if(  kSuccess != DTGetProperty( entry, name, &prop, &propSize ))
            continue;
        if(level > -1) {
            if( !strcmp (name, "name" ))
                node[level] = DT__AddChild(node[level - 1], prop);
            else
                DT__AddProperty(node[level], name, propSize, prop);
        }
    }
    DTDisposePropertyIterator(propIter);

}

static void
_RecreateFlattenedTree(DTEntry entry, int level)
{
    DTEntryIterator entryIter;

    DT__RecreateFlattenedNode(entry, level);

    if (kSuccess != DTCreateEntryIterator(entry, &entryIter)) {
            printf("Couldn't create entry iterator\n");
            return;
    }
    level++;
    while (kSuccess == DTIterateEntries( entryIter, &entry )) {
        _RecreateFlattenedTree(entry, level);
    }
    DTDisposeEntryIterator(entryIter);
}

void
DT__RecreateFlattenedTree(DTEntry entry)
{
    Node* n = DT__AddChild(NULL, "/");
    node[0] = n;

    _RecreateFlattenedTree(entry, 0);
}

