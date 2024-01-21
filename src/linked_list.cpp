#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node * next; // pointer to a struct node named next
};

typedef struct node Node;

// head and tail will point to Node (initialize to NULL)
Node* head = NULL;
Node* tail = NULL; 

void make_link (int value) {
	// make a new node with the given data
//    Node new_node;
    Node* new_node = (Node*)malloc(sizeof(Node));
	(*new_node).data = value; // (*x).y == x->y
	(*new_node).next = NULL;

	// assign head and tail
	if (head == NULL) {
		head = new_node;
		tail = new_node;
	}
	else {
		// head.next is poinint at null
		(*tail).next = new_node;
		tail = new_node;
	}
	return;

};

void print_links () {
	Node* temp = head;
	while (temp != NULL) {
		printf("%d\n", (*temp).data);
		temp = (*temp).next;
	};
	return;
};

//void free_links () {
//    Node* track = head;
//    Node* temp;
//    while (track != NULL) {
//        temp = track;
//        track = (*track).next;
//        free(temp);
//    };
//    return;
//};

void free_links () {
    Node* temp;
    Node* track = head;
    while (track != NULL) {
        temp = track;
        track = (*track).next;
        free(temp);
    };
    return;
};

int main () {
	make_link(1);
	make_link(2);
    make_link(1002);
	print_links();
	return 0;
}
