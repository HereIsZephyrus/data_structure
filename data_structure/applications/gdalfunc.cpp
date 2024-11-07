//
//  gdalfunc.cpp
//  data_structure
//
//  Created by ChanningTong on 11/7/24.
//
#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/triangulate.h>
#include <CGAL/Polygon_2.h>
#include "gdalfunc.hpp"
using std::string;
using std::vector;

using singlegeo = std::vector<glm::vec2>;
using K = CGAL::Simple_cartesian<double>;
//using Polygon2 = CGAL::Polygon_2<CGAL::Simple_cartesian<double> >;
void extractVerticesFromGeometry(OGRGeometry* geom, std::vector< std::vector<glm::vec2> >& vertices) {
    OGRwkbGeometryType type = geom->getGeometryType();
    std::cout<<type<<std::endl;
    switch (type) {
        case wkbPoint:
            extractVerticesPoint(geom,vertices);
            break;
        case wkbLineString:
            extractVerticesLine(geom,vertices);
            break;
        case wkbPolygon:
            extractVerticesPolygon(geom,vertices);
            break;
        case wkbMultiPoint:
            extractVerticesMultiPoint(geom,vertices);
            break;
        case wkbMultiLineString:
            extractVerticesMultiLine(geom,vertices);
            break;
        case wkbMultiPolygon:
            extractVerticesMultiPolygon(geom,vertices);
            break;
        default:
            std::cout<<"unknown type"<<std::endl;
            break;
    }
    if (geom->getGeometryType() == wkbPolygon) {
        
    } else if (geom->getGeometryType() == wkbMultiPoint) {
        // 处理多点
        OGRMultiPoint* multiPoint = static_cast<OGRMultiPoint*>(geom);
        for (int i = 0; i < multiPoint->getNumGeometries(); ++i) {
            OGRPoint* point = static_cast<OGRPoint*>(multiPoint->getGeometryRef(i));
            vertices.push_back({point->getX(), point->getY()});
        }
    } else if (geom->getGeometryType() == wkbMultiLineString) {
        // 处理多线
        OGRMultiLineString* multiLine = static_cast<OGRMultiLineString*>(geom);
        for (int i = 0; i < multiLine->getNumGeometries(); ++i) {
            OGRLineString* line = static_cast<OGRLineString*>(multiLine->getGeometryRef(i));
            for (int j = 0; j < line->getNumPoints(); ++j) {
                OGRPoint* point = nullptr;
                line->getPoint(i, point);
                vertices.push_back({point->getX(), point->getY()});
            }
        }
    } else if (geom->getGeometryType() == wkbMultiPolygon) {
        // 处理多多边形
        OGRMultiPolygon* multiPolygon = static_cast<OGRMultiPolygon*>(geom);
        for (int i = 0; i < multiPolygon->getNumGeometries(); ++i) {
            OGRPolygon* polygon = static_cast<OGRPolygon*>(multiPolygon->getGeometryRef(i));
            OGRLinearRing* exteriorRing = polygon->getExteriorRing();
            if (exteriorRing) {
                for (int j = 0; j < exteriorRing->getNumPoints(); ++j) {
                    OGRPoint* point = nullptr;
                    exteriorRing->getPoint(i, point);
                    vertices.push_back({point->getX(), point->getY()});
                }
            }
            // 提取内环
            for (int j = 0; j < polygon->getNumInteriorRings(); ++j) {
                OGRLinearRing* interiorRing = polygon->getInteriorRing(j);
                if (interiorRing) {
                    for (int k = 0; k < interiorRing->getNumPoints(); ++k) {
                        OGRPoint* point = nullptr;
                        interiorRing->getPoint(i, point);
                        vertices.push_back({point->getX(), point->getY()});
                    }
                }
            }
        }
    }
}
void extractVerticesPoint(OGRGeometry* geom,std::vector<singlegeo>&vertices){
    OGRPoint* point = static_cast<OGRPoint*>(geom);
    singlegeo v1{glm::vec2(point->getX(), point->getY())};
    vertices.push_back(v1);
}
void extractVerticesLine(OGRGeometry* geom,std::vector<singlegeo>&vertices){
    OGRLineString* line = static_cast<OGRLineString*>(geom);
    singlegeo v1;
    for (int i = 0; i < line->getNumPoints(); ++i) {
        OGRPoint* point = nullptr;
        line->getPoint(i, point);
        v1.push_back(glm::vec2(point->getX(), point->getY()));
    }
    vertices.push_back(v1);
}
void extractVerticesPolygon(OGRGeometry*geom,std::vector<singlegeo>& vertices){
    OGRPolygon* polygon = static_cast<OGRPolygon*>(geom);
    OGRLinearRing* exteriorRing = polygon->getExteriorRing();
    Polygon_2 cgalPolygon;
    for (int i = 0; i < outerRing->getNumPoints(); ++i) {
        OGRPoint point = outerRing->getPoint(i);
        cgalPolygon.push_back(K::Point_2(point.getX(), point.getY()));
    }

    // 处理内环
    for (int j = 0; j < ogrPolygon->getNumInteriorRings(); ++j) {
        const OGRLinearRing* innerRing = ogrPolygon->getInteriorRing(j);
        Polygon_2 hole;
        for (int k = 0; k < innerRing->getNumPoints(); ++k) {
            OGRPoint point = innerRing->getPoint(k);
            hole.push_back(K::Point_2(point.getX(), point.getY()));
        }
        cgalPolygon.add_hole(hole);
    }

    // 进行三角剖分
    std::vector<K::Triangle_2> triangles;
    CGAL::triangulate(cgalPolygon, std::back_inserter(triangles));
}
void extractVerticesMultiPoint(OGRGeometry*geom,std::vector<singlegeo>& vertices){

}
void extractVerticesMultiLine(OGRGeometry*geom,std::vector<singlegeo>& vertices){

}
void extractVerticesMultiPolygon(OGRGeometry*geom,std::vector<singlegeo>& vertices){
    
}
