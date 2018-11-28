#include <gtest/gtest.h>
#include "mapper.h"

TEST(MapperTest, FindRoute) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 2.0);
    mapper.addRoute("alice", "charlie", 5.0);
    mapper.addRoute("bob", "charlie", 1.0);
    mapper.addRoute("david", "elizabeth", 1.0);

    ASSERT_EQ(mapper.nextNode("alice", "charlie"), "bob");
    ASSERT_EQ(mapper.nextNode("bob", "bob"), "bob");
    ASSERT_THROW(mapper.nextNode("alice", "david"), decltype(mapper)::NoRouteException);
}

TEST(MapperTest, AddRouteSameNode) {
    Mapper mapper;
    ASSERT_THROW(mapper.addRoute("alice", "alice", 1.0), decltype(mapper)::CyclicRouteException);
}

TEST(MapperTest, RemoveRouteSuccess) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 5.0);
    mapper.removeRoute("alice", "bob");
}

TEST(MapperTest, RemoveRouteFailure) {
    Mapper mapper;
    ASSERT_THROW(mapper.removeRoute("alice", "bob"), decltype(mapper)::NoNodeException);
}
