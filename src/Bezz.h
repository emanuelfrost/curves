#include "ofMain.h"


class Line {
public:
    float p;
    ofPoint start;
    ofPoint end;
};

class Bezier {
public:
    ofPoint start;
	ofPoint controlPoint1;
    ofPoint controlPoint2;
    ofPoint end;
};

class BezierPoint {
public:
	float p;
    int index;
	double t;
    ofPoint pos;
	double angle;
	// ofPoint leftEdge;
	// ofPoint rightEdge;
    //ofPolyline leftPoly;
    //ofPolyline rightPoly;
	
	// bool leftEdgeIntersects;
	// bool rightEdgeIntersects;
    //bool leftPolyIntersects;
	//bool rightPolyIntersects;

	bool operator==(const BezierPoint& rhs) const {
		return
		index == rhs.index
		&& t == rhs.t
        && p == rhs.p
		&& pos == rhs.pos
		&& angle == rhs.angle
		;
	}
};


class Bezz {
public:
    ofPoint getRotatedLine(ofPoint start, double len, double angle);
    double getBezierAngleInRadians(double t, double sx, double sy, double cp1x, double cp1y, double cp2x, double cp2y, double ex, double ey);
    //bool isBezierPointLeftEdgeIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints);
    //bool isBezierPointRightEdgeIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints);

    //bool isBezierPointLeftPolylineIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints);
    //bool isBezierPointRightPolylineIntersecting(const BezierPoint& point, vector<vector<BezierPoint>>& segmentBezierPoints);

    Line buildSegmentLine(int index, int noOfSegments, const ofRectangle& bounds, float curveSmoothing);
    Bezier buildSegmentBezier(int totalNoOfSegments, int index, const Line& current, const float currentWidth, const Line& next, const float nextWidth);
    BezierPoint buildBezierPoint(float pointP, int index, const Bezier& b, float t, float width);
    vector<BezierPoint> buildBezierPoints(int bezierIndex, int noOfBeziers, int noOfSegmentPoints, const Bezier& b, float width);
    //ofPolyline buildClosedBezierPolyline(vector<vector<BezierPoint>>& segmentBezierPoints);
    bool circleIntersectsOtherCircle(ofPoint p1, ofPoint p2, float r1, float r2);
    //ofPolyline buildClosedNonIntersectingBezierPolyline(vector<vector<BezierPoint>>& segmentBezierPoints);
    ofPolyline buildRotatedRectangle(ofPoint center, float angle, float width, float height);

    ofPath buildClosedPath(const ofPolyline& polyLine, ofColor color);
    bool isPolylineIntersecting(const ofPolyline& a, const ofPolyline& b);
    float getLineWidth(float p, float widthAlgorithm, float minWidth, float baseWidth, float widthSmoothing);
    
};
