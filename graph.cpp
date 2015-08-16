#include "graph.h"
#include "graphedge.h"
#include "graphnode.h"

#include <QMutexLocker>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPen>

#include <gvc.h>
#include <cgraph.h>

static const double DEFAULT_DPI = 68.0;

namespace {
#define STRING_DATA(str) str.toLatin1().data()

inline Agraph_t *qg_agopen(const QString &name, Agdesc_t desc, Agdisc_t *disc)
{
    return agopen(STRING_DATA(name), desc, disc);
}

inline Agnode_t *qg_agnode(Agraph_t *g, const QString &name, int createflag)
{
    return agnode(g, STRING_DATA(name), createflag);
}

inline Agedge_t *qg_agedge(Agraph_t *g, Agnode_t *t, Agnode_t *h, const QString &name, int createflag)
{
    return agedge(g, t, h, STRING_DATA(name), createflag);
}

inline QString qg_agget(void *obj, const QString &name, const QString &defaultValue = QString())
{
    QString result = agget(obj, STRING_DATA(name));
    return result.isEmpty() ? defaultValue : result;
}

inline int qg_agset(void *obj, const QString &name, const QString &value)
{
    return agsafeset(obj, STRING_DATA(name), STRING_DATA(value), "");
}
}

using namespace QuickGraphviz;

Graph::Graph(QObject *parent)
    : QObject(parent)
{
}

void Graph::addNode(GraphNode *node)
{
    if (!node)
        return;
    QMutexLocker locker(&m_mutex);
    if (m_nodes.contains(node->nodeId()))
        m_nodes.take(node->nodeId())->deleteLater();
    node->setParent(this);
    m_nodes[node->nodeId()] = node;
}

void Graph::addEdge(GraphEdge *edge)
{
    if (!edge)
        return;
    QMutexLocker locker(&m_mutex);
    edge->setParent(this);
    m_edges << edge;
}

QSet<GraphEdge *> Graph::edges() const
{
    return m_edges;
}

QSet<GraphNode *> Graph::nodes() const
{
    return m_nodes.values().toSet();
}

void Graph::buildGraph()
{
    auto builder = [this]() {
        QMutexLocker locker(&m_mutex);
        GVC_t *context = gvContext();
        Agraph_t *graph = qg_agopen(m_name, Agundirected, NULL);
        qg_agset(graph, "overlap", "prizm");
        qg_agset(graph, "dpi", "120,0");
        qg_agset(graph, "splines", "true");

        QHash<QString, Agnode_t *> graphvizNodes;
        QSet<Agedge_t *> graphvizEdges;

        for (auto node : m_nodes.values()) {
            auto graphvizNode = qg_agnode(graph, node->nodeId(), TRUE);
            qg_agset(graphvizNode, "label", node->text());
            if (node->text().isEmpty()) {
                qg_agset(graphvizNode, "shape", "circle");
                qg_agset(graphvizNode, "style", "filled");
                qg_agset(graphvizNode, "width", "0.1");
                qg_agset(graphvizNode, "height", "0.1");
            }
            graphvizNodes[node->nodeId()] = graphvizNode;
        }

        for (auto edge : m_edges) {
            if (!graphvizNodes.contains(edge->sourceId()) || !graphvizNodes.contains(edge->targetId())) {
                qDebug() << Q_FUNC_INFO << "Edge" << edge->sourceId() << "to" << edge->targetId() << "not found, skipping";
                continue;
            }
            auto graphvizEdge = qg_agedge(graph, graphvizNodes[edge->sourceId()], graphvizNodes[edge->targetId()], "", TRUE);
            graphvizEdges << graphvizEdge;
        }

        gvLayout(context, graph, "neato");

        double graphDpi = qg_agget(graph, "dpi", "96,0").replace(",", ".").toDouble();
        double dpiFactor = graphDpi / DEFAULT_DPI;
        double width = qAbs(GD_bb(graph).LL.x - GD_bb(graph).UR.x) * dpiFactor;
        double height = qAbs(GD_bb(graph).LL.y - GD_bb(graph).UR.y) * dpiFactor;

        double xOffset = -GD_bb(graph).LL.x;
        double yOffset = -GD_bb(graph).LL.y;

        QImage graphImage(width, height, QImage::Format_ARGB32);
        graphImage.fill(Qt::white);
        {
            QPainter painter(&graphImage);
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            painter.setPen(QPen(QBrush(Qt::black), 2.0));
            for (auto node : graphvizNodes.values()) {
                QPointF nodeCenter((ND_coord(node).x + xOffset) * dpiFactor,
                                   height - ((ND_coord(node).y + yOffset) * dpiFactor));
                double width = ND_width(node) * graphDpi;
                double height = ND_height(node) * graphDpi;
                QRectF nodeRect(nodeCenter.x() - width / 2,
                                nodeCenter.y() - height / 2,
                                width, height);
                painter.drawEllipse(nodeRect);
                painter.drawText(nodeRect, Qt::AlignCenter, ND_label(node)->text);
            }
            for (auto edge : graphvizEdges.values()) {
                QPainterPath path;
                splines *splinesList = ED_spl(edge);
                qDebug() << splinesList->list;
                if (!splinesList->list)
                    continue;
                bezier bezierInfo = splinesList->list[0];
                qDebug() << bezierInfo.sflag << bezierInfo.eflag << bezierInfo.size;
                if (bezierInfo.sflag) {
                    path.moveTo((bezierInfo.sp.x + xOffset) * dpiFactor,
                                height - ((bezierInfo.sp.y + yOffset) * dpiFactor));
                    path.lineTo((bezierInfo.list[0].x + xOffset) * dpiFactor,
                                height - ((bezierInfo.list[0].y + yOffset) * dpiFactor));
                } else {
                    path.moveTo((bezierInfo.list[0].x + xOffset) * dpiFactor,
                                height - ((bezierInfo.list[0].y + yOffset) * dpiFactor));
                }
                for (int i = 1; (i + 2) < bezierInfo.size; i += 3) {
                    QPointF firstPoint((bezierInfo.list[i].x + xOffset) * dpiFactor,
                                        height - ((bezierInfo.list[i].y + yOffset) * dpiFactor));
                    QPointF secondPoint((bezierInfo.list[i + 1].x + xOffset) * dpiFactor,
                                        height - ((bezierInfo.list[i + 1].y + yOffset) * dpiFactor));
                    QPointF endPoint((bezierInfo.list[i + 2].x + xOffset) * dpiFactor,
                                        height - ((bezierInfo.list[i + 2].y + yOffset) * dpiFactor));
                    path.cubicTo(firstPoint, secondPoint, endPoint);
                }
                painter.drawPath(path);
            }
        }

        emit graphBuilt(graphImage);

        for (auto edge : graphvizEdges.values())
            agdelete(graph, edge);
        for (auto node : graphvizNodes.values())
            agdelete(graph, node);
        gvFreeLayout(context, graph);
        agclose(graph);
        gvFreeContext(context);
    };
    builder();
}

QString Graph::name() const
{
    return m_name;
}

void Graph::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

