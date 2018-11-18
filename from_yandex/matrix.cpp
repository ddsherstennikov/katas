#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;



void zero(vector<vector<int>>& G)
{
	int N = G.size();
	int M = G[0].size();
	int zeroout_row = 0;

	for (int row = 1; row < M; row++)
	{
		for (int col = 0; col < N; col++)
		{
			if (G[col][row - 1] == 0)
			{
				zeroout_row = min(zeroout_row, 1);
				if (G[col][row] != 0)
				{
					G[col][row] = 0;
				}
				else
				{
					zeroout_row = 2;
				}
			}
		}

		if (zeroout_row)
		{
			for (int col = 0; col < N; col++)
				G[col][row - 1] = 0;
			zeroout_row = max(zeroout_row - 1, 0);
		}
	}

	for (int col = 0; col < N; col++)
		if (G[col][M - 1] == 0)
			swap(G[col], vector<int>(M, 0));
}



int main()
{
	int N, M;
	
	cin >> N >> M;

	vector<vector<int>> G(N, vector<int>(M, 0));

	for (int row = 0; row < M; row++)
		for (int col = 0; col < N; col++)
			cin >> G[col][row];
		
	zero(G);

	for (int row = 0; row < M; row++)
	{
		for (int col = 0; col < N; col++)
			cout << G[col][row] << " ";
		cout << endl;
	}


    return 0;
}

