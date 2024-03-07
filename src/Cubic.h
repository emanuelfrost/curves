#ifndef _PENNER_CUBIC
#define _PENNER_CUBIC

//https://github.com/jesusgollonet/ofpennereasing/blob/master/PennerEasing/Linear.h
class Cubic {
	
	public:
		
		static float easeIn(float t,float b , float c, float d);
		static float easeOut(float t,float b , float c, float d);
		static float easeInOut(float t,float b , float c, float d);
};

#endif