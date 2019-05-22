# create paf file using minimap2

--cs=long is necessary parameter

	minimap2 --cs=long -x asm5  draft_genome.fasta truth.fasta  > truth_to_draft.paf
	minimap2 --cs=long  draft_genome.fasta raw_read.fasta  > raw_to_draft.paf

# calculater num of insertion/deletion

	cs_long truth_to_draft.paf

# create base cout table
one paf column : contig name, draft position, draft base, A, T, C, G, insertion, deletion
two paf column : contig name, draft position, draft base, 1(A), 1(T), 1(C), 1(G), 1(insertion), 1(deletion), 2(A), 2(T), 2(C), 2(G), 2(insertion), 2(deletion)

	baseCount draft_genome.fasta raw_to_draft.paf
	baseCount draft_genome.fasta ref_to_draft.paf raw_to_draft.paf
