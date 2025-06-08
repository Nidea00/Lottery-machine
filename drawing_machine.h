#include<random>
#include<cmath>

class bingo_generator{
private:
	int count_none ;
	
	const int max_count_none = 20;
	const double p2_head = 1.0/3.0, base_c = 0.02 ;
	const double alpha;
	
	std::mt19937 rng ;
		
	inline double p1(){
		return base_c + (1-base_c) * pow( (double)count_none/(max_count_none-1), alpha) ;
	}
	
	bool bern(const double prob){
		std::uniform_real_distribution<double> uniform(0.0, 1.0);
		return uniform(rng) <= prob ;
	}
	
public:
	bingo_generator(const int seed, const double alpha)
	: alpha(alpha)
	{
		count_none = 15 ;
		rng.seed(seed) ;
	}
	
	bingo_generator(
		const int initial_garanteed,
		const int upper_bound,
		const int seed,
	    const double c, // base probability c
		const double a) // alpha
		: max_count_none(upper_bound), base_c(c), alpha(a)
	{
		count_none = max_count_none - initial_garanteed ;
		rng.seed(seed) ;
	}
	
	int draw(){
		/*
		Head Prize: Return 1
		Second Prize: Return 2
		None: Return 0 ;
		*/
		
		if( bern( p1() ) ){
			count_none = 0 ;
			
			if( bern( p2_head ) ) return 1 ;
			else return 2 ;
		}
		count_none += 1 ;
		
		return 0 ;		
	}
};