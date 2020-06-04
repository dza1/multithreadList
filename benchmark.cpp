
/** @file benchmark.cpp
 * @author Daniel Zainzinger
 * @date 2.6.2020
 *
 * @brief Functions for benchmarking
 */
#include "benchmark.hpp"
#include <stdint.h>
#include "stdio.h"

/**
 * @brief Calculates the average of repeated benchmark tests
 *
 * @param[in]   benchmark[]  	Array of benchmarks, from which the mean should be calculated
 * @param[out]  averBench  		benchmark struct, which includes the mean values
 */
void averBenchm(benchMark_t benchmark[],benchMark_t *averBench){

uint64_t preTime=0;
uint64_t pregoTo=0;
uint64_t precores=0;

uint64_t mainTime=0;
uint64_t maingoTo=0;
uint16_t maincores=0;

uint64_t checkTime=0;
uint64_t checkgoTo=0;
uint16_t checkcores=0;
	for (size_t i = 0; i < REPEAT_TESTS; i++)
	{
		preTime+=benchmark[i].pre.time;
		pregoTo+=benchmark[i].pre.goToStart;
		precores+=benchmark[i].pre.cores;

		mainTime+=benchmark[i].main.time;
		maingoTo+=benchmark[i].main.goToStart;
		maincores+=benchmark[i].main.cores;

		checkTime+=benchmark[i].check.time;
		checkgoTo+=benchmark[i].check.goToStart;
		checkcores+=benchmark[i].check.cores;
	}
	averBench->pre.time			=preTime/REPEAT_TESTS;
	averBench->pre.goToStart	=pregoTo/REPEAT_TESTS;
	averBench->pre.cores		=precores/REPEAT_TESTS;

	averBench->main.time		=mainTime/REPEAT_TESTS;
	averBench->main.goToStart	=maingoTo/REPEAT_TESTS;
	averBench->main.cores		=maincores/REPEAT_TESTS;

	averBench->check.time		=checkTime/REPEAT_TESTS;
	averBench->check.goToStart	=checkgoTo/REPEAT_TESTS;
	averBench->check.cores		=checkcores/REPEAT_TESTS;
	

}