# Suffix Array Construction and Exact Pattern Matching 

## Introduction
Suffix array is a sorted arrays of all the suffixes of a string. It was proposed by Manber and Myers in 1990. Suffix arrays(SA) were proposed as an space efficient alternative to suffix trees. Some of the popular usage of suffix arrays are for pattern matching, text compression, bioinformatics, regular expression matching etc. Here, we will discuss exact pattern matching using suffix arrays. We discuss the implementation of the following algorithms here: 
1. Suffic array construction.
2. Lcp array construction from suffix array.
3. Lcp binary tree construction.
4. Exact pattern matching using suffix arrays.

## Suffix Array construction
We use the algorithm proposed by Manber and Myers for suffix array construction. This method takes more time to construct suffix array when compared to suffix array construction from suffix tree but it is more space efficient. Since space efficiency is the main reason for choosing suffix trees over suffix array we are using the more space efficient algorithm. This algorithm requires buckets for storing the start and end position of buckets, we define a data structure `Bucket` to store these values. The class `Buckets` is defined as follows: 



