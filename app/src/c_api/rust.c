#include "aes.h"
#include "coin.h"
#include "cx.h"
#include "jubjub.h"
#include "os.h"
#include <bolos_target.h>
#include <inttypes.h>
#include <zxformat.h>
#include <zxmacros.h>

#define CTX_REDJUBJUB "Zcash_RedJubjubH"
#define CTX_REDJUBJUB_LEN 16
#define CTX_REDJUBJUB_HASH_LEN 64

#define CTX_EXPAND_SEED "Zcash_ExpandSeed"
#define CTX_EXPAND_SEED_LEN 16
#define CTX_EXPAND_SEED_HASH_LEN 64

#include <bolos_target.h>
#include <inttypes.h>
#include <stddef.h>

#if defined(TARGET_NANOS) || defined(TARGET_NANOX) ||                          \
    defined(TARGET_NANOS2) || defined(TARGET_STAX)
#include "lcx_rng.h"
unsigned char *bolos_cx_rng(uint8_t *buffer, size_t len) {
  cx_rng_no_throw(buffer, len);
  return buffer;
}
#endif

void c_blake2b32_withpersonal(const uint8_t *person, const uint8_t *a,
                              uint32_t a_len, uint8_t *out) {
  cx_blake2b_t ctx;
  cx_blake2b_init2_no_throw(&ctx, 256, NULL, 0, (uint8_t *)person, 16);
  cx_hash_no_throw(&ctx.header, CX_LAST, a, a_len, out, 256);
};

void c_blake2b64_withpersonal(const uint8_t *person, const uint8_t *a,
                              uint32_t a_len, uint8_t *out) {
  cx_blake2b_t ctx;
  cx_blake2b_init2_no_throw(&ctx, 512, NULL, 0, (uint8_t *)person, 16);
  cx_hash_no_throw(&ctx.header, CX_LAST, a, a_len, out, 512);
};

void c_zcash_blake2b_redjubjub(const uint8_t *a, uint32_t a_len,
                               const uint8_t *b, uint32_t b_len, uint8_t *out) {
  cx_blake2b_t ctx;
  cx_blake2b_init2_no_throw(&ctx, 8 * CTX_REDJUBJUB_HASH_LEN, NULL, 0,
                            (uint8_t *)CTX_REDJUBJUB, CTX_REDJUBJUB_LEN);
  cx_hash_no_throw(&ctx.header, 0, a, a_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, CX_LAST, b, b_len, out, CTX_REDJUBJUB_HASH_LEN);
}

void c_zcash_blake2b_expand_seed(const uint8_t *a, uint32_t a_len,
                                 const uint8_t *b, uint32_t b_len,
                                 uint8_t *out) {
  cx_blake2b_t ctx;
  cx_blake2b_init2_no_throw(&ctx, 8 * CTX_EXPAND_SEED_HASH_LEN, NULL, 0,
                            (uint8_t *)CTX_EXPAND_SEED, CTX_EXPAND_SEED_LEN);
  cx_hash_no_throw(&ctx.header, 0, a, a_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, CX_LAST, b, b_len, out,
                   CTX_EXPAND_SEED_HASH_LEN);
}

void c_zcash_blake2b_expand_vec_two(const uint8_t *a, uint32_t a_len,
                                    const uint8_t *b, uint32_t b_len,
                                    const uint8_t *c, uint32_t c_len,
                                    uint8_t *out) {
  cx_blake2b_t ctx;
  cx_blake2b_init2_no_throw(&ctx, 8 * CTX_EXPAND_SEED_HASH_LEN, NULL, 0,
                            (uint8_t *)CTX_EXPAND_SEED, CTX_EXPAND_SEED_LEN);
  cx_hash_no_throw(&ctx.header, 0, a, a_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, 0, b, b_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, CX_LAST, c, c_len, out,
                   CTX_EXPAND_SEED_HASH_LEN);
}

void c_zcash_blake2b_expand_vec_four(const uint8_t *a, uint32_t a_len,
                                     const uint8_t *b, uint32_t b_len,
                                     const uint8_t *c, uint32_t c_len,
                                     const uint8_t *d, uint32_t d_len,
                                     const uint8_t *e, uint32_t e_len,
                                     uint8_t *out) {
  cx_blake2b_t ctx;
  cx_blake2b_init2_no_throw(&ctx, 8 * CTX_EXPAND_SEED_HASH_LEN, NULL, 0,
                            (uint8_t *)CTX_EXPAND_SEED, CTX_EXPAND_SEED_LEN);
  cx_hash_no_throw(&ctx.header, 0, a, a_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, 0, b, b_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, 0, c, c_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, 0, d, d_len, NULL, 0);
  cx_hash_no_throw(&ctx.header, CX_LAST, e, e_len, out,
                   CTX_EXPAND_SEED_HASH_LEN);
}

void zcash_blake2b_hash_two(const uint8_t *perso, uint32_t perso_len,
                            const uint8_t *a, uint32_t a_len, const uint8_t *b,
                            uint32_t b_len, uint8_t *out, uint32_t out_len) {
  cx_blake2b_t zcashHashBlake2b;
  cx_blake2b_init2_no_throw(&zcashHashBlake2b, 8 * out_len, NULL, 0,
                            (uint8_t *)perso, perso_len);
  cx_hash_no_throw(&zcashHashBlake2b.header, 0, a, a_len, NULL, 0);
  cx_hash_no_throw(&zcashHashBlake2b.header, CX_LAST, b, b_len, out, out_len);
}

uint16_t fp_uint64_to_str(char *out, uint16_t outLen, const uint64_t value,
                          uint8_t decimals) {
  return fpuint64_to_str(out, outLen, value, decimals);
}

