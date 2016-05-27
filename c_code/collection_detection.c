
//矩形碰撞检测
//描述：1的右边大于2的左边
//      1的左边小于2的右边
//      1的下边大于2的上边
//		1的上边小于2的下边

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