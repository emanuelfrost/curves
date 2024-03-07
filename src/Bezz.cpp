#include "Bezz.h"
#include "Linear.h"
#include "Cubic.h"

ofPoint Bezz::getRotatedLine(ofPoint start, double len, double angle){
    double x = start.x + len * glm::cos(angle);
    double y = start.y + len * glm::sin(angle);

    ofPoint t;
    t.x = x;
    t.y = y;

    return t;
}

//https://github.com/jesusgollonet/ofpennereasing/blob/master/PennerEasing/Linear.h
//https://www.independent-software.com/determining-coordinates-on-a-html-canvas-bezier-curve.html
double Bezz::getBezierAngleInRadians(double t, double sx, double sy, double cp1x, double cp1y, double cp2x, double cp2y, double ex, double ey){
    double dx = glm::pow(1 - t, 2) * (cp1x - sx) + 2 * t * (1 - t) * (cp2x - cp1x) + t * t * (ex - cp2x);
    double dy = glm::pow(1 - t, 2) * (cp1y - sy) + 2 * t * (1 - t) * (cp2y - cp1y) + t * t * (ey - cp2y);
    return -atan2(dx, dy) + 0.5 * PI;
}

// bool Bezz::isBezierPointLeftEdgeIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints){
//     for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
//         for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
//             const BezierPoint& b = *points;
            
//             if(b == point)
//                 continue;

//             ofPoint intersectionPoint;
//             bool intersects = ofLineSegmentIntersection(point.pos, point.leftEdge, b.pos, b.leftEdge, intersectionPoint);

//             if(intersects == true){
//                 //ofLog(OF_LOG_NOTICE, "bezier point intersects");
//                 return true;
//             }
//         }
//     }
//     return false;
// }

// bool Bezz::isBezierPointRightEdgeIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints){
//     for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
//         for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
//             const BezierPoint& b = *points;
            
//             if(b == point)
//                 continue;

//             ofPoint intersectionPoint;
//             bool intersects = ofLineSegmentIntersection(point.pos, point.rightEdge, b.pos, b.rightEdge, intersectionPoint);

//             if(intersects == true){
//                 //ofLog(OF_LOG_NOTICE, "bezier point intersects");
//                 return true;
//             }
//         }
//     }
//     return false;
// }


// bool Bezz::isBezierPointLeftPolylineIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints){
//     for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
//         for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
//             const BezierPoint& b = *points;
            
//             if(b == point)
//                 continue;

//             ofPoint intersectionPoint;
//             bool intersects = isPolylineIntersecting(point.leftPoly, b.leftPoly);

//             if(intersects == true){
//                 //ofLog(OF_LOG_NOTICE, "bezier polyline intersects");
//                 return true;
//             }
//         }
//     }
//     return false;
// }


// bool Bezz::isBezierPointRightPolylineIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints){
//     for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
//         for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
//             const BezierPoint& b = *points;
            
//             if(b == point)
//                 continue;

//             ofPoint intersectionPoint;
//             bool intersects = isPolylineIntersecting(point.rightPoly, b.rightPoly);

//             if(intersects == true){
//                 //ofLog(OF_LOG_NOTICE, "bezier polyline intersects");
//                 return true;
//             }
//         }
//     }
//     return false;
// }






float Bezz::getLineWidth(float p, float widthAlgorithm, float minWidth, float baseWidth, float widthSmoothing){
    float widthDiff = minWidth-baseWidth;
    float w = ofLerp(Linear::easeIn(p, baseWidth, widthDiff, 1.0), Cubic::easeIn(p, baseWidth, widthDiff, 1.0), widthSmoothing);
    return w;
}




Line Bezz::buildSegmentLine(int index, int noOfSegments, const ofRectangle& drawingArea, float curveSmoothing){
    float p = index/(float)((int)noOfSegments);
    float y = ofLerp(Linear::easeIn(p, drawingArea.y, drawingArea.getHeight(), 1.0), Cubic::easeIn(p, drawingArea.y, drawingArea.getHeight(), 1.0), curveSmoothing);
    ofPoint start = ofPoint(drawingArea.x, y);
    ofPoint end = ofPoint(drawingArea.x+drawingArea.getWidth(), y);

    Line lineTemp;
    lineTemp.start = start;
    lineTemp.end = end;
    lineTemp.p = p;

    return lineTemp;
}

