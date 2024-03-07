#include "ofApp.h"
#include "Cubic.h"
#include "Linear.h"

//https://github.com/jesusgollonet/ofpennereasing/tree/master/PennerEasing
//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);	
	ofSetFrameRate(60);
    ofSetWindowShape(1024, 1024);
    //ofSetWindowPosition(10, 10);    
    gui.setup();
    gui.add(offsetX.setup("Offset X", 0.25, 0.0, 1.0));
    gui.add(offsetY.setup("Offset Y", 0.25, 0.0, 1.0));
    gui.add(noOfSegments.setup("No Of Segments", 4, 2, 100));
    gui.add(noOfSegmentPoints.setup("No Of Segment Points", 10, 10, 2000));
    gui.add(curveSmoothing.setup("curveSmoothing", 0.0, -1.0, 1.0));
    
    gui.add(widthAlgorithm.setup("Width algorithm", 0.0, 0.0, 1.0));
    gui.add(baseWidth.setup("Top width", 5, 0.5, 100));
    gui.add(minWidth.setup("Botton width", 5, 0.5, 100));
    gui.add(widthSmoothing.setup("Width curve smoothing", 0.0, 0.0, 1.0));
    
    gui.add(debug.setup("debug", true));
}

//--------------------------------------------------------------
void ofApp::update(){
    segments.clear();
    beziers.clear();
    polyline.clear();
    segmentBezierPoints.clear();
    filteredSegmentBezierPoints.clear();
    polylineWithBorder.clear();
    polylineWithBorderSmoothed.clear();

    polylineNoneIntersecting.clear();
    

    float w = (float)(ofGetWidth());
    float h = (float)(ofGetHeight());


    drawingArea = ofRectangle(w*ofApp::offsetX, h*ofApp::offsetY, w-(w*ofApp::offsetX*2.0), h-(h*ofApp::offsetY*2.0));
    drawingAreaWidthAdjusted = ofRectangle(w*ofApp::offsetX, h*ofApp::offsetY+baseWidth, w-(w*ofApp::offsetX*2.0), h-(h*ofApp::offsetY*2.0)-baseWidth-minWidth);

    int noOfSegments = (int)ofApp::noOfSegments;

    for(unsigned int i = 0; i <= noOfSegments; i++) {
        Line lineTemp = bezz.buildSegmentLine(i, noOfSegments, drawingAreaWidthAdjusted, curveSmoothing);
        segments.push_back(lineTemp);
    }

    int totalNoOfSegments = segments.size();
    if(totalNoOfSegments > 1) { 
        for(vector<Line>::iterator iter = segments.begin()+1; iter != segments.end(); iter++) {
            int index = std::distance(segments.begin(), iter-1);
            const Line& next = *iter;
            const Line& current = *(iter - 1);

            //ofLog(OF_LOG_NOTICE, "p1: " + ofToString(current.p) + " p2: " + ofToString(next.p));

            float w1 = bezz.getLineWidth(current.p, minWidth, baseWidth, widthSmoothing);
            float w2 = bezz.getLineWidth(next.p, minWidth, baseWidth, widthSmoothing);

            Bezier bezierTemp = bezz.buildSegmentBezier(totalNoOfSegments, index, current, w1, next, w2);
            beziers.push_back(bezierTemp);
        }
    }

    for(unsigned int i = 0; i < beziers.size(); i++) {
        const Bezier& b = beziers[i];

        if(i==0){
            polyline.addVertex(ofPoint(b.start));
        }
        polyline.bezierTo(b.controlPoint1, b.controlPoint2, b.end);

        int tLength = (int)noOfSegmentPoints;
        vector<BezierPoint> points = bezz.buildBezierPoints(i, noOfSegments, tLength, b, baseWidth); 

        segmentBezierPoints.push_back(points);
    }

    // for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
    //     for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
    //         BezierPoint& b = *points;
            
    //         bool isLeftIntersecting = bezz.isBezierPointLeftEdgeIntersecting(b, segmentBezierPoints);
    //         bool isRightIntersecting = bezz.isBezierPointRightEdgeIntersecting(b, segmentBezierPoints);

    //         //bool isLeftPolyIntersecting = bezz.isBezierPointLeftPolylineIntersecting(b, segmentBezierPoints);
    //         //bool isRightPolyIntersecting = bezz.isBezierPointRightPolylineIntersecting(b, segmentBezierPoints);

    //         b.leftEdgeIntersects = isLeftIntersecting;
    //         b.rightEdgeIntersects = isRightIntersecting;

    //         //b.leftPolyIntersects = isLeftPolyIntersecting;
    //         //b.rightPolyIntersects = isRightPolyIntersecting;
    //     }
    // }

    // polylineWithBorder = bezz.buildClosedBezierPolyline(segmentBezierPoints);

    // polylineNoneIntersecting = bezz.buildClosedNonIntersectingBezierPolyline(segmentBezierPoints);

    // polylineWithBorderSmoothed = polylineWithBorder.getSmoothed(10, 0);


}



