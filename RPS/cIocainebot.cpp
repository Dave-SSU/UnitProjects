#include "stdafx.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "cIocainebot.h"

#pragma warning(push)
#pragma warning(disable: 4244) 

#define will_beat(play) ("\001\002\000"[play])
#define will_lose_to(play) ("\002\000\001"[play])


const int cIocaineBot::my_hist = 0;
const int cIocaineBot::opp_hist = 1;
const int cIocaineBot::both_hist = 2;
cIocaineBot::iocaine cIocaineBot::p;


//-----------------------------------------------------------------------------
cIocaineBot::cIocaineBot()
{
	name = "Dread Pirate Roberts";
	choice = "chooses";

	memset(opp_history, 0, sizeof(opp_history)); // [HISTORY_SIZE];
	memset(my_history, 0, sizeof(my_history)); // [HISTORY_SIZE];
}

//-----------------------------------------------------------------------------
cIocaineBot::~cIocaineBot()
{
}


//-----------------------------------------------------------------------------
int cIocaineBot::match_single(int i, int num, int *history)
{
	int *highptr = history + num;
	int *lowptr = history + i;
	while (lowptr > history && *lowptr == *highptr) --lowptr, --highptr;
	return history + num - highptr;
}

int cIocaineBot::match_both(int i, int num) 
{
	int j;
	for (j = 0; j < i && opp_history[num - j] == opp_history[i - j]
		&& my_history[num - j] == my_history[i - j]; ++j);
		return j;
}

void cIocaineBot::do_history(int age, int best[3]) 
{
	const int num = my_history[0];
	int best_length[3], i, j, w;

	for (w = 0; w < 3; ++w) best[w] = best_length[w] = 0;
	for (i = num - 1; i > num - age && i > best_length[my_hist]; --i) {
		j = match_single(i, num, my_history);
		if (j > best_length[my_hist]) {
			best_length[my_hist] = j;
			best[my_hist] = i;
			if (j > num / 2) break;
		}
	}

	for (i = num - 1; i > num - age && i > best_length[opp_hist]; --i) {
		j = match_single(i, num, opp_history);
		if (j > best_length[opp_hist]) {
			best_length[opp_hist] = j;
			best[opp_hist] = i;
			if (j > num / 2) break;
		}
	}

	for (i = num - 1; i > num - age && i > best_length[both_hist]; --i) {
		j = match_both(i, num);
		if (j > best_length[both_hist]) {
			best_length[both_hist] = j;
			best[both_hist] = i;
			if (j > num / 2) break;
		}
	}
}

/* ------------------------------------------------------------------------- */
void cIocaineBot::reset_stats(struct stats *st) 
{
	int i;
	st->age = 0;
	for (i = 0; i < 3; ++i) st->sum[st->age][i] = 0;
}

void cIocaineBot::add_stats(struct stats *st, int i, int delta) {
	st->sum[st->age][i] += delta;
}

void cIocaineBot::next_stats(struct stats *st) {
	if (st->age < HISTORY_SIZE) {
		int i;
		++(st->age);
		for (i = 0; i < 3; ++i)
			st->sum[st->age][i] = st->sum[st->age - 1][i];
	}
}

int cIocaineBot::max_stats(const struct stats *st, int age, int *which, int *score)
{
	int i;
	*which = -1;
	for (i = 0; i < 3; ++i) {
		int diff;
		if (age > st->age)
			diff = st->sum[st->age][i];
		else
			diff = st->sum[st->age][i] - st->sum[st->age - age][i];
		if (diff > *score) {
			*score = diff;
			*which = i;
		}
	}

	return -1 != *which;
}

/* ------------------------------------------------------------------------- */


void cIocaineBot::reset_predict(struct predict *pred)
{
	reset_stats(&pred->st);
	pred->last = -1;
}

