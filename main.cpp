//#include "bits/stdc++.h"
#include <set>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <list>
using namespace std;

enum {
	END,
	WAIT
};

class RoadLine {
public:
	int len, st, ed;
	list<int> waitqueue;
	vector<int> car_id;
};

class Road {
public:
	vector<RoadLine> lines;
	int id, len, maxSpeed, st, ed;
	int cnt;
	bool f;
};

class Car {
public:
	int id, st, dst, maxSpeed, time;
	int pos = 0;
	int state = 0;
};

class Cross {
public:
	int id;
	vector<int> RoadId, Order;
	vector<pair<int, int>> waitqueue;	//car_id, left_dist

	Cross() :RoadId(4), Order(4) {}
};


class AnsPath
{
public:
	int car_id, st;
	vector<int> path;
	int pi = 0, pos = 0;
};


class Graph {
public:

};


void CheckCross(vector<AnsPath> &ansPath)
{
	for (auto &e : ansPath)
	{
	}
}


int g_time = 0;

map<int, Car *> car_map;
map<int, Road *> road_map;
map<int, Cross *> cross_map;
map<int, AnsPath *> ans_map;
map<pair<int, int>, int> road_cross;

vector<Car> cars;
vector<Road> roads;
vector<Cross> cross;


void driveAllCarJustOnRoadToEndState()
{
	for (auto &road : roads)
	{
		int id = road.id, maxSpeed = road.maxSpeed;
		int len = road.len;
		int f = road.f;
		//车道
		for (auto &line : road.lines)
		{
			auto &cars = line.car_id;
			bool wait = false;
			for (int i = cars.size() - 1; i >= 0; --i)
			{
				int car_id = cars[i];
				auto car = car_map[car_id];
				int car_speed = car->maxSpeed;
				int speed = min(maxSpeed, car_speed);
				int st = line.st, ed = line.ed;

				auto ans = ans_map[car_id];

				int maxlen = INT_MAX;

				if (car->pos + speed >= len)
				{
					//进入路口
					int left_dist = min(road_map[ed]->maxSpeed, car->maxSpeed - (len - car->pos));

					if (ans->pi + 1 < ans->path.size())	//还有道路没走完
					{

						int road_next = ans->path[ans->pi + 1];

						int target_corssid = road_cross[{id, road_next}];

						cross_map[target_corssid]->waitqueue.push_back({ car_id, left_dist });
						//line.waitqueue.push_back(car_id);
						maxlen = car->pos;
						wait = true;
						car->state = WAIT;
					}
					else //路走完了
					{

					}
				}
				else if (car->pos + speed >= maxlen)
				{
					//等待前车
					if (wait) //如果前车是等待状态
					{
						line.waitqueue.push_back(car_id);
						car->state = WAIT;
					}
					else// 终止状态
					{
						car->pos = maxlen - 1;
						car->state = END;
					}
					maxlen = car->pos;
				}
				else
				{
					car->pos += speed;
					maxlen = car->pos;
					wait = false;
				}
			}
		}
	}
}

int getOrder(int old_road, int new_road)
{
	if ((new_road - old_road + 4) % 4 == 2)
		return 3;	// 直行
	else if (new_road - old_road == 1)
		return 2;	// 左转
	else return 1;	//右转
}

int canPlace(int roadid, int st)
{
	auto road = road_map[roadid];
	for (int i = 0; i < road->lines.size(); ++i)
	{
		auto &roadline = road->lines[i];
		if (roadline.st == st)
		{
			if (roadline.car_id.empty())
			{
				return i;
			}
			else
			{
				auto car_id = roadline.car_id[0];
				auto ans = ans_map[car_id];
				if (ans->pos > 0)
				{
					return i;
				}
			}
		}
	}
	return -1;
}

void driveAllWaitCar()
{
	for (auto &e : cross_map)
	{
		auto cross_id = e.first;
		auto cross = e.second;
		auto &RoadId = cross->RoadId;
		while (true)
		{
			for (auto oi : cross->Order)
			{
				if (oi == -1) continue;
				int roadid = cross->RoadId[oi];
				auto road_from = road_map[roadid];
				vector<pair<int, int>> dst_state(4);
				



			}
		}

	}
}