void check_canary() {}

void _zemu_log_stack(uint8_t *buffer) { zemu_log_stack((char *)buffer); }

void c_aes256_encryptblock(const uint8_t *key, const uint8_t *in,
                           uint8_t *out) {
  struct AES_ctx ctx;
  AES_init_ctx(&ctx, key);

  // encrypts in place, so we copy and encrypt
  MEMCPY(out, in, AES_BLOCKLEN);
  AES_ECB_encrypt(&ctx, out);
}

void c_jubjub_scalarmult(uint8_t *point, const uint8_t *scalar) {
  jubjub_extendedpoint p;
  jubjub_fq scal;
  MEMCPY(scal, scalar, JUBJUB_FIELD_BYTES);
  SWAP_ENDIAN_BYTES(scal);

  zxerr_t err = jubjub_extendedpoint_frombytes(&p, point);
  if (err != zxerr_ok) {
    MEMZERO(point, JUBJUB_FIELD_BYTES);
    return;
  }
  jubjub_extendedpoint_scalarmult(&p, scal);
  jubjub_extendedpoint_tobytes(point, p);
}

void c_jubjub_spending_base_scalarmult(uint8_t *point, const uint8_t *scalar) {
  jubjub_extendedpoint p;
  jubjub_fq scal;
  MEMCPY(scal, scalar, JUBJUB_FIELD_BYTES);
  SWAP_ENDIAN_BYTES(scal);
  MEMCPY(&p, &JUBJUB_GEN, sizeof(jubjub_extendedpoint));
  jubjub_extendedpoint_scalarmult(&p, scal);
  jubjub_extendedpoint_tobytes(point, p);
}

// Replace functions affected by non-constant time opcodes
// Overriding requires -z muldefs
// FIXME: add a python script to ensure that the correct version is used by
// inspecting app.asm
#if ENABLE_SDK_MULT
#define SWAP_BYTES(x, y, tmp)                                                  \
  {                                                                            \
    tmp = x;                                                                   \
    x = y;                                                                     \
    y = tmp;                                                                   \
  }

#define SWAP_ENDIAN(x, tmp)                                                    \
  {                                                                            \
    SWAP_BYTES(*x, *(x + 7), tmp);                                             \
    SWAP_BYTES(*(x + 1), *(x + 6), tmp);                                       \
    SWAP_BYTES(*(x + 2), *(x + 5), tmp);                                       \
    SWAP_BYTES(*(x + 3), *(x + 4), tmp);                                       \
  }

#define LMULSIZE sizeof(long long)

long long __attribute__((noinline)) __aeabi_lmul(long long a, long long b) {
  uint8_t tmp = 0;
  uint8_t *ptr = (uint8_t *)&a;

  SWAP_ENDIAN(ptr, tmp);

  ptr = (uint8_t *)&b;
  SWAP_ENDIAN(ptr, tmp);

  char result[2 * LMULSIZE];
  MEMZERO(result, 2 * LMULSIZE);
  cx_math_mult((unsigned char *)&result, (unsigned char *)&a,
               (unsigned char *)&b, LMULSIZE);

  SWAP_ENDIAN(ptr, tmp);
  ptr = (uint8_t *)&a;
  SWAP_ENDIAN(ptr, tmp);

  ptr = (uint8_t *)&result[LMULSIZE];
  SWAP_ENDIAN(ptr, tmp);
  return *((long long *)ptr);
}
#endif
/*
long long __multi3(long long a, long long b) {
    long long a_be = ((a & 0xff) << 56)
                     | ((a >> 8 & 0xff) << 48)
                     | ((a >> 16 & 0xff) << 40)
                     | ((a >> 24 & 0xff) << 32)
                     | ((a >> 32 & 0xff) << 24)
                     | ((a >> 40 & 0xff) << 16)
                     | ((a >> 48 & 0xff) << 8)
                     | ((a >> 56 & 0xff) << 0);

    long long b_be = ((b & 0xff) << 56)
                     | ((b >> 8 & 0xff) << 48)
                     | ((b >> 16 & 0xff) << 40)
                     | ((b >> 24 & 0xff) << 32)
                     | ((b >> 32 & 0xff) << 24)
                     | ((b >> 40 & 0xff) << 16)
                     | ((b >> 48 & 0xff) << 8)
                     | ((b >> 56 & 0xff) << 0);


    char result[2 * LMULSIZE];
    MEMZERO(result, 2 * LMULSIZE);
    cx_math_mult((unsigned char *) &result, (unsigned char *) &a_be, (unsigned
char *) &b_be, LMULSIZE);

    long long r_le = 0;
    r_le |= result[LMULSIZE]; r_le <<= 8;
    r_le |= result[LMULSIZE + 1]; r_le <<= 8;
    r_le |= result[LMULSIZE + 2]; r_le <<= 8;
    r_le |= result[LMULSIZE + 3]; r_le <<= 8;
    r_le |= result[LMULSIZE + 4]; r_le <<= 8;
    r_le |= result[LMULSIZE + 5]; r_le <<= 8;
    r_le |= result[LMULSIZE + 6]; r_le <<= 8;
    r_le |= result[LMULSIZE + 7];
    return r_le;
}
 */

// typedef struct {
//     unsigned quot;
//     unsigned rem;
// } __aeabi_uidivmod_result_t;
//
//// FIXME: Complete implementation, redirect and enable
////__aeabi_uidivmod_result_t __aeabi_uidivmod(unsigned numerator, unsigned
///denominator) { /    __aeabi_uidivmod_result_t r; /    return r;
////}
