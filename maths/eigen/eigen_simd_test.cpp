#include <Eigen/Core>

using namespace Eigen;

// EIGEN_ASM_COMMENT does not support __arm__
#if (defined __GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define CUSTOM_ASM_COMMENT(X) __asm__("#" X)
#elif (defined __GNUC__) && (defined __arm__)
#define CUSTOM_ASM_COMMENT(X) __asm__("@" X)
#else
#define CUSTOM_ASM_COMMENT(X)
#endif

// Check if vectorisation is enabled
#if defined EIGEN_VECTORIZE
CUSTOM_ASM_COMMENT("Vectorisation enabled");
#else
CUSTOM_ASM_COMMENT("Vectorisation NOT enabled");
#endif

// Check which instruction set is detected
#if defined EIGEN_VECTORIZE_SSE
CUSTOM_ASM_COMMENT("EIGEN_VECTORIZE_SSE");
#elif defined EIGEN_VECTORIZE_ALTIVEC
CUSTOM_ASM_COMMENT("EIGEN_VECTORIZE_ALTIVEC");
#elif defined EIGEN_VECTORIZE_NEON
CUSTOM_ASM_COMMENT("EIGEN_VECTORIZE_NEON");
#else
CUSTOM_ASM_COMMENT("No EIGEN_VECTORIZE_*");
#endif

#if defined EIGEN_VECTORIZE_NEON
// Check that NEON intrinsics work
void intrin(float32x4_t& u, float32x4_t& v, float32x4_t& w) {
    CUSTOM_ASM_COMMENT("begin intrin");
    static float32x4_t const t = {3.0f, 3.0f, 3.0f, 3.0f};
    u = vmlaq_f32(v, w, t);
    CUSTOM_ASM_COMMENT("end intrin");
}
#endif

// Vector4f, based on http://eigen.tuxfamily.org/index.php?title=Developer%27s_Corner#Studying_assembly_output
void foo(Vector4f& u, Vector4f& v, Vector4f& w) {
    CUSTOM_ASM_COMMENT("begin foo");
    u = v + (3 * w);
    CUSTOM_ASM_COMMENT("end foo");
}

// Int version of foo
void foo_i(Vector4i& u, Vector4i& v, Vector4i& w) {
    CUSTOM_ASM_COMMENT("begin foo_i");
    u = v + 3 * w;
    CUSTOM_ASM_COMMENT("end foo_i");
}

// Coefficient-wise array multiply
void arrmult(Array4f& u, Array4f& v, Array4f& w) {
    CUSTOM_ASM_COMMENT("begin arrmult");
    u = v * w;
    CUSTOM_ASM_COMMENT("end arrmult");
}

// Int version of arrmult
void arrmult_i(Array4i& u, Array4i& v, Array4i& w) {
    CUSTOM_ASM_COMMENT("begin arrmult_i");
    u = v * w;
    CUSTOM_ASM_COMMENT("end arrmult_i");
}

int main() {
    
#if defined EIGEN_VECTORIZE
    CUSTOM_ASM_COMMENT("Vectorisation enabled\n");
#else
    CUSTOM_ASM_COMMENT("Vectorisation NOT enabled\n");
#endif

    Vector4f a(1, 2, 3, 4);
    Vector4f b = 1.2f * a;
    ;
    Vector4f c;
    foo(c, a, b);

    return 0;
}
