Seedability
===

<b>Installation</b>: To compile Seedability, run make.

<b>INPUT</b>: A set of reads in multiFASTA format and a reference sequence (optional).

<b>OUTPUT</b>: The number of shared seeds and seed length.

```
   fprintf ( stdout, " Usage: seedability <options>\n" );
   fprintf ( stdout, " Standard (Mandatory):\n" );
   fprintf ( stdout, "  -q, --reads-file	<str>		multiFASTA reads filename.\n" ); 
   fprintf ( stdout, " Optional:\n" );
   fprintf ( stdout, "  -r, --ref-file		<str>		FASTA reference filename.\n" );
```

<b>License</b>: GNU GPLv3 License
