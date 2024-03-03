#ifndef FODOCOMPILER_FIXUP_H
#define FODOCOMPILER_FIXUP_H

#include <stdbool.h>

// < Fixup flags start

enum
{
  FIXUP_FLAG_RESOLVED = 0b00000001
};

// > Fixup flags end

struct fixup;

/**
 * @brief Fixes the fixup
 * Returns true if the fixup was successful
 */
typedef bool (*FIXUP_FIX)(struct fixup* fixup);

/**
 * @brief Signifies the fixup has been removed from memory.
 * The implementer of this function pointer should free any memory
 * related to the fixup
 */
typedef void (*FIXUP_END)(struct fixup* fixup);

struct fixup_config
{
  FIXUP_FIX fix;
  FIXUP_END end;
  void* private;
};

struct fixup_system
{
  // A vector of fixups
  struct vector* fixups;
};

struct fixup
{
  int flags;
  struct fixup_system* system;
  struct fixup_config config;
};

// Fixup functions start

struct fixup_system* fixup_sys_new();
struct fixup_config* fixup_config(struct fixup* fixup);
void fixup_free(struct fixup* fixup);
void fixup_start_iteration(struct fixup_system* system);
struct fixup* fixup_next(struct fixup_system* system);
void fixup_sys_free(struct fixup_system* system);
int fixup_sys_unresolved_fixups_count(struct fixup_system* system);
struct fixup* fixup_register(struct fixup_system* system, struct fixup_config* config);
bool fixup_resolve(struct fixup* fixup);
void* fixup_private(struct fixup* fixup);
bool fixups_resolve(struct fixup_system* system);

// Fixup functions end

#endif