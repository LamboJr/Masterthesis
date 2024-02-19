#ifndef POKEMONBUFFER_HEADER
#define POKEMONBUFFER_HEADER

	u32 BlockInitSequence[16] = {
	0xBBBB,0x00C8,0x0081,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	};
	u16 NoPokemon[50] = {
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000
	};
	u16 Magikarp_pokemonbuffer[50] = {
	 0x0018, 0x0000, 0xc371, 0x60ac, 0xd5c7, 0xdddb, 0xd5df, 0xe4e6,
	 0xffff, 0x0205, 0xd5be, 0xdde2, 0xe0d9, 0x00ff, 0x8560, 0x0000,
	 0xc3e8, 0x60ac, 0xc360, 0x60ac, 0x3c69, 0x60ac, 0xc3ff, 0x601a,
	 0xc369, 0x60ac, 0xc66c, 0x65a9, 0xc369, 0x60ac, 0xc369, 0x60ac,
	 0xc369, 0x60ac, 0x9b69, 0x422c, 0xc369, 0x60ac, 0xc369, 0x60ac,
	 0x0000, 0x0000, 0xff01, 0x000b, 0x000b, 0x0005, 0x0006, 0x0006,
	 0x0005, 0x0005};

	u16 *PokemonTeam[6] = {Magikarp_pokemonbuffer,Magikarp_pokemonbuffer,NoPokemon,NoPokemon,NoPokemon,NoPokemon
	};



	//u32 *PokemonTeam[6] = {Latios_pokemonbuffer,Metagross_pokemonbuffer,Breloom_pokemonbuffer,Salamence_pokemonbuffer,Scizor_pokemonbuffer,Skarmory_pokemonbuffer};


#endif
