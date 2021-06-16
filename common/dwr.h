
#ifndef _DWR_H_
#define _DWR_H_

#include <stdint.h>
#include <stdarg.h>

#include "build.h"
#include "sprites.h"

#define DWR_VERSION "2.2"

#ifdef  DWR_RELEASE
#define VERSION DWR_VERSION
#elif DWR_ALPHA
#define VERSION DWR_VERSION " alpha " BUILD
#else
#define VERSION DWR_VERSION " beta " BUILD
#endif

#define DEFAULT_FLAGS "VQQVAQABEAAAAA=="
#define CHEST_COUNT 31

#define BIG_SWAMP_OFFSET 22
#define CURSED_PRINCESS_OFFSET 64
#define RANDOM_ENEMY_DROPS_OFFSET 36
#define RANDOM_ENEMY_STATS_OFFSET 34
#define NO_SCREEN_FLASH_OFFSET 76
#define CONSISTENT_STATS_OFFSET 38
#define THREES_COMPANY_OFFSET 66
#define INVISIBLE_HERO_OFFSET 60
#define NOIR_MODE_OFFSET 74
#define NO_KEYS_OFFSET 50
#define SCARED_SLIMES_OFFSET 40
#define MODERN_SPELLS_OFFSET 72
#define NO_NUMBERS_OFFSET 58
#define OPEN_CHARLOCK_OFFSET 46
#define PERMANENT_REPEL_OFFSET 26
#define INVISIBLE_NPCS_OFFSET 62
#define REPEL_IN_DUNGEONS_OFFSET 24
#define SHORT_CHARLOCK_OFFSET 48
#define FAST_TEXT_OFFSET 42
#define NO_HURTMORE_OFFSET 56
#define RANDOM_PRICES_OFFSET 12
#define RANDOM_XP_REQS_OFFSET 14
#define PERMANENT_TORCH_OFFSET 28
#define SHUFFLE_CHESTS_OFFSET 0
#define DEATH_NECKLACE_OFFSET 18
#define RANDOMIZE_GROWTH_OFFSET 2
#define SPEED_HACKS_OFFSET 44
#define RANDOMIZE_MUSIC_OFFSET 68
#define RANDOM_MAP_OFFSET 4
#define RANDOMIZE_PATTERNS_OFFSET 30
#define DISABLE_MUSIC_OFFSET 70
#define MENU_WRAP_OFFSET 16
#define RANDOMIZE_SPELLS_OFFSET 6
#define TORCH_IN_BATTLE_OFFSET 20
#define RANDOMIZE_SHOPS_OFFSET 10
#define HEAL_HURT_B4_MORE_OFFSET 8
#define RANDOMIZE_ZONES_OFFSET 32

