#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <utility>
#include <unordered_map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

class Mapper {
    public:
        typedef std::string Node;
        typedef double Cost;

        class CyclicRouteException: public std::exception {
            const char *what() const noexcept { return "Cyclic route"; }
        };

        class NoRouteException: public std::exception {
            const char *what() const noexcept { return "No route"; }
        };

        class NoNodeException: public std::exception {
            const char *what() const noexcept { return "No node"; }
        };

        Mapper() {}
        virtual ~Mapper() {}

        void addRoute(const Node &node1, const Node &node2, Cost cost);
        void removeRoute(const Node &node1, const Node &node2);

        Node nextNode(const Node &source, const Node &dest);
    private:
        boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
            boost::no_property, boost::property<boost::edge_weight_t, Cost> > adjacencyList;
        typedef boost::graph_traits<decltype(adjacencyList)>::vertex_descriptor VertexDescriptor;
        std::unordered_map<Node, VertexDescriptor> nodesMapping;
        std::unordered_map<VertexDescriptor, Node> nodesReverseMapping;
};

#endif // MAPPER_H
