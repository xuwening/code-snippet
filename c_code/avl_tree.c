#include "stdio.h"
#include "stdlib.h"

//AVL��
typedef struct tag_avl_tree_node
{
	int data;
	struct tag_avl_tree_node *l_tree;
	struct tag_avl_tree_node *r_tree;
	int height;
}avl_tree_node_t;

typedef struct tag_avl_tree 
{
	avl_tree_node_t *root;
	//...
}avl_tree_t;

int tree_height(avl_tree_node_t *avl_node)
{
	if (avl_node == NULL)
		return -1;
	return avl_node->height;
}

//����AVL��
avl_tree_t *create_avl_tree()
{
	avl_tree_t *avl_tree = (avl_tree_t *)malloc(sizeof(avl_tree_t));
	if (avl_tree == NULL)
		return NULL;

	memset(avl_tree, 0, sizeof(avl_tree_t));
	return avl_tree;
}

avl_tree_node_t *create_avl_tree_node(int data)
{
	avl_tree_node_t *avl_node = (avl_tree_node_t *)malloc(sizeof(avl_tree_node_t));
	if (avl_node == NULL)
		return NULL;
	memset(avl_node, 0, sizeof(avl_tree_node_t));
	avl_node->data = data;
	return avl_node;
}

//ll����ת
avl_tree_node_t *ll_rotate(avl_tree_node_t *avl_node)
{
	avl_tree_node_t *avl_tree_node = avl_node->l_tree;
	avl_node->l_tree = avl_tree_node->r_tree;
	avl_tree_node->r_tree = avl_node;

	avl_node->height = max(tree_height(avl_node->l_tree), tree_height(avl_node->r_tree)) + 1;
	avl_tree_node->height = max(tree_height(avl_tree_node->l_tree), avl_node->height) + 1;
	return avl_tree_node;
}

//rr����ת
avl_tree_node_t *rr_rotate(avl_tree_node_t *avl_node)
{
	avl_tree_node_t *avl_tree_node = avl_node->r_tree;
	avl_node->r_tree = avl_tree_node->l_tree;
	avl_tree_node->l_tree = avl_node;

	avl_node->height = max(tree_height(avl_node->l_tree), tree_height(avl_node->r_tree)) + 1;
	avl_tree_node->height = max(avl_node->height, tree_height(avl_tree_node->r_tree)) + 1;
	return avl_tree_node;
}

//lr����ת
avl_tree_node_t *lr_rotate(avl_tree_node_t *avl_node)
{
	//�ȶ�����rr��ת
	avl_node->l_tree = rr_rotate(avl_node->l_tree);
	return ll_rotate(avl_node);
}

//rl����ת
avl_tree_node_t *rl_rotate(avl_tree_node_t *avl_node)
{
	avl_node->r_tree = ll_rotate(avl_node->r_tree);
	return rr_rotate(avl_node);
}

//����һ���ڵ�
avl_tree_node_t *insert_avl_node(avl_tree_node_t *avl_node, int data)
{
	//�����ڸýڵ㣬�򴴽�
	if (avl_node == NULL)
		return create_avl_tree_node(data);
	
	//�Ѿ������ýڵ���
	if (avl_node->data > data) //��������:l
	{
		avl_node->l_tree = insert_avl_node(avl_node->l_tree, data);
		if (tree_height(avl_node->l_tree) - tree_height(avl_node->r_tree) == 2) //������ʧ��
		{
			if (avl_node->l_tree->data > data) //l
				avl_node = ll_rotate(avl_node);
			else if (avl_node->l_tree->data < data)  //r
				avl_node = lr_rotate(avl_node);
		}
	}
	else if (avl_node->data < data)  //��������:r
	{
		avl_node->r_tree = insert_avl_node(avl_node->r_tree, data);
		if (tree_height(avl_node->l_tree) - tree_height(avl_node->r_tree) == -2)  //������ʧ��
		{
			if (avl_node->r_tree->data > data) //l
				avl_node = rl_rotate(avl_node);
			else if (avl_node->r_tree->data < data) //r
				avl_node = rr_rotate(avl_node);
		}
	}
	else
	{
		//�ýڵ��Ȼ����...
		printf("insert node is exist.\n");
	}

	avl_node->height = max(tree_height(avl_node->l_tree), tree_height(avl_node->r_tree)) + 1;
	return avl_node;
}

avl_tree_node_t * remove_min_node(avl_tree_node_t *tree_node, int *data)
{
	if (tree_node == NULL) return NULL;
	if (tree_node->l_tree == NULL)
	{
		*data = tree_node->data;
		free(tree_node);
		return NULL;
	}
	else
	{
		return remove_min_node(tree_node->l_tree, data);
	}
}

//ɾ���ڵ�
avl_tree_node_t *delete_tree_node(avl_tree_node_t *tree_root, int data)
{
	if (tree_root == NULL) return NULL;

	if (tree_root->data == data)
	{
		avl_tree_node_t *temp_node = tree_root;
		if (tree_root->l_tree != NULL && tree_root->r_tree != NULL)
		{	
			tree_root->r_tree = remove_min_node(tree_root->r_tree, &tree_root->data); 
			return tree_root; 
		}
		else if (tree_root->l_tree == NULL)
			tree_root = tree_root->r_tree;
		else if (tree_root->r_tree == NULL)
			tree_root = tree_root->l_tree;
		else	//l and r is null
			tree_root = NULL;

		free(temp_node);
	}
	else if (tree_root->data > data)
		tree_root->l_tree = delete_tree_node(tree_root->l_tree, data);
	else
		tree_root->r_tree = delete_tree_node(tree_root->r_tree, data);

	return tree_root;
}

//���ҽڵ�
avl_tree_node_t *search_tree_node(avl_tree_node_t *tree_root, int data)
{
	if (tree_root == NULL)
		return NULL;

	if (tree_root->data > data)
		return search_tree_node(tree_root->l_tree, data);
	else if(tree_root->data < data)
		return search_tree_node(tree_root->r_tree, data);
	else
		return tree_root;
}

//ɾ��avl��
void delete_tree(avl_tree_node_t *tree_root)
{
	if (tree_root == NULL)
		return;

	delete_tree(tree_root->l_tree);
	delete_tree(tree_root->r_tree);
	free(tree_root);
}

//��ӡavl��
void print_tree(avl_tree_node_t *tree_root)
{
    if (NULL == tree_root)
        return;
	
    static int n = 0;
	
    print_tree(tree_root->l_tree);
    printf("[%d]data = %d\n", ++n, tree_root->data);
    print_tree(tree_root->r_tree);
}


//=============================================================
//����
//=============================================================
void main(void)
{
	int buf[] = {3, 1, 9, 7, 11, 12, 13, 14, 15, 5};
	int i;

	for (i=0; i<100; i++)
	{
		//buf[i] = i;
	}

	{//test start...
		avl_tree_t *tree = create_avl_tree();
		
		for (i=0; i<sizeof(buf)/sizeof(buf[0]); i++)
		{
			tree->root = insert_avl_node(tree->root, buf[i]);
		}
		
		for (i=0; i<sizeof(buf)/sizeof(buf[0]); i++)
		{
			avl_tree_node_t *find_result = search_tree_node(tree->root, buf[i]);
			if (find_result != NULL)
				printf("find yes.[%d]\n", find_result->data);
			else
				printf("find no.\n");
		}

		tree->root = delete_tree_node(tree->root, 9);

		print_tree(tree->root);
		delete_tree(tree->root);
		tree->root = NULL;
	}//test end

	getchar();
}

