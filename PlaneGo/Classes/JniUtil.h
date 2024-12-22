#ifndef JniUtil_h__
#define JniUtil_h__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

#include <string>

class JniUtil {
public:
	static void sendStringJNI(const char *content){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
			JniMethodInfo minfo;//定义Jni函数信息结构体 
			//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
			bool isHave = JniHelper::getStaticMethodInfo(minfo,"mobi/shoumeng/planegame/SmPlane","sendString", "(Ljava/lang/String;)V");  
			if (isHave) {
				//调用此函数 
				jstring stringArg = minfo.env->NewStringUTF(content);
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, stringArg); 
			} 
#endif 
		}

	static std::string getKeyIdJNI(){
		std::string ret("");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
		JniMethodInfo minfo;//定义Jni函数信息结构体 
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
		bool isHave = JniHelper::getStaticMethodInfo(minfo,"mobi/shoumeng/planegame/SmPlane","getKeyId", "()Ljava/lang/String;");  
		if (isHave) {
			//调用此函数 
			jstring str = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID); 
			ret = JniHelper::jstring2string(str);
			return ret;
		}else{
			return ret;
		} 
#endif 
		return ret;
	}
};


#endif // JniUtil_h__
