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

        biggest = num_of_A > biggest ? num_of_A : biggest;
        biggest = num_of_T > biggest ? num_of_T : biggest;
        biggest = num_of_C > biggest ? num_of_C : biggest;
        biggest = num_of_G > biggest ? num_of_G : biggest;
        biggest = num_of_Deletion  > biggest ? num_of_Deletion  : biggest;

		if( biggest < allele_thread ) continue;
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