//TODO smooth this width consideration
Bezier Bezz::buildSegmentBezier(int totalNoOfSegments, int index, const Line& current, float currentWidth, const Line& next, const float nextWidth){
    bool isEven = index % 2 == 0;

    const ofPoint& startPoint = (isEven) ? current.start : current.end;
    const ofPoint& controlPoint1 = (isEven) ? next.start : next.end;
    const ofPoint& controlPoint2 = (isEven) ? current.end : current.start;
    const ofPoint& endPoint = (isEven) ? next.end : next.start;
    
    bool isFirst = current.p == 0.0;
    bool isLast = (next.p == 1.0);

    ofPoint startPointWidth = startPoint;
    startPointWidth.x = startPointWidth.x + ((isEven) ? currentWidth : -currentWidth);
    

    //if(isFirst == true){
        //ofLog(OF_LOG_NOTICE, "top");
    //    startPointWidth.y = startPointWidth.y + currentWidth;
    //}

    ofPoint controlPoint1Width = controlPoint1;
    controlPoint1Width.x = controlPoint1Width.x + ((isEven) ? currentWidth : -currentWidth);
    

    ofPoint endPointWidth = endPoint;
    endPointWidth.x = endPointWidth.x + ((isEven) ? -nextWidth : nextWidth);
    

    //ofLog(OF_LOG_NOTICE, ofToString(index) + " " + ofToString(totalNoOfSegments));
    //if(isLast == true){  
    //    endPointWidth.y = endPointWidth.y - currentWidth;
    //}

    ofPoint controlPoint2Width = controlPoint2;
    controlPoint2Width.x = controlPoint2Width.x + ((isEven) ? -nextWidth : nextWidth);


    Bezier bezierTemp;
    bezierTemp.start = startPointWidth;
    bezierTemp.controlPoint1 = controlPoint1Width;
    bezierTemp.controlPoint2 = controlPoint2Width;
    bezierTemp.end = endPointWidth;

    return bezierTemp;
}

BezierPoint Bezz::buildBezierPoint(float pointP, int index, const Bezier& b, float t, float width){
    glm::vec3 pos = ofBezierPoint(b.start, b.controlPoint1, b.controlPoint2, b.end, t);
    ofPoint bezierPointPosition = ofPoint(pos.x, pos.y);
    double bezierPointAngle = getBezierAngleInRadians(t, b.start.x, b.start.y, b.controlPoint1.x, b.controlPoint1.y, b.controlPoint2.x, b.controlPoint2.y, b.end.x, b.end.y);

    // float leftAngle = bezierPointAngle + ofDegToRad(90);
    // float rightAngle = bezierPointAngle - ofDegToRad(90);

    // ofPoint leftEdge = getRotatedLine(bezierPointPosition, width, leftAngle);
    // ofPoint rightEdge = getRotatedLine(bezierPointPosition, width, rightAngle);

    //ofPoint centerLeft = getRotatedLine(bezierPointPosition, width/2.0, leftAngle);
    //ofPoint centerRight = getRotatedLine(bezierPointPosition, width/2.0, rightAngle);
    
    //ofPolyline leftPoly = buildRotatedRectangle(centerLeft, leftAngle, width, intersectsWidth);
    //ofPolyline rightPoly = buildRotatedRectangle(centerRight, rightAngle, width, intersectsWidth);
    
    BezierPoint bezierPointTemp;
    bezierPointTemp.index = index;
    bezierPointTemp.t = t;
    bezierPointTemp.pos = bezierPointPosition;
    bezierPointTemp.angle = bezierPointAngle;
    bezierPointTemp.p = pointP;
    // bezierPointTemp.leftEdge = leftEdge;
    // bezierPointTemp.rightEdge = rightEdge;
    //bezierPointTemp.leftPoly = leftPoly;
    //bezierPointTemp.rightPoly = rightPoly;

    return bezierPointTemp;
}

vector<BezierPoint> Bezz::buildBezierPoints(int bezierIndex, int noOfBeziers, int noOfSegmentPoints, const Bezier& b, float width){
    vector<BezierPoint> points; 

    int totalSegmentPoints = (noOfSegmentPoints * noOfBeziers);

    for(unsigned int p = 0; p < noOfSegmentPoints; p++) {
    
        double t = (p/((double)(noOfSegmentPoints-1)));

        int indexesSoFar = (bezierIndex*noOfSegmentPoints);
        int currentIndex = (p+1)+indexesSoFar;
        
        double i = ofMap(currentIndex, 1, totalSegmentPoints, 0.0, 1.0);

        //ofLog(OF_LOG_NOTICE, "indx: " + ofToString(currentIndex) + " percentage " + ofToString(i) + " t: " + ofToString(t));
        
        BezierPoint bezierPointTemp = buildBezierPoint(i, p, b, t, width);

        points.push_back(bezierPointTemp);
    }

    return points;
}

