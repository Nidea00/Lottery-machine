#include<iostream>
#include<cmath>
#include<unordered_map>
#include<vector>
#include "drawing_machine.h"

// Parameters of Drawing Machine
const int initial_garanteed = 5, max_count_none = 20, seed = 42 ;
const double p2_head = 1.0/3.0, base_c = 0.02 ;

// Parameters of Simulated Iteration
const double alpha_bottom = 1.2, alpha_top = 1.5, Tol = 1e-6, max_iter = 100 ;
const int n_draws = 1'000'000 ;

// Expected Probability [Prob_Head, Prob_Second]
const std::vector<double> expected_p{0.05, 0.10} ;


std::vector<double> draw_many_times(const double alpha){
	// Return ratio of (Head prize, Second prize).
	
	// Storing the computed value.
	static std::unordered_map<double, std::vector<double>> val ;
	if(val.count(alpha)) return val[alpha] ;
	
	bingo_generator gen(initial_garanteed,
	                    max_count_none,
						seed,
						base_c,
						alpha) ;
	
	unsigned int counts[3] = {15, 0, 0} ;
	/*
	counts[0] : Count of Continuous 'None'
	counts[1] : Count of 'Head Prize'
	counts[2] : COunt of 'Second Prize'
	*/
	
	int prize ;
	for(int i=0; i<n_draws; i++){
		prize = gen.draw() ;
		counts[prize] += 1 ;
	}
	
	std::vector<double> ans(2) ;
	ans[0] = (double) counts[1]/n_draws ;
	ans[1] = (double) counts[2]/n_draws ;
	val[alpha] = ans ;
	
	return ans ;
}

double mean_square_error(std::vector<double> a, std::vector<double> b){
	if(a.size() > b.size()) swap(a, b) ;
	int n = a.size() ;
	
	double count = 0 ;
	for(int i=0; i<n; i++){
		count += pow(a[i]-b[i], 2);
	}
	for(int i=n; i<b.size(); i++){
		count += pow(b[i], 2) ;
	}
	count /= n ;
	
	return count ;
}

double pipe_func(const double alpha){
	return mean_square_error(draw_many_times(alpha), expected_p) ;
}

double binary_search(double left,
                     double right,
				     double (*f)(const double),
				     const double Tolerance,
				     const int max_iteration,
				     const bool show_info){
	// Using binary_search method, find the alpha that minimizes f().
	if(right < left) std::swap(left, right) ;
	
	double mid;
	for(int i=0; i<max_iteration; i++){	
		mid = left + (right - left)/2 ;
		
		if(show_info){
			printf("i = %4d, ", i);
			printf("[left, right] = [%f, %f], ", left, right) ;
			printf("f(mid) = %e\n", f(mid)) ;
		}

		if(f(mid) < Tolerance) break ;		
		(f(left) < f(right))? right = mid: left = mid ;		
	}
	
	return mid ;
}

void grid(double left, double right, int n_pts){
	std::vector<double> ratios(2) ;
	if(left > right) std::swap(left, right) ;
	
	double delta = (right-left)/n_pts ;
	for(double alpha=left; alpha<right+delta; alpha+=delta){
		ratios = draw_many_times(alpha) ;
		printf("alpha = %.2f, [r_head, r_sec] = ", alpha) ;
		printf("[%.2f%, %.2f%]\n", ratios[0]*100, ratios[1]*100) ;
	}
}
void printline(const int n){
	for(int i=0; i<n; i++) printf("-") ;
	std::cout << std::endl ;
}

int main(){
	/*
	printline(50) ;
	grid(1.0, 2.0, 100) ;
	printline(50) ;
	*/
	
	double alpha = binary_search(alpha_bottom, alpha_top, pipe_func, Tol, max_iter, false) ;
	printf("Set alpha as %f, after %d times drawing\n", alpha, n_draws) ;
	std::vector<double> ratios = draw_many_times(alpha) ;
	printf("ratio of Head Prize: %.2f%\n", ratios[0]*100) ;
	printf("ratio of Second Prize: %.2f%\n", ratios[1]*100) ;
	
	return 0 ;
}