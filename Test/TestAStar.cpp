#include "SFTest.h"
#include "map/AStarNav.h"
//#include <set>
//struct Point
//{
//	int id{0};
//	int v{ 0 };
//};
//
//struct cmp {
//	bool operator()(const Point* a, const Point* b) const{
//		return a->id != b->id && a->v < b->v;  // 从小往大输出
//	}
//};
//typedef std::set<Point*, cmp> points_type;

void SFTest::TestAStar()
{
	//points_type point_set;
	//Point* p1 = new Point; p1->id = 1; p1->v = 1;
	//Point* p2 = new Point; p2->id = 2; p2->v = 1;
	//Point* p3 = new Point; p3->id = 3; p3->v = 1;
	//point_set.emplace(p1);
	//point_set.emplace(p2);
	//point_set.emplace(p3);

	AStarNav nav = AStarNav();
	nav.LoadMap(".\\map\\map.csv");
	std::vector<Point*> point_vec;
	nav.FindPath(9, 90, point_vec);
	for (auto it : point_vec)
	{
		cout << it->m_x << "|" << it->m_y << endl;
	}
}