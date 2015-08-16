#include <QGuiApplication>

#include "graph.h"
#include "graphedge.h"
#include "graphnode.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QuickGraphviz::Graph graph;
    QuickGraphviz::GraphNode node1("first");
    QuickGraphviz::GraphNode node2("second");
    QuickGraphviz::GraphNode node3("third");
    QuickGraphviz::GraphNode node4("fourth");
    QuickGraphviz::GraphNode node5("fifth");
    QuickGraphviz::GraphNode node6("sixth");
    QuickGraphviz::GraphNode node7("seventh");
    QuickGraphviz::GraphNode node8("eighth");
    QuickGraphviz::GraphNode node9("nineth");
    QuickGraphviz::GraphNode node10("tenth");
    QuickGraphviz::GraphEdge edge1("first", "second");
    QuickGraphviz::GraphEdge edge2("first", "third");
    QuickGraphviz::GraphEdge edge3("first", "fourth");
    QuickGraphviz::GraphEdge edge4("first", "fifth");
    QuickGraphviz::GraphEdge edge5("first", "sixth");
    QuickGraphviz::GraphEdge edge6("first", "seventh");
    QuickGraphviz::GraphEdge edge7("first", "eighth");
    QuickGraphviz::GraphEdge edge8("first", "nineth");
    QuickGraphviz::GraphEdge edge9("first", "tenth");
    QuickGraphviz::GraphEdge edge10("tenth", "sixth");
    QuickGraphviz::GraphEdge edge11("third", "nineth");
    QuickGraphviz::GraphEdge edge12("tenth", "nineth");
    QuickGraphviz::GraphEdge edge13("eighth", "fourth");
    QuickGraphviz::GraphEdge edge14("sixth", "third");
    QuickGraphviz::GraphEdge edge15("second", "third");
    graph.addNode(&node1);
    graph.addNode(&node2);
    graph.addNode(&node3);
    graph.addNode(&node4);
    graph.addNode(&node5);
    graph.addNode(&node6);
    graph.addNode(&node7);
    graph.addNode(&node8);
    graph.addNode(&node9);
    graph.addNode(&node10);
    graph.addEdge(&edge1);
    graph.addEdge(&edge2);
    graph.addEdge(&edge15);
    graph.addEdge(&edge3);
    graph.addEdge(&edge4);
    graph.addEdge(&edge5);
    graph.addEdge(&edge6);
    graph.addEdge(&edge7);
    graph.addEdge(&edge8);
    graph.addEdge(&edge9);
    graph.addEdge(&edge10);
    graph.addEdge(&edge11);
    graph.addEdge(&edge12);
    graph.addEdge(&edge13);
    graph.addEdge(&edge14);
    graph.buildGraph();
    return 0;

    return a.exec();
}

