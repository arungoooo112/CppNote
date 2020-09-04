// 排序

//选择排序
#include <algorithm>
#include <vector>
#include <myio>
#include <iostream>
using namespace std;

template <typename Iterator>
void SelectionSort(Iterator first, Iterator last)
{
	while (first != last)
	{
		Iterator min = first;
		for (Iterator next = first + 1; next != last; next++)
			if (*next < *min)
				min = next;
		swap(*min, *first++);
	}
}

template <typename Iterator>
void BubbleSort(Iterator first, Iterator last)
{
	while (first != last)
	{
		bool isSorted = true;
		for (Iterator next = first; next != last - 1; ++next)
		{
			if (*(next + 1) < *next)
			{
				swap(*(next + 1), *next);
				isSorted = false;
			}
		}
		if (isSorted) return;
		last--;
	}
}


template <typename Iterator>
void InsertSort(Iterator first, Iterator last)
{
    Iterator begin = first;
    while (first != last)
    {
        for (Iterator next = first; next > begin && *next < *(next - 1); --next)
            swap(*next, *(next - 1));
        first++;
    }
}
//归并排序，迭代实现

template<typename Iterator>
void merge(Iterator start, Iterator mid, Iterator end)
{
	typedef typename iterator_traits<Iterator>::value_type value_type;
	const int nums = end - start;
	value_type* aux = new value_type[nums];

	int k = 0;
	Iterator it1 = start, it2 = mid;
	while (it1 < mid && it2 < end)
		aux[k++] = *it1 < *it2 ? *it1++ : *it2++;
	while (it1 < mid) aux[k++] = *it1++;
	while (it2 < end) aux[k++] = *it2++;

	std::copy(aux, aux + nums, start);
	delete[] aux;
}

template<typename Iterator>
void mergeSort(Iterator first, Iterator last)
{
	if (last - first <= 1) return;
	int diff = last - first;
	Iterator mid = first + diff / 2;
	mergeSort(first, mid);
	mergeSort(mid, last);
	merge(first, mid, last);
}

int main()
{
	printf("dasdas");
	vector<int> vec = { 4,8,9,4,3,1,7,1,3,4,8 };
	cout << "before sort: \n";
	my::print(vec.begin(), vec.end());
	InsertSort(vec.begin(), vec.end());
	cout << "\nafter sort: \n";
	my::print(vec.begin(), vec.end());
}