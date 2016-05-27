
typedef struct tag_list_node {
	
	char *data;
	struct tag_list_node *next;
	
}list_node_t;


list_node_t *create_list_node(char *data) {

	list_node_t *node = NULL;
	if (data) {
		node = (list_node_t *)malloc(sizeof(list_node_t));
		if (node) {
			memset(node, 0, sizeof(list_node_t));
			node->data = data;
		}
	}

	return node;
}


list_node_t *append_list_node(list_node_t *node, char *data) {

	if (node == NULL)
		return create_list_node(data);

	node->next = append_list_node(node->next, data);
	return node;
}

list_node_t *delete_list_node(list_node_t *node) {

	if (node == NULL)
		return NULL;

	if (node->next == NULL) {
		//free(node->data);
		free(node);
		return NULL;
	}

	node->next = delete_list_node(node->next);
	return node;
}

list_node_t *insert_list_node(list_node_t *node, char *data) {

	list_node_t *temp_node = create_list_node(data);

	temp_node->next = node;
	return temp_node;
}

list_node_t *remove_list_node(list_node_t *node) {

	list_node_t *temp_node = node;

	if (temp_node) {
		node = node->next;
		free(temp_node);
	}

	return node;
}


unsigned int get_node_count(list_node_t *node) {

	unsigned int count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}

	return count;
}


//------------------------------
void display_list_node(list_node_t *node) {

	if (node == NULL)
		return;

	display_list_node(node->next);
	printf("%s-->", node->data);
}

void test() {

	list_node_t *head = NULL;

	head = insert_list_node(head, "aaaaaaaa");
	head = insert_list_node(head, "bbbbbbbb");
	head = insert_list_node(head, "cccccccc");
	head = insert_list_node(head, "ddddddddd");
	head = insert_list_node(head, "eeeeeeeee");


	printf("\n node count : %d \n", get_node_count(head));

	head = remove_list_node(head);
	head = remove_list_node(head);


	display_list_node(head);

	printf("\n node count : %d \n", get_node_count(head));
}
