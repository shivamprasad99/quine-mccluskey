#include <iostream>
#include <list>
#include <stack>
#include <algorithm>
#include <vector>
#include <math.h>
#include <fstream>
#include <bitset>
using namespace std;

bool isPOS(string exp){
    for(int i=0;i<exp.length();i++){
        if(exp[i]=='(')
            return true;
    }
    return false;
}

bool isAlphabet(char c){
    int x=(int)c;
    if((x>=65 && c<=90) || (x>=97 && x<=122))
        return true;
    return false;
}

int position(string variables,char c){
    for(int i=variables.length()-1;i>=0;i--){
        if(c==variables[i])
            return i;
    }
    return -1;
}

string addDot(char c){
    string s="";
    s=s+'.'+c;
    return s;
}

string evaluateSOP(string variables,string expression){   // make truth table of size 2^n and evaluate for the given expression
    int size=pow(2,variables.length());
    int l=variables.length();
    bitset<32> table[size];
    bool minterms[size]={0},temp;


    for(int i=0;i<size;i++){
        table[i]=bitset<32>(i);
    }

    for(int i=0;i<size;i++){
        temp=1;
        for(int j=0;j<expression.length();j++){
            if(isAlphabet(expression[j])){
                char p=position(variables,expression[j]);
                if(expression[j+1]=='\''){
                    temp=temp&(!table[i][p]);
                    j++;
                }
                else{
                    temp=temp&(table[i][p]);
                }
            }
            if(expression[j]=='+' || j==expression.length()-1){
                minterms[i]=minterms[i] | temp;
                temp=1;
                continue;
            }
        }
    }
    string sop="m(";
    for(int i=0;i<size;i++){
        if(minterms[i]==1){
            sop=sop+to_string(i)+",";
        }
    }
    sop = sop.substr(0, sop.size()-1);
    sop=sop+")";
    return sop;
}

string evaluatePOS(string variables,string expression,int choice){
    int size=pow(2,variables.length());
    int l=variables.length();
    bitset<32> table[size];
    bool maxterms[size],temp;

    for(int i=0;i<size;i++){
        table[i]=bitset<32>(i);
        maxterms[i]=1;
    }

    for(int i=0;i<size;i++){
        temp=0;
        for(int j=0;j<expression.length();j++){
            if(isAlphabet(expression[j])){
                char p=position(variables,expression[j]);
                if(expression[j+1]=='\''){
                    temp=temp | (!table[i][p]);
                    j++;
                    if(expression[j+1]=='+') 
                        j++;
                }
                else{
                    temp=temp | (table[i][p]);
                    if(expression[j+1]=='+')
                        j++;
                }
            }
            else if(expression[j]==')'){
                maxterms[i]=maxterms[i]&temp;
                temp=0;
            }
        }
    }
    string pos;
    if(choice==0){
        pos="m(";
        for(int i=0;i<size;i++){
            if(maxterms[i]!=0){
                pos=pos+to_string(i)+",";
            }
        }
    }
    if(choice==1){
        pos="M(";
        for(int i=0;i<size;i++){
            if(maxterms[i]==0){
                pos=pos+to_string(i)+",";
            }
        }
    }
    pos = pos.substr(0, pos.size()-1);
    pos=pos+")";
    return pos;
}

int main(){
    char c;
    string firstToken,expression;
    ifstream fin;
    ofstream fout;
    fout.open("output.txt");
    fin.open("input.txt");
    string variables="",outVariables="";
    // each element of the vector is a list with n number of nodes and each list is a sop term
    int expressionNumber=0,choice;
    cout<<"If you want maxterms displayed for POS then press 1 and 0 if you want all minterms only (this will help in getting reduced SOP expression for POS also)\n";
    cin>>choice;

    while(fin){
        fin>>firstToken;
        expression="";
        
        if(firstToken==".input"){   //first line taken care of
            if(fout){
                fout<<".input ";
                fin.get(c);
                while(c!='\n'){
                    fin.get(c);
                    if(isAlphabet(c)){  // if the character is an alphabet then it is to be present in every expression
                        variables=variables+c;
                    }
                }
                for(int t=0;t<variables.length();t++){
                    fout<<variables[t]<<" ";
                }
                fout<<endl;
                continue;
            }
        }

        else if(firstToken==".output"){
            fin.get(c);
            while(c!='\n'){
                fin.get(c);
                if(c!=' '){
                    outVariables=outVariables+c;
                }
            }
            cout<<outVariables<<endl;
            continue;
        }

        else if(firstToken==addDot(outVariables[expressionNumber])){
            expressionNumber++;
            //cout<<firstToken<<endl;
            fin.get(c);
            while(c!='\n' && fin){
                if(isAlphabet(c) || c=='+' || c=='*' || c=='(' || c==')' || c=='\''){
                    expression=expression+c;
                }
                fin.get(c);
            }
            //cout<<expression<<endl;
            if(!isPOS(expression)){
                if(fout){
                    fout<<outVariables[expressionNumber-1]<<" = ";
                    fout<<evaluateSOP(variables,expression)<<endl;
                }
            }
            else{
                if(fout){
                    fout<<outVariables[expressionNumber-1]<<" = ";
                    fout<<evaluatePOS(variables,expression,choice)<<endl;
                }
            }
            continue;
        }
    }
    fin.close();
    fout.close();
    cout<<"Your output has been stored in output.txt\n";
}

