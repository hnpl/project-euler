// Question: https://projecteuler.net/problem=348

#include<iostream>
#include<unordered_map>
#include<string>
#include<algorithm>
#include<vector>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 1000000000;

bool is_palindrome(ui n)
{
	string str = to_string(n);
	auto length = str.size();
	for (ui i = 0; i < length/2; i++)
		if (!(str[i] == str[length-1-i]))
			return false;
	return true;
}

int main()
{
	ui ans = 0;

	unordered_map<ui, ui> freq;

	for (ui i = 2; i*i <= N; i++)
	{
		ui square = i*i;
		for (ui j = 2; j*j*j+square <= N; j++)
		{
			ui cube = j*j*j;
			ui sum = square + cube;
			if (is_palindrome(sum))
			{
				if (freq.find(sum) == freq.end())
					freq[sum] = 0;
				freq[sum] = freq[sum] + 1;
			}
		}
	}

	vector<ui> keys;
	for (auto p: freq)
		if (p.second == 4)
			keys.push_back(p.first);

	sort(keys.begin(), keys.end());	

	for (ui i = 0; i < 5; i++)
		ans = ans + keys[i];

	cout << ans << endl;
	return 0;
}
