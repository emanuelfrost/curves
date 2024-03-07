#ifndef _PENNER_LINEAR
#define _PENNER_LINEAR

//https://github.com/jesusgollonet/ofpennereasing/blob/master/PennerEasing/Linear.h
class Linear {
	
	public:
		
		static float easeNone(float t,float b , float c, float d); // ??
		static float easeIn(float t,float b , float c, float d);
		static float easeOut(float t,float b , float c, float d);
		static float easeInOut(float t,float b , float c, float d);
		
};

#endif