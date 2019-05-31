#include "baseCount.h"


bool PAF_format::OperatorSign(int position)
{
    if( ciga[position] == '=' || ciga[position] == '+' ||
        ciga[position] == '-' || ciga[position] == '*')
         return true;
    return false;
}

void Base_allele::initial()
{
    Deletion  = 0 ;
    Insertion = 0 ;
    A = 0 ;
    T = 0 ;
    C = 0 ;
    G = 0 ;
}

void Base_allele::show(bool EOL)
{
    std::cout << A << "\t" << T << "\t" << C << "\t" << G << "\t"
              << Deletion  << "\t" << Insertion << (EOL ? "\n" : "\t");
}

void Base_allele::add_allele(char allele)
{
    if( allele == '-' )Deletion++;
    else if( allele == '+' )Insertion++;
    else if( allele == 'A' || allele == 'a' )A++;
    else if( allele == 'T' || allele == 't' )T++;
    else if( allele == 'C' || allele == 'c' )C++;
    else if( allele == 'G' || allele == 'g' )G++;
}

void Align::counter(char allele, int& position)
{
    base_count[position].add_allele(allele);
    allele == '+' ? position : position++;
}

void Genome::loadDraft(int argc, char* argv[])
{
    std::ifstream draft_genome(argv[1]);
    // create arrays of draft genome sizes to store alignment info
    while( draft_genome && draft_genome.is_open() )
    {
        Contig* current_contig = new Contig();
        draft_genome >> (*current_contig).contig_name;
        draft_genome >> (*current_contig).sequence;
        // check current contig not empty line
        if( (*current_contig).contig_name.length()==0 || (*current_contig).sequence.length()==0 )break;
        // split '>'
        (*current_contig).contig_name = (*current_contig).contig_name.substr(1,(*current_contig).contig_name.length()-1);
        // push current contig/chromosome
        contigs.push_back((*current_contig));
    }
}

void Genome::parsePAF(int argc, char* argv[])
{
    // for current paf file, it will be create alignment table for each contig
    for( std::vector<Contig>::iterator current_contig = contigs.begin() ; current_contig != contigs.end() ; ++current_contig )
    {
        // new alignment table
        Align* alignTable = new Align();
        // set alignment table size, the table size will be equal contig length
        (*alignTable).base_count = new Base_allele[(*current_contig).sequence.length()];
        // initialize data for each location
        for( int position = 0 ; position < (*current_contig).sequence.length() ; position++ )
            (*alignTable).base_count[position].initial();
        // push alignment table of current contig to vector
        (*current_contig).alignVec.push_back((*alignTable));
    }

    std::ifstream paf_file(argv[1]);

    while( paf_file && paf_file.is_open() )
    {
        PAF_format paf;

        paf_file >> paf.query_name;
        paf_file >> paf.query_length;
        paf_file >> paf.query_start;
        paf_file >> paf.query_end;
        paf_file >> paf.strand;
        paf_file >> paf.target_name;
        paf_file >> paf.target_length;
        paf_file >> paf.target_start;
        paf_file >> paf.target_end;
        paf_file >> paf.residue_matches;
        paf_file >> paf.alignment_block_length;
        paf_file >> paf.mapping_quality;

        if( paf.query_name.length() == 0 ) break;

        while(paf_file && paf_file.is_open())
        {
            std::string identity_ciga;
            paf_file >> paf.ciga;
            identity_ciga = paf.ciga.substr(0,5);

            if(!identity_ciga.compare("cs:Z:"))
            {
                std::vector<Contig>::iterator current_contig;

                for( current_contig = contigs.begin(); current_contig != contigs.end(); ++current_contig )
                    if( (*current_contig).contig_name == paf.target_name )
                        break;

                std::vector<Align>::iterator current_contig_alignTable = (*current_contig).alignVec.end()-1;

                int position=5;
                while( position < paf.ciga.length() )
                {
                    if( paf.ciga[position] == '=' || paf.ciga[position] == '-' )
                    {
                        bool del = paf.ciga[position] == '-';
                        while(1)
                        {
                            position++;
                            if( paf.OperatorSign(position) || position >= paf.ciga.length() ) break;

                            (*current_contig_alignTable).counter( del ? '-' : paf.ciga[position], paf.target_start );
                        }
                    }
                    else if( paf.ciga[position] == '*' )
                    {
                        position+=3;
                        (*current_contig_alignTable).counter(paf.ciga[position-1],paf.target_start);
                    }

                    else if( paf.ciga[position] == '+' )
                    {
                        std::vector<Base_allele>::iterator insert_ref_iter     = (*current_contig_alignTable).base_count[paf.target_start].InsertionVec.begin();
                        std::vector<Base_allele>::iterator insert_ref_iter_end = (*current_contig_alignTable).base_count[paf.target_start].InsertionVec.end();
                        
                        bool vector_not_full = true;
                        
                        (*current_contig_alignTable).counter('+',paf.target_start);
                        
                        while(1)
                        {
                            position++;
                            if( paf.OperatorSign(position) || position >= paf.ciga.length() ) break;

                            if( insert_ref_iter != insert_ref_iter_end && vector_not_full )
                            {
                                (*insert_ref_iter).add_allele(paf.ciga[position]);
                                insert_ref_iter++;
                            }
                            else
                            {
                                vector_not_full = false;
                                Base_allele* temp = new Base_allele();
                                (*temp).add_allele(paf.ciga[position]);
                                (*current_contig_alignTable).base_count[paf.target_start].InsertionVec.push_back((*temp));
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    Genome genome;

    genome.loadDraft( argc-- , argv++ );
    
    while( argc > 1 )
    {
        genome.parsePAF( argc-- , argv++ );
    }
    
    // output alignment table
    for(std::vector<Contig>::iterator output_contig = genome.contigs.begin(); output_contig != genome.contigs.end(); ++output_contig)
    {
        for(int position = 0 ; position < (*output_contig).sequence.length() ; position++ )
        {
            std::cout << (*output_contig).contig_name << "\t"
                      << position << "\t"
                      << (*output_contig).sequence[position] << "\t";

            int max_insertion_size = 0;
                
            for(std::vector<Align>::iterator alignTable = (*output_contig).alignVec.begin() ; alignTable != (*output_contig).alignVec.end() ; ++alignTable )
            {
                (*alignTable).base_count[position].show(false);
                    
                max_insertion_size = max_insertion_size > (*alignTable).base_count[position].InsertionVec.size() 
                                   ? max_insertion_size : (*alignTable).base_count[position].InsertionVec.size();
            }
            std::cout<< "\n";

            // this position has occurred insertion
            // must check each alignment for insertion
            for( int insertion_stratum = 1 ; insertion_stratum <= max_insertion_size ; insertion_stratum++ )
            {
                std::cout << (*output_contig).contig_name << "\t"
                          << position << "\t"
                          << '-' << "\t";
                          
                // check the insertion of each alignment on this position
                // if there is no alignment information in this PAF file, each base will be set to 0
                for(std::vector<Align>::iterator alignTable = (*output_contig).alignVec.begin() ; alignTable != (*output_contig).alignVec.end() ; ++alignTable )
                {
                    if( (*alignTable).base_count[position].InsertionVec.size() >= insertion_stratum )
                    {
                        std::vector<Base_allele>::iterator insertion_iter = (*alignTable).base_count[position].InsertionVec.begin() + insertion_stratum - 1;
                        (*insertion_iter).show(false);
                    }
                    else
                        std::cout << "0\t0\t0\t0\t0\t0\t";
                }
                std::cout<<"\n";
            }    
        }
    }

    return 0;
}
