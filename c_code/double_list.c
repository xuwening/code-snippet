
#include "typedefs.h"


typedef struct tag_d_list 
{
	void *data;
	unsigned int data_len;
	struct tag_d_list *pre;
	struct tag_d_list *next;
}d_list_node_t;

typedef struct  
{
	unsigned int node_count;
	d_list_node_t *head;
	d_list_node_t *tail;
}d_list_t;


typedef int (*compare_func)(void *usr_data, unsigned int udata_len, void *list_data, unsigned int ldata_len);




BOOL is_empty_d_list(d_list_t *list) {
	
	if (list && list->node_count == 0 && list->head == NULL && list->tail == NULL) 
		return TRUE;
	
	return FALSE;
}


unsigned int get_d_list_node_count(d_list_t *list) {
	return list->node_count;
}


d_list_t *create_d_list() {
	d_list_t *temp_list = (d_list_t *)malloc(sizeof(d_list_t));
	if (temp_list) {
		memset(temp_list, 0, sizeof(d_list_t));
	}

	return temp_list;
}

d_list_node_t *create_d_list_node(void *data, unsigned int data_len) {

	d_list_node_t *temp_node = (d_list_node_t *)malloc(sizeof(d_list_node_t));
	if (temp_node) {
		temp_node->data = malloc(data_len+1);
		if (temp_node->data) {
			memcpy(temp_node->data, data, data_len);
			((char *)temp_node->data)[data_len] = '\0';
			temp_node->data_len = data_len;
			temp_node->pre = NULL;
			temp_node->next = NULL;
		} else {
			free(temp_node);
			temp_node = NULL;
		}
	}

	return temp_node;
}


d_list_node_t *insert_d_list_node_head_n(d_list_node_t *head, d_list_node_t *node) {
		
	if (head && node) {
		node->pre = head->pre;
		node->next = head;
		
		head->pre = node;
	}
	
	return node;
}

d_list_node_t *insert_d_list_node_tail_n(d_list_node_t *tail, d_list_node_t *node) {
	
	if (tail && node) {
		
		tail->next = node;
		node->pre = tail;
	}
	
	return node;
}


d_list_node_t *insert_d_list_node_head(d_list_node_t *head, void *data, unsigned int data_len) {

	d_list_node_t *temp_node = create_d_list_node(data, data_len);

	return insert_d_list_node_head_n(head, temp_node);
}

d_list_node_t *insert_d_list_node_tail(d_list_node_t *tail, void *data, unsigned int data_len) {
	
	d_list_node_t *temp_node = create_d_list_node(data, data_len);
	
	return insert_d_list_node_tail_n(tail, temp_node);
}


BOOL insert_d_list_head(d_list_t *list, void *data, unsigned int data_len) {
	
	if (list) {

		if (is_empty_d_list(list)) {
			list->head = insert_d_list_node_head(list->head, data, data_len);
			list->tail = list->head;
		} else {
			list->head = insert_d_list_node_head(list->head, data, data_len);
		}

		list->node_count++;
		return TRUE;
	}
	
	return FALSE;
}

BOOL insert_d_list_tail(d_list_t *list, void *data, unsigned int data_len) {

	if (list) {
		d_list_node_t *temp_node = create_d_list_node(data, data_len);

		if (is_empty_d_list(list)) {
			list->tail = insert_d_list_node_tail(list->tail, data, data_len);
			list->head = list->tail;
		} else {
			list->tail = insert_d_list_node_tail(list->tail, data, data_len);
		}

		list->node_count++;
		return TRUE;
	}

	return FALSE;
}


BOOL delete_d_list_head(d_list_t *list) {

	if (list && list->node_count > 0) {
		d_list_node_t *temp_node = list->head;
		
		list->head = list->head->next;
		list->node_count--;
		if (list->node_count == 0 && list->head == NULL) {
			list->tail = NULL;
		} else {
			list->head->pre = NULL;
		}

		free(temp_node->data);
		free(temp_node);

		return TRUE;
	}
	
	return FALSE;
}

BOOL delete_d_list_tail(d_list_t *list) {
	
	if (list && list->node_count > 0) {
		d_list_node_t *temp_node = list->tail;
		
		list->tail = list->tail->pre;
		list->node_count--;
		if (list->node_count == 0 && list->tail == NULL) {
			list->head = NULL;
		} else {
			list->tail->next = NULL;
		}
		
		free(temp_node->data);
		free(temp_node);
		
		return TRUE;
	}
	
	return FALSE;
}


BOOL remove_d_list_node(d_list_t *list, d_list_node_t *node) {
	
	if (node == NULL || list == NULL || list->node_count == 0)
		return FALSE;

	if (list->head == node) {
		list->head = node->next;
		if (list->head != NULL) {
			node->next->pre = NULL;
		} else {
			list->tail = NULL;
		}
	} else {
		node->pre->next = node->next;
		if (node->next == NULL) {
			list->tail = node->pre;
		} else {
			node->next->pre = node->pre;
		}
	}

	list->node_count--;
	return TRUE;
}

//从头部删除节点
d_list_node_t *remove_d_list_head_node(d_list_t *list) {

	d_list_node_t *temp_node = NULL;
	if (list == NULL || list->head == NULL)
		return NULL;

	temp_node = list->head;
	list->head = list->head->next;
	if (list->head == NULL)
		list->tail = NULL;
	else
		list->head->pre = NULL;

	list->node_count--;
	return temp_node;
}

