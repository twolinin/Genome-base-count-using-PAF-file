#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

struct Base_allele;
struct Chromosome;
typedef std::vector<Chromosome> Chromosomes;

struct Base_allele
{
    int A;
    int T;
    int C;
    int G;
    int Insertion;
    int Deletion;
};

struct Chromosome
{
    std::string contig_name;
    std::string sequence;
    Base_allele *base_count;
};

int main(int argc, char* argv[])
{
    std::ifstream draft_genome(argv[1]);
    std::ifstream raw_to_draft_paf(argv[2]);

    Chromosomes chr_vec;
    std::string contig_name,sequence;

    while(draft_genome && draft_genome.is_open())
    {
        Chromosome temp;

        draft_genome >> temp.contig_name;
        draft_genome >> temp.sequence;

        if( temp.contig_name.length()==0 || temp.sequence.length()==0 ) break;

        temp.contig_name = temp.contig_name.substr(1,temp.contig_name.length()-1);

        temp.base_count = new Base_allele[temp.sequence.length()];

        //std::cout << "contig_name" << temp.contig_name << "\n";
        //std::cout << "len " <<temp.sequence.length() << "\n";

        chr_vec.push_back(temp);
    }

    while(raw_to_draft_paf && raw_to_draft_paf.is_open())
    {
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

        raw_to_draft_paf >> query_name;
        raw_to_draft_paf >> query_length;
        raw_to_draft_paf >> query_start;
        raw_to_draft_paf >> query_end;
        raw_to_draft_paf >> strand;
        raw_to_draft_paf >> target_name;
        raw_to_draft_paf >> target_length;
        raw_to_draft_paf >> target_start;
        raw_to_draft_paf >> target_end;
        raw_to_draft_paf >> residue_matches;
        raw_to_draft_paf >> alignment_block_length;
        raw_to_draft_paf >> mapping_quality;

        if( query_name.length() == 0 ) break;

        /*
        std::cout << query_name  << "\t" << query_start  << "\t" << query_end  << "\t"
                  << target_name << "\t" << target_start << "\t" << target_end << "\t"
                  << strand << "\n";
        */

        while(raw_to_draft_paf && raw_to_draft_paf.is_open())
        {
            std::string ciga;
            std::string identity_ciga;
            raw_to_draft_paf >> ciga;
            identity_ciga = ciga.substr(0,5);

            if(!identity_ciga.compare("cs:Z:"))
            {

                //std::cout<<identity_ciga<<"\n";
                //std::cout<<ciga<<"\n";

                std::vector<Chromosome>::iterator chr_iter;

                for( chr_iter = chr_vec.begin(); chr_iter != chr_vec.end(); ++chr_iter )
                    if( (*chr_iter).contig_name == target_name )
                        break;

                //std::cout<< target_name << "\t" << strand << "\n";

                int i=5;
                while(i < ciga.length())
                {
                    if( ciga[i] == '=' )
                    {
                        //std::cout << i << " =\n";
                        while(1)
                        {
                            i++;
                            if( ciga[i] == '=' || ciga[i] == '-' || ciga[i] == '+' || ciga[i] == '*' || i >= ciga.length())
                                break;
                            else if( ciga[i] == 'A' || ciga[i] == 'a' )
                                (*chr_iter).base_count[target_start].A++;
                            else if( ciga[i] == 'T' || ciga[i] == 't' )
                                (*chr_iter).base_count[target_start].T++;
                            else if( ciga[i] == 'C' || ciga[i] == 'c' )
                                (*chr_iter).base_count[target_start].C++;
                            else if( ciga[i] == 'G' || ciga[i] == 'g' )
                                (*chr_iter).base_count[target_start].G++;

                            //std::cout << ciga.length() << " " << i <<" = t" << (*chr_iter).sequence[target_start] << " c"<< ciga[i] << "\n";

                            target_start++;
                        }
                    }
                    else if( ciga[i] == '-' )
                    {
                        //std::cout << i << " -\n";
                        while(1)
                        {
                            i++;
                            if( ciga[i] == '=' || ciga[i] == '-' || ciga[i] == '+' || ciga[i] == '*' )
                                break;
                            //std::cout << i << " - " << ciga[i] <<"\n";
                            (*chr_iter).base_count[target_start].Deletion++;
                            target_start++;

                        }
                    }
                    else if( ciga[i] == '+' )
                    {
                        //std::cout << i << " +\n";
                        while(1)
                        {
                            i++;
                            if( ciga[i] == '=' || ciga[i] == '-' || ciga[i] == '+' || ciga[i] == '*' )
                                break;
                            (*chr_iter).base_count[target_start].Insertion++;
                        }
                    }
                    else if( ciga[i] == '*' )
                    {
                        //std::cout << i << "*\n";
                        i+=2;
                        if( ciga[i] == 'A' || ciga[i] == 'a' )
                            (*chr_iter).base_count[target_start].A++;
                        else if( ciga[i] == 'T' || ciga[i] == 't' )
                            (*chr_iter).base_count[target_start].T++;
                        else if( ciga[i] == 'C' || ciga[i] == 'c' )
                            (*chr_iter).base_count[target_start].C++;
                        else if( ciga[i] == 'G' || ciga[i] == 'g' )
                            (*chr_iter).base_count[target_start].G++;
                        i++;
                        target_start++;
                    }
                }
                break;
            }
        }


    }

    for(std::vector<Chromosome>::iterator output_iter = chr_vec.begin(); output_iter != chr_vec.end(); ++output_iter)
    {
        for(int i =0 ; i < (*output_iter).sequence.length() ; i++ )
        {
            std::cout << (*output_iter).contig_name     << "\t"
                      << i << "\t"
                      << (*output_iter).sequence[i] << "\t"
                      << (*output_iter).base_count[i].A << "\t"
                      << (*output_iter).base_count[i].T << "\t"
                      << (*output_iter).base_count[i].C << "\t"
                      << (*output_iter).base_count[i].G << "\t"
                      << (*output_iter).base_count[i].Deletion  << "\t"
                      << (*output_iter).base_count[i].Insertion << "\n";
        }
    }

    return 0;
}
