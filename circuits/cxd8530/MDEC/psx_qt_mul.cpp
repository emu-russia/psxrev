int32_t booth_multiply_psx(uint8_t q, int16_t y)
{
	// bit count n of q (8, just as in JPEG)
	static const unsigned int precision_of_q = 8;
	// chosen such that after (n/2)-1 right-shifts by 2 the first value is at position 0
	// (ensures the correct value of each summand in relation to the slice position)
	static const unsigned int start_position = precision_of_q - 2;
	// chosen as n/2 because each cycle shifts the current bit slice position by 2;
	// thus, n/2 iterations are necessary to iterate through the entire n-bit number q
	static const unsigned int number_of_cycles = precision_of_q / 2;

	auto shift_reg = q & ~1; // bit q.0 gets special treatment
	int32_t result = 0, summand;
	for (auto cycle = 0U; cycle < number_of_cycles; ++cycle)
	{
		switch (shift_reg & 7)
		{			
		/* 000 */ case 0: summand = 0; break;
		/* 001 */ case 1: summand = y; break;
		/* 010 */ case 2: summand = y; break;
		/* 011 */ case 3: summand = 2 * y; break;
		/* 100 */ case 4: summand = -2 * y; break;
		/* 101 */ case 5: summand = -y; break;
		/* 110 */ case 6: summand = -y; break;
		/* 111 */ case 7: summand = 0; break;
		}

		result >>= 2; // ensure correct position
		shift_reg >>= 2; // shift to next bit slice

		summand <<= 1 + start_position;
		if (cycle == 0)
			// special treatment of bit q.0
			result = summand + ((q & 1) == 1 ? (y << start_position) : 0);
		else
			result = summand + result;
	}
	return result;
}
