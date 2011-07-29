#include <AudioToolbox/AudioToolbox.h>
#include <dlfcn.h>
#include <objc/runtime.h>

static bool silenceAudio = false;

static IMP oldGrowl;
static void newGrowl(id self, SEL selector, void* data)
{
	silenceAudio = data && *(int*)((char*)data + 0x80);
	if (!silenceAudio)
		oldGrowl(self, selector, data);
}

static void doObjcMagic()
{
	Class growl = (Class)objc_getClass("SPGrowlDelegate");
	Method method = class_getInstanceMethod(growl, sel_registerName("notificationWithTrackInfo:"));
	oldGrowl = method_setImplementation(method, (IMP)&newGrowl);
}

OSStatus AudioConverterFillComplexBuffer(
		AudioConverterRef instance,
		AudioConverterComplexInputDataProc dataProc,
		void* userData,
		UInt32* packetSize,
		AudioBufferList* outputData,
		AudioStreamPacketDescription* outputDescription
) {
	int i;
	typedef OSStatus(*AudioConverterFillComplexBufferFunction)(AudioConverterRef, AudioConverterComplexInputDataProc, void*, UInt32*, AudioBufferList*, AudioStreamPacketDescription*);
	static AudioConverterFillComplexBufferFunction original = NULL;
	if (!original) {
		original = (AudioConverterFillComplexBufferFunction)dlsym(RTLD_NEXT, "AudioConverterFillComplexBuffer");
		doObjcMagic();
	}
	OSStatus ret = original(instance, dataProc, userData, packetSize, outputData, outputDescription);
	for (i = 0; silenceAudio && i < outputData->mNumberBuffers; i++)
		memset(outputData->mBuffers[i].mData, 0, outputData->mBuffers[i].mDataByteSize);
	return ret;
}
