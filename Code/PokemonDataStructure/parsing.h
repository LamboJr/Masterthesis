  #include <getopt.h>
  
  
  const char usage[] = 
    "Usage: %s [options] <pokémon name> <trainer name>\n"
    "\n"
    "\t-s, --species <Pokemon Name in upper case>   \n"
    "\t                         The default is Bulbasaur.\n"
    "\t                         SEE --pkmn for list\n"
    "\t-L, --pkmn               Prints Pokemon list\n"
    "\t-i, --item <Name of ITEM in upper case>.\n"
    "\t                         The default is Nothing.\n"
    "\t                         See --itemlist for list\n"
    "\t-I, --itemlist           Prints item list\n"
    "\t-x, --experience <int>   The amount of experience that the pokémon shall have.\n"
    "\t                         The default is 1250000.\n"
    "\t-B, --pp-bonus <a>:<b>:<c>:<d>   A set of values from 1-3 of how many\n"
    "\t                                 PP-Bonuses shall have been applied to\n"
    "\t                                 each move slot. Each defaults to 3.\n"
    "\t-f, --friendship <int>   The friendship of the generated pokémon.\n"
    "\t                         Must be between 0-255; the default is 255.\n"
    "\t-m, --moves <a>:<b>:<c>:<d>  The names of the moves the pokémon shall have.\n"
    "\t-u, --movelist           Prints movelist\n"
    "\t-P, --moves-pp <a>:<b>:<c>:<d>   The current PP of each move slot.\n"
    "\t-j, --ev-hp <int>        The pokémon's HP effort value.\n"
    "\t                         Must be between 0-255; the default is 0.\n"
    "\t-v, --ev-attack <int>    The pokémon's attack effort value.\n"
    "\t                         Must be between 0-255; the default is 0.\n"
    "\t-e, --ev-defense <int>   The pokémon's defense effort value.\n"
    "\t                         Must be between 0-255; the default is 0.\n"
    "\t-V, --ev-speed <int>     The pokémon's speed effort value.\n"
    "\t                         Must be between 0-255; the default is 0.\n"
    "\t-K, --ev-special-attack <int>    The pokémon's special attack effort value.\n"
    "\t                                 Must be between 0-255; the default is 0.\n"
    "\t-E, --ev-special-defense <int>   The pokémon's HP effort value.\n"
    "\t                                 Must be between 0-255; the default is 0.\n"
    "\t-c, --coolness <int>   The pokémon's coolness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-y, --beauty <int>     The pokémon's beauty condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-C, --cuteness <int>   The pokémon's cuteness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-r, --smartness <int>  The pokémon's smartness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-T, --toughness <int>  The pokémon's toughness condition.\n"
    "\t                       Must be between 0-255; the default is 255.\n"
    "\t-F, --feel <int>       The pokémon's feel (also known as luster).\n"
    "\t                       Must be between 0-255; the default is 0.\n"
    "\t-R, --pokerus <days remaining>:<strain>  Set pokérus status.\n"
    "\t                       Both may be between 0-15; the defaults are 0.\n"
    "\t-k, --met-location <index>   Set the index of the location the pokémon was met at.\n"
    "\t                             The default is 255 [fateful encounter].\n"
    "\t-M, --met-level <int>    Set the level the pokémon was met at.\n"
    "\t                         Must be between 1-100; the default is 50.\n"
    "\t-G, --met-game <colosseum-bonus|sapphire|ruby|emerald|firered|leafgreen|colosseum-xd>\n"
    "\t                         Set the game the pokémon was met in.\n"
    "\t-b, --pokeball <master|ultra|great|standard|safari|dive|nest|repeat|timer|luxury|premier>\n"
    "\t                         Set the pokéball that the pokémon was caught in.\n"
    "\t-H, --iv-hp <int>        The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 31.\n"
    "\t-a, --iv-attack <int>    The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 31.\n"
    "\t-d, --iv-defense <int>   The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 31.\n"
    "\t-S, --iv-speed <int>     The pokémon's HP initial value.\n"
    "\t                         Must be between 0-255; the default is 31.\n"
    "\t-A, --iv-special-attack <int>    The pokémon's HP initial value.\n"
    "\t                                 Must be between 0-255; the default is 31.\n"
    "\t-D, --iv-special-defense <int>   The pokémon's HP initial value.\n"
    "\t                                 Must be between 0-255; the default is 31.\n"
    "\t-g, --egg                The pokémon will be generated as an egg.\n"
    "\t-1, --ability-primary    The pokémon will use it's primary ability.\n"
    "\t-2, --ability-secondary  The pokémon will use it's secondary ability.\n"
    "\t-p, --personality <personality>  The pokémon's personality.\n"
    "\t                                 Must be an integer between 0-4,294,967,295.\n"
    "\t                                 The default is a random number in that range.\n"
    "\t-t, --trainer <trainer id>:<male|female>   The pokémon's trainer's id and gender.\n"
    "\t                                           The trainer ID must be an integer between\n"
    "\t                                           0-4,294,967,295. The default is a random number\n"
    "\t                                           in that range and male.\n"
    "\t-N, --met-language <ja|en|fr|it|de|ko|es>  The language the pokémon was met in.\n"
    "\t                                           The default is en [English].\n"
    "\t-l, --level <int>          The pokémon's level.\n"
    "\t                           Must be between 1-100; the default is 100.\n"
    "\t-n  --nature <Name of nature>         The Pokemon's nature.\n"
    "\t                           Must be between 0-24; the default is 0.\n"
    "\t-U  --shiny                Set if Pokemon should be shiny \n"
    "\t-Y, --pokerus-left <int>   Time remaining in pokérus infection.\n"
    "\t-o, --raw                  Output as raw bytes.\n"
    "\t-O, --dump                 Output as a hexdump.\n"
    "\t-h, --help                 Display this message.\n"
    "\n";

       static const char optstring[] = 
    "12a:A:b:B:c:C:d:D:e:E:f:F:gG:hH:i:Ij:k:K:l:L"
    "m:M:n:N:oOp:P:q:Q:r:R:s:S:t:T:uUv:V:x:y:Y:";

    //Each of the charcter spefifies a option. if the option requieres an argument, the : charcter has to follow like (s:)
    //For example -h needs no argument, so H follows h =>hH

  static struct option long_options[] = {
    {"species", required_argument, NULL, 's'},
    {"item", required_argument, NULL, 'i'},
    {"experience", required_argument, NULL, 'x'},
    {"pp-bonus", required_argument, NULL, 'B'},
    {"friendship", required_argument, NULL, 'f'},
    {"moves", required_argument, NULL, 'm'},
    {"moves-pp", required_argument, NULL, 'P'},
    {"ev-hp", required_argument, NULL, 'j'},
    {"ev-attack", required_argument, NULL, 'v'},
    {"ev-defense", required_argument, NULL, 'e'},
    {"ev-speed", required_argument, NULL, 'V'},
    {"ev-special-attack", required_argument, NULL, 'K'},
    {"ev-special-defense", required_argument, NULL, 'E'},
    {"coolness", required_argument, NULL, 'c'},
    {"beauty", required_argument, NULL, 'y'},
    {"cuteness", required_argument, NULL, 'C'},
    {"smartness", required_argument, NULL, 'r'},
    {"toughness", required_argument, NULL, 'T'},
    {"feel", required_argument, NULL, 'F'},
    {"pokerus", required_argument, NULL, 'R'},
    {"met-location", required_argument, NULL, 'k'},
    {"met-level", required_argument, NULL, 'M'},
    {"met-game", required_argument, NULL, 'G'},
    {"pokeball", required_argument, NULL, 'b'},
    {"iv-hp", required_argument, NULL, 'H'},
    {"iv-attack", required_argument, NULL, 'a'},
    {"iv-defense", required_argument, NULL, 'd'},
    {"iv-speed", required_argument, NULL, 'S'},
    {"iv-special-attack", required_argument, NULL, 'A'},
    {"iv-special-defense", required_argument, NULL, 'D'},
    {"egg", no_argument, NULL, 'g'},
    {"ability-primary", no_argument, NULL, '1'},
    {"ability-secondary", no_argument, NULL, '2'},
    {"personality", required_argument, NULL, 'p'},
    {"trainer", required_argument, NULL, 't'},
    {"met-language", required_argument, NULL, 'N'},
    {"level", required_argument, NULL, 'l'},
    {"pokerus-left", required_argument, NULL, 'Y'},
    {"raw", no_argument, NULL, 'o'},
    {"dump", no_argument, NULL, 'O'},
    {"help", no_argument, NULL, 'h'},
    {"nature", required_argument, NULL, 'n'},
    {"pkmn", no_argument, NULL, 'L'},
    {"itemlist", no_argument, NULL, 'I'},
    {"movelist", no_argument, NULL, 'u'},
    {"shiny", no_argument, NULL, 'U'},
    {0, 0, 0, 0}
  };