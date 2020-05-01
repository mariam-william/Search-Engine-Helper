# Search-Engine-Helper
A parallel program to search a given corpus and return the most relevant search results. Given a corpus called Aristo Mini Corpus.
**Using Master Slave Paradigm.**

* **Aristo Mini Corpus**  - [Through this Link](https://www.kaggle.com/allenai/aristo-mini-corpus).

#### Release History
* April 2020: V1

### Code Author
* **Mariam Makram**  - [mariammakram](https://github.com/mariammakram)
* **Lauren Safwat**  - [lauren-safwat](https://github.com/lauren-safwat)

### What is Aristo Mini Corpus
The Aristo Mini corpus contains 1,197,377 science-relevant sentences drawn from public data. It provides simple science-relevant text that may be useful to help answer elementary science questions. You will work on 1500 sentence only divided across 50 File, each file is 30 lines.

* **Input:** a given query in form of a sentence or a question.
* **Output:** search results that contain all the words of the query.

## Implementation
This code is written in **C** programming language. Using **MPI** Library and using **Master-Slave paradigm.**

## Testing the Program
To test this program, you should run a VM VirtualBox on your OS.
You can run it through these commands after putting source file in Shared folder:

`mpicc SearchHelper.c -o SearchHelper.o` **For Compiling the code**
`mpiexec -n 3 ./SearchHelper.o` **For Running the code**

*hint:* 3 is number of ranks, it is changeable.
