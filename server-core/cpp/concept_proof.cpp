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

// adjacency_list<> _graph(SITECOUNT);
typedef adjacency_list<vecS, vecS, bidirectionalS, int, int> Graph;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIter;
typedef graph_traits<Graph>::in_edge_iterator InEdgeIter;

/*
typedef property_map<Graph, edge_index_t>::type EdgeIndexMap;
typedef property_map<Graph, vertex_index_t>::type VertexIndexMap;
尚未完成！
typedef iterator_property_map<
*/

Graph _graph(SITECOUNT);
int _ticket_distribution[SITECOUNT][TRAINCOUNT];
std::vector<std::vector<int> *> _same_city(SITECOUNT);

bool _inited = false;

void Init()
{
	if ( !_inited )
	{
		memset(&_ticket_distribution, 0, sizeof(_ticket_distribution));
		_ticket_distribution[北京南][G107] = 100;
		_ticket_distribution[德州东][G107] = 100;
		_ticket_distribution[济南西][G107] = 100;
		_ticket_distribution[泰安][G107] = 100;
		_ticket_distribution[曲阜东][G107] = 100;
		_ticket_distribution[蚌埠南][G107] = 100;
		_ticket_distribution[南京南][G107] = 100;
		_ticket_distribution[丹阳北][G107] = 100;
		_ticket_distribution[上海虹桥][G107] = 100;

		add_edge(北京南, 德州东, G107, _graph);
		add_edge(德州东, 济南西, G107, _graph);
		add_edge(济南西, 泰安, G107, _graph);
		add_edge(泰安, 曲阜东, G107, _graph);
		add_edge(曲阜东, 蚌埠南, G107, _graph);
		add_edge(蚌埠南, 南京南, G107, _graph);
		add_edge(南京南, 丹阳北, G107, _graph);
		add_edge(丹阳北, 上海虹桥, G107, _graph);
	
		_ticket_distribution[上海虹桥][G108] = 100;
		_ticket_distribution[无锡东][G108] = 100;
		_ticket_distribution[南京南][G108] = 100;
		_ticket_distribution[定远][G108] = 100;
		_ticket_distribution[泰安][G108] = 100;
		_ticket_distribution[济南西][G108] = 100;
		_ticket_distribution[廊坊][G108] = 100;
		_ticket_distribution[北京南][G108] = 100;
	
		add_edge(上海虹桥, 无锡东, G107, _graph);
		add_edge(无锡东, 南京南, G107, _graph);
		add_edge(南京南, 定远, G107, _graph);
		add_edge(定远, 泰安, G107, _graph);
		add_edge(泰安, 济南西, G107, _graph);
		add_edge(济南西, 廊坊, G107, _graph);
		add_edge(廊坊, 北京南, G107, _graph);

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

void dump_graph()
{
	/*
	graph_traits<Graph>::vertex_iterator v, v_end;
    for (tie(v,v_end) = vertices(_graph); v != v_end; ++v)
        put(vertex_color_t(), _graph, *v, _site_names[*v]);

    graph_traits<Graph>::edge_iterator e, e_end;
    for (tie(e,e_end) = edges(_graph); e != e_end; ++e)
        put(edge_weight_t(), _graph, *e, 3);

    dynamic_properties dp;
    dp.property("name", get(vertex_color_t(), _graph));
    dp.property("weight", get(edge_weight_t(), _graph));
	*/

    write_graphviz(std::cout, _graph, make_label_writer(_site_names));
}

BOOST_AUTO_TEST_CASE(演示查票方法)
{
	Init();

	std::vector<int> *pResult = query(北京, 上海);
	BOOST_CHECK(pResult->size() == 1);
	BOOST_CHECK(pResult->at(0) == G107);

	delete pResult;

	dump_graph();
}