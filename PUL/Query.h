#ifndef QUERY_H
#define QUERY_H

#include"graphCompressd.h"

bool query(vector< TWO_HOP>& index, int s, int t) {
	int t_o = -1;
	auto& pa = index[t].parent;
	if (!pa.isEmpty()) {
		
		/*for (const int & it : pa) {
			t_o = it;
		}
		*/
		t_o = pa.minimum();
		if (t_o == s) {
			return true;
		}
	}
	roaring::Roaring s_out = index[s].out;
	auto& t_in = index[t].in;
	auto& t_o_in = index[t_o].in;
	s_out.add(s);
	
		if (s_out.contains(t)||s_out.contains(t_o)||s_out.intersect(t_in)) {
			return true;
		}
		if (t_o != -1 && s_out.intersect(t_o_in)) {
			return true;
		}
	    return false;
}

#endif // !QUERY_H

