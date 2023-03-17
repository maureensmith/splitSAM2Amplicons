# splitSAM2Amplicons

Given SAM files with mapped read pairs based on amplicon sequencing for some statistical analyses it is better analyse the amplicons separately. Here we split the reads according to their start position within the reference sequence. 

*For now this is hard coded for out purposes but will be generalised.*
- **Amplicon 1:** positions 19 - 246 
- **Amplicon 2:** positions 227 - 437 

The SAM file must be sorted by name and containing both reads of the paired end mapping. 
Processing the read pairs one by one, we first do a sanity check if the sequence name ist identical (first column in SAM file). The fourth column contains the start position of the read with respect to the reference sequence. 

If the start positions of both read pairs are between ampli1_start and ampli2_start, they are assigned to amplicon 1, if both start positions fall in the range of ampli2_start and ampli2_end they are assigned to amplicon 2. If the read pairs are discordant, they are filtered out (but written into a third SAM file).

The read pairs are written into 3 SAM files in different directories, which are created in the same location as the parent-directory of the original/input SAM file: 

- SAM_ampli1/sample.sam
- SAM_ampli2/sample.sam
- SAM_filtered/sample.sam


The program is called from the command line with
```
splitSAM2amplicons <samfile>
```

and requires the following parameter

| parameter       | type          | description  |
| :-------------  |:-------------:| :-----|
| samfile         | (string)      | path to SAM file containing mapped paired reads in  |


*(futere parameters tba)*