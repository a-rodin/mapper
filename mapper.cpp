#include "mapper.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

void Mapper::addRoute(const Mapper::Node &node1, const Mapper::Node &node2, Mapper::Cost cost) {
    if (node1 == node2) {
        throw CyclicRouteException();
    }
    if (nodesMapping.find(node1) == nodesMapping.end()) {
        auto vertexDescriptor = add_vertex(adjacencyList);
        nodesMapping[node1] = vertexDescriptor;
        nodesReverseMapping.push_back(node1);
    }
    if (nodesMapping.find(node2) == nodesMapping.end()) {
        auto vertexDescriptor = add_vertex(adjacencyList);
        nodesMapping[node2] = vertexDescriptor;
        nodesReverseMapping.push_back(node2);
    }

    if (boost::edge(nodesMapping[node1], nodesMapping[node2], adjacencyList).second) {
        throw RouteAlreadyExistsException();
    }

    add_edge(nodesMapping[node1], nodesMapping[node2], cost, adjacencyList);
}

void Mapper::removeRoute(const Mapper::Node &node1, const Mapper::Node &node2) {
    if (nodesMapping.find(node1) == nodesMapping.end()) {
        throw NoNodeException();
    }
    if (nodesMapping.find(node2) == nodesMapping.end()) {
        throw NoNodeException();
    }
    auto e = boost::edge(nodesMapping[node1], nodesMapping[node2], adjacencyList);
    if (!e.second) {
        throw NoRouteException();
    }
    remove_edge(e.first, adjacencyList);

    boost::graph_traits<decltype(adjacencyList)>::adjacency_iterator vi, viEnd;
    boost::tie(vi, viEnd) = adjacent_vertices(nodesMapping[node1], adjacencyList);
    if (vi == viEnd) {
        removeNode(node1);
    }
    boost::tie(vi, viEnd) = adjacent_vertices(nodesMapping[node2], adjacencyList);
    if (vi == viEnd) {
        removeNode(node2);
    }
}

void Mapper::removeNode(const Mapper::Node &node) {
    auto vertexDescriptor = nodesMapping[node];
    remove_vertex(vertexDescriptor, adjacencyList);

    // invalidate all vertex descriptors
    for (auto i = vertexDescriptor + 1; i < nodesReverseMapping.size(); ++i) {
        nodesReverseMapping[i - 1] = nodesReverseMapping[i];
        nodesMapping[nodesReverseMapping[i - 1]]--;
    }
    nodesReverseMapping.pop_back();
    nodesMapping.erase(nodesMapping.find(node));
}

Mapper::Node Mapper::nextNode(const Mapper::Node &source, const Mapper::Node &dest) {
    if (nodesMapping.find(source) == nodesMapping.end()) {
        throw NoNodeException();
    }
    if (nodesMapping.find(dest) == nodesMapping.end()) {
        throw NoNodeException();
    }

    std::vector<VertexDescriptor> p(num_vertices(adjacencyList));
    std::vector<int> d(num_vertices(adjacencyList));
    VertexDescriptor sourceNode = nodesMapping[source];

    dijkstra_shortest_paths(adjacencyList, sourceNode,
            predecessor_map(boost::make_iterator_property_map(p.begin(),
                    get(boost::vertex_index, adjacencyList))).
            distance_map(boost::make_iterator_property_map(d.begin(),
                    get(boost::vertex_index, adjacencyList))));

    VertexDescriptor currentNode = nodesMapping[dest];
    while (p[currentNode] != sourceNode) {
        if (currentNode == p[currentNode]) {
            throw NoRouteException();
        }
        currentNode = p[currentNode];
    }
    return nodesReverseMapping[currentNode];
}
