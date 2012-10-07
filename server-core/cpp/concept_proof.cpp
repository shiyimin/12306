#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include "sites.h"
#include "trains.h"

#define BOOST_TEST_DYN_LINK 
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

// 下面的头文件可选
#include "site_names.h"
#include <boost/graph/graphviz.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, bidirectionalS, int, int> Graph;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIter;
typedef graph_traits<Graph>::in_edge_iterator InEdgeIter;

Graph _graph(SITECOUNT);
int _ticket_distribution[SITECOUNT][TRAINCOUNT][SEAT_TYPE_COUNT];
std::vector<std::vector<int> *> _same_city(SITECOUNT);

bool _inited = false;

void adjust_ticket_distribution(Graph graph, int start, int end, int train)
{
	int site = start;

	do
	{
		OutEdgeIter out, out_end, out_update;

		for ( tie(out, out_end) = out_edges(site, graph);
				out != out_end;
				++out)
		{
			// 经过站点的火车
			int t = graph[*out];
			if ( t == train ) 
			{
				int target_site = target(*out, graph);
				
				// 更新火车可售票的数目
				_ticket_distribution[target_site][train][硬座] += _ticket_distribution[site][train][硬座];
				_ticket_distribution[target_site][train][硬卧] += _ticket_distribution[site][train][硬卧];
				_ticket_distribution[target_site][train][软卧] += _ticket_distribution[site][train][软卧];
				_ticket_distribution[target_site][train][无座] += _ticket_distribution[site][train][无座];

				site = target_site;
				break;
			}
		}
	} while ( site != end );
}

void Init()
{
	if ( !_inited )
	{
		memset(_ticket_distribution, 0, sizeof(_ticket_distribution));
		add_edge(北京南, 德州东, G107, _graph);
		add_edge(德州东, 济南西, G107, _graph);
		add_edge(济南西, 泰安, G107, _graph);
		add_edge(泰安, 曲阜东, G107, _graph);
		add_edge(曲阜东, 蚌埠南, G107, _graph);
		add_edge(蚌埠南, 南京南, G107, _graph);
		add_edge(南京南, 丹阳北, G107, _graph);
		add_edge(丹阳北, 上海虹桥, G107, _graph);

		// 假设G107有9节硬座车厢，4节硬卧，1节软卧
		// 给济南预留了2节硬座车厢，1节硬卧
		// 一节硬座车厢座位是118个，一节硬卧是66个铺位，软卧没有查到资料，当作跟硬卧相同，最多可以卖一半硬座的站票
		_ticket_distribution[北京南][G107][硬座] = (9 - 2) * 118;
		_ticket_distribution[北京南][G107][硬卧] = 3 * 66;
		_ticket_distribution[北京南][G107][软卧] = 1 * 66;
		_ticket_distribution[北京南][G107][无座] = (9 - 2) * 118 / 2;
				
		_ticket_distribution[济南西][G107][硬座] = 2 * 118;
		_ticket_distribution[济南西][G107][硬卧] = 1 * 66;
		_ticket_distribution[济南西][G107][无座] = 2 * 118 / 2;

		adjust_ticket_distribution(_graph, 北京南, 上海虹桥, G107);
	
		add_edge(上海虹桥, 无锡东, G108, _graph);
		add_edge(无锡东, 南京南, G108, _graph);
		add_edge(南京南, 定远, G108, _graph);
		add_edge(定远, 泰安, G108, _graph);
		add_edge(泰安, 济南西, G108, _graph);
		add_edge(济南西, 廊坊, G108, _graph);
		add_edge(廊坊, 北京南, G108, _graph);

		// 不过为了做测试用，每个车厢只有3张票，只有两节硬座和硬卧车厢
		_ticket_distribution[上海虹桥][G108][硬座] = 1 * 3;
		_ticket_distribution[上海虹桥][G108][硬卧] = 1 * 3;
		_ticket_distribution[上海虹桥][G108][软卧] = 0;
		_ticket_distribution[上海虹桥][G108][无座] = 0;
			
		/* 给南京预留了1节硬座车厢，1节硬卧 - 为了方便测试，先不考虑
		_ticket_distribution[南京南][G108][硬座] = 1 * 3;
		_ticket_distribution[南京南][G108][硬卧] = 1 * 3;
		_ticket_distribution[南京南][G108][无座] = 0;
		*/
		
		adjust_ticket_distribution(_graph, 上海虹桥, 北京南, G108);

		{
			int cities[] = { 上海, 上海虹桥, 上海南, 上海西 };
			_same_city[上海] = new std::vector<int>(cities, cities + sizeof(cities) / sizeof(int));
		}

		{
			int cities[] = { 北京, 北京北, 北京东, 北京南, 北京西 };
			_same_city[北京] = new std::vector<int>(cities, cities + sizeof(cities) / sizeof(int));
		}

		_inited = true;
	}
}

int remain_tickets(int start, int train, int seat)
{
	return _ticket_distribution[start][train][seat];
}

