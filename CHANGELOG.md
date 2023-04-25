# Fitcrack 2.4.0
Released: 2023-04-??
* Fitcrack can now be deployed using Docker (pre-built image or custom build)
* Legacy installation ways still work and are better documented now
* Upgraded BOINC to 7.27
* Upgraded Hashcat to 6.2.6
* Live monitoring of GPU on hosts (cracking speed, utilization, temperature)
* Start/Stop/Discard/Purge features for multiple jobs at once
* Various frontend improvements and changes
* New watchdog over server daemons, BOINC, and Apache
* Auto-conversion of timezones: Users no longer need to enter time in CET
* Improvements in Trickler daemon: stability, retransmission of lost messages
* Update of Fitcrack documentation that covers new features
* Big security audit: no more insecure public endpoints, fixed various issues
* Manage user management: more descriptive, better handling of user mistakes, bugfixes
* Improved stability of measureUsage daemon - no more crashes when running for a long time
* Login button is not clickable anymore when username/password is empty
* New setting for dictionary attacks: fragmenting dictionaries on host machine
* New button to unassign host from all jobs
* New option - users can now decide whether to use optimized hashcat kernels or not
* Security: Flask now uses a pseudorandom secret token
* Runner now logs BOINC errors when sending Trickle message is not successful
* Updated Python modules and more versions are now fixed to prevent future issues
* Fixed mask loading from long mask files
* Fixed issue which allowed loading invalid masks
* Fixed PCFG file-encoding issues
* Fixed PCFG download issue for newly-generated grammars
* Fixed overwriting of PCFG and Markov files when a same name is used
* Fixed sorting of dictionary passwords with non-ASCII characters
* Failed upload of a dictionary no more leaves a ghost orphan in the database
* Fixed server dictionary browser
* Fixed missing quotes in hashcat masks
* Fixed security issue in dictionary upload endpoint
* Fixed displaying of long binary hashes in WebAdmin
* Fixed Werkzeug package version in WebAdmin
* Fixed hash extraction for some types of RAR5
* Fixed hash validation for 11300 - Bitcoin/Litecoin wallet.dat
* Fixed broken WPA hash types
* LUKS - fix for showing the password correctly
* Fixed TrueCrypt VeraCrypt hash handling  (NVIDIA clients)
* Hostname auto-detection in BOINC projects
* Fix BOINC URL_BASE for localhost builds

# Fitcrack 2.3.0
Released: 2020-10-24
* A complete redesign of the dashboard
* Improved server monitor
* New charts with better look and performance
* Upgraded to hashcat 6.1.1 (new algorithms and higher performance)
* XtoHashcat update (RAR3-p detection, fixed PKZIP, SecureZIP, ...)
* Various bugfixes (benchmarking, user management, hash processing, ...)
* Hashvalidator is no longer needed
* JQuery no longer needed
* Updated WebAdmin user manual

# Fitcrack 2.2.0
Released: 2020-06-23
* Improved job management
* Introduced bins for better organization of jobs
* New feature: Job batches
* New attack mode: PRINCE
* Improved hybrid attacks to better distribute the workload
* Improved benchmarking to be more accurate
* Improved scheduling algoritms for efficient host utilization
* Advanced system settings that gives more control to users
* Lots of bugfixes

# Fitcrack 2.1.1
Released: 2020-04-11
* Fixed file uploads – loading hashlist and hash extraction
* Fixed verification of salted hashes
* Added WebAdmin user manual: https://nesfit.github.io/fitcrack/

# Fitcrack 2.1.0
Released: 2020-03-03
* Upgraded to hashcat 5.1.0
* A complete redesign of WebAdmin app
* New dashboard
* Added system monitor page
* Improved control of cracking jobs
* New attack mode: PCFG
* Lots of bugfixes

# Fitcrack 2.0.0
Released: 2019-04-10
* Updated to BOINC server 7.13
* Uses hashcat version 4.2.1
* Installer included for easier deployment
* Various bugfixes in WebAdmin, Runner and server daemons

# Fitcrack 1, version 2018
Released 2018-09-01
* Support for hashcat 4.x
* All hashcat attack modes are now supported
* New webadmin with Python3-based backend and VueJS-based front-end
* Hashvalidator is introduced to verify the hashes

# Fitcrack 1, version 2017
Released: 2017-09-03
* Hashcat 3.x is now used as the cracking engine
* Legacy Fitcrack OpenCL/CUDA kernels are not used anymore
* Supports brute-force attack and dictionary attack

# Fitcrack 1, non-hashcat version (2016-2017)
Final release: 2017-04-05
* The original Fitcrack could serve either as a single-machine password cracking tool, or as a part of BOINC-based distributed solution.

Legacy versions are available via the Download Archive: https://fitcrack.fit.vutbr.cz/download/download-archive/