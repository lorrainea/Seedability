Seedability
===

<b>Installation</b>: To compile Seedability, run make.

<b>INPUT</b>: A set of reads in multiFASTA format and a reference sequence (optional).

<b>OUTPUT</b>: A TSV file containing an estimate of the seed length (k) and number of shared seeds (t) between every pairwise (i,j) reads (or every read i and the given reference) as well as the optimal number of shared seeds and seed length of all sequences.


Column 1: Read_i id or Ref id (if parameter -r is used).

Column 2: Read_j id or Read_i id (if parameter -r is used).

Column 4: Number of shared seeds (t) between every sequence pair.

Column 5: Length of shared seeds (k) between every sequence pair.

Seedability will also return on the command line the optimal (k,t) values for all reads.

```
Usage: seedability
Standard (Mandatory):
-q, --reads-file	<str>		multiFASTA reads filename. 
-o, --output-filename	<str>		Output filename.
Optional:
-r, --ref-file		<str>		FASTA reference filename.
```

<b>Examples</b>
```
./seedability -q ./data/synthetic/reads.fasta -o out_reads.tsv
./seedability -q ./data/synthetic/reads.fasta -r ref.fasta -o out_reads_ref.tsv
```
<b>Experimental Results</b>

<p float="left">
<img src="https://github.com/lorrainea/Seedability/blob/main/data/synthetic/map-ont_minimap2.png" width="500" height="280" />
<img src="https://github.com/lorrainea/Seedability/blob/main/data/synthetic/map-ont_seedability.png" width="500" height="280" />
</p>

The results above show that for datasets ranging from 200bp-15000bp in length and a range of divergences from 5%-25%, the average alignment identity between 100 sequence pairs is higher when using the results output by Seedability along with Minimap2, in comparison to when using the default values of Minimap2. Note that some alignment identities are empty when using the default values of Minimap2.

<b>License</b>: GNU GPLv3 License; Copyright (C) 2022 Lorraine A.K. Ayad, Rayan Chikhi and Solon P. Pissis.