//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    ofEnableAlphaBlending();


    


    ofSetColor(ofFloatColor(0.0, 0.0, 0.0, 1.0));
    for (vector< vector<BezierPoint> >::iterator segment = segmentBezierPoints.begin(); segment != segmentBezierPoints.end(); ++segment ) { 
        for (vector<BezierPoint>::iterator points = segment->begin(); points != segment->end(); points++) {
            BezierPoint& b = *points;
            float w = bezz.getLineWidth(b.p, minWidth, baseWidth, widthSmoothing);
            ofDrawCircle(b.pos, w);

        }
    }


    if(debug){
        ofSetColor(30,30,30,30);

        for (auto segment : segments) {
            ofDrawLine(segment.start, segment.end);
        }
    
        ofSetColor(255,30,30,100);

        polyline.draw();


        for(unsigned int i = 0; i < beziers.size(); i++) {
            const Bezier& b = beziers[i];

            ofDrawLine(b.start, b.controlPoint1);
            ofDrawCircle(b.controlPoint1, 2);
            ofDrawLine(b.end, b.controlPoint2);
            ofDrawCircle(b.controlPoint2, 2);
        }

        ofSetColor(255,30,30,30);
        ofDrawRectangle(drawingArea);
    }


    // if(debug){

    // for (auto segment : segments) {
    //     ofDrawLine(segment.start, segment.end);
    // }

    // ofSetColor(255,30,30,100);

    // polyline.draw();


    // for (auto bezierPointSegment : segmentBezierPoints) {
    //     for (auto bezierPoint : bezierPointSegment) {


    //         //ofColor leftPolyColor = bezierPoint.leftPolyIntersects ? ofFloatColor(1.0, 0.0, 0.0, 0.55) : ofFloatColor(0.0, 1.0, 0.0, 0.55);

    //         //ofPath leftPath = bezz.buildClosedPath(bezierPoint.leftPoly, leftPolyColor);

    //         //ofColor rightPolyColor = bezierPoint.rightPolyIntersects ? ofFloatColor(1.0, 0.0, 0.0, 0.55) : ofFloatColor(0.0, 1.0, 0.0, 0.55);

    //         //ofPath rightPath = bezz.buildClosedPath(bezierPoint.rightPoly, rightPolyColor);

    //         //leftPath.draw();
    //         //rightPath.draw();

    //         if(bezierPoint.leftEdgeIntersects){
    //             ofSetColor(255,10,30,100);
    //         }else{
    //             ofSetColor(10,255,30,100);
    //         }
    //         ofDrawLine(bezierPoint.pos, bezierPoint.leftEdge);

    //         if(bezierPoint.rightEdgeIntersects){
    //             ofSetColor(255,10,30,100);
    //         }else{
    //             ofSetColor(10,255,30,100);
    //         }

    //         ofDrawLine(bezierPoint.pos, bezierPoint.rightEdge);



    //         // ofPoint center = bezz.getRotatedLine(bezierPoint.pos, baseWidth/2.0, bezierPoint.angle + ofDegToRad(90));
    //         // ofPolyline tempRotatedRectangle = bezz.buildRotatedRectangle(center, bezierPoint.angle, 20, baseWidth);
    

    //         // ofPath tempRotatedRectanglePath;

    //         // for( int i = 0; i < tempRotatedRectangle.getVertices().size(); i++) {  
    //         //     if(i == 0) {  
    //         //         tempRotatedRectanglePath.newSubPath();  
    //         //         tempRotatedRectanglePath.moveTo(tempRotatedRectangle.getVertices()[i] );  
    //         //     } else {  
    //         //         tempRotatedRectanglePath.lineTo( tempRotatedRectangle.getVertices()[i] );  
    //         //     }  
    //         // }  

    //         // tempRotatedRectanglePath.setStrokeWidth(2.0);
    //         // tempRotatedRectanglePath.setStrokeColor(ofFloatColor(0.0, 0.0, 0.0, 0.75));
    //         // tempRotatedRectanglePath.setFillColor(ofFloatColor(0.0, 1.0, 0.0, 0.5));
    //         // tempRotatedRectanglePath.draw();

            

    //         // float rH = 20.0;
    //         // float rW = 10.0;
    //         // float rW2 = rW/2.0;
    //         // ofPolyline tempRotatedRectangle;
            
    //         // tempRotatedRectangle.addVertex(ofPoint(bezierPoint.pos.x-rW2, bezierPoint.pos.y));
    //         // tempRotatedRectangle.lineTo(ofPoint(bezierPoint.pos.x-rW2, bezierPoint.pos.y+rH));
    //         // tempRotatedRectangle.lineTo(ofPoint(bezierPoint.pos.x+rW2, bezierPoint.pos.y+rH));
    //         // tempRotatedRectangle.lineTo(ofPoint(bezierPoint.pos.x+rW2, bezierPoint.pos.y));
    //         // tempRotatedRectangle.lineTo(ofPoint(bezierPoint.pos.x-rW2, bezierPoint.pos.y));
            
    //         // tempRotatedRectangle.close();
    //         // tempRotatedRectangle.rotateRad(bezierPoint.angle, glm::vec3(bezierPoint.pos.x, bezierPoint.pos.y, tempRotatedRectangle.getCentroid2D));


            
            

    //         ofSetColor(10,255,255,100);

    //         ofDrawCircle(bezierPoint.pos, 5.0);


            
    //     }
    // }

    // ofSetColor(255,30,30,100);
    // //polylineWithBorder.draw();
    // //polylineWithBorderSmoothed.draw();

    // ofPath contourPath = bezz.buildClosedPath(polylineWithBorder, ofFloatColor(0.0, 0.0, 1.0, 0.25));
    // contourPath.draw();

    // ofPath contourNoneIntersectingPath = bezz.buildClosedPath(polylineNoneIntersecting, ofFloatColor(1.0, 0.0, 1.0, 0.0));
    // contourNoneIntersectingPath.setStrokeColor(ofFloatColor(0.0, 0.0, 0.0, 1.0));
    // contourNoneIntersectingPath.setStrokeWidth(1.0);
    // contourNoneIntersectingPath.draw();

    // // ofBeginShape();  
    // // for( int i = 0; i < polylineWithBorder.getVertices().size(); i++) {  
    // //     ofVertex(polylineWithBorder.getVertices().at(i).x, polylineWithBorder.getVertices().at(i).y);  
    // // }  
    // // ofEndShape();  

    // } else {
    //     ofPath contourNoneIntersectingPath = bezz.buildClosedPath(polylineNoneIntersecting, ofFloatColor(0.0, 0.0, 0.0, 1.0));
    //     contourNoneIntersectingPath.setStrokeColor(ofFloatColor(0.0, 0.0, 0.0, 0.0));
        
    //     contourNoneIntersectingPath.draw();

    // }

    gui.draw();



}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
