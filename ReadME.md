Seedability
===

<b>Installation</b>: To compile Seedability, run make.

<b>INPUT</b>: A set of reads in multiFASTA format and a reference sequence (optional).

<b>OUTPUT</b>: The number of shared seeds and seed length between every pairwise reads (or every read and the given reference) as well as the optimal number of shared seeds and seed length of all sequences.

```
Usage: seedability
Standard (Mandatory):
-q, --reads-file	<str>		multiFASTA reads filename. 
-o, --output-filename	<str>		Output filename.
Optional:
-r, --ref-file		<str>		FASTA reference filename.
```

<b>License</b>: GNU GPLv3 License