int main(int argc, char *argv[])
{
	std::cout << "Begin" << std::endl;

	if (argc < 5) {
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}

	fstream carPath(argv[1]);
	fstream roadPath(argv[2]);
	fstream crossPath(argv[3]);
	fstream answerPath(argv[4]);

	string line;
	while (getline(carPath, line))
	{
		stringstream ss;
		ss << line;
		char c;
		ss >> c;
		Car car;
		ss >> car.id >> c >> car.st >> c >> car.dst >> c >> car.maxSpeed >> c >> car.time;
		cars.push_back(car);

		car_map[car.id] = &car;
	}

	while (getline(roadPath, line))
	{
		stringstream ss;
		ss << line;
		char c;
		Road road;
		int cnt;
		ss >> c >> road.id >> c >> road.len >> c >> road.maxSpeed >> c >> road.cnt >> c >> road.st >> c >> road.ed >> c >> road.f;
		road.lines = vector<RoadLine>(cnt);
		for (auto &e : road.lines)
		{
			e.len = road.len;
			e.st = road.st;
			e.ed = road.ed;
		}
		if (road.f)
		{
			road.lines.resize(cnt * 2);
			for (int i = cnt; i < cnt * 2; ++i)
			{
				road.lines[i].len = road.len;
				road.lines[i].st = road.ed;
				road.lines[i].ed = road.st;
			}
			cnt *= 2;
		}
		roads.push_back(road);
		road_map[road.id] = &road;
	}


	while (getline(crossPath, line))
	{
		stringstream ss;
		ss << line;
		char c;
		Cross cros;
		ss >> c >> cros.id;
		for (int i = 0; i < 4; ++i)
		{
			ss >> c;
			ss >> cros.RoadId[i];
			cros.Order[i] = i;
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = i + 1; j < 4; ++j)
			{
				if (cros.RoadId[i] != -1 && cros.RoadId[j] != -1)
					road_cross[{cros.RoadId[i], cros.RoadId[j]}] = cros.id;
			}
		}

		cross.push_back(cros);
		cross_map[cros.id] = &cros;
		auto cmp = [&](int i, int j) {
			return cros.RoadId[i] < cros.RoadId[j];
		};
		sort(cros.Order.begin(), cros.Order.end(), cmp);
	}

	vector<AnsPath> ansPath;
	while (getline(answerPath, line))
	{
		stringstream ss;
		ss << line;
		char c;
		ss >> c;
		AnsPath ans;
		ss >> c;
		ss >> ans.car_id;
		ss >> c;
		ss >> ans.st;
		ss >> c;
		while (c != ')')
		{
			int id;
			ss >> id;
			ans.path.push_back(id);
		}
		ansPath.push_back(ans);
		ans_map[ans.car_id] = &ans;
	}
	while (true)
	{
		break;
	}

	//for (;;) {
	//	while (/* all car in road run into end state */) {
	//		foreach(roads) {
	//			/* 调整所有道路上在道路上的车辆，让道路上车辆前进，只要不出路口且可以到达终止状态的车辆
	//		   * 分别标记出来等待的车辆（要出路口的车辆，或者因为要出路口的车辆阻挡而不能前进的车辆）
	//		   * 和终止状态的车辆（在该车道内可以经过这一次调度可以行驶其最大可行驶距离的车辆）*/
	//			driveAllCarJustOnRoadToEndState(allChannle);/* 对所有车道进行调整 */

	//			/* driveAllCarJustOnRoadToEndState该处理内的算法与性能自行考虑 */
	//		}
	//	}

	//	while (/* all car in road run into end state */) {
	//		/* driveAllWaitCar() */
	//		foreach(crosses) {
	//			foreach(roads) {
	//				Direction dir = getDirection();
	//				Car car = getCarFromRoad(road, dir);
	//				if (conflict) {
	//					break;
	//				}

	//				channle = car.getChannel();
	//				car.moveToNextRoad();

	//				/* driveAllCarJustOnRoadToEndState该处理内的算法与性能自行考虑 */
	//				driveAllCarJustOnRoadToEndState(channel);
	//			}
	//		}
	//	}

	//	/* 车库中的车辆上路行驶 */
	//	driveCarInGarage();
	//}


	return 0;
}
