#include <iostream>
#include <list>
#include <stack>
#include <algorithm>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;

typedef struct Implicant {
    int* array; //integer array of size in sorted order
    int size;
    Implicant(){}
    Implicant(int _size) {
        size = _size;   //allocate size*sizeof(int) memory to array
        array = new int[size];
    }
    Implicant(int size, Implicant i1, Implicant i2) {
        this->size = size;
        this->array = new int[size];
        int i = 0, j = 0, k = 0;
        
        while(i<i1.size && j<i2.size){
            if(i1.array[i]<i2.array[j])
                this->array[k++] = i1.array[i++];
            else
                this->array[k++] = i2.array[j++];
        }
        while(i<i1.size)
            this->array[k++] = i1.array[i++];
        while(j<i2.size)
            this->array[k++] = i2.array[j++];
    //create new implicant with size. i1.array and i2.array are merged, sorted and added to array
    }
}Implicant;

typedef struct Bucket {
    list<Implicant> implicants;
    Bucket(){}
}Bucket;

typedef struct BucketStore {
    stack<Bucket*> store;
}BucketStore;

int numberOf1(int v){       //by brian kernighan
    int c; // c accumulates the total bits set in v
    for (c = 0; v; c++){
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

bool isPowerOf2(int x){
    return x && (!(x&(x-1)));
}

bool isSuperSet(Implicant x,Implicant y){   //this function returns true if all elements of x are present in y
    if(x.size>y.size)
        return false;
    for(int i=0;i<x.size;i++){
        if(!binary_search(y.array,y.array+(y.size),x.array[i]))
            return false;
    }
    return true;
}

bool checkPresence(Implicant x,Bucket pImp){    // checks if all the elements of the implicant x are already present in the Bucket of prime implicants
    for(list<Implicant>::iterator it = pImp.implicants.begin(); it != pImp.implicants.end() ;++it){ // need to check all the implicants in pImp for superset
        if(isSuperSet(x,*it))
            return true;
    }
    return false;
}

int findInImplicant(Implicant x,list<int> minterms){   // returns number of minterms present in the current implicant
    int count=0;
    for(list<int>::iterator it=minterms.begin();it!=minterms.end();++it){
        for(int i=0;i<x.size;i++)
            if(x.array[i]==*it)
                count++;
    }
    return count;
}

bool notAlreadyThere(Bucket b,Implicant e){
    for(list<Implicant>::iterator it=b.implicants.begin();it!=b.implicants.end();++it){
        int count=0,i;
        if((*it).size==e.size){
            for(i=0;i<(*it).size;i++){
                if(e.array[i]==(*it).array[i])
                    count++;
            }
            if(count==i)
                return false;
        }
    }
    return true;
}


Bucket* stepB1_12(list<int> minterms,int &size){
    minterms.sort();
    int size_of_bucket = ceil(log2(minterms.back()))+1;
    Bucket* pBucket = new Bucket[size_of_bucket];
    for(list<int>::iterator it=minterms.begin(); it != minterms.end(); ++it){
        Implicant e(1);
        e.array[0]=*it;
        //cout<<*it<<" "<<numberOf1(*it)<<endl;
        pBucket[numberOf1(*it)].implicants.push_back(e);
    }
    size=size_of_bucket;
    return pBucket;
}


Bucket* stepB1_3(Bucket* pBucket,int size,int &change){
    change=0;
    Bucket* nBucket = new Bucket[size];
    for(int i=0;i<size;i++){
        for(list<Implicant>::iterator it1=pBucket[i].implicants.begin(); it1 != pBucket[i].implicants.end(); ++it1){
            Implicant r1 = *it1;
            for(list<Implicant>::iterator it2=pBucket[i+1].implicants.begin(); it2 != pBucket[i+1].implicants.end(); ++it2){
                Implicant r2 = *it2;
                if(isPowerOf2(r2.array[0]-r1.array[0])){
                    int d = r2.array[0]-r1.array[0],k;
                    for(k=1;k<r1.size;k++){
                        if(r2.array[k]-r1.array[k]!=d)
                            break;
                    }
                    if(k==r1.size){
                        Implicant e(2*r1.size,r1,r2);
                        if(notAlreadyThere(nBucket[i],e)){
                            nBucket[i].implicants.push_back(e);
                            change++;
                        }
                    }
                }
            }
        }
    }
    return nBucket;
}


Bucket prime_implicants(BucketStore bs,int size){
    Bucket pImp;
    while(!bs.store.empty()){
        Bucket* tBucket = bs.store.top();
        bs.store.pop();
        size++;
        for(int i=0;i<size;i++){    // traverse through the bucket array
            for(list<Implicant>::iterator it = tBucket[i].implicants.begin(); it != tBucket[i].implicants.end(); ++it){ // traverse through the list in a bucket
                for(int j=0;j<(*it).size;j++){
                    if(!checkPresence(*it,pImp)){   // if superset not present then add to list of pImp
                        pImp.implicants.push_back(*it);
                    }
                }
            }
        }
    }
    return pImp;
}


Bucket ess_prime_implicants(Bucket primeImp,list<int> minterms){    // this function returns the essential prime implicants in a bucket after comparing
    Bucket eImp;
    vector<list<Implicant>::iterator> redundant;
    for(list<int>::iterator it=minterms.begin();it!=minterms.end();++it){
        // if *it is present only once in primeImp then that implicant will be pushed in eImp
        int count=0;
        Implicant es;
        for(list<Implicant>::iterator it1=primeImp.implicants.begin();it1!=primeImp.implicants.end();++it1){
            for(int i=0;i<(*it1).size;i++){
                if((*it1).array[i]==*it){
                    es=*it1;
                    count++;
                }
            }
        }
        if(count==1 && notAlreadyThere(eImp,es)){
            eImp.implicants.push_back(es);
        }    
    }
    //cout<<eImp.implicants.size()<<endl;
    while(!minterms.empty()){
        for(list<Implicant>::iterator it1=eImp.implicants.begin();it1!=eImp.implicants.end();++it1){    // removing minterms which have already appeared
            for(int i=0;i<(*it1).size;i++){ // traverse array of an essential implicant 
                minterms.remove((*it1).array[i]);   //remove coresponding minterm
            }
        }
        if(!minterms.empty()){
            for(list<Implicant>::iterator it1=primeImp.implicants.begin();it1!=primeImp.implicants.end();++it1){    // removing prime implicants which are redundant
                if(findInImplicant(*it1,minterms)==0)
                    redundant.push_back(it1);   // iterators of all the redundant implicants are in vector
            }
            for(auto i=redundant.begin();i!=redundant.end();++i){
                primeImp.implicants.erase(*i);
            }
            redundant.clear();
            //now all the redundant implicants have been deleted Need to find next essential implicant
            int m=0;
            Implicant es;
            for(list<Implicant>::iterator it1=primeImp.implicants.begin();it1!=primeImp.implicants.end();++it1){
                if(m<findInImplicant(*it1,minterms)){
                    m=findInImplicant(*it1,minterms);
                    es=*it1;
                }
            }
            eImp.implicants.push_back(es);
        }
    }
    return eImp;
}


string solve(Bucket* pBucket,int size,list<int> minterms,string variables){
    BucketStore bs;
    int change = 2;
    string reducedExp="";
    while(change>=1 && size>=1){
        if(change>=1){  // if any merger has taken place then pBucket will be pushed to the stack
            bs.store.push(pBucket);
            size--; // size is 1 less than the size of the topmost pushed bucket array
        }
        pBucket=stepB1_3(pBucket,size,change);  // returned nBucket is saved in pBucket. size of this is size-1      
    }
    // step 1 is completed but there are duplicate implicants in the lists
    // proceeding to step 2 - finding the prime implicants
    

    Bucket primeImp=prime_implicants(bs,size);

    // step 2 is completed now
    // proceed to find the essential prime implicants
    Bucket essImp=ess_prime_implicants(primeImp,minterms);
    int i,j;
    for(list<Implicant>::iterator it=essImp.implicants.begin(); it!=essImp.implicants.end() ; ++it){
        if(it!=essImp.implicants.begin())
            reducedExp+=" + ";
        for(i=0;i<variables.length();i++){
            //cout<<variables<<endl;
            bool b=((*it).array[0]>>i) & 1; // finds the ith bit of the array[0];
            for(j=0;j<(*it).size;j++){
                if(((*it).array[j]>>i) & 1 != b)
                    break;
            }
            //cout<<"J="<<j<<endl;
            if(j==(*it).size){
                if(b==1)
                    reducedExp+=variables[i];
                else
                    reducedExp=reducedExp + variables[i] + "'";
            }
        }
    }
    return reducedExp;
}


int main(){
    BucketStore bs;
    string variables;
    int input=0;
    int choice;
    cout<<"Enter 1 to read from output file and 2 to give custom input\n";
    cin>>choice;

    if(choice==1){
        ifstream fin;
        ofstream fout;
        char c;
        fin.open("output.txt");
        fout.open("outputB.txt");
        string firstToken,temp;
        while(fin){
            fin>>firstToken;
            if(firstToken==".input"){
                fout<<firstToken<<' ';
                fin.get(c);
                while(c!='\n'){
                    fin.get(c);
                    if(isalnum(c)){  // if the character is an alphabet then it is to be present in every expression
                        variables+=c;
                        fout<<c<<" ";
                    }
                }
                fout<<endl;
            }

            else{   // each line is a minterm expression
                list<int> minterms;
                string num="";
                fin.get(c);fin.get(c);fin.get(c);fin.get(c);
                char d=c;
                if(d=='m'){
                    if(firstToken!=temp)
                        fout<<firstToken<<" = ";
                    while(c!='\n'){
                        fin.get(c);
                        if(isdigit(c)){
                            while(isdigit(c)){
                                num=num+c;
                                fin.get(c);
                            }
                            minterms.push_back(stoi(num));
                            //cout<<minterms.back()<<" ";
                            num="";
                        }
                    }
                    // minterms is created 
                    if(!minterms.empty()){
                        int size=0,size1;
                        Bucket* pBucket=stepB1_12(minterms,size);
                        size1=size;
                        //cout<<size<<endl;
                        fout<<solve(pBucket,size,minterms,variables)<<endl;
                    }
                    temp=firstToken;
                }
                else{
                    while(c!='\n'){
                        fin.get(c);
                    }
                }
            }

        }
        cout<<"Your final reduced expressions have been saved in outputB.txt\n";
        fin.close();
        fout.close();
    }
    
    else if(choice==2){
        list<int> minterms;
        cout<<"Enter the list of minterms and enter -1 to stop\n";  // need to remove all duplicates
        while(input!=-1){
            cin>>input;
            if(input>=0){
                minterms.push_back(input);
            } 
            else 
                break;
        }
        int size=0,size1;
        Bucket* pBucket=stepB1_12(minterms,size);
        size1=size;
        cout<<size<<endl;
        cout<<"Enter "<<size1<<" literals from lsb to msb"<<endl;
        for(int i=0;i<size1;i++){
            char c;
            cin>>c;
            variables+=c;
        }
        cout<<solve(pBucket,size,minterms,variables)<<endl;
    }
    else
        return 0;
}


    // while(!bs.store.empty()){
    //     Bucket* tBucket = bs.store.top();
    //     bs.store.pop();
    //     size++;
    //     for(int i=0;i<size;i++){
    //         cout<<i<<"---> ";
    //         for(list<Implicant>::iterator it=tBucket[i].implicants.begin();it!=tBucket[i].implicants.end();++it){
    //             for(int j=0;j<(*it).size;j++){
    //                 cout<<(*it).array[j]<<",";
    //             }
    //             cout<<" ";
    //         }
    //         cout<<endl;
    //     }
    //     cout<<endl;
    // }

    // cout<<"Prime Implicants are:\n";
    // for(list<Implicant>::iterator it=primeImp.implicants.begin(); it!=primeImp.implicants.end() ; ++it){
    //     cout<<"( ";
    //     for(int j=0;j<(*it).size;j++){
    //         cout<<(*it).array[j]<<" ";
    //     }
    //     cout<<")   ";
    // }

    // Bucket essImp=ess_prime_implicants(primeImp,minterms);
    // ofstream fout;
    // fout.open("outputB.txt");
    // cout<<"Essential Prime Implicants are:\n";
    // for(list<Implicant>::iterator it=essImp.implicants.begin(); it!=essImp.implicants.end() ; ++it){
    //     cout<<"( ";
    //     for(int j=0;j<(*it).size;j++){
    //         cout<<(*it).array[j]<<" ";
    //     }
    //     cout<<")   ";
    // }
    // cout<<endl<<endl;