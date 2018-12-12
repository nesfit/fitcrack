# Runner
is hashcat wrapper designed to be used as either standalone tool simplifying
control of hashcat, or as middleware in **BOINC** system. This README is primary
focused on the middleware desctiption but if you exclude all the **BOINC** parts it
should work the same way for the standalone tool and all example commands are
in Linux bash.

### Middleware
As already said **Runner** is hashcat <=> boinc-client middleware. Simply all it does is: 
1. convertion of `config` in TLV format to hashcat parameters, 
2. launching and monitoring progress of **hashcat**,
3. gathering of hashcat results and communicating them to server.

It is launched by the boinc-client just as `$ ./runner64.bin`. All information
needed by **Runner** are stored in several files which are meant to be in the
same directory as the executable.

The files are:
* required files:
  * hashcatkernels1\_7.zip - containing all **hashcat** file like OpenCL kernels,
    .hcstat, .hctune, etc.   
  * config - as mentioned this is TLV formated file, supported options are
    specified bellow.
  * data - file with **hashcat** acceptable hash to be cracked.  

* optional files:
  * dict[1-2] - files with the dictionary of passwords saved in them, in
    **hashcat** acceptable format  

* output files:
  * out - this output file where **Runner** saves the **hashcat** results in the
    server understandable format.  
  * stderr.txt - program's log.  

All mention files can contain BOINC-like soft-link to the real file. You can
make this soft-link by making text files with this XML element
`<soft_link>REPLACE_WITH_PATH_TO_REAL_FILE</soft_link>` Runner then resolves it
to the absolute path to the file which it then uses instead of the soft link.
Examples are provided in directory *./testground*

#### Host specific configuration
**Runner** also supports the host based specification of **Hashcat** parameters
it is supposed to be used for the specification of workload(`-w`), which OpenCL
devices to use(`-d`), whether should **Hashcat** ignore errors(`--force`) and
such thing which aren't general by their nature. 

File with these configs should be placed at `/etc/<BOINC_project_name>.conf` on 
Linux and under `C:\ProgramData\BOINC\<BOINC_project_name>.conf`. If you would 
like to run **Runner** as standalone then create config inside the same 
directory but name it`standalone.conf`.

Format is just a part of hashcat command.

##### Example
```
$ cat /etc/fitcrack.conf 
-w 1 -d 1 --force
```
It specifies to use workload 1, device with id 1 and by-pass all possible OpenCL
errors or driver incompatibilities.

#### Attack types supported by Runner 
* 0 - Straight (Dictionary attack)
* 1 - Combination (Combinator attack - two dictionaries)
* 3 - Brute-force (Mask attack)

##### Attack subtypes 
* attack\_mode = 0 (Straight) 
   * 0 - Basic dictionary attack (name of the dictionary is in th field **dict1**)
   * 1 - Dictionary attack with rules (name of the file with rules is **rules**)
* attack\_mode = 1 (Combination)  
   * 0 - Basic combinator attack (passwords are created by combination of dictionaries **dict1** and **dict2**)
   * 1 - Combinator attack with rule for modification of left dictionary (rule is in the field **rule_left**)
   * 2 - Combinator attack with rule for modification of right dictionary (rule is in the field **rule_right**)
   * 3 - Combinator attakc with rules for both dictionaries (**rule_left**, **rule_right**)
* attack\_mode = 3 (Brute-force with mask)  
  * 0 - Basic brute-force with mask a default (**hashcat.hcstat**) file
  * 1 - Brute-force with basic 2D hcstat file (name of the hcstat file is **markov**)
  * 2 - Brute-force with 3D hcstat file (name of the hcstat file is **markov**)

| **Name of the field** |**Data type**| **Description**								    | **Hashcat argument** |
|-----------------------|-------------|-----------------------------------------------------------------------------|----------------------|
|attack_mode	        |UInt         | Hashcat attack mode							    | -a		   |
|attack_submode         |UInt         | Attack mode submode	 			    			    |		      	   |
|hash_type              |UInt         | Hashcat hash type					    		    | -m              	   |
|name                   |String       | Name of the task					    		    |                 	   |                
|charset1	        |String       | String containing HEX encoded custom charset				    		    | -1	      	   |
|charset2	        |String       | String containing HEX encoded custom charset   			    			    | -2 	      	   |
|charset3	        |String       | String containing HEX encoded custom charset   			    			    | -3 	      	   |
|charset4               |String       | String containing HEX encoded custom charset   			    			    | -4 	      	   |
|rule_left              |String       | Rule for the left dictionary  			    			    | -j	      	   |
|rule_right             |String       | Rule for the right dictionary 			    			    | -k	      	   |
|mask	                |String       | Hashcat mask					    			    |   	      	   |
|start_index            |BigUInt      | Computation start index				    			    | --skip	      	   |
|hc_keyspace            |BigUInt      | Amount of hashes to compute			    			    | --limit         	   |
|mask_hc_keyspace       |BigUInt      | Hashcat keyspace of the mask			    			    |   	      	   |
|mode                   |String       | Type of the task benchmark, normal(cracking), benchmark-all (b, n, a)	    | --benchmark     	   |
|markov_threshold	|UInt	      | Threshold value for markov chains					    |			   |

##### Example of TLV config 
```
|||attack|String|4|dict|||
|||attack_mode|UInt|1|0|||
|||hash_type|UInt|4|9400|||
|||start_index|BigUInt|1|0|||
|||hc_keyspace|BigUInt|6|135985|||
|||mode|String|1|n|||
```

More TLV sample can be found in the directory *./sample*.

It is also possible to have external password generator which output can be
redirected to **Runner**'s stdin. **Runner** then redirects its stdin to be 
**Hascat**'s stdin without even touching the data provided.

## Compilation

### Prerequisitied
Toolchain g++4.8, toolchain MinGW64-3.0.1 and standard C and C++ libraries and header. Hashcat
requires OpenCL ICD loader to be installed. For more about that go to [Hashcat
websites](http://hashcat.net)

### Make
Runner comes with cross-compilation **Makefile**. MinGW64 project is used for
compilation of Windows binary. In directory *./lib* are precompiled **BOINC**
and **libzip** and **zlib** static libraries. Runner is aimed to be compiled by
g++4.8 same as with MinGW64-g++4.8. If you would want to use some higher version
you will probably need to recompile those static libraries by yourself.

Linux compilation: 

`$ make linux`

Windows compilation: 

`$ make windows`

If you would want to compile both by "one" command you can do it like this:

`$ make linux && make cleanobj && make windows`

Binaries are then stored in the *./bin* directory.

The default binaries are build on Ubuntu 14.04 64-bit machine with g++4.8 and
Ubuntu 14.04 default MinGW g++ which is also 4.8 and are targeted for Linux
64-bit machines and Windows Vista or later 64-bit machines.

##### Execution example
`$ ./runner64.bin`

##### Example of minimal amount of file need for cracking task
```
$ ls
    config
    data
    hashcat64.bin
    hashcatkernel1_7.zip
    runner64.bin
```