//从尾部删除节点
d_list_node_t *remove_d_list_tail_node(d_list_t *list) {

	d_list_node_t *temp_node = NULL;
	if (list == NULL || list->tail == NULL)
		return NULL;

	temp_node = list->tail;
	list->tail = list->tail->pre;
	if (list->tail == NULL)
		list->head = NULL;
	else
		list->tail->next = NULL;

	list->node_count--;
	return temp_node;
}


BOOL delete_d_list_node_all(d_list_t *list) {

	if (list) {
		while (delete_d_list_head(list)) {

		}

		if (list->node_count == 0) {
			return TRUE;
		}
	}

	return FALSE;
}

void destroy_d_list(d_list_t *list) {
	if (list == NULL)
		return;
	delete_d_list_node_all(list);
	free(list);
}


d_list_node_t *find_d_list_node(d_list_t *list, void *user_data, unsigned int data_len, compare_func func) {
	
	d_list_node_t *temp_node = NULL;
	if (list && list->node_count > 0) {
		temp_node = list->head;
		while (temp_node)
		{
			if (func(user_data, data_len, temp_node->data, temp_node->data_len) == 0) {
				break;
			}

			temp_node = temp_node->next;
		}
	}

	return temp_node;
}

d_list_node_t *get_d_list_node(d_list_t *list, int idx) {
	d_list_node_t *temp_node = NULL;
	if (list && list->node_count >= abs(idx) && idx != 0) {

		if (idx > 0) {
			temp_node = list->head;
			while (--idx)
			{
				temp_node = temp_node->next;
			}
		} else {
			temp_node = list->tail;
			while (++idx)
			{
				temp_node = temp_node->pre;
			}
		}
	}

	return temp_node;
}

//------------------------------------------
void display_d_list(d_list_t *list) {

	d_list_node_t *temp_node;

	if (list == NULL)
		return;

	temp_node = list->head;
	printf("\n____head____ : %d \n", get_d_list_node_count(list));
	while (temp_node)
	{
		printf(" %s -->", (char *)temp_node->data);
		temp_node = temp_node->next;
	}

	temp_node = list->tail;
	printf("\n____tail____ : %d \n", get_d_list_node_count(list));
	while (temp_node)
	{
		printf(" %s -->", (char *)temp_node->data);
		temp_node = temp_node->pre;
	}
}


int compare_nod_data(void *usr_data, unsigned int sdata_len, void *list_data, unsigned int ldata_len) {
	return strcmp((char *)usr_data, (char *)list_data);
}

void test_d_list() {

	BOOL ret;
	char *pstr;
	d_list_node_t *temp_node;

	d_list_t *ls = create_d_list();

	if (ls == NULL) {
		perror("create d list err.\n");
		return;
	}

	display_d_list(ls);

	pstr = "aaa";
	ret = insert_d_list_head(ls, pstr, strlen(pstr));
	display_d_list(ls);

	pstr = "bbbb";
	ret = insert_d_list_head(ls, pstr, strlen(pstr));
	display_d_list(ls);

	pstr = "cccc";
	ret = insert_d_list_head(ls, pstr, strlen(pstr));
	display_d_list(ls);

	
	pstr = "ddddd";
	ret = insert_d_list_head(ls, pstr, strlen(pstr));
	display_d_list(ls);

	
	/*
	temp_node = remove_d_list_head_node(ls);
	printf("\nddddddddddd %s dddddddd\n", (char *)temp_node->data);
	temp_node = remove_d_list_head_node(ls);
	printf("\nddddddddddd %s dddddddd\n", (char *)temp_node->data);

	pstr = "cccc";
	{
		d_list_node_t *temp = find_d_list_node(ls, pstr, strlen(pstr), compare_nod_data);
		if (temp == NULL || !remove_d_list_node(ls, temp)) {
			printf("\n remove error .\n");
		}

		ls->head = insert_d_list_node_head_n(ls->head, temp);
	}
		
	
	if (!remove_d_list_node(ls, ls->head)) {
		printf("\n remove error .\n");
	}
	if (!remove_d_list_node(ls, ls->tail)) {
		printf("\n remove error .\n");
	}


	pstr = "eee";
	ret = insert_d_list_tail(ls, pstr, strlen(pstr));
	display_d_list(ls);


	pstr = "fffff";
	ret = insert_d_list_tail(ls, pstr, strlen(pstr));
	display_d_list(ls);


	pstr = "gggggggg";
	ret = insert_d_list_tail(ls, pstr, strlen(pstr));
	display_d_list(ls);


	
	pstr = "ddddd";
	{
		d_list_node_t *temp = find_d_list_node(ls, pstr, strlen(pstr), compare_nod_data);
		printf("\n####################\n");
		printf("find data: %s ", (char *)temp->data);
		printf("\n####################\n");

		temp = get_d_list_node(ls, 1);
		printf("get data: %s ", (char *)temp->data);
		printf("\n####################\n");
	}
	


	delete_d_list_head(ls);
	display_d_list(ls);

	delete_d_list_tail(ls);
	display_d_list(ls);

	delete_d_list_node_all(ls);
	*/
		
		display_d_list(ls);
}