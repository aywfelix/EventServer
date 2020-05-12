#include "SFTest.h"
#include "map/AStarNav.h"

void SFTest::TestAStar()
{
	AStarNav nav = AStarNav();
	nav.LoadMap(".\\map\\map.csv");
	std::vector<Point*> point_vec;
	nav.FindPath(9, 90, point_vec);
	for (auto it : point_vec)
	{
		cout << it->m_x << "|" << it->m_y <<" : "<<it->m_id<< endl;
	}

}