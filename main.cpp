// JiggleFixFlexible.cpp
#include <jni.h>
#include <android/log.h>
#include <sys/mman.h>
#include <cstdint>

#define LOG_TAG "JiggleFixFlexible"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define PAGE_SIZE 4096

// Alamat deltaTime di GTA SA Android v1.80 (ubah sesuai versi)
constexpr uintptr_t DELTA_TIME_ADDR = 0x95B938;

// Atur target FPS di sini
constexpr float TARGET_FPS = 120.0f;

// Hitung deltaTime stabil dari target FPS
constexpr float STABLE_DELTA_TIME = 1.0f / TARGET_FPS;

bool MakeMemoryWritable(void* addr) {
    uintptr_t pageStart = (uintptr_t)addr & ~(PAGE_SIZE - 1);
    if (mprotect((void*)pageStart, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
        LOGI("mprotect failed at %p", addr);
        return false;
    }
    return true;
}

void PatchDeltaTime() {
    float* pDeltaTime = (float*)DELTA_TIME_ADDR;

    if (!MakeMemoryWritable(pDeltaTime)) {
        LOGI("Failed to make deltaTime memory writable");
        return;
    }

    *pDeltaTime = STABLE_DELTA_TIME;
    LOGI("Patched deltaTime at %p to %f (for target FPS %.1f)", pDeltaTime, STABLE_DELTA_TIME, TARGET_FPS);
}

extern "C" {

JNIEXPORT void JNICALL
Java_com_rockstargames_gtasa_mod_JiggleFixFlexible_nativeOnLoad(JNIEnv* env, jobject) {
    LOGI("JiggleFixFlexible plugin loaded");
    PatchDeltaTime();
}

}
