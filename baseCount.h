#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

struct PAF_format
{
    bool OperatorSign(int position);

    std::string query_name;
    int query_length;
    int query_start;
    int query_end;
    char strand;
    std::string target_name;
    int target_length;
    int target_start;
    int target_end;
    int residue_matches;
    int alignment_block_length;
    int mapping_quality;
    std::string ciga;
};
    
struct Base_allele
{
    void initial();
    void add_allele(char allele);
    void show(bool EOL);

    int A;
    int T;
    int C;
    int G;
    int Deletion;
    int Insertion;
    std::vector<Base_allele> InsertionVec;
};

struct Align
{
    void counter(char allele, int& position);
    
    Base_allele *base_count;
};

struct Contig
{
    std::string contig_name;
    std::string sequence;
    std::vector<Align> alignVec;
};

struct Genome
{
    void loadDraft(int argc, char* argv[]);
    void parsePAF(int argc, char* argv[]);

    std::vector<Contig> contigs;
};
