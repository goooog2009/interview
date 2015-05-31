all: main
	@echo "Running tests"
	@./main
	@echo "All done"

main: main.cpp util.h min_window.h task_scheduler.h perfect_square_numbers.h check_string_pattern.h radix_sort.h count_bits.h max_number.h remove_dups.h effective_modern_cpp.h missing_number.h spell_int.h sum_to_zero.h stack_min.h queue_min.h find_name_in_tweet.h lru_cache.h lfu_cache.h lfu_cache_fast.h find_char.h check_square_points.h word_break.h roman_numbers.h quadtree.h kd_tree.h nth_element.h happy_numbers.h NxN_grid_of_words.h longest_increasing_subsequence.h max_of_all_subarrays.h max_product_subarray.h spiral_matrix_traversal.h longest_common_substring.h air_tickets_itinerary.h canoe4kids.h overlapping_jobs.h zigzag_tree.h rotate_square_matrix.h max_cluster.h check_swap_to_sort.h count_min_tree_nodes.h
	g++ -O3 -std=c++14 -o main main.cpp