```
https://github.com/yukiteruono/pbsim2
https://github.com/lh3/minimap2/blob/master/misc/paftools.js
```
The data within this folder was simulated using the pbsim2 tool and chromosome 1 of the human genome.
paftools.js is a script used to output the simulated sequences in fasta format.
The datasets were generated using the following command:

```
reads=simulated-CHM13.chr1.reads.10X
ref=CHM13.chr1.fa

~/tools/PBSIM-PacBio-Simulator/data/model_qc_clr --accuracy-mean e --accuracy-sd 0 --depth 10 --prefix $reads --length-mean ell

paftools.js pbsim2fq $ref.fai "$reads"_*.maf > $reads.fa
rm -f "$reads"_*.maf "$reads"_*.ref "$reads"_*.fastq

```
where e is the alignment identity (0.85 and 0.9) and ell is the average length of the query sequences (200 and 500).
