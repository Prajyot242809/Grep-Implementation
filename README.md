# Grep-Implementation
A C++ implementation of 'grep'.
It supports common flags, recursive search, inversion, etc.

#Features 
- Search in files
- Search in multiple files
- Flags:
  - '-n'  It shows line numbers for matching pattern
  - '-i'  Searches upper as well as lower case
  - '-v'  Gives non matching lines from the files
  - '-c'  Count total no. of matching lines
  - '-l'  Prints files containing the pattern
  - '-r'  Recursive search in given directory
  - '--color'  Highlight pattern in files in BOLD CYAN
- Use of KMP algorithm for fast searching 


# Searching a pattern
- grep pattern file flags
  
