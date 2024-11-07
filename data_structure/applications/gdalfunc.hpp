//
//  gdalfunc.hpp
//  data_structure
//
//  Created by ChanningTong on 11/7/24.
//

#ifndef gdalfunc_hpp
#define gdalfunc_hpp
#include <vector>
#include <cstring>
#include <string>
#include <gdal.h>
#include <gdal_priv.h>
#include <gdal_utils.h>
#include <ogrsf_frmts.h>
#include "../OpenGL/graphing.hpp"
void extractVerticesPoint(OGRGeometry* geom,std::vector<std::vector<glm::vec2>>& vertices);
void extractVerticesLine(OGRGeometry* geom,std::vector<std::vector<glm::vec2>>& vertices);
void extractVerticesPolygon(OGRGeometry* geom,std::vector<std::vector<glm::vec2>>& vertices);
void extractVerticesMultiPoint(OGRGeometry* geom,std::vector<std::vector<glm::vec2>>& vertices);
void extractVerticesMultiLine(OGRGeometry* geom,std::vector<std::vector<glm::vec2>>& vertices);
void extractVerticesMultiPolygon(OGRGeometry* geom,std::vector<std::vector<glm::vec2>>& vertices);
void extractVerticesFromGeometry(OGRGeometry* geom, std::vector<glm::vec2>& vertices);
#endif /* gdalfunc_hpp */
