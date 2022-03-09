# create paf file using minimap2 

--cs=long is necessary parameter

	minimap2 --cs=long -x asm5  draft_genome.fasta truth.fasta  > truth_to_draft.paf
	minimap2 --cs=long  draft_genome.fasta raw_read.fasta  > raw_to_draft.paf

# calculater num of insertion/deletion

	cs_long truth_to_draft.paf

# create base cout table
one paf column : contig name, draft position, draft base, A, T, C, G, insertion, deletion

two paf column : contig name, draft position, draft base, 1(A), 1(T), 1(C), 1(G), 1(insertion), 1(deletion), 2(A), 2(T), 2(C), 2(G), 2(insertion), 2(deletion)

	baseCount draft_genome.fasta raw_to_draft.paf > raw_to_draft.table
	baseCount draft_genome.fasta truth_to_draft.paf raw_to_draft.paf > truth_raw_to_draft.table

one paf

	contig    13353   A       15      0       0       0       0       0
	contig    13354   G       0       0       0       15      0       0
	contig    13355   C       0       0       0       0       15      0
	contig    13356   C       0       0       15      0       0       0
	contig    13357   C       0       0       15      0       0       0
	contig    13358   G       0       0       0       15      0       15
	contig    13358   -       0       15      0       0       0       0
	contig    13359   G       0       0       0       15      0       0
	contig    13360   C       0       0       15      0       0       0

two paf

	contig    6       C       0       0       15      0       0       0       0       0       22      0       2       0
	contig    7       C       0       0       15      0       0       0       0       0       25      0       0       0
	contig    8       C       0       0       15      0       0       0       0       0       26      0       0       1
	contig    8       -       0       0       0       0       0       0       0       0       0       1       0       0
	contig    8       -       0       0       0       0       0       0       1       0       0       0       0       0
	contig    9       A       15      0       0       0       0       0       26      0       0       0       0       1
	contig    9       -       0       0       0       0       0       0       1       0       0       0       0       0
	contig    9       -       0       0       0       0       0       0       0       1       0       0       0       0
	contig    10      T       0       15      0       0       0       0       0       26      0       0       1       0
	contig    11      T       0       15      0       0       0       0       0       26      0       0       1       1
	contig    11      -       0       0       0       0       0       0       0       0       1       0       0       0
	contig    11      -       0       0       0       0       0       0       0       0       1       0       0       0
	contig    11      -       0       0       0       0       0       0       1       0       0       0       0       0
	contig    12      C       0       0       15      0       0       0       0       0       25      0       2       1

# create new genome using count table

	createGenome raw_to_draft.table > raw_polish.fasta