// ofPolyline Bezz::buildClosedBezierPolyline(vector<vector<BezierPoint>>& segmentBezierPoints){
//     ofPolyline polylineWithBorderTemp;

//     for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
//         for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
//             const BezierPoint& b = *points;


//             //if(segment == segmentBezierPoints.begin() && points == segment->begin()) // first element
//             //{
//                 //polylineWithBorder.curveTo(b.leftEdge);
//                 // do something
//             //}
            
//             // if(std::next(curve) == bezierPoints.end()) // last element
//             // {
//             //     // do something else
//             // }

//             if(segment == segmentBezierPoints.begin() && points == segment->begin()) // first element
//             {
//                 polylineWithBorderTemp.addVertex(b.pos);
//                 polylineWithBorderTemp.lineTo(b.leftEdge);
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//             } 
//             else if(std::next(segment) == segmentBezierPoints.end() && std::next(points) == segment->end()) // last element
//             {
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
                
//             }
//             else if(std::next(points) == segment->end()) // last element
//             {
                
//             }
//             else{

//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//             }
//         }
//     }

//     for (vector< vector<BezierPoint> >::reverse_iterator segment = segmentBezierPoints.rbegin(); segment != segmentBezierPoints.rend(); ++segment ) { 
//         for (vector<BezierPoint>::reverse_iterator points = segment->rbegin(); points != segment->rend(); points++) {
//             const BezierPoint& b = *points;

//             if(segment == segmentBezierPoints.rbegin() && points == segment->rbegin()) // first element
//             {
                
//                 polylineWithBorderTemp.lineTo(b.rightEdge);
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
                
//                 // do something
//             }
//             else if(std::next(segment) == segmentBezierPoints.rend() && std::next(points) == segment->rend()) // last element
//             {
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//                 polylineWithBorderTemp.lineTo(b.pos);
//             }
//             else if(std::next(points) == segment->rend()) // last element
//             {
                
//             }
//             else{

//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//             }
//         }
        
//     }

//     polylineWithBorderTemp.close(); 

//     return polylineWithBorderTemp;
// }



// ofPolyline Bezz::buildClosedNonIntersectingBezierPolyline(vector<vector<BezierPoint>>& segmentBezierPoints){
//     ofPolyline polylineWithBorderTemp;

//     for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
//         for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
//             const BezierPoint& b = *points;

//             //if(b.leftEdgeIntersects == true)
//             //    continue;

//             if(segment == segmentBezierPoints.begin() && points == segment->begin()) // first element
//             {
//                 polylineWithBorderTemp.addVertex(b.pos);
//                 polylineWithBorderTemp.lineTo(b.leftEdge);
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//             } 
//             else if(std::next(segment) == segmentBezierPoints.end() && std::next(points) == segment->end()) // last element
//             {
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//                 polylineWithBorderTemp.curveTo(b.leftEdge);
                
//             }
//             else if(std::next(points) == segment->end()) // last element
//             {
                
//             }
//             else{

//                 polylineWithBorderTemp.curveTo(b.leftEdge);
//             }
//         }
//     }

//     for (vector< vector<BezierPoint> >::reverse_iterator segment = segmentBezierPoints.rbegin(); segment != segmentBezierPoints.rend(); ++segment ) { 
//         for (vector<BezierPoint>::reverse_iterator points = segment->rbegin(); points != segment->rend(); points++) {
//             const BezierPoint& b = *points;

//             //if(b.rightEdgeIntersects == true)
//             //    continue;


//             if(segment == segmentBezierPoints.rbegin() && points == segment->rbegin()) // first element
//             {
                
//                 polylineWithBorderTemp.lineTo(b.rightEdge);
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
                
//                 // do something
//             }
//             else if(std::next(segment) == segmentBezierPoints.rend() && std::next(points) == segment->rend()) // last element
//             {
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//                 polylineWithBorderTemp.lineTo(b.pos);
//             }
//             else if(std::next(points) == segment->rend()) // last element
//             {
                
//             }
//             else{

//                 polylineWithBorderTemp.curveTo(b.rightEdge);
//             }
//         }
        
//     }

//     polylineWithBorderTemp.close(); 

//     return polylineWithBorderTemp;
// }



