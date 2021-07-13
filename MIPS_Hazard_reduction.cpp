/*
    AUTHOR:
    IMT2018003 ADITYA SHETH
    IMT2018073 SOHAM KOLHE
    IMT2018505 AVIK BHATNAGAR
*/

#include<bits/stdc++.h>
using namespace std;

/*
    Assumptions:
    1) Statement type: add w1 w2 w3
    2) Instruction memory starts from 0 .    
*/

// map<string,string> reg_to_binary;
// map<string,int> word_to_line;

vector<string> Split_into_words(string str) 
{ 
    vector<string> v;
    string word = ""; 
    for (auto x : str) 
    { 
        if (x == ' ') 
        { 
            v.push_back(word); 
            word = ""; 
        } 
        else
        {
            word = word + x;
        }
    }
    v.push_back(word);
    return v;
}

bool check_dependency(string i1, string i2)
{
    vector<string> s1,s2;
    s1 = Split_into_words(i1);
    s2 = Split_into_words(i2);
    int i;
    for(i=1;i<s1.size();i++)
    {
        if(s2[1]==s1[i])
        {
            return true;
        }
    }

    for(i=1;i<s2.size();i++)
    {
        if(s1[1]==s2[i])
        {
            return true;
        }
    }
    return false;    
}

vector<int> reorder_instructions(vector<set<int>> &out,vector<set<int>> &in)
{
    /*
    Points:
    We know the dependencies of graph:
        i) in gives the incoming nodes of graph and stores from where the edge is coming.
        ii) out gives the outgoing nodes of a graph and stores the nodes where the edge is going.
    The algorithm tries to reduce the hazard by following greedy heuristics. We just need to make sure that 
    the dependancy graph remains topologically sorted.
    The basic idea implemented is- try to select any instruction which has no dependent instruction executed 
    in last two instructions. If no such instruction exist execute any candidate instruction.
    
    What is candidate_node: Any node which has all the dependent nodes already taken is contained in candidate_node set.

    Note: The problem we are solving is NP-complete and there is not yet any efficient polynomial time solution which gives
    us the most optimal answer. Therefore, we use some heuristics to do the task.
    */
	int n=out.size();
	set<int> candidate_nodes;   // candidate nodes which can be removed at any stage, all the dependent instructions have been executed
	int dependancy[n];          //0 means candidate node, -1 means already done, +ve number is trivial.
	for(int i=0;i<n;i++)
	{
		dependancy[i]=in[i].size();
		if(dependancy[i]==0)
		{
			candidate_nodes.insert(i);
		}
	}
	
	vector<int> ans;
	for(int i=0;i<n;i++)
	{
		assert(candidate_nodes.size()>0);
		if(i==0)
		{		
			auto v=candidate_nodes.begin();
			ans.push_back(*v);
			candidate_nodes.erase(v);
			dependancy[*v]=-1;
		}
		else if(i==1)
		{
			for(auto &x: candidate_nodes)
			{
				if(!out[ans[i-1]].count(x))
				{
					ans.push_back(x);
					dependancy[x]=-1;
					candidate_nodes.erase(x);
					break;
				}
			}
		}
		else
		{			
			for(auto &x: candidate_nodes)
			{
				if(!out[ans[i-1]].count(x) && !out[ans[i-2]].count(x))
				{
					ans.push_back(x);
					dependancy[x]=-1;
					candidate_nodes.erase(x);
					break;
				}
			}
		}
		if(ans.size()==i)
		{
			auto v=candidate_nodes.begin();
			ans.push_back(*v);
			candidate_nodes.erase(v);
			dependancy[*v]=-1;
		}
		for(auto x: out[ans[i]])
		{
			dependancy[x]--;
			if(dependancy[x]==0)
			{
				candidate_nodes.insert(x);
			}
		}
	}
	return ans;
}

int main()
{
    // freopen("output.txt","w",stdout);
    // freopen("input.asm","r",stdin);
    vector<string> instructions;
    while(true)
    {
        string cur;
        getline(cin,cur);
        if(cur=="end")
        {
            break;
        }
        instructions.push_back(cur);
    }
    int n=instructions.size();
    vector<set<int>> in(n),out(n);
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(check_dependency(instructions[i],instructions[j]))
            {
                out[i].insert(j);
                in[j].insert(i);
            }
        }
    }
    vector<int> ans=reorder_instructions(out,in);
    for(int i=0;i<ans.size();i++)
    {
        cout<<instructions[ans[i]]<<endl;
    }
}

/*
Test cases:

check in whatsapp group for answer.
lw $1 $0
lw $2 $0
add $3 $1 $2
sw $3 $0
lw $4 $0
add $3 $1 $4
sw $3 $0
end

Output:
lw $1 $0
lw $2 $0
lw $4 $0
add $3 $1 $2
sw $3 $0
add $3 $1 $4
sw $3 $0


add $4 $1 $1
add $7 $4 $9
lw $2 $8 
sub $8 $1 $2
sw $8 $2
sub $2 $8 $4
lw $8 $1
add $8 $4 $2
end


addi $s1 $0 10
lw $t0 $s0
srl $t1 $t0 1
add $t2 $t1 $s1
sw $t2 $s0
end
*/

/*
While presentation, draw the dependancy graph and explain.
*/