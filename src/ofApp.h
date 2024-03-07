#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Bezz.h"



class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
		

		//int noOfSegments = 2;

		ofxFloatSlider offsetX;
		ofxFloatSlider offsetY;
		ofxFloatSlider baseWidth;
		ofxFloatSlider minWidth;
		ofxFloatSlider widthSmoothing;
		ofxFloatSlider curveSmoothing;
		ofxFloatSlider noOfSegmentPoints;
		ofxFloatSlider noOfSegments;
		ofxFloatSlider widthAlgorithm;

		ofxPanel gui;
		ofxToggle debug;

		vector < Line > segments;
		vector < Bezier > beziers;
		vector < vector < BezierPoint > > segmentBezierPoints;
		vector < vector < BezierPoint > > filteredSegmentBezierPoints;
		ofPolyline polyline;
		ofPolyline polylineWithBorder;
		ofPolyline polylineNoneIntersecting;
		ofPolyline polylineWithBorderSmoothed;
		ofRectangle drawingArea;
		ofRectangle drawingAreaWidthAdjusted;
		Bezz bezz;
};