#define BIG_SWAMP(x)          (x->flags[BIG_SWAMP_OFFSET / 8] & 1 << (BIG_SWAMP_OFFSET % 8))
#define CURSED_PRINCESS(x)    (x->flags[CURSED_PRINCESS_OFFSET / 8] & 1 << (CURSED_PRINCESS_OFFSET % 8))
#define RANDOM_ENEMY_DROPS(x) (x->flags[RANDOM_ENEMY_DROPS_OFFSET / 8] & 1 << (RANDOM_ENEMY_DROPS_OFFSET % 8))
#define RANDOM_ENEMY_STATS(x) (x->flags[RANDOM_ENEMY_STATS_OFFSET / 8] & 1 << (RANDOM_ENEMY_STATS_OFFSET % 8))
#define NO_SCREEN_FLASH(x)    (x->flags[NO_SCREEN_FLASH_OFFSET / 8] & 1 << (NO_SCREEN_FLASH_OFFSET % 8))
#define CONSISTENT_STATS(x)   (x->flags[CONSISTENT_STATS_OFFSET / 8] & 1 << (CONSISTENT_STATS_OFFSET % 8))
#define THREES_COMPANY(x)     (x->flags[THREES_COMPANY_OFFSET / 8] & 1 << (THREES_COMPANY_OFFSET % 8))
#define INVISIBLE_HERO(x)     (x->flags[INVISIBLE_HERO_OFFSET / 8] & 1 << (INVISIBLE_HERO_OFFSET % 8))
#define NOIR_MODE(x)          (x->flags[NOIR_MODE_OFFSET / 8] & 1 << (NOIR_MODE_OFFSET % 8))
#define NO_KEYS(x) (x->flags[NO_KEYS_OFFSET / 8] & 1 << (NO_KEYS_OFFSET % 8))
#define SCARED_SLIMES(x) (x->flags[SCARED_SLIMES_OFFSET / 8] & 1 << (SCARED_SLIMES_OFFSET % 8))
#define MODERN_SPELLS(x) (x->flags[MODERN_SPELLS_OFFSET / 8] & 1 << (MODERN_SPELLS_OFFSET % 8))
#define NO_NUMBERS(x) (x->flags[NO_NUMBERS_OFFSET / 8] & 1 << (NO_NUMBERS_OFFSET % 8))
#define OPEN_CHARLOCK(x) (x->flags[OPEN_CHARLOCK_OFFSET / 8] & 1 << (OPEN_CHARLOCK_OFFSET % 8))
#define PERMANENT_REPEL(x) (x->flags[PERMANENT_REPEL_OFFSET / 8] & 1 << (PERMANENT_REPEL_OFFSET % 8))
#define INVISIBLE_NPCS(x) (x->flags[INVISIBLE_NPCS_OFFSET / 8] & 1 << (INVISIBLE_NPCS_OFFSET % 8))
#define REPEL_IN_DUNGEONS(x) (x->flags[REPEL_IN_DUNGEONS_OFFSET / 8] & 1 << (REPEL_IN_DUNGEONS_OFFSET % 8))
#define SHORT_CHARLOCK(x) (x->flags[SHORT_CHARLOCK_OFFSET / 8] & 1 << (SHORT_CHARLOCK_OFFSET % 8))
#define FAST_TEXT(x) (x->flags[FAST_TEXT_OFFSET / 8] & 1 << (FAST_TEXT_OFFSET % 8))
#define NO_HURTMORE(x) (x->flags[NO_HURTMORE_OFFSET / 8] & 1 << (NO_HURTMORE_OFFSET % 8))
#define RANDOM_PRICES(x) (x->flags[RANDOM_PRICES_OFFSET / 8] & 1 << (RANDOM_PRICES_OFFSET % 8))
#define RANDOM_XP_REQS(x) (x->flags[RANDOM_XP_REQS_OFFSET / 8] & 1 << (RANDOM_XP_REQS_OFFSET % 8))
#define PERMANENT_TORCH(x) (x->flags[PERMANENT_TORCH_OFFSET / 8] & 1 << (PERMANENT_TORCH_OFFSET % 8))
#define SHUFFLE_CHESTS(x) (x->flags[SHUFFLE_CHESTS_OFFSET / 8] & 1 << (SHUFFLE_CHESTS_OFFSET % 8))
#define DEATH_NECKLACE(x) (x->flags[DEATH_NECKLACE_OFFSET / 8] & 1 << (DEATH_NECKLACE_OFFSET % 8))
#define RANDOMIZE_GROWTH(x) (x->flags[RANDOMIZE_GROWTH_OFFSET / 8] & 1 << (RANDOMIZE_GROWTH_OFFSET % 8))
#define SPEED_HACKS(x) (x->flags[SPEED_HACKS_OFFSET / 8] & 1 << (SPEED_HACKS_OFFSET % 8))
#define RANDOMIZE_MUSIC(x) (x->flags[RANDOMIZE_MUSIC_OFFSET / 8] & 1 << (RANDOMIZE_MUSIC_OFFSET % 8))
#define RANDOM_MAP(x) (x->flags[RANDOM_MAP_OFFSET / 8] & (1 << (RANDOM_MAP_OFFSET % 8)))
#define RANDOMIZE_PATTERNS(x) (x->flags[RANDOMIZE_PATTERNS_OFFSET / 8] & 1 << (RANDOMIZE_PATTERNS_OFFSET % 8))
#define DISABLE_MUSIC(x) (x->flags[DISABLE_MUSIC_OFFSET / 8] & 1 << (DISABLE_MUSIC_OFFSET % 8))
#define MENU_WRAP(x) (x->flags[MENU_WRAP_OFFSET / 8] & 1 << (MENU_WRAP_OFFSET % 8))
#define RANDOMIZE_SPELLS(x) (x->flags[RANDOMIZE_SPELLS_OFFSET / 8] & 1 << (RANDOMIZE_SPELLS_OFFSET % 8))
#define TORCH_IN_BATTLE(x) (x->flags[TORCH_IN_BATTLE_OFFSET / 8] & 1 << (TORCH_IN_BATTLE_OFFSET % 8))
#define RANDOMIZE_SHOPS(x) (x->flags[RANDOMIZE_SHOPS_OFFSET / 8] & 1 << (RANDOMIZE_SHOPS_OFFSET % 8))
#define HEAL_HURT_B4_MORE(x) (x->flags[HEAL_HURT_B4_MORE_OFFSET / 8] & 1 << (HEAL_HURT_B4_MORE_OFFSET % 8))
#define RANDOMIZE_ZONES(x) (x->flags[RANDOMIZE_ZONES_OFFSET / 8] & 1 << (RANDOMIZE_ZONES_OFFSET % 8))

#define FAST_XP(x) (x->flags[52 / 8] & 1 << (52 % 8))
#define VERY_FAST_XP(x) (x->flags[52 / 8] & 2 << (52 % 8))


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the dw_rom struct
 *
 * @param rom An uninitialized dw_rom
 * @param input_file The file to read the rom data from
 * @param flags The flags received from the user.
 * @return A boolean indicating whether initialization was sucessful
 */
BOOL dwr_init(dw_rom *rom, const char *input_file, char *flags, char *final_flags);

/**
 * Randomizes a Dragon Warrior rom file
 *
 * @param input_file The name of the input file
 * @param seed The seed number to use for the random number generator
 * @param flags The flags to use for randomization options
 * @param output_dir The directory to write the new file to
 * @return A checksum for the new rom. This checksum is taken before certain
 *      options which don't affect gameplay.
 */
uint64_t dwr_randomize(const char* input_file, uint64_t seed, char *flags,
        const char *sprite_name, const char* output_dir);

#ifdef __cplusplus
}
#endif
#endif

