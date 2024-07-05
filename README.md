# BachelorThesis

For Remote At Uni
  - run make qemu in wsl and copy paste the qemu command the makefile produces (right after choosing which lua entry to execute)
        adding -accel kvm -cpu host to the qemu command in neccessary for mpk to be available
  - for this to work put in the same location of the remote machine the 
        obj/l4/<platform>/bin/<platform>_gen
        aswell as the 
        obj/fiasco/<platform>/fiasco
        files

General MakeFiles
  - make clean removes (or maybe marks every file to be removed at next make) all build files of that would be done by this makefile
  - make compiles the files

On WSL Do Config Self Config
 Done for basic Build:
  - fiasco kernel:
    - in src/fiasco do make B=../../obj/fiasco/builddir
    - goto build dir
    - then optionally do make config 
    - then make -j4

  - l4 framework:
    - in src/l4 make B=../../obj/l4/builddir
    - goto builddir
    - then optionally do make config
    - then make -j4
    - in src/l4/conf/Makeconf.boot (Makeconf.boot.example renamed) 
         (this can be done at any step or be ommited if done before)
        for MODULE_SEARCH_PATH add all required modules (for base builddir of fiascor kernel)
  - then back in l4 builddir (that makefile knows the src folder by default)
      build the image needed for example 
    - make uimage E=<buildImage> (build image is an entry in src/l4/conf/modules.list)
    - for starting qemu directly do make qemu E=<buildImage>
  
On WSL Auto Config Fast start
  - root directory
    - make setup -> setups obj folder for specific platform
    - make -> compiles everything needed
    - make qemu -> runs everything on qemu
    -- note: in obj/l4/platform/conf/Makeconf.boot muss im zweifel die zeile 
             QEMU_OPTIONS += -serial stdio rausgenommen werden


Own Code
  - makefiles
    - in the l4 src conf dir edit the modules.list to add your own modules list above all your projects.
      - this will later add all the configs for building/running l4re
      - with: include /absolute/path/to/your/own/modules.list
    - first of in makefile the l4dir has to be manually edited to point the the l4 src folder
      - L4DIR variable can be relative to the location of each makefile (increasing ../)
      - include $(L4DIR)/mk/prog.mk is for programs so often servers or clients
      - include $(L4DIR)/mk/subdir.mk is to import everything from a subdirectory
      - since we dont build in different folders we can just say in top level makefile
          O = relative/path/to/obj/l4/platform/
    - then with make we build everything in the makefile and in all makefiles below
    - make clean removes all builds in that makefile