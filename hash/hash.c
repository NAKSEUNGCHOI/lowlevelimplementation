#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 10

typedef struct KeyValuePair{
	char* key;
	char* value;
	struct KeyValuePair* next;
} KeyValuePair;

typedef struct HashMap {
	KeyValuePair* buckets[HASH_TABLE_SIZE];
} map;

map* map_create(void) {
	// allocate table.
	map* hashTable = (map*)malloc(sizeof(map) * 1);
	
	// initialize to NULL.
	int i = 0;
	for (i = 0; i < HASH_TABLE_SIZE; i++) {
		hashTable->buckets[i] = NULL;
	}

	return hashTable;
}

unsigned int hashFunction(const char *key) {
	unsigned long int value = 0;
	unsigned int i = 0;
	unsigned int key_len = strlen(key);

	// 31 is a common heurisitc in hashing algorithms.
	for (i = 0; i < key_len; i++) {
		value = value * 31 + key[i];
	}

	// 0 <= value < HASH_TABLE_SIZE
	value = value % HASH_TABLE_SIZE;
	
	return value;
}

KeyValuePair* createKeyValuePair(const char* key, char* value) {
	KeyValuePair* pair = 0;
	pair = malloc(sizeof(KeyValuePair));
	if (!pair) {
		printf("memory not allocated for key and pair\n");
		return NULL;
	}
	// strdup: allocate memory and copy into pair->key and pair->value
	pair->key = strdup(key);
	pair->value = strdup(value);
	pair->next = NULL;
	return pair;
}

void insert(map* mp, const char* key, char* value) {
	unsigned int index = hashFunction(key);

	KeyValuePair* newPair = createKeyValuePair(key, value);
	if (!mp->buckets[index]) {
		// no collision, insert at the beginning.
		mp->buckets[index] = newPair;
		return;
	}
	// if collision, insert at next
	KeyValuePair* current = mp->buckets[index];
	while (current->next) {
		current = current->next;
	}
	current->next = newPair;
}

char* get (map* mp, const char* key) {
	unsigned int index = hashFunction(key);
	KeyValuePair* current = mp->buckets[index];

	while (current) {
		if (strcmp(current->key, key) == 0) {
			return current->value;
		}
		current = current->next;
	}
	return NULL;
}

void delete (map* mp, const char* key) {
	unsigned int index = hashFunction(key);
	KeyValuePair* current = mp->buckets[index];
	KeyValuePair* previous = NULL;

	while (current) {
		if (strcmp(current->key, key) == 0) {
			if (!previous) {
				mp->buckets[index] = current->next;
			} else {
				previous->next = current->next;
			}
			free(current->key);
			if (current->value) {
				free(current->value);
			}
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

void printHashMap(map* mp) {
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		printf("Bucket %d: ", i);
		KeyValuePair* current = mp->buckets[i];
		while (current) {
			printf("(%s, %s) -> ", current->key, current->value);
			current = current->next;
		}
		printf("NULL\n");
	}
}

void freeHashMap(map* mp) {
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		KeyValuePair* current = mp->buckets[i];
		while (current) {
			KeyValuePair* temp = current;
			current = current->next;
			free(temp->key);
			free(temp->value);
			free(temp);
		}
	}
}

int main () {
	map *mp = map_create();
	
	insert(mp, "Alice", "23");
	insert(mp, "Bob", "30");
	insert(mp, "Charvan", "32");
	insert(mp, "David", "29");
	insert(mp, "Eva", "27");

	printf("HashMap Insertion\n");
	printHashMap(mp);

	printf("\nValue for key 'Bob': %s\n", get(mp, "Bob"));
	printf("\nValue for key 'David': %s\n", get(mp, "David"));
	
	delete(mp, "Charvan");

	printf("\nhashmap after deletion:\n");
	printHashMap(mp);
	
	freeHashMap(mp);

	return 0;
}
