#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <chrono>
using namespace std;
const int M = 10009;

class Words
{
public:
	string key;
	int value;
	bool IsDeleted = false;
	Words() 
	{}
};

class HashMapTable
{
private:
	Words** array;
	int size;
	int count;
public:
	HashMapTable(int s)
	{
		count = 0;
		size = s;
		array = new Words * [size];
		for (int i = 0; i < size; i++) 
		{
			array[i] = NULL;
		}
	}
	long long HashFunc(string const& s)
	{
		const int p = 31;
		const int m = this->size;
		long long hash_value = 0;
		long long p_pow = 1;
		for (char c : s)
		{
			hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
			p_pow = (p_pow * p) % m;
		}
		return hash_value;
	}
	void resize(int new_size)
	{
		bool NotPrime = false;
		int nearest_prime_number_above = new_size + 1;
		while (true)
		{
			NotPrime = false;
			if (nearest_prime_number_above < 2)
			{
				nearest_prime_number_above++;
			}
			int maxN = (int)sqrt((int)nearest_prime_number_above) + 1;
			for (int i = 2; i <= maxN; i++)
			{
				if (nearest_prime_number_above % i == 0)
				{
					NotPrime = true;
					break;
				}
			}
			if (NotPrime)
			{
				nearest_prime_number_above++;
			}
			else
			{
				break;
			}
		}
		Words** old_array = this->array;
		int old_size = this->size;
		this->count = 0;
		this->array = new Words * [nearest_prime_number_above];
		for (int i = 0; i < nearest_prime_number_above; i++)
		{
			array[i] = NULL;
		}
		this->size = nearest_prime_number_above;
		for (int i = 0; i < old_size; i++)
		{
			if (old_array[i] != NULL && old_array[i]->IsDeleted == false)
			{
				Insert(old_array[i]->key);
			}
		}
		delete[] old_array;
	}
	void Insert(string k)
	{
		long long hash_index = HashFunc(k);
		int v = 1;
		while (array[hash_index] != NULL && array[hash_index]->key != k && array[hash_index]->IsDeleted == false)
		{
			hash_index = (hash_index + 1) % size;
		}
		if (array[hash_index] != NULL && array[hash_index]->key == k && array[hash_index]->IsDeleted == false)
		{
			v = array[hash_index]->value + 1;
		}
		array[hash_index] = new Words();
		array[hash_index]->key = k;
		array[hash_index]->value = v;
		count++;
		if (2*count > size)
		{
			int new_size = 2 * size;
			resize(new_size);
		}
	}
	int getValue(string k)
	{
		if (count == 0)
		{
			cout << "Empty" << '\n';
			return 0;
		}
		long long hash_index = HashFunc(k);
		long long temp = hash_index;
		while (true)
		{
			if (array[hash_index] == NULL)
			{
				cout << "Not found" << '\n';
				return 0;
			}
			else if (array[hash_index]->key != k || (array[hash_index]->key == k && array[hash_index]->IsDeleted == true))
			{
				hash_index = (hash_index + 1) % size;
			}
			else
			{
				break;
			}
			if (hash_index == temp)
			{
				temp = -1;
				break;
			}
		}
		if (temp == -1)
		{
			cout << "Not found" << '\n';
			return 0;
		}
		else
		{
			return array[hash_index]->value;
		}
	}
	void Remove(string k) 
	{
		if (count == 0)
		{
			cout << "Empty" << '\n';
			return;
		}
		long long hash_index = HashFunc(k);
		long long temp = hash_index;
		while (true)
		{
			if (array[hash_index] == NULL)
			{
				cout << "Not found" << '\n';
				return;
			}
			else if (array[hash_index]->key != k || (array[hash_index]->key == k && array[hash_index]->IsDeleted == true))
			{
				hash_index = (hash_index + 1) % size;
			}
			else
			{
				break;
			}
			if (hash_index == temp)
			{
				temp = -1;
				break;
			}
		}
		if (temp == -1)
		{
			cout << "Not found";
			return;
		}
		else
		{
			array[hash_index]->IsDeleted = true;
			count--;
			if (8*count < size)
			{
				int new_size = size / 2;
				resize(new_size);
			}
		}
	}
	~HashMapTable() 
	{
		for (int i = 0; i < size; i++)
		{
			if (array[i] != NULL)
			{
				delete array[i];
				array[i] = NULL;
			}
		}
	}
};
int main()
{
	char text[M];
	cin.getline(text, M, '\n');
	HashMapTable hash(M);
	std::chrono::steady_clock::time_point start, end;
	std::chrono::duration<double, std::milli> duration;
	start = std::chrono::steady_clock::now();
	for (int i = 0; i < M; i++)
	{
		if (text[i] == '\0')
		{
			break;
		}
		string temp;
		while (text[i] != ' ')
		{
			if (text[i] == '\0')
			{
				i --;
				break;
			}
			temp += text[i];
			i++;
		}
		hash.Insert(temp);
	}
	end = std::chrono::steady_clock::now();
	duration = end - start;
	cout << "Insertion of whole text with HashMapTable took " << duration.count()<< '\n';
	unordered_map<string, int> c_map;
	start = std::chrono::steady_clock::now();
	for (int i = 0; i < M; i++)
	{
		if (text[i] == '\0')
		{
			break;
		}
		string temp;
		while (text[i] != ' ')
		{
			if (text[i] == '\0')
			{
				i--;
				break;
			}
			temp += text[i];
			i++;
		}
		if (c_map.count(temp))
		{
			c_map[temp]++;
		}
		else
		{
			c_map[temp] = 1;
		}
	}
	end = std::chrono::steady_clock::now();
	duration = end - start;
	cout << "Insertion of whole text with unordered_map took " << duration.count() << '\n';
	string word;
	int v;
	int c;
	while (1) {
		cout << "1.Insert element into the table" << '\n';
		cout << "2.Search element from the key" << '\n';
		cout << "3.Delete element at a key" << '\n';
		cout << "4.Exit" << '\n';
		cout << "Enter your choice: ";
		cin >> c;
		switch (c) {
		case 1:
			cout << "Enter the word to be inserted: ";
			cin >> word;
			start = std::chrono::steady_clock::now();
	        hash.Insert(word);
			end = std::chrono::steady_clock::now();
			duration = end - start;
			cout << "Inserting of one element with HashMapTable took " << duration.count() << '\n';
			start = std::chrono::steady_clock::now();
			if (c_map.count(word))
			{
				c_map[word]++;
			}
			else
			{
				c_map[word] = 1;
			}
			end = std::chrono::steady_clock::now();
			duration = end - start;
			cout << "Inserting of one element with unordered_map took " << duration.count() << '\n';
			break;
		case 2:
			cout << "Enter the word to be searched: ";
			cin >> word;
			cout << "The number of the word " << word <<" searched with HashMapTable "<< " is: ";
			start = std::chrono::steady_clock::now();
			cout << hash.getValue(word) << '\n';
			end = std::chrono::steady_clock::now();
			duration = end - start;
			cout << "Searching with HashMapTable took " << duration.count() << '\n';
			cout << "The number of the word " << word << " searched with unordered_map " << " is: ";
			start = std::chrono::steady_clock::now();
			cout << c_map.find(word)->second << '\n';
			end = std::chrono::steady_clock::now();
			duration = end - start;
			cout << "Searching with unordered_map took " << duration.count() << '\n';
			break;
		case 3:
			cout << "Enter the word to be deleted: ";
			cin >> word;
			start = std::chrono::steady_clock::now();
			hash.Remove(word);
			end = std::chrono::steady_clock::now();
			duration = end - start;
			cout << "Deleting with HashMapTable took " << duration.count() << '\n';
			start = std::chrono::steady_clock::now();
			c_map.erase(word);
			end = std::chrono::steady_clock::now();
			duration = end - start;
			cout << "Deleting with unordered_map took " << duration.count() << '\n';
			break;
		case 4:
			exit(1);
		default:
			cout << "\nEnter correct option\n";
		}
	}
	return 0;
}