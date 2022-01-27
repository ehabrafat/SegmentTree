#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <limits>   
using namespace std;

int nextPowOf2(int n) {
	return (int)pow(2, static_cast<int>(log2(n) + 0.99));
}
template <class _Type>
class SegmentTree {
public:
	SegmentTree(const vector<_Type>& input, function<_Type(_Type first, _Type last)> cmpFn) : cmpFn(cmpFn), input(input), segTree(nextPowOf2(input.size()) * 2 - 1), size(input.size()) {
		constructTree(0, input.size() - 1, 0);
		if (cmpFn(1, 2) == 2) OUT = numeric_limits<_Type>::min();
		else OUT = numeric_limits<_Type>::max();
	}
	_Type query(int qstart, int qend) {
		if (qstart < 0 || qend >= size) return OUT;
		return query_helper(qstart, qend, 0, size - 1, 0);
	}
private:
	vector<_Type> segTree;
	vector<_Type> input;
	uint64_t size;
	function<_Type(_Type first, _Type last)> cmpFn;
	_Type OUT;
	void constructTree(int low, int high, int pos) {
		if (low == high) {
			segTree[pos] = input[low];
			return;
		}
		int mid = (low + high) / 2;
		constructTree(low, mid, 2 * pos + 1);
		constructTree(mid + 1, high, 2 * pos + 2);
		segTree[pos] = cmpFn(segTree[2 * pos + 1], segTree[2 * pos + 2]);
	}
	_Type query_helper(int qstart, int qend, int low, int high, int pos) {
		if (low >= qstart && high <= qend) return segTree[pos]; // total overlap
		if (low > qend || high < qstart) return OUT; // no overlap
		int mid = (low + high) / 2; // partial overlap
		return cmpFn( query_helper(qstart, qend, low, mid, 2 * pos + 1),
			query_helper(qstart, qend, mid + 1, high, 2 * pos + 2));
		}
};

int main() {
	vector<int> nums{ 1,3,-1,-3,5,3,12,9 };
	auto cmp = [](int x, int y) {return max(x, y); };
	SegmentTree<int> segTree(nums, cmp);
	cout << segTree.query(0, 2) << endl; // 3
	cout << segTree.query(1, 4) << endl; // 5
	cout << segTree.query(3, 7) << endl; // 12
}
