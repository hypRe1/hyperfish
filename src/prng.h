#pragma once

typedef unsigned long long U64;

// random seed for the PRNG
U64 random_state = 11719352968791081550;

// XORShift64 https://en.wikipedia.org/wiki/Xorshift
U64 get_random_U64_number() {
	U64 new = random_state;
	new ^= new << 13;
	new ^= new >> 7;
	new ^= new << 17;
    random_state = new;
	return new;
}
