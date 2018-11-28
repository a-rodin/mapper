#include <gtest/gtest.h>
#include "mapper.h"

TEST(MapperTest, FindRouteFullyConnected) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);
    mapper.addRoute("alice", "charlie", 1.0);
    mapper.addRoute("alice", "david", 1.0);
    mapper.addRoute("bob", "charlie", 1.0);
    mapper.addRoute("bob", "david", 1.0);
    mapper.addRoute("charlie", "david", 1.0);

    ASSERT_EQ(mapper.nextNode("alice", "bob"), "bob");
    ASSERT_EQ(mapper.nextNode("alice", "charlie"), "charlie");
    ASSERT_EQ(mapper.nextNode("alice", "david"), "david");
    ASSERT_EQ(mapper.nextNode("bob", "charlie"), "charlie");
    ASSERT_EQ(mapper.nextNode("bob", "david"), "david");
    ASSERT_EQ(mapper.nextNode("charlie", "david"), "david");
}

TEST(MapperTest, FindRouteDisconnected) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);
    mapper.addRoute("charlie", "david", 1.0);

    ASSERT_THROW(mapper.nextNode("alice", "david"), decltype(mapper)::NoRouteException);
}

TEST(MapperTest, FindRouteSameNode) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);
    mapper.addRoute("bob", "charlie", 1.0);
    mapper.addRoute("charlie", "alice", 1.0);

    ASSERT_EQ(mapper.nextNode("alice", "alice"), "alice");
}

TEST(MapperTest, FindRouteNoSource) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);

    ASSERT_THROW(mapper.nextNode("charlie", "alice"), decltype(mapper)::NoNodeException);
}

TEST(MapperTest, FindRouteNoDest) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);

    ASSERT_THROW(mapper.nextNode("alice", "charlie"), decltype(mapper)::NoNodeException);
}

TEST(MapperTest, FindRoute1) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 3.0);
    mapper.addRoute("bob", "charlie", 2.0);
    mapper.addRoute("alice", "charlie", 1.0);

    ASSERT_EQ(mapper.nextNode("alice", "charlie"), "charlie");
}

TEST(MapperTest, FindRoute2) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 3.0);
    mapper.addRoute("bob", "charlie", 2.0);
    mapper.addRoute("alice", "charlie", 10.0);

    ASSERT_EQ(mapper.nextNode("alice", "charlie"), "bob");
}

TEST(MapperTest, AddRouteSameNode) {
    Mapper mapper;
    ASSERT_THROW(mapper.addRoute("alice", "alice", 1.0), decltype(mapper)::CyclicRouteException);
}

TEST(MapperTest, AddRouteTwice) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);

    ASSERT_THROW(mapper.addRoute("alice", "bob", 2.0), decltype(mapper)::RouteAlreadyExistsException);
}

TEST(MapperTest, RemoveRouteKeepAnotherRoute) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);
    mapper.addRoute("bob", "charlie", 1.0);
    mapper.addRoute("charlie", "alice", 1.0);
    mapper.removeRoute("alice", "bob");

    ASSERT_EQ(mapper.nextNode("alice", "bob"), "charlie");
}

TEST(MapperTest, RemoveRouteKeepDisconnected) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);
    mapper.addRoute("charlie", "david", 1.0);
    mapper.addRoute("alice", "david", 1.0);
    mapper.removeRoute("alice", "david");

    ASSERT_THROW(mapper.nextNode("alice", "david"), decltype(mapper)::NoRouteException);
}

TEST(MapperTest, RemoveRouteWithNodes) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 5.0);
    mapper.addRoute("charlie", "david", 3.0);
    mapper.addRoute("elizabeth", "fred", 2.0);
    mapper.removeRoute("alice", "bob");

    ASSERT_THROW(mapper.nextNode("alice", "alice"), decltype(mapper)::NoNodeException);
    ASSERT_THROW(mapper.nextNode("bob", "bob"), decltype(mapper)::NoNodeException);
    ASSERT_EQ(mapper.nextNode("charlie", "charlie"), "charlie");
    ASSERT_EQ(mapper.nextNode("david", "david"), "david");
    ASSERT_EQ(mapper.nextNode("charlie", "david"), "david");
    ASSERT_EQ(mapper.nextNode("elizabeth", "fred"), "fred");
}

TEST(MapperTest, RemoveRouteNoFirstNode) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);

    ASSERT_THROW(mapper.removeRoute("charlie", "bob"), decltype(mapper)::NoNodeException);
}

TEST(MapperTest, RemoveRouteNoSecondNode) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);

    ASSERT_THROW(mapper.removeRoute("alice", "charlie"), decltype(mapper)::NoNodeException);
}

TEST(MapperTest, RemoveAndAdd) {
    Mapper mapper;
    mapper.addRoute("alice", "bob", 1.0);
    mapper.removeRoute("alice", "bob");
    mapper.addRoute("alice", "bob", 2.0);

    ASSERT_EQ(mapper.nextNode("alice", "bob"), "bob");
}
