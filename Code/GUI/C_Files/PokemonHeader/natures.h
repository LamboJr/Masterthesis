// Pokemon natures
#define NATURE_HARDY    0
#define NATURE_LONELY   1
#define NATURE_BRAVE    2
#define NATURE_ADAMANT  3
#define NATURE_NAUGHTY  4
#define NATURE_BOLD     5
#define NATURE_DOCILE   6
#define NATURE_RELAXED  7
#define NATURE_IMPISH   8
#define NATURE_LAX      9
#define NATURE_TIMID    10
#define NATURE_HASTY    11
#define NATURE_SERIOUS  12
#define NATURE_JOLLY    13
#define NATURE_NAIVE    14
#define NATURE_MODEST   15
#define NATURE_MILD     16
#define NATURE_QUIET    17
#define NATURE_BASHFUL  18
#define NATURE_RASH     19
#define NATURE_CALM     20
#define NATURE_GENTLE   21
#define NATURE_SASSY    22
#define NATURE_CAREFUL  23
#define NATURE_QUIRKY   24
#define NUM_NATURES     25

#define ATTACK_STAT_NUM 0
#define DEFENSE_STAT_NUM 1
#define SPEED_STAT_NUM 2
#define SPEZ_ATTACK_STAT_NUM 3
#define SPEZ_DEFENSE_STAT_NUM 4
#define NUM_NATURE_STATS 5

extern const float NatureModifierTable[NUM_NATURES][NUM_NATURE_STATS];
extern char nature_string[][20];

#define NatureModifer_Attack(Nature)  NatureModifierTable[(Nature)][ATTACK_STAT_NUM]
#define NatureModifer_Defense(Nature)  NatureModifierTable[(Nature)][DEFENSE_STAT_NUM]
#define NatureModifer_Speed(Nature)  NatureModifierTable[(Nature)][SPEED_STAT_NUM] 
#define NatureModifer_SpezAttack(Nature)  NatureModifierTable[(Nature)][SPEZ_ATTACK_STAT_NUM]
#define NatureModifer_SpezDefense(Nature)  NatureModifierTable[(Nature)][SPEZ_DEFENSE_STAT_NUM] 

#define NatureModifer(Nature)(Stat) NatureModifierTable[(Nature)][(Stat)]
