#pragma once

#include <vector>
#include <map>
#include <queue>
// 二维地图A*算法实现

const int OBLIQUE = 14;  // 斜线移动权重为14
const int STEP = 10;
struct Point
{
	Point(int id, int reachable);
	~Point();

	void CalcF() { m_F = m_G + m_H; }
	bool IsReachable()
	{
		if (m_reachable == 0) return true;
		return false;
	}
	bool operator>(const Point* point) // 优先队列小顶堆
	{
		if (m_F > point->m_F)
		{
			return true;
		}
		return false;
	}

	int m_F{ 0 };  // F = G + H
	int m_G{ 0 };  // G 表示从起点 A 移动到网格上指定方格的移动耗费 (可沿斜方向移动)
	int m_H{ 0 };  // H 表示从指定的方格移动到终点 B 的预计耗费 (H 有很多计算方法, 这里我们设定只可以上下左右移动)
	int m_id; // 二维数组转换成一维数组的位置编号 0, 1, 2,....
	int m_reachable;  // 0 可达 1 不可达
	Point* m_parent{nullptr};  // 保存父节点
	int m_close{0};  // 0 未加入closelist 1 加入closelist
	int m_x{0};
	int m_y{0};
};


//--------------
//|0 1 3 4 5
//|6 7 8 9 10
//|11 12 13 14 15
//|....
typedef std::vector<Point*> point_vec_type;
typedef std::map<int, Point*> point_map_type;
typedef std::priority_queue<Point*, point_vec_type> point_queue_type;
class AStarNav
{
public:
	AStarNav();
	~AStarNav();

	bool FindPath(int start_id, int end_id, std::vector<Point*>& find_path);
	bool CanReach(int start_id, int end_id);

	bool LoadMap(const char* path);
	int GetPointId(int i, int j);
private:
	void GetAroundPoints(int point_id, point_vec_type& point_set);
	void GetPos(int& i, int& j, int point_id);
	bool StopSearch(int target_id);
	Point* GetPoint(int point_id);
	Point* GetMinFInOpenList();
	template <typename T>
	bool IsInList(const T& t, Point* point);
	void InOpenList(Point* point);
	void OutOpenList(Point* point);
	int CalcG(const Point* start, const Point* point);
	int CalcH(const Point* end, const Point* point);
private:
	point_map_type m_close_list;  // 检查完毕列表
	point_map_type m_open_list;   // 待检查列表
	point_queue_type m_open_queue; // 查找最小值的辅助结构
	point_map_type m_points; // 地图所有格子
	int m_map_width{ 0 };
	int m_map_hight{ 0 };
};

