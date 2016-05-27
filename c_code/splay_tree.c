
//splay tree node struct
typedef struct splay_tree_node 
{
	struct splay_tree_node *left;
	struct splay_tree_node *right;
	int data;
}splay_tree_node_t;


//申请一个节点
splay_tree_node_t *create_splay_node(int data)
{
	splay_tree_node_t *node = (splay_tree_node_t *)malloc(sizeof(splay_tree_node_t));
	if (node)
	{
		memset(node, 0, sizeof(splay_tree_node_t));
		node->data = data;
	}
	return node;
}

//释放节点
void release_splay_node(splay_tree_node_t *node)
{
	free(node);
}

//左旋转
splay_tree_node_t *left_rotate(splay_tree_node_t *node)
{
	splay_tree_node_t *r_node;
	if (node == NULL)
		return NULL;

	r_node = node->right;
	node->right = r_node->left;
	r_node->left = node;

	return r_node;
}

//右旋转
splay_tree_node_t *right_rotate(splay_tree_node_t *node)
{
	splay_tree_node_t *l_node;
	if (node == NULL)
		return NULL;

	l_node = node->left;
	node->left = l_node->right;
	l_node->right = node;
	return l_node;
}

//自平衡
splay_tree_node_t *splay_self(splay_tree_node_t *node, int data)
{
	splay_tree_node_t *left_tree, *right_tree;
	splay_tree_node_t temp_node;

	if (node == NULL)
		return NULL;

	temp_node.left = temp_node.right = NULL;
	left_tree = right_tree = &temp_node;

	while (1)
	{
		if (data < node->data)
		{
			if (node->left == NULL)
				break;
			
			//右旋转
			if (data < node->left->data)
				node = right_rotate(node);
			if (node->left == NULL)
				break;

			right_tree->left = node;
			right_tree = node;
			node = node->left;
		}
		else if (data > node->data)
		{
			if (node->right == NULL)
				break;

			//左旋转
			if (data < node->right->data)
				node = left_rotate(node);
			if (node->right == NULL)
				break;

			left_tree->right = node;
			left_tree = node;
			node = node->right;
		}
		else //==
		{
			break;
		}
	}

	left_tree->right = node->left;
	right_tree->left = node->right;
	node->left = temp_node.right;  //?
	node->right = temp_node.left;

	return node;
}

//插入
splay_tree_node_t *insert_splay_tree(splay_tree_node_t *node, int data)
{
	splay_tree_node_t *root_node = NULL;
	splay_tree_node_t *temp_node = create_splay_node(data);
	if (temp_node == NULL)
		return NULL;
	
	if (node == NULL) //root
		return temp_node;

	root_node = splay_self(node, data);
	if (data < root_node->data) //left
	{
		temp_node->left = root_node->left;
		temp_node->right = root_node;
		root_node->left = NULL;
	}
	else if (data > root_node->data)  //right
	{
		temp_node->left = root_node;
		temp_node->right = root_node->right;
		root_node->right = NULL;
	}
	else //exist
	{
		release_splay_node(temp_node);
		temp_node = node;
	}

	return temp_node;
}

//删除
splay_tree_node_t *delete_splay_tree_node(splay_tree_node_t *node, int data)
{
	splay_tree_node_t *temp_node = NULL;
	splay_tree_node_t *del_node = NULL;

	if (node == NULL)
		return NULL;

	temp_node = splay_self(node, data);
	del_node = temp_node;
	if (data == temp_node->data) //==
	{
		if (temp_node->left == NULL)
			temp_node = temp_node->right;
		else
			temp_node = temp_node->left;

		release_splay_node(del_node);
		del_node = NULL;
	}

	return temp_node;
}

//查询节点
splay_tree_node_t *search_splay_tree_node(splay_tree_node_t *node, int data)
{
	splay_tree_node_t *temp_node = NULL;
	temp_node = splay_self(node, data);
	return data == temp_node->data ? temp_node : NULL;
}

//遍历所有节点
void traversing_splay_tree(splay_tree_node_t *node)
{
	if (node == NULL)
		return;

	traversing_splay_tree(node->left);
	printf("%d ", node->data);
	traversing_splay_tree(node->right);
}

 
#define        NUM        20
 void main() {
     splay_tree_node_t* root;
     int i;
 
     root = NULL;              /* the empty tree */
 
     for(i = NUM; i > 0; i--)
	 {
         root = insert_splay_tree(root, i);
		 traversing_splay_tree(root);
		 printf("\n");
	 }
 
     traversing_splay_tree(root);
     printf("\n");
 
#if 1
     for(i = 1; i <= NUM; i++)
         root = delete_splay_tree_node(root, i);

     printf("\n");
     traversing_splay_tree(root);
#endif
	 getchar();

 }
