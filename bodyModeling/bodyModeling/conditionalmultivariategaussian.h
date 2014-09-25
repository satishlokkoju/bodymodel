#ifndef _conditionalmultivariategaussian_h_
#define _conditionalmultivariategaussian_h_

#include "opencv2/core/core.hpp"
#include <iostream>

using namespace std;
class conditionalmultigaussian
{
	conditionalmultigaussian()
	{

	};
	~conditionalmultigaussian()
	{

	};
private:
	
	vector<float> mu;
	vector<float> sigma_by_column;
#if 0
	max_index: 0,
	number_of_variables: 0,
	all_indices: [],
	is_conditioned: [],
	is_active: [],
	max_number_of_conditioned_variables: 100,
	
	number_of_conditioned_variables: 0,
	conditioned_indices: [],
	conditioned_values_by_index: {},
	previous_conditioned_values_by_index: {},
	conditioned_value_offsets: [],
	active_index_to_condition_indices_index: {},
	
	number_of_unconditioned_variables: 0,
	unconditioned_indices: [],
	conditioned_means: [],
	all_values: [],
	
	mu_1: {},
	mu_2: {},
	sigma_21_and_22: {},
	sigma_22: [],
	sigma_22_inverse: [],
	sigma_22_inverse_transpose: [],
	sigma_22_inverse_times_offsets: [],
	previous_sigma_22_inverse_times_offsets: [],
	sigma_21_times_sigma_22_inverse: {},
#endif

};
#endif