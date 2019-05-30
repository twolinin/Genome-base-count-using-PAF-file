#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    std::ifstream table(argv[1]);
    std::string contig_name;
    std::string current_contig;
    int  draft_position;
    char draft_allele;
    int num_of_A;
    int num_of_T;
    int num_of_C;
    int num_of_G;
    int num_of_Deletion;
    int num_of_Insertion;

    bool new_contig = true;
	int allele_thread = 10;

    while( table && table.is_open() )
    {
        int biggest = 0;
		int bigger = 0;

        table >> current_contig;
        table >> draft_position;
        table >> draft_allele;
        table >> num_of_A;
        table >> num_of_T;
        table >> num_of_C;
        table >> num_of_G;
        table >> num_of_Deletion;
        table >> num_of_Insertion;

        if( current_contig.length() == 0 )break;
        if( new_contig || contig_name != current_contig )
        {
            if( !new_contig )
                std::cout<< "\n";

            new_contig = false;

            contig_name = current_contig;
            std::cout<< '>' << contig_name << "\n";
        }

		(num_of_A > biggest) ? bigger=biggest, biggest=num_of_A : 
        (num_of_A > bigger)  ? bigger=num_of_A : num_of_A;
		(num_of_T > biggest) ? bigger=biggest, biggest=num_of_T : 
        (num_of_T > bigger)  ? bigger=num_of_T : num_of_T;
		(num_of_C > biggest) ? bigger=biggest, biggest=num_of_C : 
        (num_of_C > bigger)  ? bigger=num_of_C : num_of_C;
		(num_of_G > biggest) ? bigger=biggest, biggest=num_of_G : 
        (num_of_G > bigger)  ? bigger=num_of_G : num_of_G;
		(num_of_Deletion > biggest) ? bigger=biggest, biggest=num_of_Deletion : 
        (num_of_Deletion > bigger)  ? bigger=num_of_Deletion : num_of_Deletion;

		// zero read alignment region
		if( biggest == 0 && draft_allele != '-')std::cout<< draft_allele;
		else if( biggest == 0 && draft_allele == '-')continue;
		// at least one allele support origin base, keep origin result
		/*else if( draft_allele == 'A' && num_of_A != 0 )std::cout<< draft_allele;
		else if( draft_allele == 'T' && num_of_T != 0 )std::cout<< draft_allele;
		else if( draft_allele == 'C' && num_of_C != 0 )std::cout<< draft_allele;
		else if( draft_allele == 'G' && num_of_G != 0 )std::cout<< draft_allele;*/
		// biggest less than threshold, keep origin result
		else if( biggest < allele_thread && draft_allele != '-' ) std::cout<< draft_allele;
		else if( biggest < allele_thread && draft_allele == '-' ) continue;	
		// unable to determine the result to change, keep origin result
		else if( biggest == bigger && draft_allele != '-' )std::cout<< draft_allele;
		else if( biggest == bigger && draft_allele == '-' )continue;		
		// the biggest is to support this position to be replaced
        else if( biggest == num_of_A )std::cout<< 'A';
        else if( biggest == num_of_T )std::cout<< 'T';
        else if( biggest == num_of_C )std::cout<< 'C';
        else if( biggest == num_of_G )std::cout<< 'G';

        /*
        std::cout<< contig_name << "\t"
                 << draft_position << "\t"
                 << draft_allele << "\t"
                 << num_of_A << "\t"
                 << num_of_T << "\t"
                 << num_of_C << "\t"
                 << num_of_G << "\t"
                 << num_of_Deletion << "\t"
                 << num_of_Insertion << "\n";
        */
    }

}
