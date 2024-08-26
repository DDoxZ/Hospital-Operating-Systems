# Operating Systems Project

## Identificação
**This Project was made in collaboration with two other classmates:**<br>
Daniela Camarinha 58199 <br>
Diogo Lopes 60447<br>
Pedro Silva 59886<br>

## About

This project aims to simulate the admission and the service of pacients in a hospital, which involves 3 steps: Admission by
a receptionist, the forwarding of pacients into a waiting room and the service giving by a doctor. There are also logs to 
register how and when this process is done.
This aims to get students familiar with the C language and POSIX.

## What we learned
 - processes and threads
 - memory managment
 - logging and alarms
 - signals

---
## Operations
The CLi program contains the following operations:<br>
1. ```add pacient doctor``` - does the admission in the hospital indicating that the pacient wants to schedule an appointment with the doctor
2. ```info id``` - check the status of the admission with a specific id
3. ```status``` - checking the status of the data buffer that stores all of the data needed for the program
4. ```help``` - shows aditional information on these operations
5. ```end``` - ends the execution of the program


---
## Directories

The hOSpital project is divided into these directories:
- ```bin```: contains the executable file
- ```include```: contains the files with the extension .h
- ```obj```: contains the files with the .o extension
- ```src```: contains the source files with the .c extension

---
## Compilation

```bash
   make clean
```
```bash
   make
```

---
## Execution

```bash
   ./bin/hOSpital config.txt
```

