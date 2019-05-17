# create paf file using minimap2
--cs=long is necessary parameter
	minimap2 --cs=long -x asm5  draft_genome.fasta truth.fasta  > truth_to_draft.paf
	minimap2 --cs=long  draft_genome.fasta raw_read.fasta  > raw_to_draft.paf

# calculater num of insertion/deletion

	cs_long truth_to_draft.paf

# create base cout table
column : contig name, draft position, draft base, A, T, C, G, insertion, deletion

	baseCount draft_genome.fasta raw_to_draft.paf
