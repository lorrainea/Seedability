Seedability
===

<b>Installation</b>: To compile Seedability, run make.

<b>INPUT</b>: A set of reads in multiFASTA format and a reference sequence (optional).

<b>OUTPUT</b>: A TSV file containing an estimate of the seed length (k) and number of shared seeds (t) between every pairwise (i,j) reads (or every read i and the given reference) as well as the optimal number of shared seeds and seed length of all sequences.


Column 1: Read_i id or Ref id (if parameter -r is used).

Column 2: Read_j id or Read_i id (if parameter -r is used).

Column 4: Number of shared seeds (t) between every sequence pair.

Column 5: Length of shared seeds (k) between every sequence pair.

```
Usage: seedability
Standard (Mandatory):
-q, --reads-file	<str>		multiFASTA reads filename. 
-o, --output-filename	<str>		Output filename.
Optional:
-r, --ref-file		<str>		FASTA reference filename.
-l, --min-k		<int>		Minimum k value to explore (Default: 3).
-k, --max-k		<int>		Maximum k value to explore (Default: 15).
-d, --delta		<double>	Threshold allowance between best alignment identity and alignment identity for larger k values (Default: 0.05).

```

<b>Examples</b>
```
./seedability -q ./data/synthetic/reads.fasta -o out_reads.tsv
./seedability -q ./data/synthetic/reads.fasta -r ./data/synthetic/ref.fasta -o out_reads_ref.tsv
```
<b>Experimental Results</b>

<p float="left">
<img src="https://github.com/lorrainea/Seedability/blob/main/data/synthetic/map-ont_minimap2.png" width="500" height="280" />
<img src="https://github.com/lorrainea/Seedability/blob/main/data/synthetic/map-ont_seedability.png" width="500" height="280" />
</p>

The results above show that for datasets ranging from 100bp-15000bp in length and a range of divergences from 5%-25%, the average alignment identity between 100 sequence pairs is higher when using the results output by Seedability along with Minimap2, in comparison to when using the default values of Minimap2. Note that some sequences are unmapped when using the default values of Minimap2.

<b>Citation</b>:
```
Lorraine A K Ayad and others, Seedability: Optimising alignment parameters for sensitive sequence comparison, Bioinformatics Advances, 2023;, vbad108, https://doi.org/10.1093/bioadv/vbad108
```

<b>License</b>: GNU GPLv3 License; Copyright (C) 2023 Lorraine A.K. Ayad, Rayan Chikhi and Solon P. Pissis.

