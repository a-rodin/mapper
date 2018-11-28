#include "mapper.h"
#include <functional>
#include <limits>
#include <cmath>
#include <boost/heap/binomial_heap.hpp>

void Mapper::addRoute(const Mapper::Node &node1, const Mapper::Node &node2, Mapper::Cost cost) {
    if (node1 == node2) {
        throw CyclicRouteException();
    }
    adjacencyList[node1][node2] = cost;
    adjacencyList[node2][node1] = cost;
}

void Mapper::removeRoute(const Mapper::Node &node1, const Mapper::Node &node2) {
    auto node1Iterator = adjacencyList.find(node1);
    if (node1Iterator == adjacencyList.end()) {
        throw NoNodeException();
    }
    auto node2Iterator = node1Iterator->second.find(node2);
    if (node2Iterator == node1Iterator->second.end()) {
        throw NoNodeException();
    }
    node1Iterator->second.erase(node2Iterator);
    if (node1Iterator->second.empty()) {
        adjacencyList.erase(node1Iterator);
    }

    node1Iterator = adjacencyList.find(node2);
    node2Iterator = node1Iterator->second.find(node1);
    node1Iterator->second.erase(node2Iterator);
    if (node1Iterator->second.empty()) {
        adjacencyList.erase(node1Iterator);
    }
}

Mapper::Node Mapper::nextNode(const Mapper::Node &source, const Mapper::Node &dest) {
    if (source == dest) {
        return dest;
    }

    typedef std::pair<Cost, Node> WaitingPair;
    boost::heap::binomial_heap<WaitingPair,
        boost::heap::compare<std::greater<WaitingPair> > > waitingHeap;
    std::unordered_map<Node, decltype(waitingHeap)::handle_type> waitingHandles;
    std::unordered_map<Node, Cost> costs;
    std::unordered_map<Node, Node> prev;

    costs[source] = 0.0;
    for (auto i: adjacencyList) {
        if (i.first != source) {
            costs[i.first] = std::numeric_limits<Cost>::infinity();
        }
        waitingHandles[i.first] = waitingHeap.push(WaitingPair(costs[i.first], i.first));
    }

    while (!waitingHeap.empty()) {
        auto node = waitingHeap.top().second;
        waitingHeap.pop();
        waitingHandles.erase(waitingHandles.find(node));

        for (auto i: adjacencyList[node]) {
            if (waitingHandles.find(i.first) == waitingHandles.end()) {
                continue;
            }
            auto alt = costs[node] + i.second;
            if (alt < costs[i.first]) {
                costs[i.first] = alt;
                prev[i.first] = node;
                waitingHeap.update(waitingHandles[i.first], WaitingPair(alt, i.first));
            }
        }
    }
    if (std::isinf(costs[dest])) {
        throw NoRouteException();
    }
    auto current = dest;
    while (prev[current] != source) {
        current = prev[current];
    }
    return current;
}
