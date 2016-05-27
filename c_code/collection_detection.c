
//������ײ���
//������1���ұߴ���2�����
//      1�����С��2���ұ�
//      1���±ߴ���2���ϱ�
//		1���ϱ�С��2���±�

typedef struct tag_point
{
	int x;
	int y;
}point_t;

typedef struct tag_rectangle
{
	point_t pt1;
	point_t pt2;
}rectangle_t;


bool rect_collision_detection(rectangle_t *rt1, rectangle_t *rt2)
{
	if (rt1.pt2.x > rt2.pt1.x && rt1.pt1.x < rt2.pt2.x
		&& rt1.pt2.y > rt2.pt1.y && rt1.pt1.y < rt2.pt2.y)
		return true;
	return false;
}