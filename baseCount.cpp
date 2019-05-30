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
		
		//std::cout << "create array " << "\t"
		//          << (*current_contig).contig_name << "\t" 
		//          << (*current_contig).sequence.length() << "\n";
		
        // check current contig not empty line
        if( (*current_contig).contig_name.length()==0 || (*current_contig).sequence.length()==0 )break;
        // split '>'
        (*current_contig).contig_name = (*current_contig).contig_name.substr(1,(*current_contig).contig_name.length()-1);
        // create two arrays to store alignment infomation
        (*current_contig).ref_align.base_count = new Base_allele[(*current_contig).sequence.length()];
        (*current_contig).raw_align.base_count = new Base_allele[(*current_contig).sequence.length()];
        // initial all array
        for( int i = 0 ; i < (*current_contig).sequence.length() ; i++ )
		{
            (*current_contig).ref_align.base_count[i].initial();
            (*current_contig).raw_align.base_count[i].initial();
		}
        // push current contig/chromosome
        contigs.push_back((*current_contig));
        //std::cout << (*current_contig).contig_name << "\t" << (*current_contig).sequence.length() <<"\n";
    }
}

void Genome::parsePAF(int argc, char* argv[], std::string source)
{
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

        //std::cout << paf.query_name  << "\t" << paf.query_start  << "\t" << paf.query_end  << "\t"
        //          << paf.target_name << "\t" << paf.target_start << "\t" << paf.target_end << "\t"
        //          << paf.strand << "\n";

        while(paf_file && paf_file.is_open())
        {
            std::string identity_ciga;
            paf_file >> paf.ciga;
            identity_ciga = paf.ciga.substr(0,5);

            if(!identity_ciga.compare("cs:Z:"))
            {
                std::vector<Contig>::iterator contig_iter;

                for( contig_iter = contigs.begin(); contig_iter != contigs.end(); ++contig_iter )
                    if( (*contig_iter).contig_name == paf.target_name )
                        break;

                Align* ref_align = &(*contig_iter).ref_align;
                Align* raw_align = &(*contig_iter).raw_align;

                int i=5;
                while( i < paf.ciga.length() )
                {
					if( paf.ciga[i] == '=' || paf.ciga[i] == '-' )
                    {
                        bool del = paf.ciga[i] == '-';
                        while(1)
                        {
							i++;
                            if( paf.OperatorSign(i) || i >= paf.ciga.length() ) break;

                            source == "ref" ? (*ref_align).counter( del ? '-' : paf.ciga[i], paf.target_start )
                                            : (*raw_align).counter( del ? '-' : paf.ciga[i], paf.target_start );
                        }
                    }
                    else if( paf.ciga[i] == '*' )
                    {
                        i+=3;
                        source == "ref" ? (*ref_align).counter(paf.ciga[i-1],paf.target_start)
                                        : (*raw_align).counter(paf.ciga[i-1],paf.target_start);
                    }

                    else if( paf.ciga[i] == '+' )
                    {
                        std::vector<Base_allele>::iterator insert_ref_iter = (*ref_align).base_count[paf.target_start].InsertionVec.begin();
						//std::vector<Base_allele>::iterator insert_ref_iter_end = (*ref_align).base_count[paf.target_start].InsertionVec.end();
                        int ref_vec_size = (*ref_align).base_count[paf.target_start].InsertionVec.size();
                        int ref_vec_position = 0;
						bool vector_not_full = false;
						
						std::vector<Base_allele>::iterator insert_raw_iter = (*raw_align).base_count[paf.target_start].InsertionVec.begin();
						int raw_vec_size = (*raw_align).base_count[paf.target_start].InsertionVec.size();
                        int raw_vec_position = 0;
						
						if(source == "ref")
							(*ref_align).counter('+',paf.target_start);
						else
							(*raw_align).counter('+',paf.target_start);
						
                        while(1)
                        {
                            i++;
                            if(paf.OperatorSign(i) || i >= paf.ciga.length()) break;
                            if(source == "ref")
                            {
                                /*if( insert_ref_iter == insert_ref_iter_end && vector_not_full )
								{
									(*insert_ref_iter).add_allele(paf.ciga[i]);
                                    insert_ref_iter++;
								}
								else
								{
									vector_not_full = false;
									Base_allele* temp = new Base_allele();
                                    (*temp).add_allele(paf.ciga[i]);
                                    (*ref_align).base_count[paf.target_start].InsertionVec.push_back((*temp));
								}*/
								
								if( ref_vec_position < ref_vec_size )
                                {
                                    (*insert_ref_iter).add_allele(paf.ciga[i]);

                                    insert_ref_iter++;
                                    ref_vec_position++;
                                }
                                else
                                {
                                    Base_allele* temp = new Base_allele();
                                    (*temp).add_allele(paf.ciga[i]);
                                    (*ref_align).base_count[paf.target_start].InsertionVec.push_back((*temp));
                                }
                            }
                            else
                            {
                                if( raw_vec_position < raw_vec_size )
                                {
                                    (*insert_raw_iter).add_allele(paf.ciga[i]);

                                    insert_raw_iter++;
                                    raw_vec_position++;
                                }
                                else
                                {
                                    Base_allele* temp = new Base_allele();
                                    (*temp).add_allele(paf.ciga[i]);
                                    (*raw_align).base_count[paf.target_start].InsertionVec.push_back((*temp));
                                }
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

    genome.parsePAF( argc-- , argv++ , "ref" );
    if(argc==1)
    {
        for(std::vector<Contig>::iterator output_iter = genome.contigs.begin(); output_iter != genome.contigs.end(); ++output_iter)
        {
            for(int i =0 ; i < (*output_iter).sequence.length() ; i++ )
            {
                std::cout << (*output_iter).contig_name << "\t"
                          << i << "\t"
                          << (*output_iter).sequence[i] << "\t";

                (*output_iter).ref_align.base_count[i].show(true);

                std::vector<Base_allele>::iterator ref_insert_iter = (*output_iter).ref_align.base_count[i].InsertionVec.begin();
                int ref_insert_size = (*output_iter).ref_align.base_count[i].InsertionVec.size();

                for( ref_insert_iter; ref_insert_size > 0 ; ++ref_insert_iter, ref_insert_size--)
                {
                    std::cout << (*output_iter).contig_name << "\t"
                              << i << "\t"
                              << '-' << "\t";

                    if(ref_insert_size>0)
                        (*ref_insert_iter).show(true);
                }
            }
        }
        return 0;
    }

    genome.parsePAF( argc , argv , "raw" );

    for(std::vector<Contig>::iterator output_iter = genome.contigs.begin(); output_iter != genome.contigs.end(); ++output_iter)
    {
        for(int i =0 ; i < (*output_iter).sequence.length() ; i++ )
        {
            std::cout << (*output_iter).contig_name << "\t"
                      << i << "\t"
                      << (*output_iter).sequence[i] << "\t";

            (*output_iter).ref_align.base_count[i].show(false);
            (*output_iter).raw_align.base_count[i].show(true);

            std::vector<Base_allele>::iterator ref_insert_iter = (*output_iter).ref_align.base_count[i].InsertionVec.begin();
            std::vector<Base_allele>::iterator raw_insert_iter = (*output_iter).raw_align.base_count[i].InsertionVec.begin();
            int ref_insert_size = (*output_iter).ref_align.base_count[i].InsertionVec.size();
            int raw_insert_size = (*output_iter).raw_align.base_count[i].InsertionVec.size();

            for( ref_insert_iter, raw_insert_iter ;
                 ref_insert_size > 0 || raw_insert_size > 0 ;
                 ++ref_insert_iter, ++raw_insert_iter, ref_insert_size--, raw_insert_size-- )
            {
                std::cout << (*output_iter).contig_name << "\t"
                          << i << "\t"
                          << '-' << "\t";

                if(ref_insert_size>0)
                    (*ref_insert_iter).show(false);
                else
                    std::cout << "0\t0\t0\t0\t0\t0\t";

                if(raw_insert_size>0)
                    (*raw_insert_iter).show(true);
                else
                    std::cout << "0\t0\t0\t0\t0\t0\n";
            }


        }

    }

    return 0;
}
