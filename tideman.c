#include <stdio.h>
#include <bsd/string.h>
#include <stdbool.h>
#define MAX 9
#define SIZE 50

typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
char candidates[MAX][SIZE];
pair pairs[MAX * (MAX - 1) / 2];

bool locked[MAX][MAX];
int preferences[MAX][MAX];
int pair_count;
int candidate_count;

bool vote(int rank, char name[SIZE], int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, char *argv[])
{
    // Checking for invalid use case
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }
    // Initializing array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        strlcpy(candidates[i], argv[i + 1],sizeof(candidates));
    }

    // Clearing graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count;
    printf("Number of voters: ");
    scanf("%i", &voter_count);

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Initializing rank[] with candidate count
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char name[SIZE];
            printf("Rank %i: ", j + 1);
            scanf("%s", name);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
        printf("\n");
    }
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks array after a new vote
bool vote(int rank, char name[SIZE], int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // update ranks array for each voter with canidated index
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences after voter's rank
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // update preferences 2D array based on ranks array
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if(preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
  for (int i = 0; i < pair_count; i++)
  {
    int max = i;
    for (int j = i + 1; j < pair_count; j++)
    {
       if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[max].winner][pairs[max].loser])
       {
         max = j;
       }
    }
    pair temp = pairs[i];
    pairs[i] = pairs[max];
    pairs[max] = temp;
  }
  return;
}

bool is_circle(int loser, int winner)
{
    // Base Case 1: if path exist
    if (loser == winner) {
        return true; // it forms a cycle
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if(locked[loser][i]) //check if loser is locked with a candidate
        {
            return is_circle(i, winner); // check if that candidate is locked with  winner
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {

      if (!is_circle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool winner = true;
    int can_index = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            // checking the candidate columm with i
            if(locked[j][i] == true)
            {
                winner = false;
                break;
            }
        }
        if(winner == true)
        {
            can_index = i;
            break;
        }
    }
    if(winner == true)
    {
        printf("The winner is %s\n", candidates[can_index]);
    }
}
