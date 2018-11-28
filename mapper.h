#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <utility>
#include <unordered_map>

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
        std::unordered_map<Node, std::unordered_map<Node, Cost> > adjacencyList;
};

#endif // MAPPER_H
