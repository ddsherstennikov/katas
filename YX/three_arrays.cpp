#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;



vector<array<int,3>> find_x_components(vector<int> & a_unsorted, vector<int>& b_unsorted, vector<int>& c_unsorted, int x)
{
	vector<array<int,3>> v;
	
	vector<int> a = a_unsorted;
	vector<int> b = b_unsorted;
	vector<int> c = c_unsorted;
	
	int N = a.size();
	int M = b.size();
	int K = c.size();
	
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	sort(c.begin(), c.end());
	
	if (a.back() + b.back() + c.back() < x || a[0] + b[0] + c[0] > x)
		return v;
	
	for (int i=0; i<N; i++)
	{
		int j = 0;
		int k = K-1;
		
		while (j < M-1 || k > 0)
		{
			if (a[i] + b[j] + c[k] == x)
			{
				v.push_back({i, j, k});
				if (j < M - 2)
					j++;
				else if (k > 1)
					k--;
				else
					break;
			}
			else if (a[i] + b[j] + c[k] < x)
			{
				if (j < M - 2)
					j++;
				else
					break;
			}
			else if (a[i] + b[j] + c[k] > x)
			{
				if (k > 1)
					k--;
				else
					break;
			}
		}
	}
	
	for (auto& arr : v)
	{
		auto it = find(a_unsorted.begin(), a_unsorted.end(), a[arr[0]]);
		auto jt = find(b_unsorted.begin(), b_unsorted.end(), b[arr[1]]);
		auto kt = find(c_unsorted.begin(), c_unsorted.end(), c[arr[2]]);
		
		arr = {it-a_unsorted.begin(), jt-b_unsorted.begin(), kt-c_unsorted.begin()};
	}
	
	return v;
}



int main()
{
	int x, N, M, K;
	cin >> x;
	cin >> N >> M >> K;
	
	vector<int> a(N);
	vector<int> b(M);
	vector<int> c(K);
	
	for (int i=0; i<N; i++)
		cin >> a[i];
	for (int i=0; i<M; i++)
		cin >> b[i];
	for (int i=0; i<K; i++)
		cin >> c[i];
		
	vector<array<int,3>> v = find_x_components(a, b, c, x);
	
	if (v.empty())
		cout << "NULL";
	
	for(auto& arr : v)
		cout << arr[0] << " " << arr[1] << " " << arr[2] << endl;
}