/* last: opponent's last move (-1 if none)
| guess: algorithm's prediction of opponent's next move */
void cIocaineBot::do_predict(struct predict *pred, int last, int guess) 
{
	if (-1 != last) {
		const int diff = (3 + last - pred->last) % 3;
		add_stats(&pred->st, will_beat(diff), 1);
		add_stats(&pred->st, will_lose_to(diff), -1);
		next_stats(&pred->st);
	}

	pred->last = guess;
}

void cIocaineBot::scan_predict(struct predict *pred, int age, int *move, int *score)
{
	int i;
	if (max_stats(&pred->st, age, &i, score)) 
		*move = ((pred->last + i) % 3);
}

/* ------------------------------------------------------------------------- */

const int cIocaineBot::ages[] = { 1000, 100, 10, 5, 2, 1 };


// ----------------------------------------------------------------------------
int cIocaineBot::biased_roshambo(double prob_rock, double prob_paper)
{
	/* roshambo with given probabilities of rock, paper, or scissors */
	double choice = rand() / MAXRANDOM;

	if (choice < prob_rock) { return(rock); }
	else if (choice < prob_rock + prob_paper) { return(paper); }
	else /* choice >= prob_rock + prob_paper */ { return(scissors); }
}

 int cIocaineBot::do_iocaine(iocaine *i) 
 {
	const int num = my_history[0];
	const int last = (num > 0) ? opp_history[num] : -1;
	const int guess = biased_roshambo(1.0 / 3.0, 1.0 / 3.0);
	int w, a, p;

	if (0 == num) {
		for (a = 0; a < num_ages; ++a) {
			reset_predict(&i->pr_meta[a]);
			for (p = 0; p < 2; ++p) {
				for (w = 0; w < 3; ++w)
					reset_predict(&i->pr_history[a][w][p]);
				reset_predict(&i->pr_freq[a][p]);
			}
		}
		for (p = 0; p < 2; ++p) reset_stats(&i->stats[p]);
		reset_predict(&i->pr_random);
		reset_predict(&i->pr_fixed);
	}
	else {
		add_stats(&i->stats[0], my_history[num], 1);
		add_stats(&i->stats[1], opp_history[num], 1);
	}

	for (a = 0; a < num_ages; ++a) {
		int best[3];
		do_history(ages[a], best);
		for (w = 0; w < 3; ++w) {
			const int b = best[w];
			if (0 == b) {
				do_predict(&i->pr_history[a][w][0], last, guess);
				do_predict(&i->pr_history[a][w][1], last, guess);
				continue;
			}
			do_predict(&i->pr_history[a][w][0], last, my_history[b + 1]);
			do_predict(&i->pr_history[a][w][1], last, opp_history[b + 1]);
		}

		for (p = 0; p < 2; ++p) {
			int best = -1, freq;
			if (max_stats(&i->stats[p], ages[a], &freq, &best))
				do_predict(&i->pr_freq[a][p], last, freq);
			else
				do_predict(&i->pr_freq[a][p], last, guess);
		}
	}

	do_predict(&i->pr_random, last, guess);
	do_predict(&i->pr_fixed, last, 0);

	for (a = 0; a < num_ages; ++a) {
		int aa, score = -1, move = -1;
		for (aa = 0; aa < num_ages; ++aa) {
			for (p = 0; p < 2; ++p) {
				for (w = 0; w < 3; ++w)
					scan_predict(&i->pr_history[aa][w][p],
						ages[a], &move, &score);
				scan_predict(&i->pr_freq[aa][p], ages[a], &move, &score);
			}
		}

		scan_predict(&i->pr_random, ages[a], &move, &score);
		scan_predict(&i->pr_fixed, ages[a], &move, &score);
		do_predict(&i->pr_meta[a], last, move);
	}

	{
		int score = -1, move = -1;
		for (a = 0; a < num_ages; ++a)
			scan_predict(&i->pr_meta[a], HISTORY_SIZE, &move, &score);
		return move;
	}
}

/* ------------------------------------------------------------------------- */

bool cIocaineBot::choose(void)
{
	reset();

	int choice =  do_iocaine(&p);
	setShape(choice);
	return (bool)(nullptr != pChosenShape);;
}

#pragma warning(pop)
