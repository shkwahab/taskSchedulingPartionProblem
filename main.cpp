#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

struct Task
{
    int priority;
    int deadline;
};
vector<Task> tasks;

void addTask(int priority, int deadline)
{
    Task task;
    task.priority = priority;
    task.deadline = deadline;
    tasks.push_back(task);
}

bool findPartition(vector<int> &arr, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    if (sum % 2 != 0)
    {
        return false;
    }
    vector<vector<bool>> dp(n + 1, vector<bool>(sum / 2 + 1, false));
    for (int i = 0; i <= n; i++)
    {
        dp[i][0] = true;
    }
    for (int i = 1; i <= sum / 2; i++)
    {
        dp[0][i] = false;
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= sum / 2; j++)
        {
            dp[i][j] = dp[i - 1][j];
            if (arr[i - 1] <= j)
            {
                dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i - 1]];
            }
        }
    }
    return dp[n][sum / 2];
}

void subsetTasks(vector<Task> &subset)
{
    cout << "Subset of Tasks:" << endl;
    for (int i = 0; i < subset.size(); i++)
    {
        cout << "Task " << i + 1 << ": Priority = " << subset[i].priority << " Deadline = " << subset[i].deadline << endl;
    }
}

/*
scheduleTasks() function, I have used the dynamic programming approach to divide the tasks into two subsets based on their deadlines and priorities such that the workload is balanced as much as possible. Also, I have used std::none_of to check if a task is not present in a vector of tasks.
*/
void scheduleTasks()
{
    int n = tasks.size();

    // Sort tasks by their deadlines
    sort(tasks.begin(), tasks.end(), [](Task a, Task b)
         { return a.deadline < b.deadline; });

    // Create a 2D array to store the maximum priority that can be achieved for a given set of tasks and a given total time
    vector<vector<int>> dp(n + 1, vector<int>(n + 1));

    // Fill the dp array
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            dp[i][j] = dp[i - 1][j];
            if (j >= tasks[i - 1].deadline)
            {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - tasks[i - 1].deadline] + tasks[i - 1].priority);
            }
        }
    }

    // Create two vectors to store the tasks in each subset
    vector<Task> subset1, subset2;

    // Backtrack to find the tasks in each subset
    int i = n, j = n / 2;
    while (i > 0 && j > 0)
    {
        if (dp[i][j] == dp[i - 1][j])
        {
            i--;
        }
        else
        {
            subset1.push_back(tasks[i - 1]);
            i--;
            j -= tasks[i - 1].deadline;
        }
    }

    for (int k = 0; k < n; k++)
    {
        if (std::none_of(subset1.begin(), subset1.end(), [&](Task &t)
                         { return t.priority == tasks[k].priority && t.deadline == tasks[k].deadline; }))
            subset2.push_back(tasks[k]);
    }

    // Print the tasks in each subset
    subsetTasks(subset1);
    subsetTasks(subset2);
}
int main()
{
    addTask(5, 3);
    addTask(3, 2);
    addTask(1, 4);
    addTask(2, 5);
    addTask(4, 1);
    scheduleTasks();
    return 0;
}