bool Bezz::circleIntersectsOtherCircle(ofPoint p1, ofPoint p2, float r1, float r2){


    
    float d = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
 
    if (d <= r1 - r2) {
        return true;
        //console.log("Circle B is inside A");
    } else if (d <= r2 - r1) {
        return true;
        //console.log("Circle A is inside B");
    } else if (d < r1 + r2) {
        return true;
        //console.log("Circle intersect to each other");
    } else if (d == r1 + r2) {
        return true;
        //console.log("Circle touch to each other");
    } else {
        return false;
        //console.log("Circle not touch to each other");
    }
}


ofPolyline Bezz::buildRotatedRectangle(ofPoint center, float angle, float width, float height){
    //TOP RIGHT VERTEX:
    ofPoint Top_Right;
    Top_Right.x = center.x + ((width / 2) * cos(angle)) - ((height / 2) * sin(angle));
    Top_Right.y = center.y + ((width / 2) * sin(angle)) + ((height / 2) * cos(angle));

    //TOP LEFT VERTEX:
    ofPoint Top_Left;
    Top_Left.x = center.x - ((width / 2) * cos(angle)) - ((height / 2) * sin(angle));
    Top_Left.y = center.y - ((width / 2) * sin(angle)) + ((height / 2) * cos(angle));

    //BOTTOM LEFT VERTEX:
    ofPoint Bot_Left;
    Bot_Left.x = center.x - ((width / 2) * cos(angle)) + ((height / 2) * sin(angle));
    Bot_Left.y = center.y - ((width / 2) * sin(angle)) - ((height / 2) * cos(angle));

    //BOTTOM RIGHT VERTEX:
    ofPoint Bot_Right;
    Bot_Right.x = center.x + ((width / 2) * cos(angle)) + ((height / 2) * sin(angle));
    Bot_Right.y = center.y + ((width / 2) * sin(angle)) - ((height / 2) * cos(angle));

    ofPolyline tempRotatedRectangle;
    
    tempRotatedRectangle.addVertex(Bot_Left);
    tempRotatedRectangle.lineTo(Top_Left);
    tempRotatedRectangle.lineTo(Top_Right);
    tempRotatedRectangle.lineTo(Bot_Right);
    tempRotatedRectangle.lineTo(Bot_Left);
    tempRotatedRectangle.close();
    
    return tempRotatedRectangle;
}


ofPath Bezz::buildClosedPath(const ofPolyline& polyLine, ofColor color){
    ofPath tempRotatedRectanglePath;
    for( int i = 0; i < polyLine.getVertices().size(); i++) {  
        if(i == 0) {  
            tempRotatedRectanglePath.newSubPath();  
            tempRotatedRectanglePath.moveTo(polyLine.getVertices()[i] );  
        } else {  
            tempRotatedRectanglePath.lineTo( polyLine.getVertices()[i] );  
        }  
    }  

    tempRotatedRectanglePath.close();  
    tempRotatedRectanglePath.simplify();  

    tempRotatedRectanglePath.setFillColor(color);

    return tempRotatedRectanglePath;
}

bool Bezz::isPolylineIntersecting(const ofPolyline& a, const ofPolyline& b){
    
    for(int polyi = 0; polyi < 2; ++polyi)
    {
      const ofPolyline& polygon = polyi == 0 ? a : b;
 
      for(int i1 = 0; i1 < polygon.size(); ++i1)
        {
          const int i2 = (i1 + 1) % polygon.size();
 
          const double normalx = polygon.getVertices()[i2].y - polygon.getVertices()[i1].y;
          const double normaly = polygon.getVertices()[i2].x - polygon.getVertices()[i1].x;
 
          double minA = std::numeric_limits<double>::max();
          double maxA = std::numeric_limits<double>::min();
          for(int ai = 0; ai < a.size(); ++ai)
            {
              const double projected = normalx * a.getVertices()[ai].x +
                normaly * a.getVertices()[ai].y;
              if( projected < minA ) minA = projected;
              if( projected > maxA ) maxA = projected;
            }
 
          double minB = std::numeric_limits<double>::max();
          double maxB = std::numeric_limits<double>::min();
          for(int bi = 0; bi < b.size(); ++bi)
            {
              const double projected = normalx * b.getVertices()[bi].x +
                normaly * b.getVertices()[bi].y;
              if( projected < minB ) minB = projected;
              if( projected > maxB ) maxB = projected;
            }
 
          if( maxA < minB || maxB < minA )
            return false;
        }
    }
 
  return true;
}