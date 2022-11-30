# Experiment for Implementing Eraser 

##  Step 1: Git clone this repository

Use ***git clone*** command to pull all the content of this repository to your PC or Laptop, with a certain directory, here we call it {YOUR GIT DIR}.

##  Step 2: Do decompression experiments

***Do vcdiff***

Enter the directory "{YOUR GIT DIR}/decompression/open-vcdiff/".

Refer to the README included in the directory.

***Do zlib***

Enter the directory "{YOUR GIT DIR}/decompression/zlibdecom/".

Compile  the program by 

```
g++ gzip.cpp -o gzip -lz
```

Then run

```
./gzip compressed-data
```

##  Step 3: Do preprocess experiments

***Do eraser***

Enter the directory "{YOUR GIT DIR}/preprocess/eraser/".

Compile  the program by

```
g++ vc.cpp -o vc -lvcdcom -lvcddec
```

Then run

```
./vc data dictionary
```

***Do twins***

Enter the directory "{YOUR GIT DIR}/preprocess/twins/".

Compile  the program by

```
g++ twins.cpp -o twins -lz
```

Then run

```
./twins compressed-data
```

##  Step 4: Do matching experiments

***Do vcdiff_matching***

In your Ubuntu shell command prompt, enter the directory "{YOUR GIT DIR}/matching/vcdiff_matching/".

Then run the `compile.sh`. It will build two files named "bin" and "build".

Enter the directory "bin", select the program you want to run. 

For example, run 

```bash
./eraser-ac data dictionary pattern
```

you will get the time of eraser-ac. The others are similar with it and will not be repeated.

***Do zgrep***

Enter the directory "{YOUR GIT DIR}/matching/zgrep/".

Compile the two files separately. Run

```
./configure
make && make install
```

Then you can get the matching and decompression time of zgrep by run the follwing command respectively.

```
gzip -cdfq compressed-data > outcome
grep -cbf pattern.txt data.txt
```