std::vector<int> *query(int start, int end)
{
	std::vector<int> trains_start, trains_end;
	for ( std::vector<int>::iterator it = _same_city[start]->begin(); 
		  it != _same_city[start]->end();
		  it++ )
	{
		OutEdgeIter out, out_end;
		for ( tie(out, out_end) = out_edges((*it), _graph);
			  out != out_end;
			  ++out)
		{
			trains_start.push_back(_graph[*out]);
		}
	}

	for ( std::vector<int>::iterator it = _same_city[end]->begin();
		  it != _same_city[end]->end();
		  it++ )
	{
		InEdgeIter in, in_end;
		for ( tie(in, in_end) = in_edges((*it), _graph);
			  in != in_end;
			  ++in )
		{
			trains_end.push_back(_graph[*in]);
		}
	}

	std::vector<int> *pRet = new std::vector<int>();
	set_intersection(trains_start.begin(), trains_start.end(),
		             trains_end.begin(), trains_end.end(), std::back_inserter(*pRet));

	return pRet;
}

int make_order(int start, int end, int train, int seat)
{
	int site = start;
	std::vector<int> sites;

	do
	{
		OutEdgeIter out, out_end, out_update;

		for ( tie(out, out_end) = out_edges(site, _graph);
				out != out_end;
				++out)
		{
			// 经过站点的火车
			int t = _graph[*out];
			if ( t == train ) 
			{
				int target_site = target(*out, _graph);
				sites.push_back(site);
				site = target_site;
				break;
			}
		}
	} while ( site != end );
	
	std::vector<int>::iterator reverter = sites.end();
	for (std::vector<int>::iterator it = sites.begin();
		 it != sites.end();
		 ++it )
	{
		// 中间只要有任何一站没有票，就放弃
		if ( _ticket_distribution[*it][train][seat] == 0 )
		{
			reverter = it;
			break;
		}
		else
		{
			__sync_fetch_and_sub(&_ticket_distribution[*it][train][seat], 1);
		}
	}

	// 需要回滚
	if ( reverter != sites.end() )
	{
		for ( std::vector<int>::iterator it = sites.begin();
			  it != reverter;
			  ++it )
		{
			__sync_fetch_and_add(&_ticket_distribution[*it][train][seat], 1);
		}

		return -1;
	}
	else 
	{
		// 当前的总票数就是乘客买票的座位号，可以推演出来
		return _ticket_distribution[start][train][seat];
	}
}

void dump_graph()
{
    write_graphviz(std::cout, _graph, make_label_writer(_site_names));
}

BOOST_AUTO_TEST_CASE(演示查票方法)
{
	Init();

	std::vector<int> *pResult = query(北京, 上海);
	BOOST_CHECK(pResult->size() == 1);
	BOOST_CHECK(pResult->at(0) == G107);

	delete pResult;
}

BOOST_AUTO_TEST_CASE(演示买票)
{
	Init();

	int order = make_order(上海虹桥, 北京南, G108, 硬座);
	BOOST_CHECK(order == 2);
	BOOST_CHECK(remain_tickets(上海虹桥, G108, 硬座) == 2);
	BOOST_CHECK(remain_tickets(南京南, G108, 硬座) == 2);	
	BOOST_CHECK(remain_tickets(济南西, G108, 硬座) == 2);

	order = make_order(上海虹桥, 济南西, G108, 硬座);
	BOOST_CHECK(order == 1);
	BOOST_CHECK(remain_tickets(上海虹桥, G108, 硬座) == 1);
	BOOST_CHECK(remain_tickets(南京南, G108, 硬座) == 1);
	BOOST_CHECK(remain_tickets(济南西, G108, 硬座) == 2);
	// 济南下车，廊坊还是可以买同一个座位的票
	BOOST_CHECK(remain_tickets(廊坊, G108, 硬座) == 2);

	order = make_order(南京南, 泰安, G108, 硬座);
	BOOST_CHECK(order == 0);
	BOOST_CHECK(remain_tickets(上海虹桥, G108, 硬座) == 1);
	BOOST_CHECK(remain_tickets(南京南, G108, 硬座) == 0);
	BOOST_CHECK(remain_tickets(济南西, G108, 硬座) == 2);
	BOOST_CHECK(remain_tickets(廊坊, G108, 硬座) == 2);
		
	// 中间因为有南京南到泰安的票，因此就没票了，不过不知道铁道部的算法是不是这个样子
	order = make_order(上海虹桥, 济南西, G108, 硬座);
	BOOST_CHECK(order == -1);

	// 前后两段还是可以买票的
	order = make_order(上海虹桥, 南京南, G108, 硬座);
	BOOST_CHECK(remain_tickets(上海虹桥, G108, 硬座) == 0);
	BOOST_CHECK(order == 0);
	
	order = make_order(济南西, 北京南, G108, 硬座);
	BOOST_CHECK(order == 0);
	order = make_order(廊坊, 北京南, G108, 硬座);
	BOOST_CHECK(order == 0);
}