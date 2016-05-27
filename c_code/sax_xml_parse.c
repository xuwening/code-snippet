
#include <stdlib.h>
#include <stdio.h>

//xml [sax] 解析
struct sax_call_back 
{
	void (*start_document_parse)(char *name, char *attr, char *val);
	void (*end_document_parse)();
	void (*start_element_parse)(char *element, char *attr, char *val);
	void (*element_value_parse)(char *ele_value);
	void (*end_element_parse)(char *element);
};
struct sax_call_back g_xml_callback;

void init_sax_call_back(struct sax_call_back *sax_call_back)
{
	if (sax_call_back != NULL)
		memcpy(&g_xml_callback, sax_call_back, sizeof(struct sax_call_back));
}

void start_document_parse(char *name, char *attr[], char *val[])
{
	int i = 0;
	printf("---------doc start---------->\n");
	while (attr[i] != NULL && *attr[i] != '\0')
	{
		printf("%s : %s --- ", attr[i], val[i]);
		i++;
	}
	printf("\n");
}

void end_document_parse()
{
	printf("---------doc end---------->\n");
}

void start_element_parse(char *element, char *attr[], char *val[])
{
	int i = 0;
	printf("----->element:%s\n", element);
	while (attr[i] != NULL && *attr[i] != '\0')
	{
		printf("%s : %s --- ", attr[i], val[i]);
		i++;
	}
	printf("\n");
}

void element_value_parse(char *ele_value)
{
	printf("element--value : %s\n", ele_value);
}

void end_element_parse(char *element)
{
	printf("-----element--end: %s\n", element);
}


//获取一个节点
int get_node(char *buffer, int buf_len, char *dest, int dest_len)
{
	int len = 0;
	int start_idx = 0;

	while (len < buf_len && buffer[len] != '<') len++;
	start_idx = len+1;

	while (len < buf_len && buffer[len] != '>') len++;
	if (len < buf_len)
	{
		memcpy(dest, &buffer[start_idx], len - start_idx);
		return len + 1;
	}

	return 0;
}

//获取节点值
int get_node_value(char *buffer, int buf_len)
{
	int len = 0;
	while (len < buf_len && buffer[len] != '<') len++;
	return len < buf_len ? len : 0;
}

int get_node_name(char *buffer, int buf_len, char *dest)
{
	int idx=0;
	while (idx < buf_len && buffer[idx] != ' ')
		*dest++ = buffer[idx++];
	return idx;
}

int get_node_attr(char *buffer, int buf_len, char *attr[], char *val[])
{
	int attr_count = 0;
	int idx = 0;

	while (idx < buf_len)
	{
		int attr_len = 0;
		int val_len = 0;
		while (buffer[idx] == ' ') idx++; //去掉空格
		while (idx < buf_len && buffer[idx] != '=')
			attr[attr_count][attr_len++] = buffer[idx++];
		idx++;
		while (idx < buf_len && buffer[idx] == ' ') idx++;
		while (idx < buf_len && buffer[idx] != ' ')
			val[attr_count][val_len++] = buffer[idx++];
		attr_count++;
	}

	return attr_count;
}

int malloc_xml_buffer(char **buf, int lines, int size)
{
	int i;
	for (i=0; i<lines; i++)
	{
		buf[i] = (char *)malloc(size);
		if (buf[i] == NULL)
			return -1;
		memset(buf[i], 0, size);
	}

	return 0;
}

void clear_xml_buffer(char **buf, int lines, int size)
{
	int i;
	for (i=0; i<lines; i++)
		memset(buf[i], 0, size);
}

void free_xml_buffer(char **buf, int lines)
{
	int i;
	for (i=0; i<lines; i++)
	{
		free(buf[i]);
		buf[i] = NULL;
	}
}

void parse_xml_buffer(char *buffer, int buf_len)
{
	int i = 0;
	char element[1024] = {0};
	char destbuffer[1024] = {0};
	char name[128] = {0};

	char *attr[12] = {0};
	char *val[12] = {0};

	if (malloc_xml_buffer(attr, 12, 128) == -1)
		return;
	if (malloc_xml_buffer(val, 12, 128) == -1)
		return;

	while (i = get_node(buffer, buf_len, destbuffer, sizeof(destbuffer)))
	{
		buffer += i;
		buf_len -= i;

		i = get_node_name(destbuffer, strlen(destbuffer), name);
		i = get_node_attr(destbuffer+i, strlen(destbuffer)-i, attr, val);
		//parse
		if (*destbuffer == '?')
			g_xml_callback.start_document_parse(name, attr, val);//call start_doc
		else if (*destbuffer == '/')
			g_xml_callback.end_element_parse(name+1);//call element_end
		else
			g_xml_callback.start_element_parse(name, attr, val);//call element_start

		i = get_node_value(buffer, buf_len);
		memset(element, 0, sizeof(element));
		memcpy(element, buffer, i);

		//call element_value;
		g_xml_callback.element_value_parse(element);

		buffer += i;
		buf_len -= i;
		memset(destbuffer, 0, sizeof(destbuffer));
		clear_xml_buffer(val, 12, 128);
		clear_xml_buffer(attr, 12, 128);
		memset(name, 0, sizeof(name));
	}

	//call end_doc
	end_document_parse();

	//release mem
	free_xml_buffer(attr, 12);
}


char *xml_string = " <?xml version=\"1.0\"  encoding=\"UTF-8\"?> "
" <employees  type=\"1\" class=\" test\"> "
"<employee>"
"<name>我的世界好好玩啊。<\/name>"
"<sex>m<\/sex>"
"<age>30<\/age> "
"<\/employee> "
"<\/employees>";

void main(void)
{
	int i = 0;
	char element[100] = {0};
	char attr[8][128] = {0};
	char val[8][128] = {0};
	char destbuffer[128] = {0};

	struct sax_call_back temp_cb = {
		start_document_parse,
			end_document_parse,
			start_element_parse,
			element_value_parse,
			end_element_parse,
	};
	
	init_sax_call_back(&temp_cb);

	parse_xml_buffer(xml_string, strlen(xml_string));

	getchar();
}