
#ifndef in
#define in
#define out
#endif

#define EXPAND_BLOCK_NUM 10

typedef struct  
{
	BOOL expandabilit;
	unsigned int expand_blocks;

	unsigned int block_size;
	unsigned int total_block;

	unsigned int current_block;
	unsigned int tail_block;
	unsigned int used_block;

	void *block_buffer;
}seq_queue_t;



seq_queue_t *create_seq_queue(in unsigned int block_size, in unsigned int total_block, in BOOL expand) {

	seq_queue_t *queue = (seq_queue_t *)malloc(sizeof(seq_queue_t));
	if (queue) {

		memset(queue, 0, sizeof(seq_queue_t));
		queue->block_buffer = malloc(total_block * block_size);
		if (queue->block_buffer == NULL) {
			free(queue);
			queue = NULL;
		} else {
			queue->block_size = block_size;
			queue->total_block = total_block;
			queue->expandabilit = expand;
			queue->expand_blocks = EXPAND_BLOCK_NUM;
		}
	}

	return queue;
}

void destroy_seq_queue(in seq_queue_t *queue) {

	if (queue == NULL)
		return ;

	free(queue->block_buffer);
	free(queue);
}


BOOL expand_seq_queue(in seq_queue_t *queue) {

	void *new_buffer;
	BOOL ret = FALSE;
	if (queue == NULL || !queue->expandabilit)
		return FALSE;

	new_buffer = malloc((queue->total_block+queue->expand_blocks) * queue->block_size);
	if (new_buffer) {
		memcpy(new_buffer, (char *)queue->block_buffer, queue->total_block * queue->block_size);
		queue->total_block += queue->expand_blocks;
		queue->current_block = (queue->used_block + queue->tail_block) % queue->total_block;

		free(queue->block_buffer);
		queue->block_buffer = new_buffer;
		ret = TRUE;
	}

	return ret;
}


BOOL en_seq_queue(in seq_queue_t *queue, in void *data) {

	if (queue == NULL || data == NULL)
		return FALSE;

	if (queue->used_block >= queue->total_block) {
		if (!expand_seq_queue(queue)) {
			return FALSE;
		}
	}

	memcpy((char *)queue->block_buffer + queue->current_block * queue->block_size
		, data, queue->block_size);
	queue->current_block++;
	queue->current_block = queue->current_block % queue->total_block;
	queue->used_block++;

	return TRUE;
}

BOOL de_seq_queue(in seq_queue_t *queue, out void *data) {

	if (queue == NULL || queue->used_block == 0)
		return FALSE;

	memcpy(data, (char *)queue->block_buffer + queue->tail_block * queue->block_size
		, queue->block_size);
	queue->tail_block++;
	queue->used_block--;


	return TRUE;
}

unsigned int get_count_seq_queue(in seq_queue_t *queue) {
	return queue->used_block;
}

unsigned int get_total_seq_queue(in seq_queue_t *queue) {
	return queue->total_block;
}


///////////////////////////////////////


typedef struct  
{
	int age;
	int book;
}test_t;

void test_seq_queue() {

	seq_queue_t *queue;

	test_t data;

	queue = create_seq_queue(sizeof(test_t), 3, TRUE);
	if (queue) {

		data.age = 123;
		data.book = 9;
		en_seq_queue(queue, &data);

		data.age = 123;
		data.book = 10;
		en_seq_queue(queue, &data);
	
		data.age = 123;
		data.book = 11;
		en_seq_queue(queue, &data);

		data.age = 123;
		data.book = 12;
		en_seq_queue(queue, &data);

		printf("\ntotal : %d, use: %d \n", queue->total_block, queue->used_block);


		de_seq_queue(queue, &data);
		printf("\ndata: %d %d \n", data.age, data.book);
		de_seq_queue(queue, &data);
		printf("\ndata: %d %d \n", data.age, data.book);
		de_seq_queue(queue, &data);
		printf("\ndata: %d %d \n", data.age, data.book);
		printf("\ntotal : %d, use: %d \n", queue->total_block, queue->used_block);
		de_seq_queue(queue, &data);
		printf("\ndata: %d %d \n", data.age, data.book);
		printf("\ntotal : %d, use: %d \n", queue->total_block, queue->used_block);
		de_seq_queue(queue, &data);
		printf("\ndata: %d %d \n", data.age, data.book);
		printf("\ntotal : %d, use: %d \n", queue->total_block, queue->used_block);
		de_seq_queue(queue, &data);
		printf("\ndata: %d %d \n", data.age, data.book);
		printf("\ntotal : %d, use: %d \n", queue->total_block, queue->used_block);
	}
}